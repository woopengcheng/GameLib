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
using System.Reflection;
using Behaviac.Design.Attributes;
using Behaviac.Design.Properties;

namespace Behaviac.Design
{
    public interface UIObject
    {
        Behaviac.Design.ObjectUI.ObjectUIPolicy CreateUIPolicy();
        object[] GetExcludedEnums(DesignerEnum enumAttr);
    }

    public interface DefaultObject : UIObject
    {
        string Description { get; }
        string Label { get; }
        string DocLink { get; }
        Nodes.BehaviorNode Behavior { get; }
        int Id { get; }
        bool CanBeAttached { get; }
        bool IsFSM { get; }
    }

    public interface ISerializableData
    {
        string GetDisplayValue();
        string GetExportValue();
    }

    public enum Language { System = 0, English, Chinese };

    [Flags]
    public enum ValueTypes
    {
        //return bool
        Bool = 1,

        //return byte, short, int, long
        Int = 2,

        //return float, double
        Float = 4,

        String = 8, 

        Array = 16,

        //all types
        All = Bool | Int | Float | String
    }

    [Behaviac.Design.EnumDesc("PluginBehaviac.Nodes.ComputeOpr", "计算作符", "计算操作符选择")]
    public enum ComputeOperator
    {
        [Behaviac.Design.EnumMemberDesc("Add", "+")]
        Add,

        [Behaviac.Design.EnumMemberDesc("Sub", "-")]
        Sub,

        [Behaviac.Design.EnumMemberDesc("Mul", "*")]
        Mul,

        [Behaviac.Design.EnumMemberDesc("Div", "/")]
        Div,

        //[Behaviac.Design.EnumMemberDesc("Invalid", "x")]
        //Invalid
    }

    [Behaviac.Design.EnumDesc("PluginBehaviac.Nodes.ComputeOpr", "计算作符", "计算操作符选择")]
    public enum OperatorTypes
    {
        [Behaviac.Design.EnumMemberDesc("Assign", "=")]
        Assign,

        [Behaviac.Design.EnumMemberDesc("Add", "+")]
        Add,

        [Behaviac.Design.EnumMemberDesc("Sub", "-")]
        Sub,

        [Behaviac.Design.EnumMemberDesc("Mul", "*")]
        Mul,

        [Behaviac.Design.EnumMemberDesc("Div", "/")]
        Div,

        [Behaviac.Design.EnumMemberDesc("Equal", "==")]
        Equal,

        [Behaviac.Design.EnumMemberDesc("NotEqual", "!=")]
        NotEqual,

        [Behaviac.Design.EnumMemberDesc("Greater", ">")]
        Greater,

        [Behaviac.Design.EnumMemberDesc("Less", "<")]
        Less,

        [Behaviac.Design.EnumMemberDesc("GreaterEqual", ">=")]
        GreaterEqual,

        [Behaviac.Design.EnumMemberDesc("LessEqual", "<=")]
        LessEqual,

        [Behaviac.Design.EnumMemberDesc("Invalid", "#")]
        Invalid
    }

    [Flags]
    public enum MethodType
    {
        None = 0,

        //all the methods
        Method = 1,

        //getter is the method who returns something
        Getter = 2,

        //getter is the method who returns something plus the name event
        Event_Obsolete = 4,

        AllowNullMethod = 8,

        Status = 16,

        Task = 32
    }

    public enum MemberType
    {
        Property = 0,
        Method,
        Task
    }

    public class MethodDef : ICloneable, ISerializableData
    {
        public class Param
        {
            private ParameterInfo _paramInfo = null;

            private string _name = "";
            public string Name
            {
                get
                {
                    if (_paramInfo != null)
                    { return _paramInfo.Name; }

                    return (this._property.Property != null) ? this._property.Property.Name : _name;
                }

                set
                {
                    _name = value;

                    if (_type != null)
                    { _attribute = Plugin.InvokeTypeCreateDesignerProperty("Arguments", _name, _type, float.MinValue, float.MaxValue); }
                }
            }

            private Type _type = null;
            public Type Type
            {
                get
                {
                    if (_paramInfo != null)
                    { return _paramInfo.ParameterType; }

                    if (_bParamFromStruct)
                    {
                        ParInfo par = this._value as ParInfo;

                        if (par != null)
                        { return par.Type; }

                        return this._property.Property.PropertyType;
                    }

                    if (this._value != null)
                    {
                        if (this._value is VariableDef)
                        {
                            VariableDef varDef = this._value as VariableDef;
                            return varDef.ValueType;
                        }

                        return this._value.GetType();
                    }

                    return _type;
                }

                set
                {
                    _type = value;
                    _nativeType = Plugin.GetNativeTypeName(_type);

                    if (_type != null)
                    {
                        _attribute = Plugin.InvokeTypeCreateDesignerProperty("Arguments", _name, _type, float.MinValue, float.MaxValue);
                        _value = Plugin.DefaultValue(_type);
                    }
                }
            }

            private bool _isOut = false;
            public bool IsOut
            {
                get { return _isOut; }
                set
                {
                    _isOut = value;

                    this._nativeType = this._nativeType.Replace("&", "");

                    if (value)
                    { this._nativeType += "&"; }
                }
            }

            private bool _isRef = false;
            public bool IsRef
            {
                get { return _isRef; }
                set
                {
                    _isRef = value;

                    this._nativeType = this._nativeType.Replace("&", "");

                    if (value)
                    { this._nativeType += "&"; }
                }
            }

            private object _value = null;
            public object Value
            {
                get { return _value; }
                set
                {
                    _value = value;

                    if (_bParamFromStruct && _value != null)
                    {
                        //_property.Property.SetValue(_value, _object, null);
                        if ((_value is VariableDef) || (_value is ParInfo))
                        {
                            //do nothing
                        }
                        else
                        {
                            string valueStr = _value.ToString();
                            _property.SetValueFromString(null, _object, valueStr, null);
                        }
                    }
                }
            }

            private DesignerProperty _attribute;
            public DesignerProperty Attribute
            {
                get { return _attribute; }
            }

            private string _nativeType;
            public string NativeType
            {
                get { return _nativeType; }
            }

            private string _displayName;
            public string DisplayName
            {
                get { return string.IsNullOrEmpty(_displayName) ? this.Name : _displayName; }
                set { _displayName = value; }
            }

            private string _description;
            public string Description
            {
                get { return _description; }
            }

            private bool _bParamFromStruct = false;
            public bool IsFromStruct
            {
                get { return this._bParamFromStruct; }
            }

            private bool _isArrayIndex = false;
            public bool IsArrayIndex
            {
                get { return this._isArrayIndex; }
                set { this._isArrayIndex = value; }
            }

