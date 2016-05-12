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
using System.Collections.Generic;
using System.Text;
using System.IO;
using Behaviac.Design;
using Behaviac.Design.Nodes;
using Behaviac.Design.Attributes;
using Behaviac.Design.Attachments;
using PluginBehaviac.Properties;
using PluginBehaviac.DataExporters;
using PluginBehaviac.NodeExporters;

namespace PluginBehaviac.Exporters
{
    public class ExporterCs : Behaviac.Design.Exporters.Exporter
    {
        public ExporterCs(BehaviorNode node, string outputFolder, string filename, List<string> includedFilenames = null)
            : base(node, outputFolder, filename + ".cs", includedFilenames)
        {
            _outputFolder = Path.Combine(Path.GetFullPath(_outputFolder), "behaviac_generated");
        }

        public override Behaviac.Design.FileManagers.SaveResult Export(List<BehaviorNode> behaviors, bool exportUnifiedFile, bool generateCustomizedTypes)
        {
            string behaviorFilename = "behaviors/generated_behaviors.cs";
            string agentFolder = string.Empty;
            Behaviac.Design.FileManagers.SaveResult result = VerifyFilename(ref behaviorFilename, ref agentFolder);
            if (Behaviac.Design.FileManagers.SaveResult.Succeeded == result)
            {
                string behaviorFolder = Path.GetDirectoryName(behaviorFilename);
                clearFolder(behaviorFolder);
                //clearFolder(agentFolder);

                ExportBehaviors(behaviors, behaviorFilename, exportUnifiedFile);

                ExportCustomizedMembers(agentFolder);

                if (generateCustomizedTypes)
                {
                    ExportAgents(agentFolder);

                    ExportCustomizedTypes(agentFolder);
                }
            }

            return result;
        }

        private void clearFolder(string folder)
        {
            DirectoryInfo dirInfo = new DirectoryInfo(folder);
            if (dirInfo.Exists)
            {
                foreach (FileInfo file in dirInfo.GetFiles())
                {
                    try
                    {
                        File.SetAttributes(file.FullName, FileAttributes.Normal);
                        if (file.Extension == ".cs")
                            file.Delete();
                    }
                    catch
                    {
                    }
                }
            }
        }

        private void ExportBehaviors(List<BehaviorNode> behaviors, string filename, bool exportUnifiedFile)
        {
            using (StreamWriter file = new StreamWriter(filename))
            {
                ExportHead(file, filename);

                ExportMemberDeclaration(file);

                if (exportUnifiedFile)
                {
                    foreach (BehaviorNode behavior in behaviors)
                    {
                        ExportBody(file, behavior);
                    }
                }
                else
                {
                    foreach (BehaviorNode behavior in behaviors)
                    {
                        string behaviorFilename = behavior.RelativePath;
                        behaviorFilename = behaviorFilename.Replace("\\", "/");
                        behaviorFilename = Path.ChangeExtension(behaviorFilename, "cs");
                        behaviorFilename = Path.Combine("behaviors", behaviorFilename);

                        string agentFolder = string.Empty;

                        Behaviac.Design.FileManagers.SaveResult result = VerifyFilename(ref behaviorFilename, ref agentFolder);
                        if (Behaviac.Design.FileManagers.SaveResult.Succeeded == result)
                        {
                            using (StreamWriter behaviorFile = new StreamWriter(behaviorFilename))
                            {
                                ExportHead(behaviorFile, behaviorFilename);

                                ExportBody(behaviorFile, behavior);

                                ExportTail(behaviorFile);

                                behaviorFile.Close();
                            }
                        }
                    }
                }

                ExportTail(file);

                file.Close();
            }
        }

        private Behaviac.Design.FileManagers.SaveResult VerifyFilename(ref string behaviorFilename, ref string agentFolder)
        {
            behaviorFilename = Path.Combine(_outputFolder, behaviorFilename);
            agentFolder = Path.Combine(_outputFolder, "types");

            // get the abolute folder of the file we want to export
            string folder = Path.GetDirectoryName(behaviorFilename);
            if (!Directory.Exists(folder))
                Directory.CreateDirectory(folder);

            if (!Directory.Exists(agentFolder))
                Directory.CreateDirectory(agentFolder);

            // verify it can be writable
            Behaviac.Design.FileManagers.SaveResult result = Behaviac.Design.FileManagers.FileManager.MakeWritable(behaviorFilename, Resources.ExportFileWarning);
            return result;
        }

