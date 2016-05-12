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
    public class PropertyCppExporter
    {
        public static string GenerateCode(Behaviac.Design.PropertyDef property, MethodDef.Param arrayIndexElement, bool isRefParam, StreamWriter stream, string indent, string typename, string var, string caller)
        {
            if (property.IsPar || property.IsCustomized)
                return ParInfoCppExporter.GenerateCode(property, isRefParam, stream, indent, typename, var, caller);

            string prop = GetProperty(property, arrayIndexElement, stream, indent, var, caller);

            if (!string.IsNullOrEmpty(var))
            {
                if (string.IsNullOrEmpty(typename))
                {
                    stream.WriteLine("{0}{1} = {2};", indent, var, prop);
                }
                else
                {
                    stream.WriteLine("{0}{1} {2} = {3};", indent, DataCppExporter.GetGeneratedNativeType(property.NativeType), var, prop);
                }
            }

            return prop;
        }

        public static void PostGenerateCode(Behaviac.Design.PropertyDef property, StreamWriter stream, string indent, string typename, string var, string caller, string setValue = null)
        {
            if (property.IsPar || property.IsCustomized)
            {
                ParInfoCppExporter.PostGenerateCode(property, stream, indent, typename, var, caller);
                return;
            }

            string agentName = GetGenerateAgentName(property, var, caller);
            string prop = setValue;
            if (setValue == null)
            {
                if (property.IsPublic)
                {
                    prop = string.Format("(({0}*){1})->{2}", property.ClassName, agentName, property.BasicName);
                }
                else
                {
                    string propName = property.Name.Replace("::", "_");
                    propName = propName.Replace("[]", "");
                    string nativeType = DataCppExporter.GetGeneratedNativeType(property.NativeType);
                    prop = string.Format("(({0}*){1})->_Get_Property_<{2}PROPERTY_TYPE_{3}, {4} >()", property.ClassName, agentName, getNamespace(property.ClassName), propName, nativeType);
                }
            }

            string propBasicName = property.BasicName.Replace("[]", "");
            uint id = Behaviac.Design.CRC32.CalcCRC(propBasicName);
            stream.WriteLine("{0}BEHAVIAC_ASSERT(behaviac::MakeVariableId(\"{1}\") == {2}u);", indent, propBasicName, id);
            stream.WriteLine("{0}{1}->SetVariable(\"{2}\", {3}, {4}u);", indent, agentName, propBasicName, prop, id);
        }

        private static string getNamespace(string className)
        {
            if (!string.IsNullOrEmpty(className))
            {
                int index = className.LastIndexOf(":");
                if (index > 1)
                    return className.Substring(0, index - 1) + "::";
            }

            return string.Empty;
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
                return ParInfoCppExporter.GetProperty(property, arrayIndexElement, stream, indent);

            string propName = DataCppExporter.GetPropertyBasicName(property, arrayIndexElement);
            string nativeType = DataCppExporter.GetPropertyNativeType(property, arrayIndexElement);

            if (property.IsPublic)
            {
                string className = property.ClassName;

                if (property.IsStatic)
                {
                    return string.Format("{0}::{1}", className, propName);
                }
                else
                {
                    return string.Format("(({0}*){1})->{2}", className, agentName, propName);
                }
            }

            propName = property.Name.Replace("::", "_");
            propName = propName.Replace("[]", "");
            return string.Format("(({0}*){1})->_Get_Property_<{2}PROPERTY_TYPE_{3}, {4} >()", property.ClassName, agentName, getNamespace(property.ClassName), propName, nativeType);
        }

        public static string GetProperty(Behaviac.Design.PropertyDef property, MethodDef.Param arrayIndexElement, StreamWriter stream, string indent, string var, string caller)
        {
            string agentName = GetGenerateAgentName(property, var, caller);

            if (property.Owner != Behaviac.Design.VariableDef.kSelf)
            {
                stream.WriteLine("{0}Agent* {1} = Agent::GetInstance(pAgent, \"{2}\");", indent, agentName, property.Owner);
                stream.WriteLine("{0}BEHAVIAC_ASSERT({1});", indent, agentName);
            }

            string prop = getProperty(property, arrayIndexElement, agentName, stream, indent);

            return prop;
        }
    }
}