            private DesignerPropertyInfo _property;
            private object _object = null;
            public Param(DesignerPropertyInfo property, object obj)
            {
                _paramInfo = null;
                _property = property;
                _object = obj;
                _value = property.GetValue(obj);
                _nativeType = (_value != null) ? Plugin.GetNativeTypeName(_value.GetType()) : string.Empty;
                DesignerProperty pattr = property.Attribute;
                _displayName = pattr.DisplayName;
                _description = pattr.Description;
                _attribute = pattr;

                _bParamFromStruct = true;
            }

            public Param(string category, ParameterInfo pi, object v, string nativeType, string displayName, string description, bool isOut, bool isRef, float rangeMin, float rangeMax)
            {
                _paramInfo = pi;
                _value = v;
                _nativeType = nativeType;
                _isOut = isOut;
                _isRef = isRef;
                _displayName = displayName;
                _description = description;

                if (_paramInfo != null)
                {
                    DesignerProperty attr = null;

                    DesignerProperty[] attributes = (DesignerProperty[])_paramInfo.GetCustomAttributes(typeof(DesignerProperty), false);

                    if (attributes.Length > 0)
                    { attr = attributes[0]; }

                    if (attr != null)
                    { _attribute = attr; }

                    else if (_paramInfo.ParameterType.IsEnum)
                    { _attribute = new DesignerEnum(_paramInfo.Name, "", category, DesignerProperty.DisplayMode.Parameter, 0, DesignerProperty.DesignerFlags.NoFlags, ""); }

                    else
                    { _attribute = Plugin.InvokeTypeCreateDesignerProperty(category, _paramInfo.Name, _paramInfo.ParameterType, rangeMin, rangeMax); }
                }
            }

            public Param(Param other)
            {
                _paramInfo = other._paramInfo;
                _attribute = other._attribute;
                _nativeType = other._nativeType;
                _displayName = other._displayName;
                _description = other._description;
                _name = other._name;
                _type = other._type;
                _isOut = other._isOut;
                _isRef = other._isRef;
                _value = Plugin.CloneValue(other._value);
                _isArrayIndex = other._isArrayIndex;
            }

            // Customized Parameter
            public Param(string name, Type type, string nativeName, string displayName, string description)
            {
                _name = name;
                _type = type;
                _nativeType = Plugin.GetNativeTypeName(type);
                _displayName = displayName;
                _description = description;

                if (_type != null)
                {
                    _attribute = Plugin.InvokeTypeCreateDesignerProperty("Arguments", _name, _type, float.MinValue, float.MaxValue);
                    _value = Plugin.DefaultValue(_type);
                }
            }

            public string GetDisplayValue(object obj)
            {
                ParInfo par = this._value as ParInfo;

                if (par != null)
                {
                    return par.GetDisplayValue();
                }

                VariableDef var = this._value as VariableDef;

                if (var != null)
                {
                    return var.GetDisplayValue();
                }

                //if (obj == null)
                //{
                //    return this._value.ToString();
                //}

                return _attribute.GetDisplayValue(this._value);
            }

            public string GetExportValue(object obj)
            {
                //Debug.Check(_bParamFromStruct);

                ParInfo par = this._value as ParInfo;

                if (par != null)
                {
                    return par.GetExportValue();
                }

                VariableDef var = this._value as VariableDef;

                if (var != null)
                {
                    return var.GetExportValue();
                }

                if (obj == null)
                {
                    return this._value.ToString();
                }

                return this._attribute.GetExportValue(obj, this._value);
            }

            public bool IsLocalVar
            {
                get
                {
                    if (this._value is ParInfo)
                    { return true; }

                    VariableDef v = this._value as VariableDef;
                    return (v != null) ? v.IsLocalVar : false;
                }
            }

            public bool IsProperty
            {
                get
                {
                    VariableDef v = this._value as VariableDef;
                    return (v != null) ? v.IsProperty : false;
                }
            }

            public bool IsPureConstDatum
            {
                get
                {
                    ParInfo par = this._value as ParInfo;

                    if (par != null)
                    { return false; }

                    VariableDef v = this._value as VariableDef;

                    if (v != null)
                    { return v.IsConst; }

                    return true;
                }
            }

            public PropertyDef Property
            {
                get
                {
                    VariableDef v = this._value as VariableDef;

                    if (v != null && v.IsProperty)
                    { return v.Property; }

                    return null;
                }
            }
        }

        public MethodDef(AgentType agentType, MemberType memberType, bool isChangeableType, bool isPublic, bool isStatic, string classname, string owner, string name, string displayName, string description, string nativeReturnType, Type returnType, bool isActionMethodOnly, List<Param> pars)
        {
            _agentType = agentType;
            _isChangeableType = isChangeableType;
            _isPublic = isPublic;
            _isStatic = isStatic;
            _classname = classname;
            _owner = owner;
            _name = name;
            _displayName = displayName;
            _description = description;
            _nativeReturnType = string.IsNullOrEmpty(nativeReturnType) ? Plugin.GetNativeTypeName(returnType) : nativeReturnType;
            _returnType = returnType;
            _isActionMethodOnly = isActionMethodOnly;
            _params = pars;
            _memberType = memberType;
            _isCustomized = (memberType == Design.MemberType.Task);
        }

        public MethodDef(MethodDef other)
        {
            CopyFrom(other);
        }

        // Customized Method
        public MethodDef(AgentType agentType, MemberType memberType, string classname, string name, string displayName, string description, string nativeReturnType, Type returnType)
        {
            _agentType = agentType;
            _isChangeableType = false;
            _isCustomized = true;
            _memberType = memberType;
            _isPublic = true;
            _isStatic = false;
            _classname = classname;
            _owner = classname;
            _name = name;
            _displayName = displayName;
            _description = description;
            _nativeReturnType = string.IsNullOrEmpty(nativeReturnType) ? Plugin.GetNativeTypeName(returnType) : nativeReturnType;
            _returnType = returnType;
            _isActionMethodOnly = false;
        }

        public object Clone()
        {
            return new MethodDef(this);
        }

        public void CopyFrom(MethodDef other)
        {
            _agentType = other._agentType;
            _isChangeableType = other._isChangeableType;
            _isPublic = other._isPublic;
            _isStatic = other._isStatic;
            _classname = other._classname;
            _owner = other._owner;
            _name = other._name;
            _displayName = other._displayName;
            _description = other._description;
            _nativeReturnType = other._nativeReturnType;
            _returnType = other._returnType;
            _isActionMethodOnly = other._isActionMethodOnly;
            _memberType = other._memberType;
            _isCustomized = other._isCustomized;

            _params.Clear();
            foreach (Param param in other.Params)
                _params.Add(new Param(param));
        }

        AgentType _agentType = null;
        public AgentType AgentType
        {
            get { return _agentType; }
        }

        bool _isChangeableType = false;
        public bool IsChangeableType
        {
            get { return _isChangeableType; }
        }

        private bool _isInherited = false;
        public bool IsInherited
        {
            get { return _isInherited; }
            set { _isInherited = value; }
        }