        private void ExportHead(StreamWriter file, string exportFilename)
        {
            string wsfolder = Path.GetDirectoryName(Workspace.Current.FileName);
            exportFilename = Behaviac.Design.FileManagers.FileManager.MakeRelative(wsfolder, exportFilename);
            exportFilename = exportFilename.Replace("\\", "/");

            // write comments
            file.WriteLine("// ---------------------------------------------------------------------");
            file.WriteLine("// This file is auto-generated by behaviac designer, so please don't modify it by yourself!");
            file.WriteLine("// Export file: {0}", exportFilename);
            file.WriteLine("// ---------------------------------------------------------------------\r\n");

            // write the namespaces for the game agents
            file.WriteLine("using System;");
            file.WriteLine("using System.Collections;");
            file.WriteLine("using System.Collections.Generic;");
            file.WriteLine("using System.Reflection;\r\n");

            // write namespace
            file.WriteLine("namespace behaviac\r\n{");
        }

        private void ExportMemberDeclaration(StreamWriter file)
        {
            // write the AgentExtra_Generated class
            file.WriteLine("\tclass AgentExtra_Generated");
            file.WriteLine("\t{");
            file.WriteLine("\t\tprivate static Dictionary<string, FieldInfo> _fields = new Dictionary<string, FieldInfo>();");
            file.WriteLine("\t\tprivate static Dictionary<string, PropertyInfo> _properties = new Dictionary<string, PropertyInfo>();");
            file.WriteLine("\t\tprivate static Dictionary<string, MethodInfo> _methods = new Dictionary<string, MethodInfo>();");
            file.WriteLine();
            file.WriteLine("\t\tpublic static object GetProperty(behaviac.Agent agent, string property)");
            file.WriteLine("\t\t{");
            file.WriteLine("\t\t\tType type = agent.GetType();");
            file.WriteLine("\t\t\tstring propertyName = type.FullName + property;");
            file.WriteLine("\t\t\tif (_fields.ContainsKey(propertyName))");
            file.WriteLine("\t\t\t{");
            file.WriteLine("\t\t\t\treturn _fields[propertyName].GetValue(agent);");
            file.WriteLine("\t\t\t}");
            file.WriteLine();
            file.WriteLine("\t\t\tif (_properties.ContainsKey(propertyName))");
            file.WriteLine("\t\t\t{");
            file.WriteLine("\t\t\t\treturn _properties[propertyName].GetValue(agent, null);");
            file.WriteLine("\t\t\t}");
            file.WriteLine();
            file.WriteLine("\t\t\twhile (type != typeof(object))");
            file.WriteLine("\t\t\t{");
            file.WriteLine("\t\t\t\tFieldInfo field = type.GetField(property, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Static);");
            file.WriteLine("\t\t\t\tif (field != null)");
            file.WriteLine("\t\t\t\t{");
            file.WriteLine("\t\t\t\t\t_fields[propertyName] = field;");
            file.WriteLine("\t\t\t\t\treturn field.GetValue(agent);");
            file.WriteLine("\t\t\t\t}");
            file.WriteLine();
            file.WriteLine("\t\t\t\tPropertyInfo prop = type.GetProperty(property, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Static);");
            file.WriteLine("\t\t\t\tif (prop != null)");
            file.WriteLine("\t\t\t\t{");
            file.WriteLine("\t\t\t\t\t_properties[propertyName] = prop;");
            file.WriteLine("\t\t\t\t\treturn prop.GetValue(agent, null);");
            file.WriteLine("\t\t\t\t}");
            file.WriteLine();
            file.WriteLine("\t\t\t\ttype = type.BaseType;");
            file.WriteLine("\t\t\t}");
            file.WriteLine("\t\t\tDebug.Check(false, \"No property can be found!\");");
            file.WriteLine("\t\t\treturn null;");
            file.WriteLine("\t\t}\r\n");

            file.WriteLine("\t\tpublic static void SetProperty(behaviac.Agent agent, string property, object value)");
            file.WriteLine("\t\t{");
            file.WriteLine("\t\t\tType type = agent.GetType();");
            file.WriteLine("\t\t\tstring propertyName = type.FullName + property;");
            file.WriteLine("\t\t\tif (_fields.ContainsKey(propertyName))");
            file.WriteLine("\t\t\t{");
            file.WriteLine("\t\t\t\t_fields[propertyName].SetValue(agent, value);");
            file.WriteLine("\t\t\t\treturn;");
            file.WriteLine("\t\t\t}");
            file.WriteLine();
            file.WriteLine("\t\t\tif (_properties.ContainsKey(propertyName))");
            file.WriteLine("\t\t\t{");
            file.WriteLine("\t\t\t\t_properties[propertyName].SetValue(agent, value, null);");
            file.WriteLine("\t\t\t\treturn;");
            file.WriteLine("\t\t\t}");
            file.WriteLine();
            file.WriteLine("\t\t\twhile (type != typeof(object))");
            file.WriteLine("\t\t\t{");
            file.WriteLine("\t\t\t\tFieldInfo field = type.GetField(property, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Static);");
            file.WriteLine("\t\t\t\tif (field != null)");
            file.WriteLine("\t\t\t\t{");
            file.WriteLine("\t\t\t\t\t_fields[propertyName] = field;");
            file.WriteLine("\t\t\t\t\tfield.SetValue(agent, value);");
            file.WriteLine("\t\t\t\t\treturn;");
            file.WriteLine("\t\t\t\t}");
            file.WriteLine();
            file.WriteLine("\t\t\t\tPropertyInfo prop = type.GetProperty(property, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Static);");
            file.WriteLine("\t\t\t\tif (prop != null)");
            file.WriteLine("\t\t\t\t{");
            file.WriteLine("\t\t\t\t\t_properties[propertyName] = prop;");
            file.WriteLine("\t\t\t\t\tprop.SetValue(agent, value, null);");
            file.WriteLine("\t\t\t\t\treturn;");
            file.WriteLine("\t\t\t\t}");
            file.WriteLine();
            file.WriteLine("\t\t\t\ttype = type.BaseType;");
            file.WriteLine("\t\t\t}");
            file.WriteLine("\t\t\tDebug.Check(false, \"No property can be found!\");");
            file.WriteLine("\t\t}\r\n");

            file.WriteLine("\t\tpublic static object ExecuteMethod(behaviac.Agent agent, string method, object[] args)");
            file.WriteLine("\t\t{");
            file.WriteLine("\t\t\tType type = agent.GetType();");
            file.WriteLine("\t\t\tstring methodName = type.FullName + method;");
            file.WriteLine("\t\t\tif (_methods.ContainsKey(methodName))");
            file.WriteLine("\t\t\t{");
            file.WriteLine("\t\t\t\treturn _methods[methodName].Invoke(agent, args);;");
            file.WriteLine("\t\t\t}");
            file.WriteLine();
            file.WriteLine("\t\t\twhile (type != typeof(object))");
            file.WriteLine("\t\t\t{");
            file.WriteLine("\t\t\t\tMethodInfo m = type.GetMethod(method, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Static);");
            file.WriteLine("\t\t\t\tif (m != null)");
            file.WriteLine("\t\t\t\t{");
            file.WriteLine("\t\t\t\t\t_methods[methodName] = m;");
            file.WriteLine("\t\t\t\t\treturn m.Invoke(agent, args);");
            file.WriteLine("\t\t\t\t}");
            file.WriteLine();
            file.WriteLine("\t\t\t\ttype = type.BaseType;");
            file.WriteLine("\t\t\t}");
            file.WriteLine("\t\t\tDebug.Check(false, \"No method can be found!\");");
            file.WriteLine("\t\t\treturn null;");
            file.WriteLine("\t\t}");
            file.WriteLine("\t}\r\n");
        }

