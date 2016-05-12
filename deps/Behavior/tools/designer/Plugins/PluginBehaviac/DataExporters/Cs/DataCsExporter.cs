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
using System.Reflection;
using Behaviac.Design;
using Behaviac.Design.Attributes;

namespace PluginBehaviac.DataExporters
{
    public class DataCsExporter
    {
        public static string GetExportNativeType(string typeName)
        {
            if (string.IsNullOrEmpty(typeName))
                return string.Empty;

            typeName = Plugin.GetNativeTypeName(typeName, true);

            typeName = typeName.Replace("unsigned ", "u");
            typeName = typeName.Replace("signed ", "");
            typeName = typeName.Replace("const ", "");
            typeName = typeName.Replace("behaviac::wstring", "string");
            typeName = typeName.Replace("behaviac::string", "string");
            typeName = typeName.Replace("std::string", "string");
            typeName = typeName.Replace("char*", "string");
            typeName = typeName.Replace("ubyte", "byte");

            typeName = typeName.Trim();

            return typeName;
        }

        public static string GetGeneratedNativeType(string typeName)
        {
            typeName = GetExportNativeType(typeName);

            typeName = typeName.Replace("::", ".");
            typeName = typeName.Replace("*", "");
            typeName = typeName.Replace("&", "");
            typeName = typeName.Replace("ullong", "ulong");
            typeName = typeName.Replace("llong", "long");

            typeName = typeName.Trim();

            if (typeName.StartsWith("vector<"))
                typeName = typeName.Replace("vector<", "List<");

            return typeName;
        }

        public static string GetGeneratedNativeType(Type type)
        {
            if (type == null)
                return string.Empty;

            if (Plugin.IsArrayType(type))
            {
                Type itemType = type.GetGenericArguments()[0];
                return string.Format("List<{0}>", DataCsExporter.GetGeneratedNativeType(itemType));
            }

            return GetGeneratedNativeType(type.Name);
        }

        public static string GetGeneratedParType(Type type)
        {
            if (type == null)
                return string.Empty;

            string typeName = GetGeneratedNativeType(type);

            if (typeName.StartsWith("List<"))
                typeName = typeName.Replace("List<", "vector<");

            return typeName;
        }

        public static string GetGeneratedDefaultValue(Type type, string typename, string defaultValue = null)
        {
            if (type == typeof(void))
                return null;

            string value = (defaultValue == null) ? DesignerPropertyUtility.RetrieveExportValue(Plugin.DefaultValue(type)) : defaultValue;

            if (type == typeof(char))
            {
                value = "(char)0";
            }
            else if (Plugin.IsStringType(type))
            {
                value = "\"" + value + "\"";
            }
            else if (Plugin.IsEnumType(type))
            {
                value = string.Format("{0}.{1}", typename, value);
            }
            else if (Plugin.IsArrayType(type))
            {
                value = "null";
            }
            else if (Plugin.IsCustomClassType(type))
            {
                if (Plugin.IsRefType(type))
                    value = "null";
                else
                    value = "new " + typename + "()";
            }

            return value;
        }

        public static string GetGeneratedPropertyDefaultValue(PropertyDef prop, string typename)
        {
            return (prop != null) ? GetGeneratedDefaultValue(prop.Type, typename, prop.DefaultValue) : null;
        }

        public static string GetPropertyBasicName(Behaviac.Design.PropertyDef property, MethodDef.Param arrayIndexElement)
        {
            string propName = property.BasicName;

            if (property != null && property.IsArrayElement && arrayIndexElement != null)
            {
                propName = propName.Replace("[]", "");
            }

            return propName;
        }

        public static string GetPropertyNativeType(Behaviac.Design.PropertyDef property, MethodDef.Param arrayIndexElement)
        {
            string nativeType = DataCsExporter.GetGeneratedNativeType(property.NativeType);

            return nativeType;
        }