        bool _isCustomized = false;
        public bool IsCustomized
        {
            get { return !IsInherited && _isCustomized; }
            set { _isCustomized = value; }
        }

        bool _isPublic = true;
        public bool IsPublic
        {
            get { return _isPublic; }
            set { _isPublic = value; }
        }

        bool _isStatic = false;
        public bool IsStatic
        {
            get { return _isStatic; }
            set { _isStatic = value; }
        }

        private string _owner = string.Empty;
        public string Owner
        {
            get { return _owner; }
            set { _owner = value; }
        }

        private string _name = string.Empty;
        public string Name
        {
            get
            {
                // full name
                return _name.Contains(":") ? _name : (this.ClassName + "::" + _name);
            }
            set
            {
                // full name
                _name = value.Contains(":") ? value : (this.ClassName + "::" + value);
            }
        }

        // The Old Name is only used for modifying the method
        private string _oldName = string.Empty;
        public string OldName
        {
            get { return _oldName; }
            set { _oldName = value; }
        }

        public string BasicName
        {
            get
            {
                if (!string.IsNullOrEmpty(_name))
                {
                    string[] name = _name.Split(new char[] { ':' }, StringSplitOptions.RemoveEmptyEntries);

                    if (name.Length > 0)
                    { return name[name.Length - 1]; }
                }

                return string.Empty;
            }
        }

        private string _classname = string.Empty;
        public string ClassName
        {
            get
            {
                if (!string.IsNullOrEmpty(_classname))
                { return _classname; }

                if (!string.IsNullOrEmpty(_name))
                {
                    int index = _name.LastIndexOf(":");

                    if (index != -1)
                    { return _name.Substring(0, index - 1); }
                }

                return _owner;
            }
        }

        public string PrototypeName
        {
            get
            {
                string ret = this.DisplayName;
                string paraStr = "(";

                for (int i = 0; i < this._params.Count; ++i)
                {
                    if (i > 0)
                    {
                        paraStr += ",";
                    }

                    paraStr += this._params[i].DisplayName;
                }

                paraStr += ")";

                if (this._params.Count > 0)
                {
                    ret += paraStr;
                }

                return ret;
            }
        }

        private MemberType _memberType = MemberType.Method;
        public MemberType MemberType
        {
            get { return this._memberType; }
        }
        public bool IsNamedEvent
        {
            get { return _memberType == MemberType.Task; }
        }

        private string _nativeReturnType = "void";
        public string NativeReturnType
        {
            get { return _nativeReturnType; }
            set { _nativeReturnType = value; }
        }

        private Type _returnType = typeof(void);
        public Type ReturnType
        {
            get { return _returnType; }
            set { _returnType = value; }
        }

        private string _displayName = string.Empty;
        public string DisplayName
        {
            get
            {
                if (Plugin.UseBasicDisplayName && !string.IsNullOrEmpty(_displayName))
                {
                    string[] name = _displayName.Split(new char[] { ':' }, StringSplitOptions.RemoveEmptyEntries);

                    if (name.Length > 0)
                    {
                        //if (name.Length > 1 && Plugin.IsInstanceName(name[0], null))
                        //    return _displayName;

                        return name[name.Length - 1];
                    }
                }

                return !string.IsNullOrEmpty(_displayName) ? _displayName : this.BasicName;
            }

            set { _displayName = value; }
        }

        public string FullDisplayName
        {
            get { return _displayName; }
        }

        private string _description = string.Empty;
        public string BasicDescription
        {
            get { return _description; }
            set { _description = value; }
        }

        public string Description
        {
            get
            {
                return Resources.Prototype + "  " + this.NativeReturnType + " " + this.Name + this.GetParamPrototype() + "\n" +
                Resources.Description + "  " + _description;
            }
        }

        private List<Param> _params = new List<Param>();
        public List<Param> Params
        {
            get { return _params; }
        }

        private bool _isActionMethodOnly = false;
        public bool IsActionMethodOnly
        {
            get { return _isActionMethodOnly; }
        }

        abstract class StructParamBase_t
        {
            public abstract List<Param> GetParams(int index);
            public abstract void AddParam(int index, Param param);
        }

        class StructParam_t : StructParamBase_t
        {
            public Type type;
            private List<Param> ps;

            public StructParam_t()
            {
                type = null;
                this.ps = new List<Param>();
            }

            public override List<Param> GetParams(int index)
            {
                Debug.Check(index == -1);
                return this.ps;
            }

            public override void AddParam(int index, Param param)
            {
                Debug.Check(index == -1);
                this.ps.Add(param);
            }
        };

        class StructArrayParam_t : StructParamBase_t
        {
            private Dictionary<int, List<Param>> ps;

            public StructArrayParam_t()
            {
                ps = new Dictionary<int, List<Param>>();
            }

            public override List<Param> GetParams(int index)
            {
                Debug.Check(index != -1);

                if (this.ps.ContainsKey(index))
                {
                    return this.ps[index];
                }

                return null;
            }

            public override void AddParam(int index, Param param)
            {
                Debug.Check(index != -1);

                if (!this.ps.ContainsKey(index))
                {
                    this.ps[index] = new List<Param>();
                }

                this.ps[index].Add(param);
            }
        };

        private Dictionary<string, StructParamBase_t> _structParams = new Dictionary<string, StructParamBase_t>();
        public List<Param> GetParams(DesignerStructPropertyInfo structParam)
        {
            if (_structParams.ContainsKey(structParam.Name))
            {
                List<Param> ps = _structParams[structParam.Name].GetParams(structParam.ElmentIndexInArray);

                if (ps != null)
                {
                    return ps;
                }

            }
            else
            {
                if (structParam.ElmentIndexInArray == -1)
                {
                    _structParams[structParam.Name] = new StructParam_t();

                }
                else
                {
                    _structParams[structParam.Name] = new StructArrayParam_t();
                }

            }

            IList<DesignerPropertyInfo> properties = DesignerProperty.GetDesignerProperties(structParam.Type, DesignerProperty.SortByDisplayOrder);
            foreach (DesignerPropertyInfo property in properties)
            {
                Param v = new Param(property, structParam.Owner);
                _structParams[structParam.Name].AddParam(structParam.ElmentIndexInArray, v);
            }

            return _structParams[structParam.Name].GetParams(structParam.ElmentIndexInArray);
        }

        public Param GetParam(string paramName, DesignerPropertyInfo param, int indexInArray = -1)
        {
            if (_structParams.ContainsKey(paramName))
            {
                List<Param> ps = _structParams[paramName].GetParams(indexInArray);

                if (ps != null)
                {
                    foreach (Param p in ps)
                    {
                        if (p.Name == param.Property.Name && p.Type == param.Property.PropertyType)
                        {
                            return p;
                        }
                    }

                }
                else
                {
                    Debug.Check(true);
                }
            }

            return null;
        }