        private string getValidFilename(string filename)
        {
            filename = filename.Replace('/', '_');
            filename = filename.Replace('-', '_');

            return filename;
        }

        private void ExportBody(StreamWriter file, BehaviorNode behavior)
        {
            behavior.PreExport();

            string filename = Path.ChangeExtension(behavior.RelativePath, "").Replace(".", "");
            filename = filename.Replace('\\', '/');

            // write comments
            file.WriteLine("\t// Source file: {0}\r\n", filename);

            string btClassName = string.Format("bt_{0}", getValidFilename(filename));
            string agentType = behavior.AgentType.AgentTypeName;

            // create the class definition of its attachments
            ExportAttachmentClass(file, btClassName, (Node)behavior);

            // create the class definition of its children
            foreach (Node child in ((Node)behavior).GetChildNodes())
                ExportNodeClass(file, btClassName, agentType, behavior, child);

            // create the bt class
            file.WriteLine("\tpublic static class {0}\r\n\t{{", btClassName);

            // export the build function
            file.WriteLine("\t\tpublic static bool build_behavior_tree(BehaviorTree bt)\r\n\t\t{");
            file.WriteLine("\t\t\tbt.SetClassNameString(\"BehaviorTree\");");
            file.WriteLine("\t\t\tbt.SetId(-1);");
            file.WriteLine("\t\t\tbt.SetName(\"{0}\");", filename);
            file.WriteLine("\t\t\tbt.IsFSM = {0};", ((Node)behavior).IsFSM ? "true" : "false");
            file.WriteLine("#if !BEHAVIAC_RELEASE");
            file.WriteLine("\t\t\tbt.SetAgentType(\"{0}\");", agentType.Replace("::", "."));
            file.WriteLine("#endif");
            if (!string.IsNullOrEmpty(((Behavior)behavior).Domains))
            {
                file.WriteLine("\t\t\tbt.SetDomains(\"{0}\");", ((Behavior)behavior).Domains);
            }
            if (((Behavior)behavior).DescriptorRefs.Count > 0)
            {
                file.WriteLine("\t\t\tbt.SetDescriptors(\"{0}\");", DesignerPropertyUtility.RetrieveExportValue(((Behavior)behavior).DescriptorRefs));
            }

            ExportPars(file, agentType, "bt", (Node)behavior, "\t\t");

            // export its attachments
            ExportAttachment(file, btClassName, agentType, "bt", (Node)behavior, "\t\t\t");

            file.WriteLine("\t\t\t// children");

            // export its children
            if (((Node)behavior).IsFSM)
            {
                file.WriteLine("\t\t\t{");
                file.WriteLine("\t\t\t\tFSM fsm = new FSM();");
                file.WriteLine("\t\t\t\tfsm.SetClassNameString(\"FSM\");");
                file.WriteLine("\t\t\t\tfsm.SetId(-1);");
                file.WriteLine("\t\t\t\tfsm.InitialId = {0};", behavior.InitialStateId);
                file.WriteLine("#if !BEHAVIAC_RELEASE");
                file.WriteLine("\t\t\t\tfsm.SetAgentType(\"{0}\");", agentType.Replace("::", "."));
                file.WriteLine("#endif");

                foreach (Node child in ((Node)behavior).FSMNodes)
                {
                    ExportNode(file, btClassName, agentType, "fsm", child, 4);
                }

                file.WriteLine("\t\t\t\tbt.AddChild(fsm);");
                file.WriteLine("\t\t\t}");
            }
            else
            {
                foreach (Node child in ((Node)behavior).GetChildNodes())
                {
                    ExportNode(file, btClassName, agentType, "bt", child, 3);
                }
            }

            file.WriteLine("\t\t\treturn true;");

            // close the build function
            file.WriteLine("\t\t}");

            // close class
            file.WriteLine("\t}\r\n");

            behavior.PostExport();
        }

