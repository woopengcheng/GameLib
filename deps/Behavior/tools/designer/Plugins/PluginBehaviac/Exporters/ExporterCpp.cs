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
using Behaviac.Design.Attachments;
using PluginBehaviac.Properties;
using PluginBehaviac.NodeExporters;
using PluginBehaviac.DataExporters;
using Behaviac.Design.Attributes;

namespace PluginBehaviac.Exporters
{
    public class ExporterCpp : Behaviac.Design.Exporters.Exporter
    {
        class BehaviorCreator
        {
            public BehaviorCreator(string filename, string classname)
            {
                Filename = filename;
                Classname = classname;
            }

            public string Filename;
            public string Classname;
        }

        List<BehaviorCreator> _behaviorCreators = new List<BehaviorCreator>();

        public ExporterCpp(BehaviorNode node, string outputFolder, string filename, List<string> includedFilenames = null)
            : base(node, outputFolder, filename, includedFilenames)
        {
            //automatically create an extra level of path
            _outputFolder = Path.Combine(Path.GetFullPath(_outputFolder), "behaviac_generated");
            _filename = "behaviors/generated_behaviors.h";
        }

        public override Behaviac.Design.FileManagers.SaveResult Export(List<BehaviorNode> behaviors, bool exportUnifiedFile, bool generateCustomizedTypes)
        {
            string behaviorFilename = "behaviors/generated_behaviors.h";
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
                    ExportAgentsDefinition(agentFolder);
                    ExportAgentsImplemention(agentFolder);

                    ExportCustomizedTypesDefinition(agentFolder);
                    ExportCustomizedTypesImplemention(agentFolder);
                }
            }