        public Param GetParam(string paramName, Type strutType, object obj, DesignerPropertyInfo param)
        {
            int indexInArray = -1;

            if (!_structParams.ContainsKey(paramName))
            {
                StructParam_t ps0 = new StructParam_t();
                _structParams[paramName] = ps0;
                ps0.type = obj.GetType();

                IList<DesignerPropertyInfo> properties = DesignerProperty.GetDesignerProperties(strutType, DesignerProperty.SortByDisplayOrder);
                foreach (DesignerPropertyInfo property in properties)
                {
                    object member = property.GetValue(obj);

                    Param v = new Param(property, obj);
                    _structParams[paramName].AddParam(-1, v);
                }
            }

            StructParam_t ps1 = _structParams[paramName] as StructParam_t;
            Debug.Check(ps1 != null);

            if (ps1.type == obj.GetType())
            {
                List<Param> ps = _structParams[paramName].GetParams(indexInArray);

                if (ps != null)
                {
                    foreach (Param p in ps)
                    {
                        if (p.Name == param.Property.Name && p.Type == param.Property.PropertyType)
                        {
                            return p;
                        }
                    }

                }
                else
                {
                    Debug.Check(true);
                }
            }

            return null;
        }
#if USE_NOOP
    private static MethodDef _noop = null;
    public static MethodDef Noop {
        get
        {
            if (_noop == null) {
                _noop = new MethodDef("", typeof(void), new List<Param>());
            }

            return _noop;
        }
    }
#endif//#if USE_NOOP

        public bool CheckPar(ParInfo par)
        {
            foreach (MethodDef.Param param in this.Params)
            {
                if (param.Value is VariableDef)
                {
                    VariableDef var = param.Value as VariableDef;
                    bool bOk = var.CheckPar(par);

                    if (bOk)
                    {
                        return true;
                    }

                }
                else if (param.Value is ParInfo)
                {
                    if (par.GetExportValue() == ((ParInfo)(param.Value)).GetExportValue())
                    { return true; }
                }
            }

            return false;
        }

        public bool ShouldBeCleared(AgentType agentType)
        {
            return (this.Owner == VariableDef.kSelf &&
                    !Plugin.IsAgentDerived(this.AgentType.AgentTypeName, agentType.AgentTypeName));
        }

        public bool ResetMembers(bool check, AgentType agentType, bool clear, MethodDef method, PropertyDef property)
        {
            bool bReset = false;

            if (method != null)
            {
                if (method.OldName == this.Name)
                {
                    bReset = true;

                    if (!check)
                    {
                        this.CopyFrom(method);
                    }
                }

            }
            else if (property != null)
            {
                foreach (MethodDef.Param param in this.Params)
                {
                    if (param.Value is VariableDef)
                    {
                        VariableDef var = param.Value as VariableDef;
                        bReset |= var.ResetMembers(check, agentType, clear, property);

                    }
                    else if (param.Value is ParInfo)
                    {
                        ParInfo par = param.Value as ParInfo;

                        if (property.IsPar && (property.OldName == par.Name ||
                        !property.IsArrayElement && par.IsArrayElement && (property.OldName + "[]") == par.Name))
                        {
                            if (clear || this.ShouldBeCleared(agentType))
                            {
                                bReset = true;

                                if (!check)
                                { param.Value = Plugin.DefaultValue(param.Type); }

                            }
                            else
                            {
                                bReset = true;

                                if (!check)
                                {
                                    bool isArrayElement = par.IsArrayElement;

                                    par.CopyFrom(property);

                                    if (isArrayElement)
                                    { par.SetArrayElement(property); }
                                }
                            }
                        }
                    }
                }
            }

            return bReset;
        }

        public override string ToString()
        {
            return this.Name;
        }

        public string GetDisplayValue()
        {
            return getStringValue(true);
        }

        public string GetExportValue()
        {
            return getStringValue(false);
        }

        private string getStringValue(bool isDisplay)
        {
            if (this.Name == "NullAgent::null_method" && this.AgentType == null)
            {
                return string.Empty;
            }

#if USE_NOOP

        if (MethodDef != MethodDef.Noop)
#endif
            {
                string str = string.Empty;

                if (isDisplay)
                {
                    if (this.Owner != VariableDef.kSelf)
                    {
                        str = Plugin.GetInstanceDisplayName(this.Owner) + "." + this.DisplayName;
                    }
                    else
                    {
                        str = this.DisplayName;
                    }
                }
                else
                {
                    str = this.Owner + "." + this.Name;
                }

                str += "(";

                for (int i = 0; i < this.Params.Count; ++i)
                {
                    if (i > 0)
                    { str += ","; }

                    object para = this.Params[i].Value;
                    str += isDisplay ? DesignerPropertyUtility.RetrieveDisplayValue(para, this, this.Params[i].Name) : DesignerPropertyUtility.RetrieveExportValue(para, this, this.Params[i].Name);
                }

                str += ")";

                return str;
            }

#if USE_NOOP
        return string.Empty;
#endif
        }

        public string GetParamPrototype()
        {
            string str = "(";

            for (int i = 0; i < this.Params.Count; ++i)
            {
                if (i > 0)
                { str += ", "; }

                str += this.Params[i].NativeType;
            }

            str += ")";

            return str;
        }

        public string GetPrototype()
        {
            string str = this.Owner + "." + this.DisplayName + "(";

            for (int i = 0; i < this.Params.Count; ++i)
            {
                if (i > 0)
                { str += ", "; }

                str += this.Params[i].NativeType + " " + this.Params[i].DisplayName;
            }

            str += ")";

            return str;
        }
    }


    public class PropertyDef : ISerializableData
    {
        // Meta Property
        public PropertyDef(AgentType agentType, FieldInfo pi, bool isChangeableType, bool isStatic, bool isPublic, bool isProperty, bool isReadonly, string classname, string owner, string name, string nativeType, string displayName, string description)
        {
            _agentType = agentType;
            _propertyInfo = pi;
            _isChangeableType = isChangeableType;
            _isStatic = isStatic;
            _isPublic = isPublic;
            _isProperty = isProperty;
            _isReadonly = isReadonly;
            _classname = classname;
            _owner = owner;
            _name = name;
            _nativeType = nativeType;
            _displayName = displayName;
            _description = description;
        }

        // Customized Property
        public PropertyDef(AgentType agentType, Type type, string classname, string name, string displayName, string description)
        {
            _agentType = agentType;
            _propertyInfo = null;
            _type = type;
            _isChangeableType = false;
            _isStatic = false;
            _isPublic = true;
            _isProperty = false;
            _classname = classname;
            _owner = VariableDef.kSelf;
            _name = name;
            _nativeType = Plugin.GetNativeTypeName(type);
            _displayName = displayName;
            _description = description;
        }

        public PropertyDef(PropertyDef other)
        {
            this.CopyFrom(other);
        }

        protected PropertyDef()
        {
        }

