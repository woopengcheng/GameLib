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
using Behaviac.Design.Attributes;

namespace PluginBehaviac.DataExporters
{
    public class MethodCsExporter
    {
        public static void GenerateClassConstructor(Behaviac.Design.MethodDef method, StreamWriter stream, string indent, string var)
        {
            Debug.Check(!string.IsNullOrEmpty(var));

            string paramsName = getParamsName(var, "");
            if (!method.IsPublic)
            {
                if (method.Params.Count == 0)
                {
                    stream.WriteLine("{0}\t\t\t{1} = null;", indent, paramsName);
                }
                else
                {
                    stream.WriteLine("{0}\t\t\t{1} = new object[{2}];", indent, paramsName, method.Params.Count);
                }
            }

            for (int i = 0; i < method.Params.Count; ++i)
            {
                // const value
                if (!method.Params[i].IsProperty && !method.Params[i].IsLocalVar)
                {
                    object obj = method.Params[i].Value;
                    if (obj != null)
                    {
                        string param = getParamName(var, "", i);
                        string paramName = string.Format("{0}[{1}]", paramsName, i);
                        
                        Type type = obj.GetType();
                        if (Plugin.IsArrayType(type))
                        {
                            string typename = DataCsExporter.GetGeneratedNativeType(method.Params[i].NativeType);
                            int startIndex = typename.IndexOf('<');
                            int endIndex = typename.LastIndexOf('>');
                            string itemType = typename.Substring(startIndex + 1, endIndex - startIndex - 1);

                            ArrayCsExporter.GenerateCode(obj, stream, indent + "\t\t\t", itemType, param);
                            if (!method.IsPublic)
                            {
                                stream.WriteLine("{0}\t\t\t{1} = {2};", indent, paramName, param);
                            }
                        }
                        else if (Plugin.IsCustomClassType(type))
                        {
                            if (DesignerStruct.IsPureConstDatum(obj, method, method.Params[i].Name))
                            {
                                if (Plugin.IsRefType(type))
                                {
                                    stream.WriteLine("{0}\t\t\t{1} = null;", indent, param);
                                }
                                StructCsExporter.GenerateCode(obj, stream, indent + "\t\t\t", param, null, "");
                                if (!method.IsPublic)
                                {
                                    stream.WriteLine("{0}\t\t\t{1} = {2};", indent, paramName, param);
                                }
                            }
                        }
                        else
                        {
                            string retStr = DataCsExporter.GenerateCode(obj, stream, string.Empty, method.Params[i].NativeType, string.Empty, string.Empty);
                            if (!method.IsPublic)
                            {
                                param = paramName;
                            }
                            stream.WriteLine("{0}\t\t\t{1} = {2};", indent, param, retStr);
                        }
                    }
                }
            }
        }

        public static void GenerateClassMember(Behaviac.Design.MethodDef method, StreamWriter stream, string indent, string var)
        {
            Debug.Check(!string.IsNullOrEmpty(var));

            if (!method.IsPublic)
            {
                string paramsName = getParamsName(var, "");
                stream.WriteLine("{0}\t\tobject[] {1};", indent, paramsName);
            }

            for (int i = 0; i < method.Params.Count; ++i)
            {
                // const value
                if (!method.Params[i].IsProperty && !method.Params[i].IsLocalVar && method.Params[i].Value != null)
                {
                    Type type = method.Params[i].Value.GetType();
                    if (method.IsPublic || Plugin.IsArrayType(type) || Plugin.IsCustomClassType(type))
                    {
                        string param = getParamName(var, "", i);
                        string nativeType = DataCsExporter.GetGeneratedNativeType(method.Params[i].NativeType);
                        stream.WriteLine("{0}\t\t{1} {2};", indent, nativeType, param);
                    }
                }
            }
        }

