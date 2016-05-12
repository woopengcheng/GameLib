/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using Behaviac.Design.Properties;

namespace Behaviac.Design.Importers
{
    public class ImporterXML
    {
        const string dllName = "XMLPluginBehaviac.dll";

        private static Hashtable structNodeDict = new Hashtable();
        private static Hashtable enumNodeDict = new Hashtable();

        // propertyFullname, propertyType
        private static Dictionary<string, string> extraPropertyDict = new Dictionary<string, string>();

        private class Method
        {
            public class Param
            {
                public string Name;
                public string Type = "System.Object";
                public string IsOut = "false";
                public string IsRef = "false";
            }

            public string Name;
            public string ReturnType = "System.Object";
            public List<Param> Params = new List<Param>();
        }

        // methodFullname, methodType
        private static Dictionary<string, Method> extraMethodDict = new Dictionary<string, Method>();
        
        public static string ImportXML(string xmlDir, IList<string> xmlFilenames) {
            try {
                extraPropertyDict.Clear();

                if (xmlFilenames.Count == 0)
                {
                    return string.Empty;
                }

                if (string.IsNullOrEmpty(xmlDir))
                { return string.Empty; }

                if (!Path.IsPathRooted(xmlDir))
                { xmlDir = Path.GetFullPath(xmlDir); }

                if (!Directory.Exists(xmlDir))
                { return string.Empty; }

                // Push the cs and dll files into the system temp folder.
                string csDir = Path.Combine(Path.GetTempPath(), "Behaviac");
                string dllFilename = Path.Combine(csDir, dllName);

                dllFilename = preBuild(dllFilename, csDir);

                string[] filenames = Directory.GetFiles(xmlDir);
                bool needBuildDll = false;

                // Add the extra meta xml files firstly.
                string extraMeta = Path.Combine(xmlDir, Workspace.kExtraMeta);
                if (File.Exists(extraMeta))
                {
                    loadExtraMeta(extraMeta);
                }

                // Then convert all xml files to cs files.
                foreach (string filename in filenames)
                {
                    string file = Path.GetFileName(filename);
                    if (Workspace.kExtraMeta != file && xmlFilenames.Contains(file))
                    {
                        string csFilename = Path.Combine(csDir, file);
                        csFilename = Path.ChangeExtension(csFilename, ".cs");
                        needBuildDll |= generateCsFile(csFilename, filename);
                    }
                }

                // Build all cs files into a dll file.
                if (needBuildDll)
                { return buildDll(dllFilename, csDir); }

            } catch (Exception e) {
                string errorInfo = string.Format("The meta file in the folder '{0}' is invalid", xmlDir);
                errorInfo = errorInfo + "\r\n\r\n" + e.Message;
                System.Windows.Forms.MessageBox.Show(errorInfo, Resources.LoadError, MessageBoxButtons.OK);
            }

            return string.Empty;
        }

        private static string generateDllFilename(string csDir, string dllName) {
            string dllFilename = Path.Combine(csDir, dllName);
            int index = 0;

            while (File.Exists(dllFilename)) {
                dllFilename = Path.Combine(csDir, dllName + "_" + index);
                index++;
            }

            return dllFilename;
        }

        private static string preBuild(string dllFilename, string csDir) {
            // Delete the dll file if existing.
            if (File.Exists(dllFilename)) {
                try {
                    File.SetAttributes(dllFilename, FileAttributes.Normal);
                    File.Delete(dllFilename);

                } catch (Exception) {
                    dllFilename = generateDllFilename(csDir, dllName);
                }
            }

            // Delete all cs files if existing.
            DirectoryInfo dirInfo = new DirectoryInfo(csDir);

            if (!dirInfo.Exists) {
                dirInfo.Create();

            } else {
                foreach(FileInfo file in dirInfo.GetFiles()) {
                    try {
                        File.SetAttributes(file.FullName, FileAttributes.Normal);
                        file.Delete();

                    } catch (Exception) {
                    }
                }
            }

            return dllFilename;
        }

        private static string buildDll(string dllFilename, string csDir) {
            string compileString = "/c {0}csc /optimize+ /target:library /r:\"{1}\" /out:\"{2}\" \"{3}\" > \"{4}\"";
            string frameworkDir = RuntimeEnvironment.GetRuntimeDirectory();
            string appDir = Path.GetDirectoryName(Application.ExecutablePath);
            string referDll = Path.Combine(appDir, "BehaviacDesignerBase.dll");
            string csFilename = Path.Combine(csDir, "*.cs");
            string logFileName = Path.Combine(csDir, "XMLPluginCompile.txt");

            ProcessStartInfo psi = new ProcessStartInfo();
            psi.FileName = "cmd.exe";
            psi.Arguments = String.Format(compileString, frameworkDir, referDll, dllFilename, csFilename, logFileName);
            psi.WindowStyle = ProcessWindowStyle.Minimized;
            psi.UseShellExecute = false;
            psi.CreateNoWindow = true;

            Process proc = Process.Start(psi);
            proc.WaitForExit();

            if (File.Exists(dllFilename))
            { return dllFilename; }

            proc = Process.Start(logFileName);
            proc.WaitForExit();

            return string.Empty;
        }