        public virtual PropertyDef Clone()
        {
            return new PropertyDef(this);
        }

        public virtual void CopyFrom(PropertyDef other)
        {
            if (other == null)
                return;

            _agentType = other._agentType;
            _propertyInfo = other._propertyInfo;
            _type = other._type;
            _isChangeableType = other._isChangeableType;
            _isStatic = other._isStatic;
            _isPublic = other._isPublic;
            _isProperty = other._isProperty;
            _isReadonly = other._isReadonly;
            _classname = other._classname;
            _owner = other._owner;
            _name = other._name;
            _nativeType = other._nativeType;
            _displayName = other._displayName;
            _description = other._description;
            _isAddedAutomatically = other._isAddedAutomatically;
            _isArrayElement = other._isArrayElement;
            _isExportedButAlsoCustomized = other._isExportedButAlsoCustomized;

            if (other._variable != null)
            { _variable = (VariableDef)other._variable.Clone(); }
        }

        public void SetArrayElement(PropertyDef arrayProperty)
        {
            Debug.Check(arrayProperty != null && arrayProperty.Type != null && Plugin.IsArrayType(arrayProperty.Type));

            this.IsArrayElement = true;
            this.IsAddedAutomatically = true;
            this.Type = arrayProperty.Type.GetGenericArguments()[0];
            this.Name = arrayProperty.Name + "[]";
            this.DisplayName = arrayProperty.DisplayName + "[]";
            this.Description = this.DisplayName;
        }

        protected AgentType _agentType = null;
        public AgentType AgentType
        {
            get { return _agentType; }
        }

        private bool _isInherited = false;
        public bool IsInherited
        {
            get { return _isInherited; }
            set { _isInherited = value; }
        }

        private FieldInfo _propertyInfo = null;
        public bool IsMember
        {
            get { return _propertyInfo != null; }
        }

        private bool _isChangeableType = false;
        public bool IsChangeableType
        {
            get { return _isChangeableType; }
        }

        public virtual bool IsCustomized
        {
            get { return !IsInherited && !IsMember; }
        }

        private bool _isExportedButAlsoCustomized = false;
        public bool IsExportedButAlsoCustomized
        {
            get { return _isExportedButAlsoCustomized; }
            set { _isExportedButAlsoCustomized = value; }
        }

        public virtual bool IsPar
        {
            get { return false; }
        }

        protected Type _type = null;
        public virtual Type Type
        {
            get
            {
                if (_type != null)
                    return _type;

                if (_propertyInfo != null)
                    return _propertyInfo.FieldType;

                if (this.Variable != null)
                    return this.Variable.ValueType;

                return null;
            }

            set
            {
                _type = value;
            }
        }

        protected bool _isStatic = false;
        public bool IsStatic
        {
            get { return _isStatic; }
            set { _isStatic = value; }
        }

        protected bool _isPublic = true;
        public bool IsPublic
        {
            get { return _isPublic; }
            set { _isPublic = value; }
        }

        protected bool _isProperty = false;
        public bool IsProperty
        {
            get { return _isProperty; }
            set { _isProperty = value; }
        }

        protected bool _isReadonly = false;
        public bool IsReadonly
        {
            get { return _isReadonly; }
            set { _isReadonly = value; }
        }

        protected bool _isArrayElement = false;
        public bool IsArrayElement
        {
            get { return this._isArrayElement; }
            set { this._isArrayElement = value; }
        }

        public virtual void OnValueChanged()
        {
        }

        private VariableDef _variable = null;
        public VariableDef Variable
        {
            get { return _variable; }
            set
            {
                _variable = value;
                OnValueChanged();
            }
        }

        public string DefaultValue
        {
            get
            {
                string valueStr = string.Empty;

                if (Variable == null)
                {
                    //return string.Empty;
                    valueStr = DesignerPropertyUtility.RetrieveExportValue(Plugin.DefaultValue(this.Type));

                }
                else
                {
                    valueStr = Variable.GetRawValue();
                }

                // "string" - > string
                if (Plugin.IsStringType(this.Type))
                {
                    valueStr = valueStr.Substring(1, valueStr.Length - 2);
                }

                return valueStr;
            }
        }

        protected string _owner = string.Empty;
        public string Owner
        {
            get { return _owner; }
            set { _owner = value; }
        }

        protected string _name = string.Empty;
        public virtual string Name
        {
            get
            {
                if (string.IsNullOrEmpty(_name))
                    return string.Empty;

                // full name
                return _name.Contains(":") ? _name : (this.ClassName + "::" + _name);
            }
            set
            {
                // full name
                _name = value.Contains(":") ? value : (this.ClassName + "::" + value);
            }
        }

        // The Old Name is only used for modifying the method
        private string _oldName = string.Empty;
        public string OldName
        {
            get { return _oldName; }
            set { _oldName = value; }
        }

        public string BasicName
        {
            get
            {
                if (!string.IsNullOrEmpty(this.Name))
                {
                    string[] name = this.Name.Split(new char[] { ':' }, StringSplitOptions.RemoveEmptyEntries);

                    if (name.Length > 0)
                    { return name[name.Length - 1]; }
                }

                return string.Empty;
            }
        }

        protected string _classname = string.Empty;
        public string ClassName
        {
            get
            {
                if (!string.IsNullOrEmpty(_classname))
                { return _classname; }

                if (!string.IsNullOrEmpty(_name))
                {
                    int index = _name.LastIndexOf(":");

                    if (index != -1)
                    { return _name.Substring(0, index - 1); }
                }

                return _owner;
            }
        }

        protected string _nativeType;
        public string NativeType
        {
            get
            {
                string nativeType = _nativeType;
                if (string.IsNullOrEmpty(nativeType))
                {
                    nativeType = Plugin.GetNativeTypeName(this.Type);
                }

                //if (!nativeType.EndsWith("*") && Plugin.IsRefType(this.Type))
                //{
                //    nativeType += "*";
                //}

                return nativeType;
            }

            set { _nativeType = value; }
        }

        public string NativeItemType
        {
            get
            {
                if (this.IsArrayElement && !string.IsNullOrEmpty(this.NativeType))
                {
                    int startIndex = this.NativeType.IndexOf("<");
                    if (startIndex > 0)
                        return this.NativeType.Substring(startIndex + 1, this.NativeType.Length - startIndex - 2);
                }

                return this.NativeType;
            }
        }

        protected string _displayName = string.Empty;
        public string DisplayName
        {
            get
            {
                if (Plugin.UseBasicDisplayName && !string.IsNullOrEmpty(_displayName))
                {
                    string[] name = _displayName.Split(new char[] { ':' }, StringSplitOptions.RemoveEmptyEntries);

                    if (name.Length > 0)
                    {
                        //if (name.Length > 1 && Plugin.IsInstanceName(name[0], null))
                        //    return _displayName;

                        return name[name.Length - 1];
                    }
                }

                return string.IsNullOrEmpty(_displayName) ? this.BasicName : _displayName;
            }

            set { _displayName = value; }
        }

