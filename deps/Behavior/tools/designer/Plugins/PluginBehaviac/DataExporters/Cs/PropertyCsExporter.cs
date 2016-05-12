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

namespace PluginBehaviac.DataExporters
{
    public class PropertyCsExporter
    {
        public static string GenerateCode(Behaviac.Design.PropertyDef property, MethodDef.Param arrayIndexElement, bool isRefParam, StreamWriter stream, string indent, string typename, string var, string caller, string setValue = null)
        {
            if (property.IsPar || property.IsCustomized)
                return ParInfoCsExporter.GenerateCode(property, isRefParam, stream, indent, typename, var, caller);

            string agentName = GetGenerateAgentName(property, var, caller);
            string prop = GetProperty(property, arrayIndexElement, stream, indent, var, caller);

            if (setValue == null)
            {
                if (!string.IsNullOrEmpty(var))
                {
                    if (string.IsNullOrEmpty(typename))
                    {
                        stream.WriteLine("{0}{1} = {2};", indent, var, prop);
                    }
                    else
                    {
                        string nativeType = DataCsExporter.GetPropertyNativeType(property, arrayIndexElement);

                        stream.WriteLine("{0}{1} {2} = {3};", indent, nativeType, var, prop);
                    }
                }
            }
            else
            {
                string propBasicName = property.BasicName.Replace("[]", "");
                stream.WriteLine("{0}AgentExtra_Generated.SetProperty({1}, \"{2}\", {3});", indent, agentName, propBasicName, setValue);
            }

            return prop;
        }

        public static void PostGenerateCode(Behaviac.Design.PropertyDef property, MethodDef.Param arrayIndexElement, StreamWriter stream, string indent, string typename, string var, string caller, string setValue = null)
        {
            if (property.IsPar || property.IsCustomized)
            {
                ParInfoCsExporter.PostGenerateCode(property, arrayIndexElement, stream, indent, typename, var, caller);
                return;
            }

            if (!property.IsReadonly)
            {
                string agentName = GetGenerateAgentName(property, var, caller);
                string prop = setValue;

                string propBasicName = property.BasicName.Replace("[]", "");

                if (setValue != null)
                {
                    stream.WriteLine("{0}AgentExtra_Generated.SetProperty({1}, \"{2}\", {3});", indent, agentName, propBasicName, prop);
                }
                else
                {
                    prop = getProperty(property, arrayIndexElement, agentName, stream, indent);
                }

                uint id = Behaviac.Design.CRC32.CalcCRC(propBasicName);
                stream.WriteLine("{0}Debug.Check(behaviac.Utils.MakeVariableId(\"{1}\") == {2}u);", indent, propBasicName, id);

                if (string.IsNullOrEmpty(typename))
                {
                    typename = property.NativeType;
                }
                typename = DataCsExporter.GetGeneratedNativeType(typename);

                stream.WriteLine("{0}{1}.SetVariable<{2}>(\"{3}\", {4}, {5}u);", indent, agentName, typename, property.BasicName, prop, id);

            }
        }

        public static string GetGenerateAgentName(Behaviac.Design.PropertyDef property, string var, string caller)
        {
            string agentName = "pAgent";
            if (property.Owner != Behaviac.Design.VariableDef.kSelf)
            {
                agentName = string.Format("pAgent_{0}", string.IsNullOrEmpty(var) ? caller : var);
                agentName = agentName.Replace(".", "_");
            }
            return agentName;
        }

        private static string getProperty(Behaviac.Design.PropertyDef property, MethodDef.Param arrayIndexElement, string agentName, StreamWriter stream, string indent)
        {
            if (property.IsPar || property.IsCustomized)
                return ParInfoCsExporter.GetProperty(property, arrayIndexElement, stream, indent);

            string propName = DataCsExporter.GetPropertyBasicName(property, arrayIndexElement);
            string nativeType = DataCsExporter.GetPropertyNativeType(property, arrayIndexElement);

            if (property.IsPublic)
            {
                string className = property.ClassName.Replace("::", ".");

                if (property.IsStatic)
                {
                    return string.Format("{0}.{1}", className, propName);
                }
                else
                {
                    return string.Format("(({0}){1}).{2}", className, agentName, propName);
                }
            }

            return string.Format("({0})AgentExtra_Generated.GetProperty({1}, \"{2}\")", nativeType, agentName, propName);
        }

        public static string GetProperty(Behaviac.Design.PropertyDef property, MethodDef.Param arrayIndexElement, StreamWriter stream, string indent, string var, string caller)
        {
            string agentName = GetGenerateAgentName(property, var, caller);

            if (property.Owner != Behaviac.Design.VariableDef.kSelf)
            {
                string instanceName = property.Owner.Replace("::", ".");

                stream.WriteLine("{0}behaviac.Agent {1} = behaviac.Utils.GetParentAgent(pAgent, \"{2}\");", indent, agentName, instanceName);
                stream.WriteLine("{0}Debug.Check({1} != null || Utils.IsStaticClass(\"{2}\"));", indent, agentName, instanceName);
            }

            string prop = getProperty(property, arrayIndexElement, agentName, stream, indent);

            return prop;
        }
    }
}