        private void ExportTail(StreamWriter file)
        {
            // close namespace
            file.WriteLine("}");
        }

        private void ExportPars(StreamWriter file, string agentType, string nodeName, Node node, string indent)
        {
            if (node is Behavior)
            {
                ExportPars(file, agentType, nodeName, ((Behavior)node).LocalVars, indent);
            }
        }

        private void ExportPars(StreamWriter file, string agentType, string nodeName, List<Behaviac.Design.ParInfo> pars, string indent)
        {
            if (pars.Count > 0)
            {
                file.WriteLine("{0}\t// pars", indent);
                for (int i = 0; i < pars.Count; ++i)
                {
                    string name = pars[i].BasicName;
                    string type = DataCsExporter.GetGeneratedParType(pars[i].Type);
                    string value = pars[i].DefaultValue.Replace("\"", "\\\"");

                    file.WriteLine("{0}\t{1}.AddPar(\"{2}\", \"{3}\", \"{4}\", \"{5}\");", indent, nodeName, agentType, type, name, value);
                }
            }
        }

        private void ExportAttachmentClass(StreamWriter file, string btClassName, Node node)
        {
            foreach (Behaviac.Design.Attachments.Attachment attach in node.Attachments)
            {
                if (!attach.Enable)
                    continue;

                string nodeName = string.Format("attach{0}", attach.Id);

                AttachmentCsExporter attachmentExporter = AttachmentCsExporter.CreateInstance(attach);
                attachmentExporter.GenerateClass(attach, file, "", nodeName, btClassName);
            }
        }

        private void ExportAttachment(StreamWriter file, string btClassName, string agentType, string parentName, Node node, string indent)
        {
            if (node.Attachments.Count > 0)
            {
                file.WriteLine("{0}// attachments", indent);
                foreach (Behaviac.Design.Attachments.Attachment attach in node.Attachments)
                {
                    if (!attach.Enable || attach.IsStartCondition)
                        continue;

                    file.WriteLine("{0}{{", indent);

                    string nodeName = string.Format("attach{0}", attach.Id);

                    // export its instance and the properties
                    AttachmentCsExporter attachmentExporter = AttachmentCsExporter.CreateInstance(attach);
                    attachmentExporter.GenerateInstance(attach, file, indent, nodeName, agentType, btClassName);

                    string isPrecondition = attach.IsPrecondition && !attach.IsTransition ? "true" : "false";
                    string isEffector = attach.IsEffector && !attach.IsTransition ? "true" : "false";
                    string isTransition = attach.IsTransition ? "true" : "false";
                    file.WriteLine("{0}\t{1}.Attach({2}, {3}, {4}, {5});", indent, parentName, nodeName, isPrecondition, isEffector, isTransition);

                    if (attach is Behaviac.Design.Attachments.Event)
                    {
                        file.WriteLine("{0}\t{1}.SetHasEvents({1}.HasEvents() | ({2} is Event));", indent, parentName, nodeName);
                    }

                    file.WriteLine("{0}}}", indent);
                }
            }
        }