        public string FullDisplayName
        {
            get { return _displayName; }
        }

        protected string _description = string.Empty;
        public string BasicDescription
        {
            get { return string.IsNullOrEmpty(_description) ? this.BasicName : _description; }
            set { _description = value; }
        }

        public virtual string Description
        {
            get
            {
                return Resources.Prototype + "  " + this.NativeType + " " + this.Name + "\n" +
                Resources.Description + "  " + this.BasicDescription;
            }
            set { _description = value; }
        }

        private bool _isAddedAutomatically = false;
        public bool IsAddedAutomatically
        {
            get
            {
                if (this.BasicName.IndexOf(Nodes.Task.LOCAL_TASK_PARAM_PRE) == 0)
                    return true;

                Debug.Check(!_isAddedAutomatically || this.BasicName.EndsWith("[]"));

                return _isAddedAutomatically;
            }
            set
            {
                _isAddedAutomatically = value;
            }
        }

        public string GetDisplayValue()
        {
            string owner = (!string.IsNullOrEmpty(this.Owner) && this.Owner != VariableDef.kSelf) ? (this.Owner + ".") : "";
            return owner + this.DisplayName;
        }

        public virtual string GetExportValue()
        {
            return this.Owner + "." + this.Name;
        }

        public override string ToString()
        {
            return this.Name;
        }

        public bool ShouldBeCleared(AgentType agentType)
        {
            return (this.Owner == VariableDef.kSelf &&
                    !Plugin.IsAgentDerived(this.AgentType.AgentTypeName, agentType.AgentTypeName));
        }
    }


    public class VariableDef : ICloneable, ISerializableData
    {
        public static string kConst = "Const";
        public static string kGlobal = "Global";
        public static string kSelf = "Self";
        public static string kMethod = "::Method";
        public static string kSelfMethod = kSelf + kMethod;
        public static string kEvent = "Event";

        private string _valueClass;
        public string ValueClass
        {
            get { return _valueClass; }
            set
            {
                _valueClass = value;

                if (_property != null && _valueClass == VariableDef.kSelf)
                {
                    _property.Owner = VariableDef.kSelf;
                }
            }
        }

        public string ValueClassReal
        {
            get
            {
                if (!string.IsNullOrEmpty(_valueClass))
                {
                    int pos = this._valueClass.IndexOf(VariableDef.kMethod);

                    if (pos != -1)
                    {
                        string type = _valueClass.Substring(0, pos);
                        return type;
                    }
                }

                //not a method
                return _valueClass;
            }
        }

        public bool IsConst
        {
            get { return _valueClass == kConst; }
        }
        public bool IsLocalVar
        {
            get { return _valueClass != kConst && _property != null && _property.IsPar; }
        }
        public bool IsProperty
        {
            get { return _valueClass != kConst && _property != null; }
        }

        public object Clone()
        {
            return new VariableDef(this);
        }

        public VariableDef(VariableDef other)
        {
            if (other == null)
            {
                Value = null;
            }
            else
            {
                _property = null;

                if (other._property != null)
                {
                    if (other._property.IsPar && other._property is ParInfo)
                    { _property = new ParInfo(other._property as ParInfo); }

                    else
                    { _property = new PropertyDef(other._property); }
                }

                _value = Plugin.CloneValue(other._value);
                ValueClass = other._valueClass;

                if (other.m_arrayIndexElement != null)
                { m_arrayIndexElement = new MethodDef.Param(other.m_arrayIndexElement); }
            }
        }

        public VariableDef(object value)
        {
            Value = value;
        }

        public VariableDef(PropertyDef property, string valueType)
        {
            SetProperty(property, valueType);
        }

        public VariableDef(object value, string valueType)
        {
            _value = value;
            _valueClass = valueType;
        }

        private object _value = null;
        public object Value
        {
            get { return _value; }
            set
            {
                _value = value;
                ValueClass = VariableDef.kConst;
            }
        }

        public void SetValue(object value, string valueType)
        {
            Debug.Check(value != null && valueType == kConst);
            _value = value;
            ValueClass = valueType;
        }

        private PropertyDef _property = null;
        public PropertyDef Property
        {
            get { return _property; }
        }

        public void SetProperty(PropertyDef property, string valueType)
        {
            _property = property;
            ValueClass = valueType;
            if (_property != null)
                _property.Owner = valueType;
        }

        private string _nativeType = "";
        public string NativeType
        {
            get
            {
                if (_valueClass == kConst)
                {
                    if (string.IsNullOrEmpty(_nativeType))
                        return Plugin.GetNativeTypeName(this.ValueType);
                }
                else if (_property != null)
                {
                    return _property.NativeItemType;
                }

                return _nativeType;
            }

            set
            {
                _nativeType = value;
            }
        }

        public string DisplayName
        {
            get
            {
                if (_valueClass == kConst)
                { return Resources.Const; }

                if (_property != null)
                { return _property.DisplayName; }

                return string.Empty;
            }
        }

        public string Description
        {
            get
            {
                if (_valueClass == kConst)
                { return Resources.Const; }

                if (_property != null)
                { return _property.Description; }

                return string.Empty;
            }
        }

        public Type ValueType
        {
            get
            {
                if (_valueClass == kConst && _value != null)
                    return _value.GetType();

                if (_property != null)
                    return _property.Type;

                return null;
            }
        }

        public override string ToString()
        {
            return GetExportValue();
        }

        public string GetDisplayValue()
        {
            string str = string.Empty;

            if (_valueClass == kConst && _value != null)
            {
                str = DesignerPropertyUtility.RetrieveDisplayValue(_value, null, null);
            }

            if (_property != null)
            {
                str = DesignerPropertyUtility.RetrieveDisplayValue(_property, null, null);
            }

            str = this.GetArrryIndexStr(str, true);

            return str;
        }

        public string GetExportValue()
        {
            string str = string.Empty;

            if (_valueClass == kConst)
            {
                if (_value != null)
                {
                    string nativeType = Plugin.GetNativeTypeName(this.NativeType);
                    nativeType = nativeType.Replace("*", "");
                    str = string.Format("const {0} {1}", nativeType, DesignerPropertyUtility.RetrieveExportValue(_value, null, null));
                }
            }
            else if (_property != null)
            {
                string nativeType = Plugin.GetNativeTypeName(_property.NativeItemType);
                nativeType = nativeType.Replace("*", "");
                str = string.Format("{0} {1}", nativeType, DesignerPropertyUtility.RetrieveExportValue(_property, null, null));

                if (_property.IsStatic)
                {
                    str = str.Insert(0, "static ");
                }
            }

            str = this.GetArrryIndexStr(str, false);

            return str;
        }