        public static string GenerateCode(Behaviac.Design.MethodDef method, StreamWriter stream, string indent, string typename, string var, string caller)
        {
            Debug.Check(!string.IsNullOrEmpty(var) || !string.IsNullOrEmpty(caller));

            string allParams = string.Empty;
            string paramsName = getParamsName(var, caller);

            for (int i = 0; i < method.Params.Count; ++i)
            {
                string nativeType = DataCsExporter.GetGeneratedNativeType(method.Params[i].NativeType);
                string param = string.Empty;
                if (method.IsPublic)
                {
                    param = getParamName(var, caller, i);
                }
                else
                {
                    param = string.Format("{0}[{1}]", paramsName, i);
                }

                if (method.Params[i].IsProperty || method.Params[i].IsLocalVar) // property
                {
                    if ((method.Params[i].Property != null && method.Params[i].Property.IsCustomized) || method.Params[i].IsLocalVar)
                    {
                        ParameterCsExporter.GenerateCode(method.Params[i], stream, indent, method.IsPublic ? nativeType : "", param, caller);
                    }
                    else
                    {
                        if (method.IsPublic)
                        {
                            param = ParameterCsExporter.GenerateCode(method.Params[i], stream, indent, nativeType, "", param);
                        }
                        else
                        {
                            ParameterCsExporter.GenerateCode(method.Params[i], stream, indent, "", param, caller);
                        }
                    }

                    VariableDef v = method.Params[i].Value as VariableDef;
                    if (v != null && v.ArrayIndexElement != null)
                    {
                        PropertyDef prop = method.Params[i].Property;
                        if (prop != null && prop.IsArrayElement)
                        {
                            string property = PropertyCsExporter.GetProperty(prop, v.ArrayIndexElement, stream, indent, param, caller);
                            string propName = prop.BasicName.Replace("[]", "");

                            ParameterCsExporter.GenerateCode(v.ArrayIndexElement, stream, indent, "int", param + "_index", param + caller);
                            param = string.Format("({0})[{1}_index]", property, param);
                        }
                    }
                }
                else // const value
                {
                    object obj = method.Params[i].Value;
                    if (obj != null)
                    {
                        Type type = obj.GetType();
                        if (Plugin.IsCustomClassType(type) && !DesignerStruct.IsPureConstDatum(obj, method, method.Params[i].Name))
                        {
                            string paramName = getParamName(var, caller, i);
                            StructCsExporter.GenerateCode(obj, stream, indent, paramName, method, method.Params[i].Name);
                            if (!method.IsPublic)
                            {
                                stream.WriteLine("{0}{1} = {2};", indent, param, paramName);
                            }
                        }
                    }
                }

                if (i > 0)
                    allParams += ", ";

                if (method.Params[i].IsRef)
                {
                    param = "ref " + param;
                }
                else if (method.Params[i].IsOut)
                {
                    param = "out " + param;
                }

                allParams += param;
            }

            string agentName = "pAgent";
            if (method.Owner != Behaviac.Design.VariableDef.kSelf &&
                (!method.IsPublic || !method.IsStatic))
            {
                string instanceName = method.Owner.Replace("::", ".");
                agentName = "pAgent_" + caller;

                stream.WriteLine("{0}behaviac.Agent {1} = behaviac.Utils.GetParentAgent(pAgent, \"{2}\");", indent, agentName, instanceName);
                stream.WriteLine("{0}Debug.Check({1} != null || Utils.IsStaticClass(\"{2}\"));", indent, agentName, instanceName);
            }

            string retStr = string.Empty;

            if (method.IsPublic)
            {
                string className = method.ClassName.Replace("::", ".");
                if (method.IsStatic)
                {
                    retStr = string.Format("{0}.{1}({2})", className, method.BasicName, allParams);
                }
                else
                {
                    retStr = string.Format("(({0}){1}).{2}({3})", className, agentName, method.BasicName, allParams);
                }
            }
            else
            {
                retStr = string.Format("AgentExtra_Generated.ExecuteMethod({0}, \"{1}\", {2})", agentName, method.BasicName, paramsName);
            }

            if (!string.IsNullOrEmpty(var))
            {
                string nativeReturnType = DataCsExporter.GetGeneratedNativeType(method.NativeReturnType);
                string typeConvertStr = (nativeReturnType == "void") ? string.Empty : "(" + nativeReturnType + ")";

                stream.WriteLine("{0}{1} {2} = {3}{4};", indent, nativeReturnType, var, typeConvertStr, retStr);
            }

            return retStr;
        }

        public static void PostGenerateCode(Behaviac.Design.MethodDef method, StreamWriter stream, string indent, string typename, string var, string caller)
        {
            string paramsName = getParamsName(var, caller);

            for (int i = 0; i < method.Params.Count; ++i)
            {
                if (method.Params[i].IsRef || method.Params[i].IsOut)
                {
                    object obj = method.Params[i].Value;
                    if (obj != null)
                    {
                        string nativeType = DataCsExporter.GetGeneratedNativeType(method.Params[i].NativeType);
                        string param = string.Empty;
                        if (method.IsPublic)
                        {
                            param = getParamName(var, caller, i);
                        }
                        else
                        {
                            param = string.Format("{0}[{1}]", paramsName, i);
                        }

                        string paramName = string.Format("(({0}){1}[{2}])", nativeType, paramsName, i);

                        if (!method.Params[i].IsProperty && !method.Params[i].IsLocalVar)
                        {
                            Type type = obj.GetType();
                            if (!Plugin.IsArrayType(type) && !Plugin.IsCustomClassType(type))
                            {
                                param = paramName;
                            }
                        }
                        else
                        {
                            paramName = null;
                        }

                        ParameterCsExporter.PostGenerateCode(method.Params[i], stream, indent, nativeType, param, caller, method, paramName);
                    }
                }
            }
        }

        private static string getParamsName(string var, string caller)
        {
            Debug.Check(!string.IsNullOrEmpty(var) || !string.IsNullOrEmpty(caller));

            return (string.IsNullOrEmpty(var) ? caller : var) + "_params";
        }

        private static string getParamName(string var, string caller, int index)
        {
            Debug.Check(!string.IsNullOrEmpty(var) || !string.IsNullOrEmpty(caller));

            return (string.IsNullOrEmpty(var) ? caller : var) + "_p" + index;
        }
    }
}