            return result;
        }

        private void clearFolder(string folder)
        {
            DirectoryInfo dirInfo = new DirectoryInfo(folder);
            if (dirInfo.Exists)
            {
                foreach (FileInfo file in dirInfo.GetFiles("*.*", SearchOption.AllDirectories))
                {
                    try
                    {
                        File.SetAttributes(file.FullName, FileAttributes.Normal);
                        if (file.Extension == ".inl" || file.Extension == ".h" || file.Extension == ".cpp")
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

                _behaviorCreators.Clear();

                if (exportUnifiedFile)
                {
                    foreach (BehaviorNode behavior in behaviors)
                    {
                        behavior.PreExport();

                        _behaviorCreators.Add(ExportBody(file, behavior));

                        behavior.PostExport();
                    }
                }
                else
                {
                    foreach (BehaviorNode behavior in behaviors)
                    {
                        string behaviorFilename = behavior.RelativePath;
                        behaviorFilename = behaviorFilename.Replace("\\", "/");
                        behaviorFilename = Path.ChangeExtension(behaviorFilename, "inl");

                        file.WriteLine("#include \"{0}\"", behaviorFilename);

                        behaviorFilename = Path.Combine("behaviors", behaviorFilename);
                        string agentFolder = string.Empty;

                        Behaviac.Design.FileManagers.SaveResult result = VerifyFilename(ref behaviorFilename, ref agentFolder);

                        if (Behaviac.Design.FileManagers.SaveResult.Succeeded == result)
                        {
                            using (StreamWriter behaviorFile = new StreamWriter(behaviorFilename))
                            {
                                behavior.PreExport();

                                _behaviorCreators.Add(ExportBody(behaviorFile, behavior));

                                behavior.PostExport();

                                behaviorFile.Close();
                            }
                        }
                    }

                    file.WriteLine();
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

        private List<string> GetNamespaces(string ns)
        {
            List<string> namespaces = new List<string>();
            int startIndex = 0;

            for (int i = 0; i < ns.Length; ++i)
            {
                if (ns[i] == ':')
                {
                    Debug.Check(ns[i + 1] == ':');

                    namespaces.Add(ns.Substring(startIndex, i - startIndex));
                    startIndex = i + 2;
                    ++i;
                }
            }

            namespaces.Add(ns.Substring(startIndex, ns.Length - startIndex));

            return namespaces;
        }

        private string WriteNamespacesHead(StreamWriter file, List<string> namespaces)
        {
            string indent = string.Empty;
            for (int i = 0; i < namespaces.Count; ++i)
            {
                file.WriteLine("{0}namespace {1}", indent, namespaces[i]);
                file.WriteLine("{0}{{", indent);
                indent += '\t';
            }

            return indent;
        }

        private void WriteNamespacesTail(StreamWriter file, List<string> namespaces)
        {
            for (int i = 0; i < namespaces.Count; ++i)
            {
                string indent = string.Empty;
                for (int k = i + 1; k < namespaces.Count; ++k)
                {
                    indent += '\t';
                }

                file.WriteLine("{0}}}\r\n", indent);
            }
        }

        private void ExportHead(StreamWriter file, string exportFilename)
        {
            string wsfolder = Path.GetDirectoryName(Workspace.Current.FileName);
            exportFilename = Behaviac.Design.FileManagers.FileManager.MakeRelative(wsfolder, exportFilename);
            exportFilename = exportFilename.Replace("\\", "/");

            // write comments
            file.WriteLine("// ---------------------------------------------------------------------");
            file.WriteLine("/*\nThis file is auto-generated by behaviac designer, so please don't modify it by yourself!\n\n");
            file.WriteLine("Usage: include this file in a certain cpp accordingly, only include it once and don't include it in other cpps again.");
            file.WriteLine("(RELATIVE_PATH is the path where it is generated):");
            file.WriteLine("and you also need to include your agent types' headers before it:\n");
            file.WriteLine("      #include \"YourAgentTypes.h\"\n");
            file.WriteLine("      #include \"RELATIVE_PATH/generated_behaviors.h\"\n*/\n");
            file.WriteLine("// Export file: {0}", exportFilename);
            file.WriteLine("// ---------------------------------------------------------------------\r\n");

            // write included behaviac files
            file.WriteLine("// You should set the include path of the behaviac lib in your project\r\n// for using the following header files :");
            file.WriteLine("#pragma once");
            file.WriteLine("#include \"behaviac/behaviortree/behaviortree.h\"");
            file.WriteLine("#include \"behaviac/agent/agent.h\"");
            file.WriteLine("#include \"behaviac/agent/taskmethod.h\"\n");

            file.WriteLine("#include \"behaviac/behaviortree/nodes/actions/action.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/actions/assignment.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/actions/compute.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/actions/noop.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/actions/wait.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/actions/waitforsignal.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/actions/waitframes.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/compositestochastic.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/ifelse.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/parallel.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/query.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/referencebehavior.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/selector.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/selectorloop.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/selectorprobability.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/selectorstochastic.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/sequence.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/sequencestochastic.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/composites/withprecondition.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/conditions/and.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/conditions/conditionbase.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/conditions/condition.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/conditions/false.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/conditions/or.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/conditions/true.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratoralwaysfailure.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratoralwaysrunning.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratoralwayssuccess.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratorcount.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratorcountlimit.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratorfailureuntil.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratorframes.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratoriterator.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratorlog.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratorloop.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratorloopuntil.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratornot.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratorrepeat.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratorsuccessuntil.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratortime.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/nodes/decorators/decoratorweight.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/attachments/event.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/attachments/attachaction.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/attachments/precondition.h\"");
            file.WriteLine("#include \"behaviac/behaviortree/attachments/effector.h\"");
            file.WriteLine("#include \"behaviac/htn/task.h\"");
            file.WriteLine("#include \"behaviac/fsm/fsm.h\"");
            file.WriteLine("#include \"behaviac/fsm/state.h\"");
            file.WriteLine("#include \"behaviac/fsm/startcondition.h\"");
            file.WriteLine("#include \"behaviac/fsm/transitioncondition.h\"");
            file.WriteLine("#include \"behaviac/fsm/waitstate.h\"");
            file.WriteLine("#include \"behaviac/fsm/waitframesstate.h\"");
            file.WriteLine("#include \"behaviac/fsm/alwaystransition.h\"");
            file.WriteLine("#include \"behaviac/fsm/waittransition.h\"\r\n");

            // write included files for the game agents
            if (this.IncludedFilenames != null)
            {
                file.WriteLine("// You should set the agent header files of your game\r\n// when exporting cpp files in the behaviac editor:");
                foreach (string filename in this.IncludedFilenames)
                {
                    file.WriteLine("#include \"{0}\"", filename);
                }
                file.WriteLine("");
            }

            // write the namespaces for the game agents
            file.WriteLine("using namespace behaviac;\r\n");

            // write property and method handlers
            file.WriteLine("// Agent property and method handlers\r\n");

            foreach (AgentType agenType in Plugin.AgentTypes)
            {
                List<string> namespaces = new List<string>();
                string ns = agenType.Namespace;

                if (!string.IsNullOrEmpty(ns))
                {
                    foreach (PropertyDef property in agenType.GetProperties())
                    {
                        if (property.IsMember && !property.IsAddedAutomatically && property.ClassName == agenType.AgentTypeName)
                        {
                            namespaces = GetNamespaces(ns);
                            break;
                        }
                    }
                }

                if (namespaces.Count == 0 && !string.IsNullOrEmpty(ns))
                {
                    foreach (MethodDef method in agenType.GetMethods())
                    {
                        if (method.ClassName == agenType.AgentTypeName)
                        {
                            namespaces = GetNamespaces(ns);
                            break;
                        }
                    }
                }

                string indent = WriteNamespacesHead(file, namespaces);

                foreach (PropertyDef property in agenType.GetProperties())
                {
                    if (!property.IsPublic && property.IsMember && !property.IsAddedAutomatically && property.ClassName == agenType.AgentTypeName)
                    {
                        string propName = property.Name.Replace("::", "_");
                        string nativeType = DataCppExporter.GetBasicGeneratedNativeType(property.NativeType);

                        file.WriteLine("{0}struct PROPERTY_TYPE_{1} {{ }};", indent, propName);
                        file.WriteLine("{0}template<>  {1}& {2}::_Get_Property_<PROPERTY_TYPE_{3}>()", indent, nativeType, agenType.BasicClassName, propName);
                        file.WriteLine("{0}{{", indent);
                        if (property.IsProperty)
                        {
                            file.WriteLine("{0}\treturn *({1}*)&this->GetVariable<{1}>(\"{2}\");", indent, nativeType, property.BasicName);
                        }
                        else // field
                        {
                            if (property.IsStatic)
                            {
                                file.WriteLine("{0}\tunsigned char* pc = (unsigned char*)(&{1});", indent, property.Name);
                            }
                            else
                            {
                                file.WriteLine("{0}\tunsigned char* pc = (unsigned char*)this;", indent);
                                file.WriteLine("{0}\tpc += (int)BEHAVIAC_OFFSETOF({1}, {2});", indent, property.ClassName, property.Name);
                            }
                            file.WriteLine("{0}\treturn *(reinterpret_cast<{1}*>(pc));", indent, nativeType);
                        }

                        file.WriteLine("{0}}}\r\n", indent);
                    }
                }

                foreach (MethodDef method in agenType.GetMethods())
                {
                    if (!method.IsPublic && !method.IsCustomized && method.ClassName == agenType.AgentTypeName)
                    {
                        string paramStrDef = string.Empty;
                        string paramStr = string.Empty;
                        for (int i = 0; i < method.Params.Count; ++i)
                        {
                            if (i > 0)
                            {
                                paramStrDef += ", ";
                                paramStr += ", ";
                            }

                            string basicNativeType = DataCppExporter.GetGeneratedNativeType(method.Params[i].NativeType);
                            paramStrDef += string.Format("{0} p{1}", basicNativeType, i);
                            paramStr += string.Format("p{0}", i);
                        }

                        string methodName = method.Name.Replace("::", "_");
                        string nativeReturnType = DataCppExporter.GetGeneratedNativeType(method.NativeReturnType);
                        if (method.NativeReturnType.StartsWith("const "))
                            nativeReturnType = "const " + nativeReturnType;

                        file.WriteLine("{0}struct METHOD_TYPE_{1} {{ }};", indent, methodName);
                        file.WriteLine("{0}template<>  {1} {2}::_Execute_Method_<METHOD_TYPE_{3}>({4})", indent, nativeReturnType, agenType.BasicClassName, methodName, paramStrDef);
                        file.WriteLine("{0}{{", indent);

                        string ret = (method.NativeReturnType == "void") ? string.Empty : "return ";
                        file.WriteLine("{0}\t{1}this->{2}({3});", indent, ret, method.Name, paramStr);

                        file.WriteLine("{0}}}\r\n", indent);
                    }
                }

                WriteNamespacesTail(file, namespaces);
            }

            // create namespace
            file.WriteLine("namespace behaviac\r\n{");
        }

        private BehaviorCreator ExportBody(StreamWriter file, BehaviorNode behavior)
        {
            string filename = Path.ChangeExtension(behavior.RelativePath, "").Replace(".", "");
            filename = filename.Replace('\\', '/');

            // write comments
            file.WriteLine("\t// Source file: {0}\r\n", filename);

            string btClassName = string.Format("bt_{0}", filename.Replace('/', '_'));
            string agentType = behavior.AgentType.AgentTypeName;

            // create the class definition of its attachments
            ExportAttachmentClass(file, btClassName, (Node)behavior);

            // create the class definition of its children
            foreach (Node child in ((Node)behavior).GetChildNodes())
                ExportNodeClass(file, btClassName, agentType, behavior, child);

            // export the create function
            file.WriteLine("\tclass {0}", btClassName);
            file.WriteLine("\t{");
            file.WriteLine("\tpublic:");
            file.WriteLine("\t\tstatic bool Create(BehaviorTree* pBT)");
            file.WriteLine("\t\t{");

            file.WriteLine("\t\t\tpBT->SetClassNameString(\"BehaviorTree\");");
            file.WriteLine("\t\t\tpBT->SetId((uint16_t)-1);");
            file.WriteLine("\t\t\tpBT->SetName(\"{0}\");", filename);
            file.WriteLine("\t\t\tpBT->SetIsFSM({0});", ((Node)behavior).IsFSM ? "true" : "false");
            file.WriteLine("#if !BEHAVIAC_RELEASE");
            file.WriteLine("\t\t\tpBT->SetAgentType(\"{0}\");", agentType);
            file.WriteLine("#endif");
            if (!string.IsNullOrEmpty(((Behavior)behavior).Domains))
            {
                file.WriteLine("\t\t\tpBT->SetDomains(\"{0}\");", ((Behavior)behavior).Domains);
            }
            if (((Behavior)behavior).DescriptorRefs.Count > 0)
            {
                file.WriteLine("\t\t\tpBT->SetDescriptors(\"{0}\");", DesignerPropertyUtility.RetrieveExportValue(((Behavior)behavior).DescriptorRefs));
            }

            ExportPars(file, agentType, "pBT", (Node)behavior, "\t\t");

            // export its attachments
            ExportAttachment(file, btClassName, agentType, "pBT", (Node)behavior, "\t\t\t");

            file.WriteLine("\t\t\t// children");

            // export its children
            if (((Node)behavior).IsFSM)
            {
                file.WriteLine("\t\t\t{");
                file.WriteLine("\t\t\t\tFSM* fsm = BEHAVIAC_NEW FSM();");
                file.WriteLine("\t\t\t\tfsm->SetClassNameString(\"FSM\");");
                file.WriteLine("\t\t\t\tfsm->SetId((uint16_t)-1);");
                file.WriteLine("\t\t\t\tfsm->SetInitialId({0});", behavior.InitialStateId);
                file.WriteLine("#if !BEHAVIAC_RELEASE");
                file.WriteLine("\t\t\t\tfsm->SetAgentType(\"{0}\");", agentType);
                file.WriteLine("#endif");

                foreach (Node child in ((Node)behavior).FSMNodes)
                {
                    ExportNode(file, btClassName, agentType, "fsm", child, 4);
                }

                file.WriteLine("\t\t\t\tpBT->AddChild(fsm);");
                file.WriteLine("\t\t\t}");
            }
            else
            {
                foreach (Node child in ((Node)behavior).GetChildNodes())
                    ExportNode(file, btClassName, agentType, "pBT", child, 3);
            }

            file.WriteLine("\t\t\treturn true;");
            file.WriteLine("\t\t}");
            file.WriteLine("\t};");
            file.WriteLine();

            return new BehaviorCreator(filename, btClassName);
        }

        private void ExportTail(StreamWriter file)
        {
            file.WriteLine("\tclass CppGenerationManager : GenerationManager");
	        file.WriteLine("\t{");
	        file.WriteLine("\tpublic:");
		    file.WriteLine("\t\tCppGenerationManager()");
		    file.WriteLine("\t\t{");
            file.WriteLine("\t\t\tSetInstance(this);");
		    file.WriteLine("\t\t}\n");

            file.WriteLine("\t\tvirtual void RegisterBehaviorsImplement()");
		    file.WriteLine("\t\t{");
            for (int i = 0; i < _behaviorCreators.Count; ++i)
            {
                string filename = _behaviorCreators[i].Filename;
                string btClassName = _behaviorCreators[i].Classname;
                file.WriteLine("\t\t\tWorkspace::GetInstance()->RegisterBehaviorTreeCreator(\"{0}\", {1}::Create);", filename, btClassName);
            }
		    file.WriteLine("\t\t}");
	        file.WriteLine("\t};\n");

            file.WriteLine("\tCppGenerationManager _cppGenerationManager_;");

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
                    string type = pars[i].NativeType;
                    string value = pars[i].DefaultValue.Replace("\"", "\\\"");

                    file.WriteLine("{0}\t{1}->AddPar(\"{2}\", \"{3}\", \"{4}\", \"{5}\");", indent, nodeName, agentType, type, name, value);
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

                AttachmentCppExporter attachmentExporter = AttachmentCppExporter.CreateInstance(attach);
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
                    AttachmentCppExporter attachmentExporter = AttachmentCppExporter.CreateInstance(attach);
                    attachmentExporter.GenerateInstance(attach, file, indent, nodeName, agentType, btClassName);

                    string isPrecondition = attach.IsPrecondition && !attach.IsTransition ? "true" : "false";
                    string isEffector = attach.IsEffector && !attach.IsTransition ? "true" : "false";
                    string isTransition = attach.IsTransition ? "true" : "false";
                    file.WriteLine("{0}\t{1}->Attach({2}, {3}, {4}, {5});", indent, parentName, nodeName, isPrecondition, isEffector, isTransition);
                    file.WriteLine("{0}\t{1}->SetHasEvents({1}->HasEvents() | (Event::DynamicCast({2}) != 0));", indent, parentName, nodeName);
                    file.WriteLine("{0}}}", indent);
                }
            }
        }

        private void ExportNodeClass(StreamWriter file, string btClassName, string agentType, BehaviorNode behavior, Node node)
        {
            if (!node.Enable)
                return;

            string nodeName = string.Format("node{0}", node.Id);

            NodeCppExporter nodeExporter = NodeCppExporter.CreateInstance(node);
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
            NodeCppExporter nodeExporter = NodeCppExporter.CreateInstance(node);
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
                file.WriteLine("{0}\t{1}->AddChild({2});", indent, parentName, nodeName);
            }
            else
            {
                // add the node as its customized children
                file.WriteLine("{0}\t{1}->SetCustomCondition({2});", indent, parentName, nodeName);
            }

            // export the child nodes
            if (!node.IsFSM && !(node is ReferencedBehavior))
            {
                foreach (Node child in node.GetChildNodes())
                {
                    ExportNode(file, btClassName, agentType, nodeName, child, indentDepth + 1);
                }
            }

            file.WriteLine("{0}\t{1}->SetHasEvents({1}->HasEvents() | {2}->HasEvents());", indent, parentName, nodeName);

            // close the brackets for a better formatting in the generated code
            file.WriteLine("{0}}}", indent);
        }

        private void ExportAgentsDefinition(string agentFolder)
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
                    string filename = Path.Combine(agentFolder, agent.BasicClassName + ".h");
                    Encoding utf8WithBom = new UTF8Encoding(true);

                    using (StreamWriter file = new StreamWriter(filename, false, utf8WithBom))
                    {
                        // write comments
                        file.WriteLine("// ---------------------------------------------------------------------");
                        file.WriteLine("// This agent file is auto-generated by behaviac designer, but you should");
                        file.WriteLine("// implement the methods of the agent class if necessary!");
                        file.WriteLine("// ---------------------------------------------------------------------\r\n");

                        string headerFileMacro = string.Format("BEHAVIAC_{0}_H", agent.AgentTypeName.Replace("::", "_").ToUpperInvariant());

                        file.WriteLine("#ifndef {0}", headerFileMacro);
                        file.WriteLine("#define {0}", headerFileMacro);
                        file.WriteLine();

                        string indent = "";
                        if (!string.IsNullOrEmpty(agent.Namespace))
                        {
                            indent = "\t";

                            file.WriteLine("namespace {0}", agent.Namespace);
                            file.WriteLine("{");
                        }

                        file.WriteLine("{0}class {1} : public {2}", indent, agent.BasicClassName, agent.Base.AgentTypeName);
                        file.WriteLine("{0}{{", indent);

                        file.WriteLine("{0}public:", indent);
                        file.WriteLine("{0}\t{1}();", indent, agent.BasicClassName);
                        file.WriteLine("{0}\tvirtual ~{1}();", indent, agent.BasicClassName);
                        file.WriteLine();
                        file.WriteLine("{0}\tDECLARE_BEHAVIAC_AGENT({1}, {2})", indent, agent.BasicClassName, agent.Base.AgentTypeName);
                        file.WriteLine();

                        if (hasCustomizedProperty)
                        {
                            foreach (PropertyDef prop in properties)
                            {
                                if (prop.IsCustomized && !prop.IsPar && !prop.IsArrayElement)
                                {
                                    string publicStr = prop.IsPublic ? "public:" : "private:";
                                    string staticStr = prop.IsStatic ? "static " : "";
                                    string propType = DataCppExporter.GetGeneratedNativeType(prop.Type);

                                    file.WriteLine("{0}{1}", indent, publicStr);
                                    file.WriteLine("{0}\t{1}{2} {3};", indent, staticStr, propType, prop.BasicName);
                                    file.WriteLine();
                                }
                            }
                        }

                        if (hasCustomizedMethod)
                        {
                            foreach (MethodDef method in methods)
                            {
                                if (method.IsCustomized && !method.IsNamedEvent)
                                {
                                    string publicStr = method.IsPublic ? "public:" : "private:";
                                    string staticStr = method.IsStatic ? "static " : "";

                                    string allParams = "";
                                    foreach (MethodDef.Param param in method.Params)
                                    {
                                        if (!string.IsNullOrEmpty(allParams))
                                            allParams += ", ";

                                        allParams += DataCppExporter.GetGeneratedNativeType(param.NativeType) + " " + param.Name;
                                    }

                                    file.WriteLine("{0}{1}", indent, publicStr);
                                    file.WriteLine("{0}\t{1}{2} {3}({4});", indent, staticStr, DataCppExporter.GetGeneratedNativeType(method.ReturnType), method.BasicName, allParams);
                                    file.WriteLine();
                                }
                            }
                        }

                        //end of class
                        file.WriteLine("{0}}};", indent);

                        if (!string.IsNullOrEmpty(agent.Namespace))
                        {
                            //end of namespace
                            file.WriteLine("}");
                        }

                        file.WriteLine();
                        file.WriteLine("BEHAVIAC_DECLARE_TYPE_VECTOR_HANDLER({0}*);", agent.AgentTypeName);
                        file.WriteLine();

                        file.WriteLine("#endif");

                        file.Close();
                    }
                }
            }
        }

        private void ExportAgentsImplemention(string agentFolder)
        {
            foreach (AgentType agent in Plugin.AgentTypes)
            {
                if (!agent.IsCustomized)
                    continue;

                string filename = Path.Combine(agentFolder, agent.BasicClassName + ".cpp");
                Encoding utf8WithBom = new UTF8Encoding(true);

                using (StreamWriter file = new StreamWriter(filename, false, utf8WithBom))
                {
                    // write comments
                    file.WriteLine("// ---------------------------------------------------------------------");
                    file.WriteLine("// This agent file is auto-generated by behaviac designer, but you should");
                    file.WriteLine("// implement the methods of the agent class if necessary!");
                    file.WriteLine("// ---------------------------------------------------------------------\r\n");

                    file.WriteLine("#include \"{0}.h\"", agent.BasicClassName);
                    file.WriteLine();

                    string indent = "";
                    if (!string.IsNullOrEmpty(agent.Namespace))
                    {
                        indent = "\t";

                        file.WriteLine("namespace {0}", agent.Namespace);
                        file.WriteLine("{");
                    }

                    bool hasStaticProperties = false;
                    foreach (PropertyDef prop in agent.GetProperties())
                    {
                        if (prop.IsStatic && prop.IsCustomized && !prop.IsPar && !prop.IsArrayElement)
                        {
                            hasStaticProperties = true;

                            string propType = DataCppExporter.GetGeneratedNativeType(prop.Type);
                            string defaultValue = DataCsExporter.GetGeneratedPropertyDefaultValue(prop, propType);
                            if (defaultValue != null)
                                defaultValue = " = " + defaultValue;
                            else
                                defaultValue = "";

                            file.WriteLine("{0}{1} {2}::{3}{4};", indent, propType, agent.BasicClassName, prop.BasicName, defaultValue);
                        }
                    }

                    if (hasStaticProperties)
                        file.WriteLine();

                    file.WriteLine("{0}{1}::{1}()", indent, agent.BasicClassName);
                    file.WriteLine("{0}{{", indent);
                    foreach (PropertyDef prop in agent.GetProperties())
                    {
                        if (!prop.IsStatic && prop.IsCustomized && !prop.IsPar && !prop.IsArrayElement)
                        {
                            string propType = DataCppExporter.GetGeneratedNativeType(prop.Type);
                            string defaultValue = DataCppExporter.GetGeneratedPropertyDefaultValue(prop, propType);
                            if (defaultValue != null)
                                file.WriteLine("{0}\t{1} = {2};", indent, prop.BasicName, defaultValue);
                        }
                    }
                    file.WriteLine("{0}}}", indent);
                    file.WriteLine();

                    file.WriteLine("{0}{1}::~{1}()", indent, agent.BasicClassName);
                    file.WriteLine("{0}{{", indent);
                    file.WriteLine("{0}}}", indent);
                    file.WriteLine();

                    file.WriteLine("{0}BEGIN_PROPERTIES_DESCRIPTION({1})", indent, agent.BasicClassName);
                    file.WriteLine("{0}{{", indent);

                    string commentDisplayName = "// ";
                    if (!string.IsNullOrEmpty(agent.DisplayName) && agent.DisplayName != agent.BasicClassName)
                    {
                        commentDisplayName = "";
                    }
                    file.WriteLine("{0}\t{1}CLASS_DISPLAYNAME(L\"{2}\");", indent, commentDisplayName, agent.DisplayName);

                    string commentDescription = "// ";
                    if (!string.IsNullOrEmpty(agent.Description) && agent.Description != agent.BasicClassName)
                    {
                        commentDescription = "";
                    }
                    file.WriteLine("{0}\t{1}CLASS_DESC(L\"{2}\");", indent, commentDescription, agent.Description);

                    file.WriteLine();

                    bool hasCustomizedProperties = false;
                    foreach (PropertyDef prop in agent.GetProperties())
                    {
                        if (prop.IsCustomized && !prop.IsPar && !prop.IsArrayElement)
                        {
                            hasCustomizedProperties = true;

                            if (!string.IsNullOrEmpty(prop.DisplayName) && prop.DisplayName != prop.BasicName ||
                                !string.IsNullOrEmpty(prop.BasicDescription) && prop.BasicDescription != prop.BasicName)
                            {
                                file.WriteLine("{0}\tREGISTER_PROPERTY({1}).DISPLAYNAME(L\"{2}\").DESC(L\"{3}\");", indent, prop.BasicName, prop.DisplayName, prop.BasicDescription);
                            }
                            else
                            {
                                file.WriteLine("{0}\tREGISTER_PROPERTY({1});", indent, prop.BasicName);
                            }
                        }
                    }

                    if (hasCustomizedProperties)
                        file.WriteLine();

                    foreach (MethodDef method in agent.GetMethods())
                    {
                        if (method.IsCustomized && !method.IsNamedEvent)
                        {
                            if (!string.IsNullOrEmpty(method.DisplayName) && method.DisplayName != method.BasicName ||
                                !string.IsNullOrEmpty(method.BasicDescription) && method.BasicDescription != method.BasicName)
                            {
                                file.WriteLine("{0}\tREGISTER_METHOD({1}).DISPLAYNAME(L\"{2}\").DESC(L\"{3}\");", indent, method.BasicName, method.DisplayName, method.BasicDescription);
                            }
                            else
                            {
                                file.WriteLine("{0}\tREGISTER_METHOD({1});", indent, method.BasicName);
                            }
                        }
                    }

                    file.WriteLine("{0}}}", indent);
                    file.WriteLine("{0}END_PROPERTIES_DESCRIPTION()", indent);
                    file.WriteLine();

                    foreach (MethodDef method in agent.GetMethods())
                    {
                        if (method.IsCustomized && !method.IsNamedEvent)
                        {
                            string allParams = "";
                            foreach (MethodDef.Param param in method.Params)
                            {
                                if (!string.IsNullOrEmpty(allParams))
                                    allParams += ", ";

                                allParams += DataCppExporter.GetGeneratedNativeType(param.NativeType) + " " + param.Name;
                            }

                            string returnValue = DataCppExporter.GetGeneratedDefaultValue(method.ReturnType, method.NativeReturnType);

                            file.WriteLine("{0}{1} {2}::{3}({4})", indent, DataCppExporter.GetGeneratedNativeType(method.ReturnType), agent.BasicClassName, method.BasicName, allParams);
                            file.WriteLine("{0}{{", indent);
                            file.WriteLine("{0}\t// Write your logic codes here.", indent);
                            if (returnValue != null)
                            {
                                file.WriteLine();
                                file.WriteLine("{0}\treturn {1};", indent, returnValue);
                            }
                            file.WriteLine("{0}}}", indent);
                        }
                    }

                    if (!string.IsNullOrEmpty(agent.Namespace))
                    {
                        //end of namespace
                        file.WriteLine("}");
                    }

                    file.Close();
                }
            }
        }

        private void ExportCustomizedTypesDefinition(string agentFolder)
        {
            if (CustomizedTypeManager.Instance.Enums.Count > 0 || CustomizedTypeManager.Instance.Structs.Count > 0)
            {
                string filename = Path.Combine(agentFolder, "customizedtypes.h");
                Encoding utf8WithBom = new UTF8Encoding(true);

                using (StreamWriter file = new StreamWriter(filename, false, utf8WithBom))
                {
                    // write comments
                    file.WriteLine("// ---------------------------------------------------------------------");
                    file.WriteLine("// This file is auto-generated by behaviac designer, so please don't modify it by yourself!");
                    file.WriteLine("// ---------------------------------------------------------------------\n");

                    file.WriteLine("#pragma once");
                    file.WriteLine("#include \"behaviac/agent/agent.h\"\n");

                    file.WriteLine("// -------------------");
                    file.WriteLine("// Customized enums");
                    file.WriteLine("// -------------------\n");

                    for (int e = 0; e < CustomizedTypeManager.Instance.Enums.Count; ++e)
                    {
                        if (e > 0)
                            file.WriteLine();

                        CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[e];

                        file.WriteLine("enum {0}", customizedEnum.Name);
                        file.WriteLine("{");

                        for (int m = 0; m < customizedEnum.Members.Count; ++m)
                        {
                            if (m > 0)
                                file.WriteLine();

                            CustomizedEnum.CustomizedEnumMember member = customizedEnum.Members[m];

                            if (member.Value >= 0)
                                file.WriteLine("\t{0} = {1},", member.Name, member.Value);
                            else
                                file.WriteLine("\t{0},", member.Name);
                        }

                        file.WriteLine("};");
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

                        file.WriteLine("struct {0}", customizedStruct.Name);
                        file.WriteLine("{");

                        if (customizedStruct.Properties.Count > 0)
                        {
                            for (int m = 0; m < customizedStruct.Properties.Count; ++m)
                            {
                                if (m > 0)
                                    file.WriteLine();

                                PropertyDef member = customizedStruct.Properties[m];

                                file.WriteLine("\t{0} {1};", DataCppExporter.GetGeneratedNativeType(member.NativeType), member.BasicName);
                            }

                            file.WriteLine();
                        }

                        file.WriteLine("DECLARE_BEHAVIAC_STRUCT({0});", customizedStruct.Name);

                        file.WriteLine("};");
                    }

                    if (CustomizedTypeManager.Instance.Enums.Count > 0)
                    {
                        file.WriteLine();

                        for (int e = 0; e < CustomizedTypeManager.Instance.Enums.Count; ++e)
                        {
                            CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[e];
                            file.WriteLine("DECLARE_BEHAVIAC_ENUM({0}, {0});", customizedEnum.Name);
                        }
                    }

                    if (CustomizedTypeManager.Instance.Structs.Count > 0)
                    {
                        file.WriteLine();

                        for (int s = 0; s < CustomizedTypeManager.Instance.Structs.Count; s++)
                        {
                            CustomizedStruct customizedStruct = CustomizedTypeManager.Instance.Structs[s];
                            file.WriteLine("BEHAVIAC_DECLARE_TYPE_VECTOR_HANDLER({0});", customizedStruct.Name);
                        }
                    }
                }
            }
        }

        private void ExportCustomizedTypesImplemention(string agentFolder)
        {
            if (CustomizedTypeManager.Instance.Enums.Count > 0 || CustomizedTypeManager.Instance.Structs.Count > 0)
            {
                string filename = Path.Combine(agentFolder, "customizedtypes.cpp");
                Encoding utf8WithBom = new UTF8Encoding(true);

                using (StreamWriter file = new StreamWriter(filename, false, utf8WithBom))
                {
                    // write comments
                    file.WriteLine("// ---------------------------------------------------------------------");
                    file.WriteLine("// This file is auto-generated by behaviac designer, so please don't modify it by yourself!");
                    file.WriteLine("// ---------------------------------------------------------------------\n");

                    file.WriteLine("#include \"behaviac/agent/registermacros.h\"");
                    file.WriteLine("#include \"customizedtypes.h\"\n");

                    file.WriteLine("// -------------------");
                    file.WriteLine("// Customized enums");
                    file.WriteLine("// -------------------\n");

                    for (int e = 0; e < CustomizedTypeManager.Instance.Enums.Count; ++e)
                    {
                        if (e > 0)
                            file.WriteLine();

                        CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[e];
                        
                        file.WriteLine("BEGIN_ENUM_DESCRIPTION({0}, {0})", customizedEnum.Name);
                        file.WriteLine("{");
                        file.WriteLine("\tENUMCLASS_DISPLAY_INFO(L\"{0}\", L\"{1}\");", customizedEnum.DisplayName, customizedEnum.Description);
                        file.WriteLine();

                        for (int m = 0; m < customizedEnum.Members.Count; ++m)
                        {
                            if (m > 0)
                                file.WriteLine();

                            CustomizedEnum.CustomizedEnumMember member = customizedEnum.Members[m];

                            if (member.DisplayName != member.Name || !string.IsNullOrEmpty(member.Description))
                                file.WriteLine("\tDEFINE_ENUM_VALUE({0}, \"{0}\").DISPLAY_INFO(L\"{1}\", L\"{2}\");", member.Name, member.DisplayName, member.Description);
                            else
                                file.WriteLine("\tDEFINE_ENUM_VALUE({0}, \"{0}\");", member.Name);
                        }

                        file.WriteLine("}");
                        file.WriteLine("END_ENUM_DESCRIPTION()");
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

                        file.WriteLine("BEGIN_PROPERTIES_DESCRIPTION({0})", customizedStruct.Name);
                        file.WriteLine("{");
                        file.WriteLine("\tCLASS_DISPLAYNAME(L\"{0}\");", customizedStruct.DisplayName);
                        file.WriteLine("\tCLASS_DESC(L\"{0}\");", customizedStruct.Description);
                        file.WriteLine();

                        for (int m = 0; m < customizedStruct.Properties.Count; ++m)
                        {
                            if (m > 0)
                                file.WriteLine();

                            PropertyDef member = customizedStruct.Properties[m];
                            if (member.DisplayName != member.Name || !string.IsNullOrEmpty(member.BasicDescription))
                                file.WriteLine("\tREGISTER_PROPERTY({0}).DISPLAYNAME(L\"{1}\").DESC(L\"{2}\");", member.BasicName, member.DisplayName, member.BasicDescription);
                            else
                                file.WriteLine("\tREGISTER_PROPERTY({0});", member.BasicName);
                        }

                        file.WriteLine("}");
                        file.WriteLine("END_PROPERTIES_DESCRIPTION()");
                    }
                }
            }
        }

        private void ExportCustomizedMembers(string agentFolder)
        {
            bool hasNonParProperty = false;
            foreach (AgentType agent in Plugin.AgentTypes)
            {
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
                string filename = Path.Combine(agentFolder, "agentproperties.h");
                Encoding utf8WithBom = new UTF8Encoding(true);

                using (StreamWriter file = new StreamWriter(filename, false, utf8WithBom))
                {
                    // write comments
                    file.WriteLine("// ---------------------------------------------------------------------");
                    file.WriteLine("// This file is auto-generated by behaviac designer, so please don't modify it by yourself!");
                    file.WriteLine("// ---------------------------------------------------------------------\n");

                    file.WriteLine("#pragma once");
                    file.WriteLine("#include \"behaviac/agent/agent.h\"");
                    file.WriteLine("#include \"behaviac/agent/taskmethod.h\"");
                    file.WriteLine("#include \"behaviac/property/typeregister.h\"");

                    if (CustomizedTypeManager.Instance.Enums.Count > 0 || CustomizedTypeManager.Instance.Structs.Count > 0)
                    {
                        file.WriteLine("#include \"customizedtypes.h\"");
                    }

                    file.WriteLine("\nnamespace behaviac");
                    file.WriteLine("{");

                    file.WriteLine("\tclass CppBehaviorLoaderImplement : CppBehaviorLoader");
                    file.WriteLine("\t{");

                    file.WriteLine("\tpublic:");
                    file.WriteLine("\t\tCppBehaviorLoaderImplement()");
                    file.WriteLine("\t\t{");
                    file.WriteLine("\t\t\tAgentProperties::SetInstance(this);");
                    file.WriteLine("\t\t}\n");

                    // destructor
                    file.WriteLine("\t\tvirtual ~CppBehaviorLoaderImplement()");
                    file.WriteLine("\t\t{");
                    file.WriteLine("\t\t}\n");

                    // load
                    file.WriteLine("\t\tvirtual bool load()");
                    file.WriteLine("\t\t{");

                    ExportProperties(file);

                    ExportMethods(file);

                    file.WriteLine("\n\t\t\treturn true;");

                    file.WriteLine("\t\t}\n");

                    // RegisterCustomizedTypes_
                    file.WriteLine("\t\tvirtual void RegisterCustomizedTypes_()");
                    file.WriteLine("\t\t{");

                    for (int e = 0; e < CustomizedTypeManager.Instance.Enums.Count; ++e)
                    {
                        CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[e];
                        file.WriteLine("\t\t\tbehaviac::TypeRegister::Register<{0}>(\"{0}\");", customizedEnum.Name);
                    }
                    for (int s = 0; s < CustomizedTypeManager.Instance.Structs.Count; ++s)
                    {
                        CustomizedStruct customizedStuct = CustomizedTypeManager.Instance.Structs[s];
                        file.WriteLine("\t\t\tbehaviac::TypeRegister::Register<{0}>(\"{0}\");", customizedStuct.Name);
                    }

                    file.WriteLine("\t\t}\n");

                    // UnRegisterCustomizedTypes_
                    file.WriteLine("\t\tvirtual void UnRegisterCustomizedTypes_()");
                    file.WriteLine("\t\t{");

                    for (int e = 0; e < CustomizedTypeManager.Instance.Enums.Count; ++e)
                    {
                        CustomizedEnum customizedEnum = CustomizedTypeManager.Instance.Enums[e];
                        file.WriteLine("\t\t\tbehaviac::TypeRegister::UnRegister<{0}>(\"{0}\");", customizedEnum.Name);
                    }
                    for (int s = 0; s < CustomizedTypeManager.Instance.Structs.Count; ++s)
                    {
                        CustomizedStruct customizedStuct = CustomizedTypeManager.Instance.Structs[s];
                        file.WriteLine("\t\t\tbehaviac::TypeRegister::UnRegister<{0}>(\"{0}\");", customizedStuct.Name);
                    }

                    file.WriteLine("\t\t}");

                    file.WriteLine("\t};\n");

                    file.WriteLine("\tstatic CppBehaviorLoaderImplement cppBehaviorLoaderImplement;");

                    file.WriteLine("}");
                }
            }
        }

        private void ExportProperties(StreamWriter file)
        {
            file.WriteLine("\t\t\t// ---------------------------------------------------------------------");
            file.WriteLine("\t\t\t// properties");
            file.WriteLine("\t\t\t// ---------------------------------------------------------------------\n");

            file.WriteLine("\t\t\tAgentProperties* bb = NULL;");
            file.WriteLine("\t\t\tBEHAVIAC_UNUSED_VAR(bb);");

            foreach (AgentType agent in Plugin.AgentTypes)
            {
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
                    file.WriteLine("\n\t\t\t// {0}", agent.AgentTypeName);
                    file.WriteLine("\t\t\tbb = BEHAVIAC_NEW AgentProperties(\"{0}\");", agent.AgentTypeName);
                    file.WriteLine("\t\t\tAgentProperties::SetAgentTypeBlackboards(\"{0}\", bb);", agent.AgentTypeName);

                    foreach (PropertyDef prop in agent.GetProperties())
                    {
                        if (!prop.IsPar && !prop.IsArrayElement)
                        {
                            file.WriteLine("\t\t\tbb->AddProperty(\"{0}\", {1}, \"{2}\", \"{3}\", \"{4}\");",
                                DataCppExporter.GetExportNativeType(prop.NativeType),
                                prop.IsStatic ? "true" : "false",
                                prop.BasicName, prop.DefaultValue.Replace("\"", "\\\""), agent.AgentTypeName);
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

            file.WriteLine("\t\t\tCTagObjectDescriptor* objectDesc = NULL;");
            file.WriteLine("\t\t\tCCustomMethod* customeMethod = NULL;");
            file.WriteLine("\t\t\tBEHAVIAC_UNUSED_VAR(objectDesc);");
            file.WriteLine("\t\t\tBEHAVIAC_UNUSED_VAR(customeMethod);");

            foreach (AgentType agent in Plugin.AgentTypes)
            {
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
                    file.WriteLine("\n\t\t\t// {0}", agent.AgentTypeName);
                    file.WriteLine("\t\t\tobjectDesc = (CTagObjectDescriptor*)Agent::GetDescriptorByName(\"{0}\");", agent.AgentTypeName);

                    file.WriteLine("\t\t\tcustomeMethod = BEHAVIAC_NEW CTaskMethod(\"{0}\", \"root\");", agent.AgentTypeName);
                    file.WriteLine("\t\t\tobjectDesc->ms_methods.push_back(customeMethod);");

                    foreach (MethodDef method in methods)
                    {
                        if (method.IsNamedEvent)
                        {
                            file.WriteLine("\n\t\t\tcustomeMethod = BEHAVIAC_NEW CTaskMethod(\"{0}\", \"{1}\");", agent.AgentTypeName, method.BasicName);

                            foreach (MethodDef.Param param in method.Params)
                            {
                                file.WriteLine("\t\t\tcustomeMethod->AddParamType(\"{0}\");", DataCppExporter.GetExportNativeType(param.NativeType));
                            }

                            file.WriteLine("\t\t\tobjectDesc->ms_methods.push_back(customeMethod);");
                        }
                    }
                }
            }
        }
    }
}