        private string GetArrryIndexStr(string str, bool bDisplay)
        {
            if (this.m_arrayIndexElement != null)
            {
                Debug.Check(this.m_arrayIndexElement.IsArrayIndex);

                string arrayIndexStr = string.Empty;

                if (bDisplay)
                {
                    arrayIndexStr = this.m_arrayIndexElement.GetDisplayValue(null);

                }
                else
                {
                    arrayIndexStr = this.m_arrayIndexElement.GetExportValue(null);
                }

                string arrayIndexAccessor = string.Format("[{0}]", arrayIndexStr);
                str = str.Replace("[]", arrayIndexAccessor);
            }

            return str;
        }

        public string GetRawValue()
        {
            if (_valueClass == kConst && _value != null)
            { return DesignerPropertyUtility.RetrieveExportValue(_value, null, null); }

            return GetDisplayValue();
        }

        private bool IsGlobalType()
        {
            return _valueClass != kConst && _valueClass != kSelf && _valueClass != kSelfMethod;
        }

        public bool CheckPar(ParInfo par)
        {
            if (_property is ParInfo)
            {
                return (par.Name == _property.Name);
            }

            return false;
        }

        private MethodDef.Param m_arrayIndexElement = null;
        public MethodDef.Param ArrayIndexElement
        {
            get { return this.m_arrayIndexElement; }
            set { m_arrayIndexElement = value; }
        }

        public bool ShouldBeCleared(AgentType agentType)
        {
            if (this.IsProperty && this.Property != null)
            { return this.Property.ShouldBeCleared(agentType); }

            return false;
        }

        public bool ResetMembers(bool check, AgentType agentType, bool clear, PropertyDef property)
        {
            if (property != null && this._property != null &&
                (property.OldName == this._property.Name ||
                 !property.IsArrayElement && this._property.IsArrayElement &&
                 (property.OldName + "[]") == this._property.Name))
            {
                if (!check)
                {
                    if (clear || this._property.ShouldBeCleared(agentType))
                    {
                        this._property = null;

                    }
                    else
                    {
                        bool isArrayElement = this._property.IsArrayElement;

                        this._property.CopyFrom(property);

                        if (isArrayElement)
                        { this._property.SetArrayElement(property); }
                    }
                }

                return true;
            }

            return false;
        }
    }


    public class RightValueDef : ICloneable, ISerializableData
    {
        private VariableDef m_var = null;
        private MethodDef m_method = null;

        public object Clone()
        {
            RightValueDef clone = new RightValueDef(Var);

            if (m_var != null)
            { clone.m_var = (VariableDef)m_var.Clone(); }

            if (m_method != null)
            { clone.m_method = (MethodDef)m_method.Clone(); }

            clone.ValueClass = _valueClass;

            return clone;
        }

        public RightValueDef(VariableDef var)
        {
            m_var = var;

            if (m_var != null)
            {
                _valueClass = m_var.ValueClass;
            }
        }

        public RightValueDef(MethodDef method, string valueClass)
        {
            m_method = method;
            _valueClass = valueClass;
        }

        public VariableDef Var
        {
            get { return m_var; }
        }

        public MethodDef Method
        {
            get { return m_method; }
        }

        public Type ValueType
        {
            get
            {
                if (m_var != null)
                { return m_var.ValueType; }

                if (m_method != null)
                { return m_method.ReturnType; }

                return null;
            }
        }

        private string _valueClass;
        public string ValueClass
        {
            get { return _valueClass; }
            set
            {
                _valueClass = value;

                if (m_var != null)
                { m_var.ValueClass = _valueClass; }
            }
        }

        public bool IsMethod
        {
            get
            {
                if (!string.IsNullOrEmpty(_valueClass))
                {
                    int pos = _valueClass.IndexOf(VariableDef.kMethod);

                    if (pos != -1)
                    {
                        return true;
                    }
                }

                return false;
            }
        }

        public bool IsPublic
        {
            get
            {
                if (this.Method != null)
                {
                    return this.Method.IsPublic;
                }

                if (this.Var != null && this.Var.Property != null)
                {
                    return this.Var.Property.IsPublic;
                }

                return false;
            }
        }

        public string ValueClassReal
        {
            get
            {
                if (!string.IsNullOrEmpty(_valueClass))
                {
                    int pos = this._valueClass.IndexOf(VariableDef.kMethod);

                    if (pos != -1)
                    {
                        string type = _valueClass.Substring(0, pos);
                        return type;
                    }
                }

                //not a method
                return _valueClass;
            }
        }

        public string NativeType
        {
            get
            {
                if (m_var != null)
                { return m_var.NativeType; }

                if (m_method != null)
                { return m_method.NativeReturnType; }

                return string.Empty;
            }

            set
            {
                if (m_var != null)
                    m_var.NativeType = value;
            }
        }

        public string DisplayName
        {
            get
            {
                if (m_var != null)
                { return m_var.DisplayName; }

                if (m_method != null)
                { return m_method.DisplayName; }

                return string.Empty;
            }
        }

        public string Description
        {
            get
            {
                if (m_var != null)
                { return m_var.Description; }

                if (m_method != null)
                { return m_method.Description; }

                return string.Empty;
            }
        }

        public override string ToString()
        {
            return GetExportValue();
        }

        public string GetDisplayValue()
        {
            string str = string.Empty;

            if (m_var != null)
            {
                str = DesignerPropertyUtility.RetrieveDisplayValue(m_var, null, null);

            }
            else if (m_method != null)
            {
                str = DesignerPropertyUtility.RetrieveDisplayValue(m_method, null, null);
            }

            str = this.GetArrryIndexStr(str, true);

            return str;
        }

        public string GetExportValue()
        {
            string str = string.Empty;

            if (m_var != null)
            {
                str = DesignerPropertyUtility.RetrieveExportValue(m_var, null, null);

            }
            else if (m_method != null)
            {
                str = DesignerPropertyUtility.RetrieveExportValue(m_method, null, null);
            }

            str = this.GetArrryIndexStr(str, false);

            return str;
        }

        private string GetArrryIndexStr(string str, bool bDisplay)
        {
            if (this.Var != null && this.Var.ArrayIndexElement != null)
            {
                Debug.Check(this.Var.ArrayIndexElement.IsArrayIndex);

                string arrayIndexStr = string.Empty;

                if (bDisplay)
                {
                    arrayIndexStr = this.Var.ArrayIndexElement.GetDisplayValue(null);

                }
                else
                {
                    arrayIndexStr = this.Var.ArrayIndexElement.GetExportValue(null);
                }

                string arrayIndexAccessor = string.Format("[{0}]", arrayIndexStr);

                if (str.EndsWith("[]"))
                { str = str.Replace("[]", arrayIndexAccessor); }
            }

            return str;
        }

        public bool CheckPar(ParInfo par)
        {
            if (m_var != null)
            { return m_var.CheckPar(par); }

            if (m_method != null)
            { return m_method.CheckPar(par); }

            return false;
        }