        /// <summary>
        /// Generate code for the given value object.
        /// </summary>
        /// <param name="obj">The given object.</param>
        /// <param name="stream">The file stream for generating the codes.</param>
        /// <param name="indent">The indent string when generating the line of codes.</param>
        /// <param name="typename">The native type of the variable.</param>
        /// <param name="var">The variable for the given object when generating the codes.</param>
        /// <param name="caller">The caller for the method or the agent.</param>
        /// <returns>Returns the string generated value.</returns>
        public static string GenerateCode(object obj, StreamWriter stream, string indent, string typename, string var, string caller, string setValue = null)
        {
            string retStr = string.Empty;

            if (obj != null)
            {
                Type type = obj.GetType();

                if (obj is Behaviac.Design.MethodDef)
                {
                    Behaviac.Design.MethodDef method = obj as Behaviac.Design.MethodDef;
                    retStr = MethodCsExporter.GenerateCode(method, stream, indent, typename, var, caller);
                }
                else if (obj is Behaviac.Design.MethodDef.Param)
                {
                    Behaviac.Design.MethodDef.Param param = obj as Behaviac.Design.MethodDef.Param;
                    retStr = ParameterCsExporter.GenerateCode(param, stream, indent, typename, var, caller);
                }
                else if (obj is Behaviac.Design.ParInfo)
                {
                    Behaviac.Design.ParInfo par = obj as Behaviac.Design.ParInfo;
                    retStr = ParInfoCsExporter.GenerateCode(par, false, stream, indent, typename, var, caller);
                }
                else if (obj is Behaviac.Design.PropertyDef)
                {
                    Behaviac.Design.PropertyDef property = obj as Behaviac.Design.PropertyDef;
                    retStr = PropertyCsExporter.GenerateCode(property, null, false, stream, indent, typename, var, caller, setValue);
                }
                else if (obj is Behaviac.Design.VariableDef)
                {
                    Behaviac.Design.VariableDef variable = obj as Behaviac.Design.VariableDef;
                    retStr = VariableCsExporter.GenerateCode(variable, false, stream, indent, typename, var, caller);
                }
                else if (obj is Behaviac.Design.RightValueDef)
                {
                    Behaviac.Design.RightValueDef rightValue = obj as Behaviac.Design.RightValueDef;
                    retStr = RightValueCsExporter.GenerateCode(rightValue, stream, indent, typename, var, caller);
                }
                // Array type
                else if (Plugin.IsArrayType(type))
                {
                    retStr = var;

                    if (!string.IsNullOrEmpty(typename))
                    {
                        stream.WriteLine("{0}{1} {2};", indent, typename, var);
                    }
                    else
                    {
                        typename = DataCsExporter.GetGeneratedNativeType(type);
                    }

                    int startIndex = typename.IndexOf('<');
                    int endIndex = typename.LastIndexOf('>');
                    string itemType = typename.Substring(startIndex + 1, endIndex - startIndex - 1);

                    ArrayCsExporter.GenerateCode(obj, stream, indent, itemType, var);
                }
                // Struct type
                else if (Plugin.IsCustomClassType(type))
                {
                    retStr = var;

                    if (!string.IsNullOrEmpty(typename))
                    {
                        stream.WriteLine("{0}{1} {2};", indent, typename, var);
                    }

                    StructCsExporter.GenerateCode(obj, stream, indent, var, null, "");
                }
                // Other types
                else
                {
                    retStr = obj.ToString();

                    if (Plugin.IsStringType(type)) // string
                    {
                        retStr = string.Format("\"{0}\"", retStr);
                    }
                    else if (Plugin.IsCharType(type)) // char
                    {
                        char c = 'A';
                        if (retStr.Length >= 1)
                        {
                            c = retStr[0];
                        }

                        retStr = string.Format("\'{0}\'", c);
                    }
                    else if (Plugin.IsBooleanType(type)) // bool
                    {
                        retStr = retStr.ToLowerInvariant();
                    }
                    else if (Plugin.IsEnumType(type)) // enum
                    {
                        retStr = EnumCsExporter.GeneratedCode(obj);
                    }
                    else if (type == typeof(float)) // float
                    {
                        retStr += "f";
                    }

                    if (!string.IsNullOrEmpty(var))
                    {
                        if (string.IsNullOrEmpty(typename))
                        {
                            stream.WriteLine("{0}{1} = {2};", indent, var, retStr);
                        }
                        else
                        {
                            stream.WriteLine("{0}{1} {2} = {3};", indent, typename, var, retStr);
                        }
                    }
                }
            }

            return retStr;
        }
    }
}