        private static string GetAttribute(XmlNode node, string att)
        {
            XmlNode value = node.Attributes.GetNamedItem(att);

            if (value != null && value.NodeType == XmlNodeType.Attribute)
                return value.Value;

            return string.Empty;
        }

        private static void loadExtraMeta(string xmlFilename)
        {
            try
            {
                extraPropertyDict.Clear();

                XmlDocument xmlDoc = new XmlDocument();
                Encoding utf8WithoutBom = new UTF8Encoding(false);
                using (StreamReader fileStream = new StreamReader(xmlFilename, utf8WithoutBom))
                {
                    xmlDoc.Load(fileStream);
                }

                XmlNode rootNode = xmlDoc.DocumentElement;

                if (rootNode.Name == "extrameta")
                {
                    foreach (XmlNode typeNode in rootNode.ChildNodes)
                    {
                        if (typeNode.Name == "agents")
                        {
                            foreach (XmlNode xmlNode in typeNode.ChildNodes)
                            {
                                if (xmlNode.Name == "agent")
                                {
                                    string agentName = GetAttribute(xmlNode, "classfullname");

                                    foreach (XmlNode bbNode in xmlNode)
                                    {
                                        if (bbNode.Name == "properties")
                                        {
                                            foreach (XmlNode propNode in bbNode)
                                            {
                                                if (propNode.Name == "property")
                                                {
                                                    string propName = GetAttribute(propNode, "name");
                                                    string propType = GetAttribute(propNode, "type");

                                                    extraPropertyDict[agentName + "::" + propName] = propType;
                                                }
                                            }
                                        }
                                        else if (bbNode.Name == "methods")
                                        {
                                            foreach (XmlNode methodNode in bbNode)
                                            {
                                                if (methodNode.Name == "method")
                                                {
                                                    Method method = new Method();

                                                    method.Name = GetAttribute(methodNode, "name");
                                                    method.ReturnType = GetAttribute(methodNode, "returntype");

                                                    foreach (XmlNode paramNode in methodNode)
                                                    {
                                                        Method.Param param = new Method.Param();

                                                        param.Name = GetAttribute(paramNode, "name");
                                                        param.Type = GetAttribute(paramNode, "type");
                                                        param.IsOut = GetAttribute(paramNode, "isout");
                                                        param.IsRef = GetAttribute(paramNode, "isref");

                                                        if (string.IsNullOrEmpty(param.IsOut))
                                                            param.IsOut = "false";

                                                        if (string.IsNullOrEmpty(param.IsRef))
                                                            param.IsRef = "false";

                                                        method.Params.Add(param);
                                                    }

                                                    extraMethodDict[agentName + "::" + method.Name] = method;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch
            {
            }
        }

        private static bool generateCsFile(string csFilename, string xmlFilename) {
            try {
                XmlDocument xmlDoc = new XmlDocument();
                Encoding utf8WithoutBom = new UTF8Encoding(false);
                using (StreamReader fileStream = new StreamReader(xmlFilename, utf8WithoutBom))
                {
                    xmlDoc.Load(fileStream);
                }

                XmlNode rootNode = xmlDoc.DocumentElement;
                if (rootNode.Name != "metas")
                    return false;

                XmlNode versionNode = rootNode.Attributes["version"];
                bool bInvalid = false;

                if (versionNode == null) {
                    bInvalid = true;

                } else {
                    float version = float.Parse(versionNode.Value);

                    if (version < 2.0) {
                        bInvalid = true;
                    }
                }

                if (bInvalid) {
                    string msg = string.Format("{0}: \n{1}", Resources.InvalidMeta, xmlFilename);
                    System.Windows.Forms.MessageBox.Show(msg, Resources.LoadError, MessageBoxButtons.OK);
                    return false;
                }

                XmlNode languageNode = rootNode.Attributes["language"];
                Plugin.SourceLanguage = (languageNode != null) ? languageNode.Value : "";

                Stream s = File.Open(csFilename, FileMode.Create);
                StreamWriter wrtr = new StreamWriter(s);

                wrtr.WriteLine("using System;");
                wrtr.WriteLine("using System.Collections;");
                wrtr.WriteLine("using System.Collections.Generic;");
                wrtr.WriteLine("using System.Text;");
                wrtr.WriteLine("using Behaviac.Design;");
                wrtr.WriteLine("using Behaviac.Design.Attributes;");
                wrtr.WriteLine("");

                // types
                XmlNode typesNode = null;
                foreach(XmlNode c in rootNode.ChildNodes) {
                    if (c.Name == "types") {
                        typesNode = c;
                        break;
                    }
                }

                List<XmlNode> typesNodes = new List<XmlNode>();

                if (typesNode != null) {
                    foreach(XmlNode typeNode in typesNode.ChildNodes) {
                        typesNodes.Add(typeNode);
                    }
                }

                if (Workspace.CustomizedTypesXMLNode != null) {
                    foreach(XmlNode typeNode in Workspace.CustomizedTypesXMLNode.ChildNodes) {
                        typesNodes.Add(typeNode);
                    }
                }

                Plugin.AllMetaTypes.Clear();

                if (typesNodes.Count > 0) {
                    wrtr.WriteLine("namespace XMLPluginBehaviac");
                    wrtr.WriteLine("{");

                    enumNodeDict.Clear();
                    structNodeDict.Clear();

                    foreach(XmlNode typeNode in typesNodes) {
                        if (typeNode.Name == "enumtype") {
                            writeEnumNode(typeNode, enumNodeDict, wrtr);

                        } else if (typeNode.Name == "struct") {
                            writeStructNode(typeNode, structNodeDict, wrtr);
                        }
                    }

                    wrtr.WriteLine("}");
                }

                // agents
                XmlNode agentsNode = null;
                foreach(XmlNode c in rootNode.ChildNodes) {
                    if (c.Name == "agents") {
                        agentsNode = c;
                        break;
                    }
                }

                if (agentsNode != null) {
                    wrtr.WriteLine("namespace XMLPluginBehaviac");
                    wrtr.WriteLine("{");

                    foreach(XmlNode agentNode in agentsNode.ChildNodes) {
                        XmlNode classfullnameNode = agentNode.Attributes["classfullname"];
                        string classfullname = (classfullnameNode != null && classfullnameNode.Value.Length > 0) ? classfullnameNode.Value : "";
                        string className = HandleBaseName(classfullname);

                        Plugin.AllMetaTypes.Add(classfullname);

                        XmlAttribute baseNode = agentNode.Attributes["base"];
                        string baseName = (baseNode != null && baseNode.Value.Length > 0) ? baseNode.Value : "Agent";

                        // Then write out this class itself.
                        XmlAttribute inheritedNode = agentNode.Attributes["inherited"];
                        string isinherited = (inheritedNode != null) ? inheritedNode.Value : "false";

                        XmlNode isRefTypeNode = agentNode.Attributes["IsRefType"];
                        string isRefType = (isRefTypeNode != null) ? isRefTypeNode.Value : "false";

                        XmlNode isStaticNode = agentNode.Attributes["IsStatic"];
                        string isStatic = (isStaticNode != null) ? isStaticNode.Value : "false";

                        XmlNode displayNameNode = agentNode.Attributes["DisplayName"];
                        string displayName = (displayNameNode != null && displayNameNode.Value.Length > 0) ? displayNameNode.Value : classfullname;

                        XmlNode descNode = agentNode.Attributes["Desc"];
                        string desc = (descNode != null && descNode.Value.Length > 0) ? descNode.Value : displayName;

                        wrtr.WriteLine("\t[Behaviac.Design.ClassDesc(\"{0}\", \"{1}\", {2}, {3}, {4}, \"{5}\", \"{6}\")]", classfullname, baseName, isinherited, isRefType, isStatic, displayName, desc);
                        wrtr.WriteLine("\tpublic class {0} : {1}", HandleHierarchyName(classfullname), (baseName == "Agent") ? "Behaviac.Design.Agent" : HandleHierarchyName(baseName));
                        wrtr.WriteLine("\t{");
                        {
                            writeMembers(agentNode, className, wrtr, false);
                            wrtr.WriteLine("");
                            writeMethods(agentNode, className, wrtr);
                        }
                        wrtr.WriteLine("\t}\n");
                    }
                    wrtr.WriteLine("}");
                }

                // instances
                XmlNode instancesNode = null;
                foreach(XmlNode c in rootNode.ChildNodes) {
                    if (c.Name == "instances") {
                        instancesNode = c;
                        break;
                    }
                }

                if (instancesNode != null) {
                    foreach(XmlNode instanceNode in instancesNode.ChildNodes) {
                        if (instanceNode.Name == "instance") {
                            XmlAttribute nameNode = instanceNode.Attributes["name"];
                            XmlAttribute classNode = instanceNode.Attributes["class"];
                            XmlAttribute displayNameNode = instanceNode.Attributes["DisplayName"];
                            XmlAttribute descNode = instanceNode.Attributes["Desc"];

                            Plugin.AddInstanceName(nameNode.Value, classNode.Value, displayNameNode.Value, descNode.Value);
                        }
                    }
                }

                wrtr.Close();
                s.Close();

                return true;

            } catch (XmlException ex) {
                string msg = string.Format("{0}:{1}", ex.SourceUri, ex.Message);
                System.Windows.Forms.MessageBox.Show(msg, Resources.InvalidMeta, MessageBoxButtons.OK);
            }

            return false;
        }

        private static string fixTypeName(string typeName) {
            typeName = typeName.Replace("const ", "");
            typeName = typeName.Replace("behaviac_vector", "vector");
            typeName = typeName.Replace("behaviac_list", "list");
            typeName = typeName.Replace("vector<", "List<");
            typeName = typeName.Replace("ubyte", "byte");
            typeName = typeName.Replace("sbyte", "sbyte");
            typeName = typeName.Replace("unsigned long long", "ullong");
            typeName = typeName.Replace("signed long long", "llong");
            typeName = typeName.Replace("long long", "llong");

            //after uchar
            typeName = typeName.Replace("char*", "string");
            typeName = typeName.Replace("unsigned char*", "string");

            ////after char*
            //typeName = typeName.Replace("unsigned char", "byte");
            //typeName = typeName.Replace("signed char", "sbyte");

            ////after char*
            //typeName = typeName.Replace("char", "sbyte");

            typeName = typeName.Replace("unsigned ", "u");
            typeName = typeName.Replace("signed ", "");
            typeName = typeName.Replace("eastl_string", "string");
            typeName = typeName.Replace("std_string", "string");
            typeName = typeName.Replace("Tag_string", "string");
            typeName = typeName.Replace("Tag_wstring", "string");
            typeName = typeName.Replace("behaviac_string", "string");
            typeName = typeName.Replace("behaviac_wstring", "string");
            typeName = typeName.Replace("uchar", "byte");
            typeName = typeName.Replace("*", "");
            typeName = typeName.Replace("&", "");

            //if (typeName == "char")
            //{
            //    typeName = "sbyte";
            //}
            //else
            //{
            //    typeName = typeName.Replace("<char>", "<sbyte>");
            //}

            return Plugin.GetTypeName(typeName);
        }

        private static string HandleBaseName(string fullname) {
            if (!string.IsNullOrEmpty(fullname)) {
                string[] names = fullname.Split(new char[] { ':' }, StringSplitOptions.RemoveEmptyEntries);

                if (names.Length > 0)
                { return names[names.Length - 1]; }

            }

            return string.Empty;
        }

        private static string HandleHierarchyName(string fullname) {
            if (!string.IsNullOrEmpty(fullname)) {
                int pos = fullname.IndexOf("::");

                if (pos != -1) {
                    string longName = fullname.Replace("::", "_");
                    Plugin.NamesInNamespace[longName] = fullname;
                    return longName;
                }
            }

            return fullname;
        }

        private static void writeTypeHandler(string className, StreamWriter wrtr) {
            // Write out the hanlder of this class.
            wrtr.WriteLine("\t[TypeHandler(typeof({0}))]", className);
            wrtr.WriteLine("\tpublic class {0}TypeHandler", className);
            wrtr.WriteLine("\t{");

            wrtr.WriteLine("\t\tpublic static object Create()");
            wrtr.WriteLine("\t\t{");
            wrtr.WriteLine("\t\t\t{0} instance = ({0})DefaultValue(\"\");", className);
            wrtr.WriteLine("\t\t\treturn instance;");
            wrtr.WriteLine("\t\t}");
            wrtr.WriteLine("");

            wrtr.WriteLine("\t\tpublic static bool Parse(object parent, string paramName, string parStr, Plugin.SetValue setter)");
            wrtr.WriteLine("\t\t{");
            wrtr.WriteLine("\t\t\tDefaultObject node = parent as DefaultObject;");
            wrtr.WriteLine("\t\t\t{0} result = ({0})DesignerStruct.ParseStringValue(null, typeof({0}), paramName, parStr, node);", className);
            wrtr.WriteLine("\t\t\tsetter(result);");
            wrtr.WriteLine("\t\t\treturn true;");
            wrtr.WriteLine("\t\t}");
            wrtr.WriteLine("");

            wrtr.WriteLine("\t\tpublic static DesignerProperty CreateDesignerProperty(string category, string name, Type type, float rangeMin, float rangeMax)");
            wrtr.WriteLine("\t\t{");
            wrtr.WriteLine("\t\t\treturn new DesignerStruct(name, name, category, DesignerProperty.DisplayMode.Parameter, 0, DesignerProperty.DesignerFlags.NoFlags);");
            wrtr.WriteLine("\t\t}");
            wrtr.WriteLine("");

            wrtr.WriteLine("\t\tpublic static object DefaultValue(string defaultValue)");
            wrtr.WriteLine("\t\t{");
            wrtr.WriteLine("\t\t\treturn new {0}();", className);
            wrtr.WriteLine("\t\t}");
            wrtr.WriteLine("");

            wrtr.WriteLine("\t\tpublic static Type GetEditorType()");
            wrtr.WriteLine("\t\t{");
            wrtr.WriteLine("\t\t\treturn typeof(DesignerCompositeEditor);");
            wrtr.WriteLine("\t\t}");

            wrtr.WriteLine("\t}");
            wrtr.WriteLine("");
        }

        private static void writeStructNode(XmlNode rootNode, Hashtable nodeDict, StreamWriter wrtr) {
            XmlNode typeNode = rootNode.Attributes["Type"];

            if (typeNode != null && !string.IsNullOrEmpty(typeNode.Value)) {
                string className = HandleHierarchyName(typeNode.Value);

                if (nodeDict.ContainsKey(className))
                    return;

                Plugin.AllMetaTypes.Add(typeNode.Value);

                nodeDict.Add(className, rootNode);

                writeTypeHandler(className, wrtr);

                XmlNode isRefTypeNode = rootNode.Attributes["IsRefType"];
                string isRefType = (isRefTypeNode != null) ? isRefTypeNode.Value : "false";

                // Write out this class itself.
                wrtr.WriteLine("\t[Behaviac.Design.ClassDesc(true, {0})]", isRefType);
                wrtr.WriteLine("\tpublic class {0}", className);
                wrtr.WriteLine("\t{");

                writeMembers(rootNode, className, wrtr, true);

                wrtr.WriteLine("\t}");
                wrtr.WriteLine("");
            }
        }

        private static void writeEnumNode(XmlNode rootNode, Hashtable nodeDict, StreamWriter wrtr) {
            XmlNode typeNode = rootNode.Attributes["Type"];

            if (typeNode != null && rootNode.ChildNodes.Count > 0) {
                XmlNode firstChildNode = rootNode.ChildNodes[0];
                XmlNode firstValueNode = firstChildNode.Attributes["Value"];

                if (firstValueNode == null)
                    return;

                string firstValueName = firstValueNode.Value;

                string enumName = HandleHierarchyName(typeNode.Value);

                if (typeNode.Value == "behaviac::EBTStatus" || nodeDict.ContainsKey(enumName))
                    return;

                Plugin.AllMetaTypes.Add(typeNode.Value);

                nodeDict.Add(enumName, rootNode);

                // Write out the hanlder of this class.
                wrtr.WriteLine("\t[TypeHandler(typeof({0}))]", enumName);
                wrtr.WriteLine("\tpublic class {0}TypeHandler", enumName);
                wrtr.WriteLine("\t{");

                wrtr.WriteLine("\t\tpublic static object Create()");
                wrtr.WriteLine("\t\t{");
                wrtr.WriteLine("\t\t\t{0} instance = ({0})DefaultValue(\"\");", enumName);
                wrtr.WriteLine("\t\t\treturn instance;");
                wrtr.WriteLine("\t\t}");
                wrtr.WriteLine("");

                wrtr.WriteLine("\t\tpublic static bool Parse(object parent, string paramName, string parStr, Plugin.SetValue setter)");
                wrtr.WriteLine("\t\t{");
                wrtr.WriteLine("\t\t\t{0} result = ({0})Enum.Parse(typeof({0}), parStr, true);", enumName);
                wrtr.WriteLine("\t\t\tsetter(result);");
                wrtr.WriteLine("\t\t\treturn true;");
                wrtr.WriteLine("\t\t}");
                wrtr.WriteLine("");

                wrtr.WriteLine("\t\tpublic static DesignerProperty CreateDesignerProperty(string category, string name, Type type, float rangeMin, float rangeMax)");
                wrtr.WriteLine("\t\t{");
                wrtr.WriteLine("\t\t\treturn new DesignerEnum(name, name, category, DesignerProperty.DisplayMode.Parameter, 0, DesignerProperty.DesignerFlags.NoFlags, \"\");");
                wrtr.WriteLine("\t\t}");
                wrtr.WriteLine("");

                wrtr.WriteLine("\t\tpublic static object DefaultValue(string defaultValue)");
                wrtr.WriteLine("\t\t{");
                wrtr.WriteLine("\t\t\tArray values = Enum.GetValues(typeof({0}));", enumName);
                wrtr.WriteLine("\t\t\tforeach (object enumVal in values)");
                wrtr.WriteLine("\t\t\t{");
                wrtr.WriteLine("\t\t\t    string enumValueName = Enum.GetName(typeof({0}), enumVal);", enumName);
                wrtr.WriteLine("\t\t\t    if (enumValueName == defaultValue)");
                wrtr.WriteLine("\t\t\t    {");
                wrtr.WriteLine("\t\t\t        return enumVal;");
                wrtr.WriteLine("\t\t\t    }");
                wrtr.WriteLine("\t\t\t}");
                wrtr.WriteLine("\t\t\treturn {0}.{1};", enumName, firstValueName);
                wrtr.WriteLine("\t\t}");
                wrtr.WriteLine("");

                wrtr.WriteLine("\t\tpublic static Type GetEditorType()");
                wrtr.WriteLine("\t\t{");
                wrtr.WriteLine("\t\t\treturn typeof(DesignerEnumEditor);");
                wrtr.WriteLine("\t\t}");

                wrtr.WriteLine("\t}");
                wrtr.WriteLine("");

                // Write out this enum itself.
                XmlNode displayNameNode = rootNode.Attributes["DisplayName"];
                string displayName = (displayNameNode != null && displayNameNode.Value.Length > 0) ? displayNameNode.Value : enumName;

                XmlNode descNode = rootNode.Attributes["Desc"];
                string desc = (descNode != null && descNode.Value.Length > 0) ? descNode.Value : displayName;

                wrtr.WriteLine("\t[Behaviac.Design.EnumDesc(\"{0}\", \"{1}\", \"{2}\")]", typeNode.Value, displayName, desc);
                wrtr.WriteLine("\tpublic enum {0}", enumName);
                wrtr.WriteLine("\t{");

                foreach(XmlNode childNode in rootNode.ChildNodes) {
                    XmlNode nativeValueNode = childNode.Attributes["NativeValue"];
                    string nativeValueName = (nativeValueNode != null) ? nativeValueNode.Value : "";

                    XmlNode valueNode = childNode.Attributes["Value"];
                    string valueName = (valueNode != null) ? valueNode.Value : "";

                    displayNameNode = childNode.Attributes["DisplayName"];
                    displayName = (displayNameNode != null && displayNameNode.Value.Length > 0) ? displayNameNode.Value : valueName;

                    descNode = childNode.Attributes["Desc"];
                    desc = (descNode != null && descNode.Value.Length > 0) ? descNode.Value : displayName;

                    wrtr.WriteLine("\t\t[Behaviac.Design.EnumMemberDesc(\"{0}\", \"{1}\", \"{2}\")]", nativeValueName, displayName, desc);
                    wrtr.WriteLine("\t\t{0},\n", valueName);
                }

                wrtr.WriteLine("\t}");
                wrtr.WriteLine("");
            }
        }

        private static string getStructAttribute(string displayName, string desc, string memberType, int index, bool isReadOnly) {
            Type type = Plugin.GetTypeFromName(memberType);
            string designerType = string.Empty;

            if (Plugin.IsBooleanType(type))
            { designerType = "DesignerBoolean"; }

            else if (Plugin.IsIntergerType(type))
            { designerType = "DesignerInteger"; }

            else if (Plugin.IsFloatType(type))
            { designerType = "DesignerFloat"; }

            else if (Plugin.IsStringType(type) || Plugin.IsCharType(type))
            { designerType = "DesignerString"; }

            else if (Plugin.IsEnumType(type) || enumNodeDict.Contains(memberType))
            { designerType = "DesignerEnum"; }

            else if (Plugin.IsCustomClassType(type) || structNodeDict.Contains(memberType) || (type == null))
            { designerType = "DesignerStruct"; }

            else
            { Debug.Check(false); }

            string designerFlags = isReadOnly ? "DesignerProperty.DesignerFlags.ReadOnly | DesignerProperty.DesignerFlags.NoSave" : "DesignerProperty.DesignerFlags.NoFlags";

            if (designerType == "DesignerEnum")
            { return string.Format("[{0}(\"{1}\", \"{2}\", \"Property\", DesignerProperty.DisplayMode.NoDisplay, {3}, {4}, \"\")]", designerType, displayName, desc, index, designerFlags); }

            return string.Format("[{0}(\"{1}\", \"{2}\", \"Property\", DesignerProperty.DisplayMode.NoDisplay, {3}, {4})]", designerType, displayName, desc, index, designerFlags);
        }

        private static void writeMembers(XmlNode rootNode, string myClassName, StreamWriter wrtr, bool isStruct) {
            int index = 0;
            foreach(XmlNode childNode in rootNode.ChildNodes) {
                if (childNode.Name == "Member") {
                    XmlNode nameNode = childNode.Attributes["Name"];
                    string memberName = (nameNode != null) ? childNode.Attributes["Name"].Value : "";

                    XmlNode typeNode = childNode.Attributes["Type"];
                    string memberOriginalType = (typeNode != null) ? typeNode.Value : "";
                    string memberType = HandleHierarchyName(memberOriginalType);

                    XmlAttribute readonlyNode = childNode.Attributes["Readonly"];
                    string isReadOnly = (readonlyNode != null) ? readonlyNode.Value.ToLowerInvariant() : "false";

                    XmlAttribute staticNode = childNode.Attributes["Static"];
                    string isStatic = (staticNode != null) ? staticNode.Value.ToLowerInvariant() : "false";

                    XmlNode displayNameNode = childNode.Attributes["DisplayName"];
                    string displayName = (displayNameNode != null && displayNameNode.Value.Length > 0) ? displayNameNode.Value : memberName;

                    XmlNode descNode = childNode.Attributes["Desc"];
                    string desc = (descNode != null && descNode.Value.Length > 0) ? descNode.Value : displayName;

                    string isChangeableType = "false";
                    if (string.IsNullOrEmpty(memberType))
                    {
                        isChangeableType = "true";

                        string memberFullname = myClassName + "::" + memberName;
                        if (extraPropertyDict.ContainsKey(memberFullname))
                        {
                            memberType = extraPropertyDict[memberFullname];
                        }
                        else
                        {
                            memberType = "System.Object";
                        }
                    }

                    memberType = fixTypeName(memberType);

                    if (isStruct) {
                        Type type = Plugin.GetTypeFromName(memberType);
                        string defaultValue = string.Empty;

                        if (type == null || Plugin.IsCustomClassType(type))
                        {
                            defaultValue = string.Format(" = new {0}()", memberType);
                        }
                        else if (Plugin.IsCharType(type))
                        {
                            //defaultValue = "= \'\\0\'";
                            defaultValue = "= \'A\'";
                        }
                        else if (Plugin.IsStringType(type))
                        {
                            defaultValue = string.Format(" = string.Empty");
                        }

                        string staticStr = (isStatic == "true") ? "static " : "";

                        wrtr.WriteLine("\t\t{0}private {1} _{2}{3};", staticStr, memberType, memberName, defaultValue);
                        wrtr.WriteLine("\t\t{0}", getStructAttribute(displayName, desc, memberType, index, isReadOnly != "false"));
                        wrtr.WriteLine("\t\t{0}public {1} {2}", staticStr, memberType, memberName);
                        wrtr.WriteLine("\t\t{");
                        wrtr.WriteLine("\t\t\tget {{ return _{0}; }}", memberName);

                        if (isReadOnly == "false") {
                            wrtr.WriteLine("\t\t\tset {{ _{0} = value; }}", memberName);
                        }

                        wrtr.WriteLine("\t\t}");

                        index++;

                    } else {
                        XmlAttribute classNode = childNode.Attributes["Class"];
                        string className = (classNode != null) ? classNode.Value : myClassName;

                        XmlAttribute publicNode = childNode.Attributes["Public"];
                        string isPublic = (publicNode != null) ? publicNode.Value.ToLowerInvariant() : "false";

                        XmlAttribute propertyNode = childNode.Attributes["Property"];
                        string isProperty = (propertyNode != null) ? propertyNode.Value.ToLowerInvariant() : "false";

                        wrtr.WriteLine("\t\t[Behaviac.Design.MemberDesc(\"{0}\", {1}, {2}, {3}, {4}, {5}, \"{6}\", \"{7}\", \"{8}\")]", className, isChangeableType, isStatic, isPublic, isProperty, isReadOnly, memberOriginalType, displayName, desc);
                        wrtr.WriteLine("\t\tpublic {0} {1};", memberType, memberName);
                    }

                    wrtr.WriteLine("");
                }
            }

            // Add the ToString() method for the struct.
            if (isStruct) {
                wrtr.WriteLine("\t\tpublic override string ToString()");
                wrtr.WriteLine("\t\t{");
                wrtr.WriteLine("\t\t\treturn DesignerStruct.RetrieveExportValue(this, null, null, true, -1);");
                wrtr.WriteLine("\t\t}");
            }
        }

        private static void writeMethods(XmlNode rootNode, string myClassName, StreamWriter wrtr) {
            Dictionary<string, bool> allMethods = new Dictionary<string, bool>();
            foreach(XmlNode childNode in rootNode.ChildNodes) {
                if (childNode.Name == "Method") {
                    XmlNode nameNode = childNode.Attributes["Name"];
                    string methodName = (nameNode != null) ? nameNode.Value : "";

                    Debug.Check(!string.IsNullOrEmpty(methodName));

                    if (!string.IsNullOrEmpty(methodName)) {
                        if (!allMethods.ContainsKey(methodName)) {
                            allMethods[methodName] = true;

                        } else {
                            string errorInfo = string.Format("In class {0}, the method with the name of \"{1}\" is duplicated, so it will be ignored.", myClassName, methodName);
                            System.Windows.Forms.MessageBox.Show(errorInfo, Resources.LoadError, MessageBoxButtons.OK);
                            continue;
                        }
                    }

                    XmlNode returnNode = childNode.Attributes["ReturnType"];
                    string nativeReturnType = (returnNode != null) ? returnNode.Value : "void";
                    string isChangeableType = (returnNode == null || string.IsNullOrEmpty(returnNode.Value)) ? "true" : "false";

                    Method method = null;
                    if (isChangeableType == "true")
                    {
                        string methodFullname = myClassName + "::" + methodName;
                        if (extraMethodDict.ContainsKey(methodFullname))
                        {
                            method = extraMethodDict[methodFullname];
                            nativeReturnType = method.ReturnType;
                        }
                        else
                        {
                            nativeReturnType = "System.Object";
                        }
                    }

                    string returnType = fixTypeName(HandleHierarchyName(nativeReturnType));

                    XmlAttribute classNode = childNode.Attributes["Class"];
                    string className = (classNode != null) ? classNode.Value : myClassName;

                    XmlAttribute staticNode = childNode.Attributes["Static"];
                    string isStatic = (staticNode != null) ? staticNode.Value : "false";

                    XmlAttribute publicNode = childNode.Attributes["Public"];
                    string isPublic = (publicNode != null) ? publicNode.Value : "false";

                    XmlAttribute IsActionMethodOnlyNode = childNode.Attributes["IsActionMethodOnly"];
                    string isActionMethodOnly = (IsActionMethodOnlyNode != null) ? IsActionMethodOnlyNode.Value : "false";

                    XmlNode flagNode = childNode.Attributes["Flag"];
                    string isGetter = "false";
                    string isNamedEvent = "false";

                    if (flagNode != null) {
                        isGetter = "true";

                        if (flagNode.Value == "namedevent") {
                            isNamedEvent = "true";
                        }
                    }

                    XmlNode displayNameNode = childNode.Attributes["DisplayName"];
                    string displayName = (displayNameNode != null && displayNameNode.Value.Length > 0) ? displayNameNode.Value : methodName;

                    XmlNode descNode = childNode.Attributes["Desc"];
                    string desc = (descNode != null && descNode.Value.Length > 0) ? descNode.Value : displayName;

                    Debug.Check(isGetter.ToLower() == "false" || returnType.ToLower() != "void");

                    wrtr.WriteLine("\t\t[Behaviac.Design.MethodDesc(\"{0}\", {1}, {2}, {3}, {4}, {5}, \"{6}\", \"{7}\", \"{8}\")]", className, isChangeableType, isStatic, isPublic, isNamedEvent, isActionMethodOnly, nativeReturnType, displayName, desc);
                    wrtr.WriteLine("\t\tpublic delegate {0} {1}(", returnType, methodName);

                    int paramCount = 0;

                    for (int i = 0; i < childNode.ChildNodes.Count; ++i) {
                        XmlNode paramNode = childNode.ChildNodes[i];

                        if (paramNode.Name == "Param")
                        { paramCount++; }
                    }

                    int paramNum = 0;

                    for (int i = 0; i < childNode.ChildNodes.Count; ++i) {
                        XmlNode paramNode = childNode.ChildNodes[i];

                        if (paramNode.Name == "Param") {
                            paramNum++;

                            string paramName = "param" + i;
                            XmlNode paramDisplayNode = paramNode.Attributes["DisplayName"];
                            string paramDisplayName = (paramDisplayNode != null && paramDisplayNode.Value.Length > 0) ? paramDisplayNode.Value : paramName;

                            XmlNode paramTypeNode = paramNode.Attributes["Type"];
                            string paramNativeType = (paramTypeNode != null) ? paramTypeNode.Value : "";
                            string paramType = HandleHierarchyName(paramNativeType);
                            paramType = fixTypeName(paramType);

                            XmlNode paramDescNode = paramNode.Attributes["Desc"];
                            string paramDesc = (paramDescNode != null && paramDescNode.Value.Length > 0) ? paramDescNode.Value : paramName;

                            XmlNode defaultNode = paramNode.Attributes["Default"];
                            string defaultValue = (defaultNode != null && defaultNode.Value.Length > 0) ? defaultNode.Value : "";

                            XmlNode isOutNode = paramNode.Attributes["IsOut"];
                            string isOut = (isOutNode != null && isOutNode.Value == "true") ? "true" : "false";

                            XmlNode isRefNode = paramNode.Attributes["IsRef"];
                            string isRef = (isRefNode != null && isRefNode.Value == "true") ? "true" : "false";

                            XmlNode paramRangeMinNode = paramNode.Attributes["RangeMin"];
                            string paramRangeMin = (paramRangeMinNode != null && paramRangeMinNode.Value.Length > 0) ? paramRangeMinNode.Value : null;

                            XmlNode paramRangeMaxNode = paramNode.Attributes["RangeMax"];
                            string paramRangeMax = (paramRangeMaxNode != null && paramRangeMaxNode.Value.Length > 0) ? paramRangeMaxNode.Value : null;

                            if (isChangeableType == "true")
                            {
                                if (method != null)
                                {
                                    paramName = method.Params[i].Name;
                                    paramType = method.Params[i].Type;
                                    isOut = method.Params[i].IsOut;
                                    isRef = method.Params[i].IsRef;
                                }
                                else
                                {
                                    paramType = "System.Object";
                                }

                                paramNativeType = paramType;
                                paramType = fixTypeName(paramType);
                            }

                            if (string.IsNullOrEmpty(paramRangeMin) && string.IsNullOrEmpty(paramRangeMax)) {
                                wrtr.WriteLine("\t\t\t[Behaviac.Design.ParamDesc(\"{0}\", \"{1}\", \"{2}\", \"{3}\", {4}, {5})]", paramNativeType, paramDisplayName, paramDesc, defaultValue, isOut, isRef);

                            } else {
                                if (string.IsNullOrEmpty(paramRangeMin))
                                { paramRangeMin = float.MinValue.ToString(); }

                                if (string.IsNullOrEmpty(paramRangeMax))
                                { paramRangeMax = float.MaxValue.ToString(); }

                                wrtr.WriteLine("\t\t\t[Behaviac.Design.ParamDesc(\"{0}\", \"{1}\", \"{2}\", \"{3}\", {4}, {5}, {6}f, {7}f)]", paramNativeType, paramDisplayName, paramDesc, defaultValue, isOut, isRef, paramRangeMin, paramRangeMax);
                            }

                            wrtr.Write("\t\t\t{0} {1}", paramType, paramName);

                            if (paramNum < paramCount)
                            { wrtr.WriteLine(","); }

                        } else if (paramNode.Name == "ReturnType") {
                        }
                    }

                    wrtr.WriteLine("\n\t\t\t);\n");
                }
            }
        }
    }
}