        public bool ShouldBeCleared(AgentType agentType)
        {
            if (this.IsMethod && this.Method != null)
            { return this.Method.ShouldBeCleared(agentType); }

            if (this.Var != null)
            { return this.Var.ShouldBeCleared(agentType); }

            return false;
        }

        public bool ResetMembers(bool check, AgentType agentType, bool clear, MethodDef method, PropertyDef property)
        {
            if (this.IsMethod && this.Method != null)
            {
                if (method != null && method.OldName == this.Method.Name)
                {
                    if (!check)
                    {
                        if (clear || this.Method.ShouldBeCleared(agentType))
                        { this.m_method = null; }

                        else
                        { this.Method.CopyFrom(method); }
                    }

                    return true;
                }

                return this.Method.ResetMembers(check, agentType, clear, method, property);

            }
            else if (this.Var != null)
            {
                return this.Var.ResetMembers(check, agentType, clear, property);
            }

            return false;
        }
    }

    public class ParInfo : PropertyDef
    {
        public delegate void ParameterSettingDelegate(Nodes.Node node, ParInfo parameter);
        public event ParameterSettingDelegate ParameterSet;

        public ParInfo(Nodes.Node node, AgentType agent)
            : base(agent, null, (agent != null) ? agent.AgentTypeName : "", "", "", "")
        {
            _node = node;
        }

        public ParInfo(PropertyDef other)
        {
            this.CopyFrom(other);
        }

        public override PropertyDef Clone()
        {
            return new ParInfo(this);
        }

        public override void CopyFrom(PropertyDef other)
        {
            base.CopyFrom(other);

            if (other is ParInfo)
            {
                ParInfo otherPar = other as ParInfo;

                _node = otherPar._node;
                _eventParam = otherPar._eventParam;
                _typeName = otherPar._typeName;
                _display = otherPar._display;
            }
        }

        public ParInfo Clone(Nodes.Node node = null)
        {
            ParInfo par = new ParInfo(this);

            if (node != null)
            { par._node = node; }

            return par;
        }

        public override void OnValueChanged()
        {
            if (ParameterSet != null)
            { ParameterSet(_node, this); }
        }

        public override bool IsCustomized
        {
            get { return false; }
        }

        public override bool IsPar
        {
            get { return true; }
        }

        private Nodes.Node _node = null;
        public Nodes.Node Node
        {
            set
            {
                _node = value;
                OnValueChanged();
            }
        }

        public override string Name
        {
            set
            {
                _name = value.Contains(":") ? value : (this.ClassName + "::" + value);
                OnValueChanged();
            }
        }

        private string _typeName = string.Empty;
        public string TypeName
        {
            get { return !string.IsNullOrEmpty(_typeName) ? _typeName : ((this.Type != null) ? this.Type.FullName : ""); }
            set
            {
                _typeName = value;
                OnValueChanged();
            }
        }

        public override Type Type
        {
            get { return (_type != null) ? _type : ((this.Variable != null) ? this.Variable.ValueType : null); }
            set
            {
                if (_type != value)
                {
                    _type = value;

                    if (_type != null)
                        this.TypeName = _type.FullName;

                    if (this.Variable != null)
                        this.Variable.Value = Plugin.DefaultValue(_type);
                }
            }
        }

        public override string Description
        {
            set
            {
                _description = value;
                OnValueChanged();
            }
        }

        private string _eventParam = null;
        public string EventParam
        {
            get { return _eventParam; }
            set
            {
                if (!string.IsNullOrEmpty(value))
                {
                    _eventParam = value.Replace("::param", "_param");

                }
                else
                {
                    _eventParam = null;
                }

                OnValueChanged();
            }
        }

        private bool _display = true;
        public bool Display
        {
            get { return _display; }
            set { _display = value; }
        }

        public override string GetExportValue()
        {
            string fullname = this.Name;

            if (!this.Name.Contains(":"))
            { fullname = this.ClassName + "::" + this.Name; }

            return this.Owner + "." + fullname;
        }
    }

    public class CustomizedType
    {
        public string Name = "";
        public string Namespace = ""; // behaviac
        public string Description = "";

        private string _displayName = "";
        public string DisplayName
        {
            get { return string.IsNullOrEmpty(_displayName) ? this.Name : _displayName; }
            set { _displayName = value; }
        }

        public CustomizedType(CustomizedType other)
        {
            if (other != null)
            {
                this.Name = other.Name;
                this.Namespace = other.Namespace;
                this.Description = other.Description;
                this.DisplayName = other.DisplayName;
            }
        }

        public CustomizedType(string name, string displayName, string description)
        {
            this.Reset(name, description, displayName);
        }

        public void Reset(string name, string displayName, string description)
        {
            this.Name = name;
            this.DisplayName = displayName;
            this.Description = description;
        }
    }

    public class CustomizedEnum : CustomizedType
    {
        public class CustomizedEnumMember : CustomizedType
        {
            public int Value = -1;

            public CustomizedEnumMember(CustomizedEnumMember other)
                : base(other)
            {
                if (other != null)
                { this.Value = other.Value; }
            }
        }

        public List<CustomizedEnumMember> Members = new List<CustomizedEnumMember>();

        public CustomizedEnum(CustomizedEnum other)
            : base(other)
        {
            if (other != null)
            {
                foreach (CustomizedEnumMember member in other.Members)
                {
                    this.Members.Add(new CustomizedEnumMember(member));
                }
            }
        }

        public CustomizedEnum(string name, string displayName, string description)
            : base(name, displayName, description)
        {
        }

        public CustomizedEnumMember GetMemberByName(string memberName)
        {
            foreach (CustomizedEnumMember member in this.Members)
            {
                if (member.Name.ToLowerInvariant() == memberName.ToLowerInvariant())
                    return member;
            }

            return null;
        }
    }

    public class CustomizedStruct : CustomizedType
    {
        public List<PropertyDef> Properties = new List<PropertyDef>();

        public CustomizedStruct(CustomizedStruct other)
            : base(other)
        {
            if (other != null)
            {
                foreach (PropertyDef prop in other.Properties)
                {
                    this.Properties.Add(new PropertyDef(prop));
                }
            }
        }

        public CustomizedStruct(string name, string displayName, string description)
            : base(name, displayName, description)
        {
        }
    }

    public class CustomizedTypeManager
    {
        public List<CustomizedEnum> Enums = new List<CustomizedEnum>();
        public List<CustomizedStruct> Structs = new List<CustomizedStruct>();

        private static CustomizedTypeManager _instance = null;
        public static CustomizedTypeManager Instance
        {
            get
            {
                if (_instance == null)
                { _instance = new CustomizedTypeManager(); }

                return _instance;
            }
        }

        public bool HasCustomizedTypes()
        {
            return Enums.Count > 0 || Structs.Count > 0;
        }
    }
}