        private void ExportNodeClass(StreamWriter file, string btClassName, string agentType, BehaviorNode behavior, Node node)
        {
            if (!node.Enable)
                return;

            string nodeName = string.Format("node{0}", node.Id);

            NodeCsExporter nodeExporter = NodeCsExporter.CreateInstance(node);
            nodeExporter.GenerateClass(node, file, "", nodeName, agentType, btClassName);

            ExportAttachmentClass(file, btClassName, node);

            if (!(node is ReferencedBehavior))
            {
                foreach (Node child in node.GetChildNodes())
                {
                    ExportNodeClass(file, btClassName, agentType, behavior, child);
                }
            }
        }

        private void ExportNode(StreamWriter file, string btClassName, string agentType, string parentName, Node node, int indentDepth)
        {
            if (!node.Enable)
                return;

            // generate the indent string
            string indent = string.Empty;
            for (int i = 0; i < indentDepth; ++i)
            {
                indent += '\t';
            }

            string nodeName = string.Format("node{0}", node.Id);

            // open some brackets for a better formatting in the generated code
            file.WriteLine("{0}{{", indent);

            // export its instance and the properties
            NodeCsExporter nodeExporter = NodeCsExporter.CreateInstance(node);
            nodeExporter.GenerateInstance(node, file, indent, nodeName, agentType, btClassName);

            ExportPars(file, agentType, nodeName, node, indent);

            ExportAttachment(file, btClassName, agentType, nodeName, node, indent + "\t");

            bool isAsChild = true;
            if (node.Parent != null)
            {
                BaseNode.Connector connector = node.Parent.GetConnector(node);
                if (connector != null && !connector.IsAsChild)
                {
                    isAsChild = false;
                }
            }

            if (isAsChild)
            {
                // add the node to its parent
                file.WriteLine("{0}\t{1}.AddChild({2});", indent, parentName, nodeName);
            }
            else
            {
                // add the node as its customized children
                file.WriteLine("{0}\t{1}.SetCustomCondition({2});", indent, parentName, nodeName);
            }

            // export the child nodes
            if (!node.IsFSM && !(node is ReferencedBehavior))
            {
                foreach (Node child in node.GetChildNodes())
                {
                    ExportNode(file, btClassName, agentType, nodeName, child, indentDepth + 1);
                }
            }

            file.WriteLine("{0}\t{1}.SetHasEvents({1}.HasEvents() | {2}.HasEvents());", indent, parentName, nodeName);

            // close the brackets for a better formatting in the generated code
            file.WriteLine("{0}}}", indent);
        }

        private void ExportAgents(string agentFolder)
        {
            foreach (AgentType agent in Plugin.AgentTypes)
            {
                if (!agent.IsCustomized)
                    continue;

                IList<PropertyDef> properties = agent.GetProperties();
                bool hasCustomizedProperty = false;
                foreach (PropertyDef prop in properties)
                {
                    if (prop.IsCustomized && !prop.IsPar && !prop.IsArrayElement)
                    {
                        hasCustomizedProperty = true;
                        break;
                    }
                }

                IList<MethodDef> methods = agent.GetMethods();
                bool hasCustomizedMethod = false;
                foreach (MethodDef method in methods)
                {
                    if (method.IsCustomized && !method.IsNamedEvent)
                    {
                        hasCustomizedMethod = true;
                        break;
                    }
                }

                //if (hasCustomizedProperty || hasCustomizedMethod)
                {
                    string filename = Path.Combine(agentFolder, agent.BasicClassName + ".cs");
                    Encoding utf8WithBom = new UTF8Encoding(true);

                    using (StreamWriter file = new StreamWriter(filename, false, utf8WithBom))
                    {
                        // write comments
                        file.WriteLine("// ---------------------------------------------------------------------");
                        file.WriteLine("// This agent file is auto-generated by behaviac designer, but you should");
                        file.WriteLine("// implement the methods of the agent class if necessary!");
                        file.WriteLine("// ---------------------------------------------------------------------\r\n");

                        file.WriteLine("using System.Collections.Generic;");
                        file.WriteLine();

                        string indent = "";
                        if (!string.IsNullOrEmpty(agent.Namespace))
                        {
                            indent = "\t";

                            file.WriteLine("namespace {0}", agent.Namespace);
                            file.WriteLine("{");
                        }

                        string agentDisplayName = string.IsNullOrEmpty(agent.DisplayName) ? agent.BasicClassName : agent.DisplayName;
                        string agentDescription = string.IsNullOrEmpty(agent.Description) ? "" : agent.Description;
                        file.WriteLine("{0}[behaviac.TypeMetaInfo(\"{1}\", \"{2}\")]", indent, agentDisplayName, agentDescription);
                        file.WriteLine("{0}public class {1} : {2}", indent, agent.BasicClassName, agent.Base.AgentTypeName.Replace("::", "."));
                        file.WriteLine("{0}{{", indent);

                        if (hasCustomizedProperty)
                        {
                            file.WriteLine("{0}\t// properties", indent);
                            file.WriteLine();

                            foreach (PropertyDef prop in properties)
                            {
                                if (prop.IsCustomized && !prop.IsPar && !prop.IsArrayElement)
                                {
                                    string publicStr = prop.IsPublic ? "public " : "private ";
                                    string staticStr = prop.IsStatic ? "static " : "";
                                    string propType = DataCsExporter.GetGeneratedNativeType(prop.Type);
                                    string defaultValue = DataCsExporter.GetGeneratedPropertyDefaultValue(prop, propType);
                                    if (defaultValue != null)
                                        defaultValue = " = " + defaultValue;

                                    file.WriteLine("{0}\t[behaviac.MemberMetaInfo(\"{1}\", \"{2}\")]", indent, prop.DisplayName, prop.BasicDescription);
                                    file.WriteLine("{0}\t{1}{2}{3} {4}{5};", indent, publicStr, staticStr, propType, prop.BasicName, defaultValue);
                                    file.WriteLine();
                                }
                            }
                        }

                        if (hasCustomizedMethod)
                        {
                            file.WriteLine("{0}\t// methods", indent);
                            file.WriteLine();

                            foreach (MethodDef method in methods)
                            {
                                if (method.IsCustomized && !method.IsNamedEvent)
                                {
                                    string publicStr = method.IsPublic ? "public " : "private ";
                                    string staticStr = method.IsStatic ? "static " : "";

                                    string allParams = "";
                                    foreach (MethodDef.Param param in method.Params)
                                    {
                                        if (!string.IsNullOrEmpty(allParams))
                                            allParams += ", ";

                                        allParams += DataCsExporter.GetGeneratedNativeType(param.NativeType) + " " + param.Name;
                                    }

                                    string returnValue = DataCsExporter.GetGeneratedDefaultValue(method.ReturnType, method.NativeReturnType);

                                    file.WriteLine("{0}\t[behaviac.MethodMetaInfo(\"{1}\", \"{2}\")]", indent, method.DisplayName, method.BasicDescription);
                                    file.WriteLine("{0}\t{1}{2}{3} {4}({5})", indent, publicStr, staticStr, DataCsExporter.GetGeneratedNativeType(method.ReturnType), method.BasicName, allParams);
                                    file.WriteLine("{0}\t{{", indent);
                                    file.WriteLine("{0}\t\t// Write your logic codes here.", indent);
                                    if (returnValue != null)
                                    {
                                        file.WriteLine();
                                        file.WriteLine("{0}\t\treturn {1};", indent, returnValue);
                                    }
                                    file.WriteLine("{0}\t}}", indent);
                                    file.WriteLine();
                                }
                            }
                        }

                        //end of class
                        file.WriteLine("{0}}}", indent);

                        if (!string.IsNullOrEmpty(agent.Namespace))
                        {
                            //end of namespace
                            file.WriteLine("}");
                        }

                        file.Close();
                    }
                }
            }
        }

        private void ExportCustomizedTypes(string agentFolder)
        {
            if (CustomizedTypeManager.Instance.Enums.Count > 0 || CustomizedTypeManager.Instance.Structs.Count > 0)
            {
                string filename = Path.Combine(agentFolder, "customizedtypes.cs");
                Encoding utf8WithBom = new UTF8Encoding(true);

                using (StreamWriter file = new StreamWriter(filename, false, utf8WithBom))
                {
                    // write comments
                    file.WriteLine("// ---------------------------------------------------------------------");
                    file.WriteLine("// This file is auto-generated by behaviac designer, so please don't modify it by yourself!");
                    file.WriteLine("// ---------------------------------------------------------------------\n");

                    file.WriteLine("using System.Collections;");
                    file.WriteLine("using System.Collections.Generic;");
                    file.WriteLine();

                    //file.WriteLine("namespace behaviac");
                    //file.WriteLine("{");

                    file.WriteLine("// -------------------");
                    file.WriteLine("// Customized enums");
                    file.WriteLine("// -------------------\n");

                    for (int e = 0; e < CustomizedTypeManager.Instance.Enums.Count; ++e)
                    {
                        if (e > 0)
                            file.WriteLine();

                        CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[e];
                        file.WriteLine("[behaviac.TypeMetaInfo(\"{0}\", \"{1}\")]", customizedEnum.DisplayName, customizedEnum.Description);

                        file.WriteLine("public enum {0}", customizedEnum.Name);
                        file.WriteLine("{");

                        for (int m = 0; m < customizedEnum.Members.Count; ++m)
                        {
                            if (m > 0)
                                file.WriteLine();

                            CustomizedEnum.CustomizedEnumMember member = customizedEnum.Members[m];
                            if (member.DisplayName != member.Name || !string.IsNullOrEmpty(member.Description))
                                file.WriteLine("\t[behaviac.MemberMetaInfo(\"{0}\", \"{1}\")]", member.DisplayName, member.Description);

                            if (member.Value >= 0)
                                file.WriteLine("\t{0} = {1},", member.Name, member.Value);
                            else
                                file.WriteLine("\t{0},", member.Name);
                        }

                        file.WriteLine("}");
                    }

                    if (CustomizedTypeManager.Instance.Enums.Count > 0)
                        file.WriteLine();

                    file.WriteLine("// -------------------");
                    file.WriteLine("// Customized structs");
                    file.WriteLine("// -------------------\n");

                    for (int s = 0; s < CustomizedTypeManager.Instance.Structs.Count; s++)
                    {
                        if (s > 0)
                            file.WriteLine();

                        CustomizedStruct customizedStruct = CustomizedTypeManager.Instance.Structs[s];
                        file.WriteLine("[behaviac.TypeMetaInfo(\"{0}\", \"{1}\")]", customizedStruct.DisplayName, customizedStruct.Description);

                        file.WriteLine("public struct {0}", customizedStruct.Name);
                        file.WriteLine("{");

                        for (int m = 0; m < customizedStruct.Properties.Count; ++m)
                        {
                            if (m > 0)
                                file.WriteLine();

                            PropertyDef member = customizedStruct.Properties[m];
                            if (member.DisplayName != member.Name || !string.IsNullOrEmpty(member.BasicDescription))
                                file.WriteLine("\t[behaviac.MemberMetaInfo(\"{0}\", \"{1}\")]", member.DisplayName, member.BasicDescription);

                            file.WriteLine("\tpublic {0} {1};", DataCsExporter.GetGeneratedNativeType(member.NativeType), member.BasicName);
                        }

                        file.WriteLine("}");
                    }

                    //file.WriteLine("}");
                }
            }
        }

        private void ExportCustomizedMembers(string agentFolder)
        {
            bool hasNonParProperty = false;
            foreach (AgentType agent in Plugin.AgentTypes)
            {
                if (agent.IsCustomized)
                    continue;

                foreach (PropertyDef prop in agent.GetProperties())
                {
                    if (!prop.IsPar && !prop.IsArrayElement)
                    {
                        hasNonParProperty = true;
                        break;
                    }
                }

                if (hasNonParProperty)
                    break;
            }

            bool hasEvents = false;
            foreach (AgentType agent in Plugin.AgentTypes)
            {
                if (agent.IsCustomized)
                    continue;

                IList<MethodDef> methods = agent.GetMethods();

                foreach (MethodDef method in methods)
                {
                    if (method.IsNamedEvent)
                    {
                        hasEvents = true;
                        break;
                    }
                }

                if (hasEvents)
                    break;
            }

            if (hasNonParProperty || hasEvents)
            {
                string filename = Path.Combine(agentFolder, "AgentProperties.cs");
                Encoding utf8WithBom = new UTF8Encoding(true);

                using (StreamWriter file = new StreamWriter(filename, false, utf8WithBom))
                {
                    // write comments
                    file.WriteLine("// ---------------------------------------------------------------------");
                    file.WriteLine("// This file is auto-generated by behaviac designer, so please don't modify it by yourself!");
                    file.WriteLine("// ---------------------------------------------------------------------\n");

                    //file.WriteLine("using System.Collections.Generic;");
                    //file.WriteLine();

                    file.WriteLine("namespace behaviac");
                    file.WriteLine("{");

                    file.WriteLine("\tpartial class AgentProperties");
                    file.WriteLine("\t{");

                    // load_cs
                    file.WriteLine("\t\tstatic partial void load_cs()");
                    file.WriteLine("\t\t{");

                    ExportProperties(file);

                    ExportMethods(file);

                    file.WriteLine("\t\t}\n");

                    // RegisterTypes_
                    file.WriteLine("\t\tstatic partial void RegisterTypes_()");
                    file.WriteLine("\t\t{");

                    foreach (string type in Plugin.AllMetaTypes)
                    {
                        AgentType agentType = Plugin.GetAgentType(type);
                        bool isStatic = (agentType != null) ? agentType.IsStatic : false;

                        if (!isStatic)
                        {
                            file.WriteLine("\t\t\tbehaviac.IVariable.Register<{0}>(\"{0}\");", type.Replace("::", "."));
                        }
                        else
                        {
                            file.WriteLine("\t\t\tbehaviac.Agent.RegisterStaticClass(typeof({0}), \"{1}\", \"{2}\");", type.Replace("::", "."), agentType.DisplayName, agentType.Description);
                        }
                    }

                    file.WriteLine();

                    foreach (string type in Plugin.AllMetaTypes)
                    {
                        AgentType agentType = Plugin.GetAgentType(type);
                        if (agentType != null)
                        {
                            file.WriteLine("\t\t\tbehaviac.Workspace.Instance.AddAgentType(typeof({0}), {1});", type.Replace("::", "."), agentType.IsInherited ? "true" : "false");
                        }
                    }

                    file.WriteLine();
                    file.WriteLine("\t\t\tGeneratedRegisterationTypes = true;");

                    file.WriteLine("\t\t}\n");

                    // UnRegisterTypes_
                    file.WriteLine("\t\tstatic partial void UnRegisterTypes_()");
                    file.WriteLine("\t\t{");

                    foreach (string type in Plugin.AllMetaTypes)
                    {
                        AgentType agentType = Plugin.GetAgentType(type);
                        bool isStatic = (agentType != null) ? agentType.IsStatic : false;

                        if (!isStatic)
                            file.WriteLine("\t\t\tbehaviac.IVariable.UnRegister<{0}>(\"{0}\");", type.Replace("::", "."));
                    }

                    file.WriteLine("\t\t}");
                    file.WriteLine("\t}");
                    file.WriteLine("}");
                }
            }
        }

        private void ExportProperties(StreamWriter file)
        {
            file.WriteLine("\t\t\t// ---------------------------------------------------------------------");
            file.WriteLine("\t\t\t// properties");
            file.WriteLine("\t\t\t// ---------------------------------------------------------------------\n");

            bool hasWrittenProperties = false;

            foreach (AgentType agent in Plugin.AgentTypes)
            {
                if (agent.IsCustomized)
                    continue;

                bool hasNonParProperty = false;
                foreach (PropertyDef prop in agent.GetProperties())
                {
                    if (!prop.IsPar && !prop.IsArrayElement)
                    {
                        hasNonParProperty = true;
                        break;
                    }
                }

                if (hasNonParProperty)
                {
                    if (!hasWrittenProperties)
                    {
                        hasWrittenProperties = true;

                        file.WriteLine("\t\t\tAgentProperties bb;");
                    }

                    string agentTypeName = agent.AgentTypeName.Replace("::", ".");
                    file.WriteLine("\n\t\t\t// {0}", agentTypeName);
                    file.WriteLine("\t\t\tbb = new AgentProperties(\"{0}\");", agentTypeName);
                    file.WriteLine("\t\t\tagent_type_blackboards[\"{0}\"] = bb;", agentTypeName);

                    foreach (PropertyDef prop in agent.GetProperties())
                    {
                        if (!prop.IsPar && !prop.IsArrayElement)
                        {
                            file.WriteLine("\t\t\tbb.AddProperty(\"{0}\", {1}, \"{2}\", \"{3}\", \"{4}\");",
                                DataCsExporter.GetExportNativeType(prop.NativeType),
                                prop.IsStatic ? "true" : "false",
                                prop.BasicName, prop.DefaultValue.Replace("\"", "\\\""), agentTypeName);
                        }
                    }
                }
            }
        }

        private void ExportMethods(StreamWriter file)
        {
            file.WriteLine("\n\t\t\t// ---------------------------------------------------------------------");
            file.WriteLine("\t\t\t// tasks");
            file.WriteLine("\t\t\t// ---------------------------------------------------------------------\n");

            bool hasWrittenMethod = false;

            foreach (AgentType agent in Plugin.AgentTypes)
            {
                if (agent.IsCustomized)
                    continue;

                IList<MethodDef> methods = agent.GetMethods();
                bool hasEvents = false;

                foreach (MethodDef method in methods)
                {
                    if (method.IsNamedEvent)
                    {
                        hasEvents = true;
                        break;
                    }
                }

                if (hasEvents)
                {
                    if (!hasWrittenMethod)
                    {
                        hasWrittenMethod = true;

                        file.WriteLine("\t\t\tAgent.CTagObjectDescriptor objectDesc;");
                        file.WriteLine("\t\t\tCCustomMethod customeMethod;");
                    }

                    string agentTypeName = agent.AgentTypeName.Replace("::", ".");
                    file.WriteLine("\n\t\t\t// {0}", agentTypeName);
                    file.WriteLine("\t\t\tobjectDesc = Agent.GetDescriptorByName(\"{0}\");", agentTypeName);

                    file.WriteLine("\t\t\tcustomeMethod = new CTaskMethod(\"{0}\", \"root\");", agentTypeName);
                    file.WriteLine("\t\t\tobjectDesc.ms_methods.Add(customeMethod);");

                    foreach (MethodDef method in methods)
                    {
                        if (method.IsNamedEvent)
                        {
                            file.WriteLine("\n\t\t\tcustomeMethod = new CTaskMethod(\"{0}\", \"{1}\");", agentTypeName, method.BasicName);

                            foreach (MethodDef.Param param in method.Params)
                            {
                                file.WriteLine("\t\t\tcustomeMethod.AddParamType(\"{0}\");", DataCsExporter.GetExportNativeType(param.NativeType));
                            }

                            file.WriteLine("\t\t\tobjectDesc.ms_methods.Add(customeMethod);");
                        }
                    }
                }
            }
        }
    }
}
