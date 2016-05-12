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

#ifndef BEHAVIAC_ENGINESERVICES_STATICMETHOD_H
#define BEHAVIAC_ENGINESERVICES_STATICMETHOD_H

#include "behaviac/base/core/staticassert.h"
#include "behaviac/base/core/thread/asyncvalue.h"

#include "behaviac/base/object/typehandler.h"

//#include "behaviac/base/meta/issame.h"

class CTagObject;
/////////////////////////////////////////////////////////
////////////////     CGenericMethodStatic      ////////////////
/////////////////////////////////////////////////////////

template<typename R>
class CGenericMethodStatic_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethodStatic_);

    CGenericMethodStatic_(R(*methodPtr)(void), const char* className, const char* propertyName) : CMethodBase(propertyName, className), m_methodPtr(methodPtr)
    {
        m_bStatic = true;
    }

    CGenericMethodStatic_(const CGenericMethodStatic_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(xmlNode);
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(xmlNode);
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName);
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        (*this->m_methodPtr)();
    }

    virtual int GetParams()
    {
        return 0;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        BEHAVIAC_UNUSED_VAR(idx);

        return behaviac::string();
    }

protected:
    R(*m_methodPtr)(void);
};

template<typename R>
class CGenericMethodStatic_R : public CGenericMethodStatic_<R>
{
public:
    CGenericMethodStatic_R(R(*methodPtr)(void), const char* className, const char* propertyName) : CGenericMethodStatic_<R>(methodPtr, className, propertyName)
    {}

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic_R(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent(this->m_netRole, nameTemp.c_str());
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            R returnValue = (*this->m_methodPtr)();

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_UNUSED_VAR(params);

        R returnValue = (*this->m_methodPtr)();

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<typename R>
class CGenericMethodStatic : public CGenericMethodStatic_R<R>
{
public:
    CGenericMethodStatic(R(*methodPtr)(void), const char* className, const char* propertyName) : CGenericMethodStatic_R<R>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic(*this);
    }
};

template<>
class CGenericMethodStatic<void> : public CGenericMethodStatic_<void>
{
public:
    CGenericMethodStatic(void(*methodPtr)(void), const char* className, const char* propertyName) : CGenericMethodStatic_<void>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent(this->m_netRole, nameTemp.c_str());
            }
        }

        if (!bHandled)
#endif//BEHAVIAC_ENABLE_NETWORKD
        {
            (*this->m_methodPtr)();
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_UNUSED_VAR(params);

        (*this->m_methodPtr)();
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethodStatic1_     ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ParamType>
class CGenericMethodStatic1_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethodStatic1_);

    typedef typename StoredTypeSelector<ParamType>::ParamBaseType			ParamBaseType;
    typedef typename StoredTypeSelector<ParamType>::ParamPointerType		ParamPointerType;

    typedef typename StoredTypeSelector<ParamType>::StoredType				StoredType;

    CGenericMethodStatic1_(R(*methodPtr)(ParamType), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr), m_paramRangeValid(false), m_param()
    {
        m_bStatic = true;
		this->m_min = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethodStatic1_(const CGenericMethodStatic1_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName = copy.m_paramDisplayName;

        this->m_param = copy.m_param;
        this->m_paramRangeValid = copy.m_paramRangeValid;

		this->m_min = copy.m_min;
		this->m_max = copy.m_max;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param.LoadFromXML(parent, xmlNode, "param1");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param.SaveToXML(parent, xmlNode, "param1");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(xmlNode);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName);
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName);
            paramNode->setAttr("Desc", m_paramDesc);

            behaviac::string typeStr = ::GetTypeDescString<ParamType>();
            paramNode->setAttr("Type", typeStr.c_str());

            if (this->m_param.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param.GetValue());
            }

            if (m_paramRangeValid)
            {
                paramNode->setAttr("RangeMin", m_min);
                paramNode->setAttr("RangeMax", m_max);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType, behaviac::Meta::IsEnum<ParamBaseType>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        m_paramDisplayName = paramDisplayName;

        return *this;
    }
    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        m_paramDesc = paramDesc;

        return *this;
    }
    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        m_paramDisplayName = paramDisplayName;
        m_paramDesc = (paramDesc == 0 ? m_paramDisplayName : paramDesc);

        if (defaultParam.IsValid())
        {
            DefaulParamSetValueSelector<StoredType, ParamBaseType, behaviac::Meta::IsRefType<ParamType>::Result>::SetValue(this->m_param, defaultParam);
        }

        int typeId = GetClassTypeNumberId<ParamBaseType>();

        if (IsNumberClassTypeNumberId(typeId))
        {
            this->m_paramRangeValid = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
            this->m_min = rangeMin;
            this->m_max = rangeMax;
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 1;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr = ::GetTypeDescString<ParamType>();
                return typeStr;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        (*this->m_methodPtr)(*(ParamPointerType)param1);
    }

protected:
    R(*m_methodPtr)(ParamType);

    behaviac::wstring				m_paramDisplayName;
    behaviac::wstring				m_paramDesc;

    bool							m_paramRangeValid;
    float							m_min, m_max;

    ParamVariable<StoredType>		m_param;
};

template<typename R, class ParamType>
class CGenericMethodStatic1_R : public CGenericMethodStatic1_<R, ParamType>
{
public:
    typedef typename CGenericMethodStatic1_<R, ParamType>::ParamBaseType ParamBaseType;

    CGenericMethodStatic1_R(R(*methodPtr)(ParamType), const char* className, const char* propertyName) :
        CGenericMethodStatic1_<R, ParamType>(methodPtr, className, propertyName)
    {}

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic1_R(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                const ParamBaseType& v = this->m_param.GetValue(parent, parHolder);
                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType>(this->m_netRole, nameTemp.c_str(),
                                                                                        (PARAM_CALLEDTYPE(ParamType))v);
            }
        }

        if (!bHandled)
#endif//BEHAVIAC_ENABLE_NETWORKD
        {
            const ParamBaseType& v = this->m_param.GetValue(parent, parHolder);

            R returnValue = (*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType))v);

            this->m_param.SetVariableRegistry(parHolder, v);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 1);

        PARAM_BASETYPE(ParamType) p;
        params[0]->GetValue(p);

        R returnValue = (*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType))p);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<typename R, class ParamType>
class CGenericMethodStatic1 : public CGenericMethodStatic1_R<R, ParamType>
{
public:
    CGenericMethodStatic1(R(*methodPtr)(ParamType), const char* className, const char* propertyName) :
        CGenericMethodStatic1_R<R, ParamType>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic1(*this);
    }
};

template<class ParamType>
class CGenericMethodStatic1<void, ParamType> : public CGenericMethodStatic1_<void, ParamType>
{
public:
    typedef typename CGenericMethodStatic1_<void, ParamType>::ParamBaseType ParamBaseType;

    CGenericMethodStatic1(void(*methodPtr)(ParamType), const char* className, const char* propertyName) :
        CGenericMethodStatic1_<void, ParamType>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic1(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                const ParamBaseType& v = this->m_param.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType>(this->m_netRole, nameTemp.c_str(),
                                                                                        (PARAM_CALLEDTYPE(ParamType))v);
            }
        }

        if (!bHandled)
#endif//BEHAVIAC_ENABLE_NETWORKD
        {
            const ParamBaseType& v = this->m_param.GetValue(parent, parHolder);

            (*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType))v);

            this->m_param.SetVariableRegistry(parHolder, v);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 1);

        PARAM_BASETYPE(ParamType) p;
        params[0]->GetValue(p);

        (*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType))p);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethodStatic2_     ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ParamType1, class ParamType2>
class CGenericMethodStatic2_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethodStatic2_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType				ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType				ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType			ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType			ParamPointerType2;

    typedef typename StoredTypeSelector<ParamType1>::StoredType					StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType					StoredType2;

    CGenericMethodStatic2_(R(*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr),
        m_paramRangeValid1(false), m_paramRangeValid2(false),
        m_param1(), m_param2()
    {
        m_bStatic = true;
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethodStatic2_(const CGenericMethodStatic2_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
		this->m_min1 = copy.m_min1;
		this->m_max1 = copy.m_max1;
		this->m_min2 = copy.m_min2;
		this->m_max2 = copy.m_max2;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName);
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }
    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }
    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 2;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        (*this->m_methodPtr)(*((ParamPointerType1)param1), *((ParamPointerType2)param2));
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic2_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2);
            }
        }

        if (!bHandled)
#endif//BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 2);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
protected:
    R(*m_methodPtr)(ParamType1, ParamType2);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;

    ParamVariable<StoredType1>	m_param1;
    ParamVariable<StoredType2>	m_param2;
};

template<typename R, class ParamType1, class ParamType2>
class CGenericMethodStatic2 : public CGenericMethodStatic2_<R, ParamType1, ParamType2>
{
public:
    typedef typename CGenericMethodStatic2_<R, ParamType1, ParamType2>::StoredType1		StoredType1;
    typedef typename CGenericMethodStatic2_<R, ParamType1, ParamType2>::StoredType2		StoredType2;
    typedef typename CGenericMethodStatic2_<R, ParamType1, ParamType2>::ParamBaseType1	ParamBaseType1;
    typedef typename CGenericMethodStatic2_<R, ParamType1, ParamType2>::ParamBaseType2	ParamBaseType2;

    CGenericMethodStatic2(R(*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName) :
        CGenericMethodStatic2_<R, ParamType1, ParamType2>(methodPtr, className, propertyName)
    {}

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic2(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

            R returnValue = (*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 2);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);

        R returnValue = (*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ParamType1, class ParamType2>
class CGenericMethodStatic2<void, ParamType1, ParamType2> : public CGenericMethodStatic2_<void, ParamType1, ParamType2>
{
public:
    typedef typename CGenericMethodStatic2_<void, ParamType1, ParamType2>::StoredType1		StoredType1;
    typedef typename CGenericMethodStatic2_<void, ParamType1, ParamType2>::StoredType2		StoredType2;
    typedef typename CGenericMethodStatic2_<void, ParamType1, ParamType2>::ParamBaseType1	ParamBaseType1;
    typedef typename CGenericMethodStatic2_<void, ParamType1, ParamType2>::ParamBaseType2	ParamBaseType2;

    CGenericMethodStatic2(void(*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName) :
        CGenericMethodStatic2_<void, ParamType1, ParamType2>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic2(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 2);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethodStatic3_     ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ParamType1, class ParamType2, class ParamType3>
class CGenericMethodStatic3_ : public CMethodBase
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethodStatic3_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType		ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType		ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType		ParamPointerType3;

    typedef typename StoredTypeSelector<ParamType1>::StoredType				StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType				StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType				StoredType3;

    CGenericMethodStatic3_(R(*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr), m_param1(), m_param2(), m_param3(),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false)
    {
        m_bStatic = true;
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethodStatic3_(const CGenericMethodStatic3_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
		this->m_min1 = copy.m_min1;
		this->m_max1 = copy.m_max1;
		this->m_min2 = copy.m_min2;
		this->m_max2 = copy.m_max2;
		this->m_min3 = copy.m_min3;
		this->m_max3 = copy.m_max3;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName);
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }
    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 3;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        (*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic3_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 3);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
protected:
    R(*m_methodPtr)(ParamType1, ParamType2, ParamType3);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;

    ParamVariable<StoredType1>	m_param1;
    ParamVariable<StoredType2>	m_param2;
    ParamVariable<StoredType3>	m_param3;
};

template<typename R, class ParamType1, class ParamType2, class ParamType3>
class CGenericMethodStatic3 : public CGenericMethodStatic3_<R, ParamType1, ParamType2, ParamType3>
{
public:
    typedef typename CGenericMethodStatic3_<R, ParamType1, ParamType2, ParamType3>::StoredType1		StoredType1;
    typedef typename CGenericMethodStatic3_<R, ParamType1, ParamType2, ParamType3>::StoredType2		StoredType2;
    typedef typename CGenericMethodStatic3_<R, ParamType1, ParamType2, ParamType3>::StoredType3		StoredType3;
    typedef typename CGenericMethodStatic3_<R, ParamType1, ParamType2, ParamType3>::ParamBaseType1	ParamBaseType1;
    typedef typename CGenericMethodStatic3_<R, ParamType1, ParamType2, ParamType3>::ParamBaseType2	ParamBaseType2;
    typedef typename CGenericMethodStatic3_<R, ParamType1, ParamType2, ParamType3>::ParamBaseType3	ParamBaseType3;

    CGenericMethodStatic3(R(*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName) :
        CGenericMethodStatic3_<R, ParamType1, ParamType2, ParamType3>(methodPtr, className, propertyName)
    {}

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic3(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

            R returnValue = (*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 3);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);

        R returnValue = (*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ParamType1, class ParamType2, class ParamType3>
class CGenericMethodStatic3<void, ParamType1, ParamType2, ParamType3> : public CGenericMethodStatic3_<void, ParamType1, ParamType2, ParamType3>
{
public:
    typedef typename CGenericMethodStatic3_<void, ParamType1, ParamType2, ParamType3>::StoredType1		StoredType1;
    typedef typename CGenericMethodStatic3_<void, ParamType1, ParamType2, ParamType3>::StoredType2		StoredType2;
    typedef typename CGenericMethodStatic3_<void, ParamType1, ParamType2, ParamType3>::StoredType3		StoredType3;
    typedef typename CGenericMethodStatic3_<void, ParamType1, ParamType2, ParamType3>::ParamBaseType1	ParamBaseType1;
    typedef typename CGenericMethodStatic3_<void, ParamType1, ParamType2, ParamType3>::ParamBaseType2	ParamBaseType2;
    typedef typename CGenericMethodStatic3_<void, ParamType1, ParamType2, ParamType3>::ParamBaseType3	ParamBaseType3;

    CGenericMethodStatic3(void(*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName) :
        CGenericMethodStatic3_<void, ParamType1, ParamType2, ParamType3>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic3(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_ASSERT(params.size() == 3);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethodStatic4_     ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
class CGenericMethodStatic4_ : public CMethodBase
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethodStatic4_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamBaseType			ParamBaseType4;

    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType		ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType		ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType		ParamPointerType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamPointerType		ParamPointerType4;

    typedef typename StoredTypeSelector<ParamType1>::StoredType				StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType				StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType				StoredType3;
    typedef typename StoredTypeSelector<ParamType4>::StoredType				StoredType4;

    CGenericMethodStatic4_(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr), m_param1(), m_param2(), m_param3(), m_param4(),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false)
    {
        m_bStatic = true;
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethodStatic4_(const CGenericMethodStatic4_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;
        this->m_paramDisplayName4 = copy.m_paramDisplayName4;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_param4 = copy.m_param4;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
        this->m_paramRangeValid4 = copy.m_paramRangeValid4;
		this->m_min1 = copy.m_min1;
		this->m_max1 = copy.m_max1;
		this->m_min2 = copy.m_min2;
		this->m_max2 = copy.m_max2;
		this->m_min3 = copy.m_min3;
		this->m_max3 = copy.m_max3;
		this->m_min4 = copy.m_min4;
		this->m_max4 = copy.m_max4;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
        this->m_param4.LoadFromXML(parent, xmlNode, "param4");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
        this->m_param4.SaveToXML(parent, xmlNode, "param4");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName);
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName4);
            paramNode->setAttr("Desc", m_paramDesc4);

            behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
            paramNode->setAttr("Type", typeStr4.c_str());

            if (this->m_param4.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param4.GetValue());
            }

            if (m_paramRangeValid4)
            {
                paramNode->setAttr("RangeMin", m_min4);
                paramNode->setAttr("RangeMax", m_max4);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType4, behaviac::Meta::IsEnum<ParamBaseType4>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else if (m_paramDesc4.empty())
        {
            m_paramDesc4 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }
    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;
            m_paramDesc4 = (paramDesc == 0 ? m_paramDisplayName4 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType4>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid4 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min4 = rangeMin;
                this->m_max4 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 4;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            case 3:
            {
                behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
                return typeStr4;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        (*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3, *(ParamPointerType4)param4);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic4_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 4);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(*m_methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;
    behaviac::wstring				m_paramDisplayName4;
    behaviac::wstring				m_paramDesc4;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;
    bool							m_paramRangeValid4;
    float							m_min4, m_max4;

    ParamVariable<StoredType1>	m_param1;
    ParamVariable<StoredType2>	m_param2;
    ParamVariable<StoredType3>	m_param3;
    ParamVariable<StoredType4>	m_param4;
};

template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
class CGenericMethodStatic4 : public CGenericMethodStatic4_<R, ParamType1, ParamType2, ParamType3, ParamType4>
{
public:
    typedef typename CGenericMethodStatic4_<R, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType1 StoredType1;
    typedef typename CGenericMethodStatic4_<R, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType2 StoredType2;
    typedef typename CGenericMethodStatic4_<R, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType3 StoredType3;
    typedef typename CGenericMethodStatic4_<R, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType4 StoredType4;
    typedef typename CGenericMethodStatic4_<R, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethodStatic4_<R, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethodStatic4_<R, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethodStatic4_<R, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType4 ParamBaseType4;

    CGenericMethodStatic4(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName) :
        CGenericMethodStatic4_<R, ParamType1, ParamType2, ParamType3, ParamType4>(methodPtr, className, propertyName)
    {}

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic4(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

            R returnValue = (*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                (PARAM_CALLEDTYPE(ParamType4))v4);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 4);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);

        R returnValue = (*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3,
                            (PARAM_CALLEDTYPE(ParamType4))p4);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ParamType1, class ParamType2, class ParamType3, class ParamType4>
class CGenericMethodStatic4<void, ParamType1, ParamType2, ParamType3, ParamType4> : public CGenericMethodStatic4_<void, ParamType1, ParamType2, ParamType3, ParamType4>
{
public:
    typedef typename CGenericMethodStatic4_<void, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType1 StoredType1;
    typedef typename CGenericMethodStatic4_<void, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType2 StoredType2;
    typedef typename CGenericMethodStatic4_<void, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType3 StoredType3;
    typedef typename CGenericMethodStatic4_<void, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType4 StoredType4;
    typedef typename CGenericMethodStatic4_<void, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethodStatic4_<void, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethodStatic4_<void, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethodStatic4_<void, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType4 ParamBaseType4;

    CGenericMethodStatic4(void(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName) :
        CGenericMethodStatic4_<void, ParamType1, ParamType2, ParamType3, ParamType4>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic4(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_ASSERT(params.size() == 4);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethodStatic5_     ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
class CGenericMethodStatic5_ : public CMethodBase
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethodStatic5_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamBaseType			ParamBaseType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamBaseType			ParamBaseType5;

    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType			ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType			ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType			ParamPointerType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamPointerType			ParamPointerType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamPointerType			ParamPointerType5;

    typedef typename StoredTypeSelector<ParamType1>::StoredType		StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType		StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType		StoredType3;
    typedef typename StoredTypeSelector<ParamType4>::StoredType		StoredType4;
    typedef typename StoredTypeSelector<ParamType5>::StoredType		StoredType5;

    CGenericMethodStatic5_(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr), m_param1(), m_param2(), m_param3(), m_param4(), m_param5(),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false)
    {
        m_bStatic = true;
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethodStatic5_(const CGenericMethodStatic5_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;
        this->m_paramDisplayName4 = copy.m_paramDisplayName4;
        this->m_paramDisplayName5 = copy.m_paramDisplayName5;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_param4 = copy.m_param4;
        this->m_param5 = copy.m_param5;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
        this->m_paramRangeValid4 = copy.m_paramRangeValid4;
        this->m_paramRangeValid5 = copy.m_paramRangeValid5;
		this->m_min1 = copy.m_min1;
		this->m_max1 = copy.m_max1;
		this->m_min2 = copy.m_min2;
		this->m_max2 = copy.m_max2;
		this->m_min3 = copy.m_min3;
		this->m_max3 = copy.m_max3;
		this->m_min4 = copy.m_min4;
		this->m_max4 = copy.m_max4;
		this->m_min5 = copy.m_min5;
		this->m_max5 = copy.m_max5;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
        this->m_param4.LoadFromXML(parent, xmlNode, "param4");
        this->m_param5.LoadFromXML(parent, xmlNode, "param5");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
        this->m_param4.SaveToXML(parent, xmlNode, "param4");
        this->m_param5.SaveToXML(parent, xmlNode, "param5");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName);
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName4);
            paramNode->setAttr("Desc", m_paramDesc4);

            behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
            paramNode->setAttr("Type", typeStr4.c_str());

            if (this->m_param4.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param4.GetValue());
            }

            if (m_paramRangeValid4)
            {
                paramNode->setAttr("RangeMin", m_min4);
                paramNode->setAttr("RangeMax", m_max4);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType4, behaviac::Meta::IsEnum<ParamBaseType4>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName5);
            paramNode->setAttr("Desc", m_paramDesc5);

            behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
            paramNode->setAttr("Type", typeStr5.c_str());

            if (this->m_param5.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param5.GetValue());
            }

            if (m_paramRangeValid5)
            {
                paramNode->setAttr("RangeMin", m_min5);
                paramNode->setAttr("RangeMax", m_max5);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType5, behaviac::Meta::IsEnum<ParamBaseType5>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;

        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else if (m_paramDesc4.empty())
        {
            m_paramDesc4 = paramDesc;

        }
        else if (m_paramDesc5.empty())
        {
            m_paramDesc5 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }
    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;
            m_paramDesc4 = (paramDesc == 0 ? m_paramDisplayName4 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType4>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid4 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min4 = rangeMin;
                this->m_max4 = rangeMax;
            }
        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;
            m_paramDesc5 = (paramDesc == 0 ? m_paramDisplayName5 : paramDesc);

            if (defaultParam.IsValid())
            {
                DefaulParamSetValueSelector<StoredType5, ParamBaseType5, behaviac::Meta::IsRefType<ParamType5>::Result>::SetValue(this->m_param5, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType5>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid5 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min5 = rangeMin;
                this->m_max5 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DEFAULTVALUE(int paramIndex, const DefaultParam_t& defaultParam)
    {
        if (paramIndex == 0)
        {
            DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;

        }
        else if (paramIndex == 1)
        {
            DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;

        }
        else if (paramIndex == 2)
        {
            DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);;

        }
        else if (paramIndex == 3)
        {
            DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);;

        }
        else if (paramIndex == 4)
        {
            DefaulParamSetValueSelector<StoredType5, ParamBaseType5, behaviac::Meta::IsRefType<ParamType5>::Result>::SetValue(this->m_param5, defaultParam);;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "not supported param");
        }

        return *this;
    }
    virtual int GetParams()
    {
        return 5;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            case 3:
            {
                behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
                return typeStr4;
            }

            case 4:
            {
                behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
                return typeStr5;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4, const void* param5)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        (*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3, *(ParamPointerType4)param4, *(ParamPointerType5)param5);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic5_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 5);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(*m_methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;
    behaviac::wstring				m_paramDisplayName4;
    behaviac::wstring				m_paramDesc4;
    behaviac::wstring				m_paramDisplayName5;
    behaviac::wstring				m_paramDesc5;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;
    bool							m_paramRangeValid4;
    float							m_min4, m_max4;
    bool							m_paramRangeValid5;
    float							m_min5, m_max5;

    ParamVariable<StoredType1>	m_param1;
    ParamVariable<StoredType2>	m_param2;
    ParamVariable<StoredType3>	m_param3;
    ParamVariable<StoredType4>	m_param4;
    ParamVariable<StoredType5>	m_param5;
};

template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
class CGenericMethodStatic5 : public CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>
{
public:
    typedef typename CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType1 StoredType1;
    typedef typename CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType2 StoredType2;
    typedef typename CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType3 StoredType3;
    typedef typename CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType4 StoredType4;
    typedef typename CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType5 StoredType5;
    typedef typename CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType5 ParamBaseType5;

    CGenericMethodStatic5(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName) :
        CGenericMethodStatic5_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(methodPtr, className, propertyName)
    {}

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic5(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

            R returnValue = (*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                (PARAM_CALLEDTYPE(ParamType4))v4,
                                (PARAM_CALLEDTYPE(ParamType5))v5);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 5);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);

        R returnValue = (*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3,
                            (PARAM_CALLEDTYPE(ParamType4))p4,
                            (PARAM_CALLEDTYPE(ParamType5))p5);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
class CGenericMethodStatic5<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> : public CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>
{
public:
    typedef typename CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType1 StoredType1;
    typedef typename CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType2 StoredType2;
    typedef typename CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType3 StoredType3;
    typedef typename CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType4 StoredType4;
    typedef typename CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType5 StoredType5;
    typedef typename CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType5 ParamBaseType5;

    CGenericMethodStatic5(void(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName) :
        CGenericMethodStatic5_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic5(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_ASSERT(params.size() == 5);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethodStatic6_     ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
class CGenericMethodStatic6_ : public CMethodBase
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethodStatic6_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamBaseType			ParamBaseType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamBaseType			ParamBaseType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamBaseType			ParamBaseType6;

    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType			ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType			ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType			ParamPointerType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamPointerType			ParamPointerType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamPointerType			ParamPointerType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamPointerType			ParamPointerType6;

    typedef typename StoredTypeSelector<ParamType1>::StoredType		StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType		StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType		StoredType3;
    typedef typename StoredTypeSelector<ParamType4>::StoredType		StoredType4;
    typedef typename StoredTypeSelector<ParamType5>::StoredType		StoredType5;
    typedef typename StoredTypeSelector<ParamType6>::StoredType		StoredType6;

    CGenericMethodStatic6_(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr), m_param1(), m_param2(), m_param3(), m_param4(), m_param5(), m_param6(),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false), m_paramRangeValid6(false)
    {
        m_bStatic = true;
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min6 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max6 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethodStatic6_(const CGenericMethodStatic6_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;
        this->m_paramDisplayName4 = copy.m_paramDisplayName4;
        this->m_paramDisplayName5 = copy.m_paramDisplayName5;
        this->m_paramDisplayName6 = copy.m_paramDisplayName6;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_param4 = copy.m_param4;
        this->m_param5 = copy.m_param5;
        this->m_param6 = copy.m_param6;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
        this->m_paramRangeValid4 = copy.m_paramRangeValid4;
        this->m_paramRangeValid5 = copy.m_paramRangeValid5;
        this->m_paramRangeValid6 = copy.m_paramRangeValid6;
		this->m_min1 = copy.m_min1;
		this->m_max1 = copy.m_max1;
		this->m_min2 = copy.m_min2;
		this->m_max2 = copy.m_max2;
		this->m_min3 = copy.m_min3;
		this->m_max3 = copy.m_max3;
		this->m_min4 = copy.m_min4;
		this->m_max4 = copy.m_max4;
		this->m_min5 = copy.m_min5;
		this->m_max5 = copy.m_max5;
		this->m_min6 = copy.m_min6;
		this->m_max6 = copy.m_max6;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
        this->m_param4.LoadFromXML(parent, xmlNode, "param4");
        this->m_param5.LoadFromXML(parent, xmlNode, "param5");
        this->m_param6.LoadFromXML(parent, xmlNode, "param6");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
        this->m_param4.SaveToXML(parent, xmlNode, "param4");
        this->m_param5.SaveToXML(parent, xmlNode, "param5");
        this->m_param6.SaveToXML(parent, xmlNode, "param6");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName);
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName4);
            paramNode->setAttr("Desc", m_paramDesc4);

            behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
            paramNode->setAttr("Type", typeStr4.c_str());

            if (this->m_param4.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param4.GetValue());
            }

            if (m_paramRangeValid4)
            {
                paramNode->setAttr("RangeMin", m_min4);
                paramNode->setAttr("RangeMax", m_max4);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType4, behaviac::Meta::IsEnum<ParamBaseType4>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName5);
            paramNode->setAttr("Desc", m_paramDesc5);

            behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
            paramNode->setAttr("Type", typeStr5.c_str());

            if (this->m_param5.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param5.GetValue());
            }

            if (m_paramRangeValid5)
            {
                paramNode->setAttr("RangeMin", m_min5);
                paramNode->setAttr("RangeMax", m_max5);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType5, behaviac::Meta::IsEnum<ParamBaseType5>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName6);
            paramNode->setAttr("Desc", m_paramDesc6);

            behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
            paramNode->setAttr("Type", typeStr6.c_str());

            if (this->m_param6.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param6.GetValue());
            }

            if (m_paramRangeValid6)
            {
                paramNode->setAttr("RangeMin", m_min6);
                paramNode->setAttr("RangeMax", m_max6);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType6, behaviac::Meta::IsEnum<ParamBaseType6>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;

        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;

        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else if (m_paramDesc4.empty())
        {
            m_paramDesc4 = paramDesc;

        }
        else if (m_paramDesc5.empty())
        {
            m_paramDesc5 = paramDesc;

        }
        else if (m_paramDesc6.empty())
        {
            m_paramDesc6 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }
    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;
            m_paramDesc4 = (paramDesc == 0 ? m_paramDisplayName4 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType4>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid4 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min4 = rangeMin;
                this->m_max4 = rangeMax;
            }
        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;
            m_paramDesc5 = (paramDesc == 0 ? m_paramDisplayName5 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType5>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid5 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min5 = rangeMin;
                this->m_max5 = rangeMax;
            }
        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;
            m_paramDesc6 = (paramDesc == 0 ? m_paramDisplayName6 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType6>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid6 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min6 = rangeMin;
                this->m_max6 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DEFAULTVALUE(int paramIndex, const DefaultParam_t& defaultParam)
    {
        if (paramIndex == 0)
        {
            DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;

        }
        else if (paramIndex == 1)
        {
            DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;

        }
        else if (paramIndex == 2)
        {
            DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);;

        }
        else if (paramIndex == 3)
        {
            DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);;

        }
        else if (paramIndex == 4)
        {
            DefaulParamSetValueSelector<StoredType5, ParamBaseType5, behaviac::Meta::IsRefType<ParamType5>::Result>::SetValue(this->m_param5, defaultParam);;

        }
        else if (paramIndex == 5)
        {
            DefaulParamSetValueSelector<StoredType6, ParamBaseType6, behaviac::Meta::IsRefType<ParamType6>::Result>::SetValue(this->m_param6, defaultParam);;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "not supported param");
        }

        return *this;
    }
    virtual int GetParams()
    {
        return 6;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            case 3:
            {
                behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
                return typeStr4;
            }

            case 4:
            {
                behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
                return typeStr5;
            }

            case 5:
            {
                behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
                return typeStr6;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4, const void* param5, const void* param6)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        (*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3, *(ParamPointerType4)param4, *(ParamPointerType5)param5, *(ParamPointerType6)param6);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic6_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 6);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(*m_methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;
    behaviac::wstring				m_paramDisplayName4;
    behaviac::wstring				m_paramDesc4;
    behaviac::wstring				m_paramDisplayName5;
    behaviac::wstring				m_paramDesc5;
    behaviac::wstring				m_paramDisplayName6;
    behaviac::wstring				m_paramDesc6;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;
    bool							m_paramRangeValid4;
    float							m_min4, m_max4;
    bool							m_paramRangeValid5;
    float							m_min5, m_max5;
    bool							m_paramRangeValid6;
    float							m_min6, m_max6;

    ParamVariable<StoredType1>	m_param1;
    ParamVariable<StoredType2>	m_param2;
    ParamVariable<StoredType3>	m_param3;
    ParamVariable<StoredType4>	m_param4;
    ParamVariable<StoredType5>	m_param5;
    ParamVariable<StoredType6>	m_param6;
};

template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
class CGenericMethodStatic6 : public CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>
{
public:
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType1 StoredType1;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType2 StoredType2;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType3 StoredType3;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType4 StoredType4;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType5 StoredType5;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType6 StoredType6;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType6 ParamBaseType6;

    CGenericMethodStatic6(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName) :
        CGenericMethodStatic6_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(methodPtr, className, propertyName)
    {}

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic6(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

            R returnValue = (*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                (PARAM_CALLEDTYPE(ParamType4))v4,
                                (PARAM_CALLEDTYPE(ParamType5))v5,
                                (PARAM_CALLEDTYPE(ParamType6))v6);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 6);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);

        R returnValue = (*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3,
                            (PARAM_CALLEDTYPE(ParamType4))p4,
                            (PARAM_CALLEDTYPE(ParamType5))p5,
                            (PARAM_CALLEDTYPE(ParamType6))p6);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
class CGenericMethodStatic6<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> : public CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>
{
public:
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType1 StoredType1;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType2 StoredType2;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType3 StoredType3;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType4 StoredType4;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType5 StoredType5;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType6 StoredType6;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType6 ParamBaseType6;

    CGenericMethodStatic6(void(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName) :
        CGenericMethodStatic6_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic6(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_ASSERT(params.size() == 6);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethodStatic7_     ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
class CGenericMethodStatic7_ : public CMethodBase
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethodStatic7_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamBaseType			ParamBaseType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamBaseType			ParamBaseType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamBaseType			ParamBaseType6;
    typedef typename StoredTypeSelector<ParamType7>::ParamBaseType			ParamBaseType7;

    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType			ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType			ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType			ParamPointerType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamPointerType			ParamPointerType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamPointerType			ParamPointerType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamPointerType			ParamPointerType6;
    typedef typename StoredTypeSelector<ParamType7>::ParamPointerType			ParamPointerType7;

    typedef typename StoredTypeSelector<ParamType1>::StoredType		StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType		StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType		StoredType3;
    typedef typename StoredTypeSelector<ParamType4>::StoredType		StoredType4;
    typedef typename StoredTypeSelector<ParamType5>::StoredType		StoredType5;
    typedef typename StoredTypeSelector<ParamType6>::StoredType		StoredType6;
    typedef typename StoredTypeSelector<ParamType7>::StoredType		StoredType7;

    CGenericMethodStatic7_(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr), m_param1(), m_param2(), m_param3(), m_param4(), m_param5(), m_param6(), m_param7(),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false), m_paramRangeValid6(false), m_paramRangeValid7(false)
    {
        m_bStatic = true;
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min6 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max6 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min7 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max7 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethodStatic7_(const CGenericMethodStatic7_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;
        this->m_paramDisplayName4 = copy.m_paramDisplayName4;
        this->m_paramDisplayName5 = copy.m_paramDisplayName5;
        this->m_paramDisplayName6 = copy.m_paramDisplayName6;
        this->m_paramDisplayName7 = copy.m_paramDisplayName7;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_param4 = copy.m_param4;
        this->m_param5 = copy.m_param5;
        this->m_param6 = copy.m_param6;
        this->m_param7 = copy.m_param7;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
        this->m_paramRangeValid4 = copy.m_paramRangeValid4;
        this->m_paramRangeValid5 = copy.m_paramRangeValid5;
        this->m_paramRangeValid6 = copy.m_paramRangeValid6;
        this->m_paramRangeValid7 = copy.m_paramRangeValid7;
		this->m_min1 = copy.m_min1;
		this->m_max1 = copy.m_max1;
		this->m_min2 = copy.m_min2;
		this->m_max2 = copy.m_max2;
		this->m_min3 = copy.m_min3;
		this->m_max3 = copy.m_max3;
		this->m_min4 = copy.m_min4;
		this->m_max4 = copy.m_max4;
		this->m_min5 = copy.m_min5;
		this->m_max5 = copy.m_max5;
		this->m_min6 = copy.m_min6;
		this->m_max6 = copy.m_max6;
		this->m_min7 = copy.m_min7;
		this->m_max7 = copy.m_max7;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
        this->m_param4.LoadFromXML(parent, xmlNode, "param4");
        this->m_param5.LoadFromXML(parent, xmlNode, "param5");
        this->m_param6.LoadFromXML(parent, xmlNode, "param6");
        this->m_param7.LoadFromXML(parent, xmlNode, "param7");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
        this->m_param4.SaveToXML(parent, xmlNode, "param4");
        this->m_param5.SaveToXML(parent, xmlNode, "param5");
        this->m_param6.SaveToXML(parent, xmlNode, "param6");
        this->m_param7.SaveToXML(parent, xmlNode, "param7");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName);
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName4);
            paramNode->setAttr("Desc", m_paramDesc4);

            behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
            paramNode->setAttr("Type", typeStr4.c_str());

            if (this->m_param4.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param4.GetValue());
            }

            if (m_paramRangeValid4)
            {
                paramNode->setAttr("RangeMin", m_min4);
                paramNode->setAttr("RangeMax", m_max4);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType4, behaviac::Meta::IsEnum<ParamBaseType4>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName5);
            paramNode->setAttr("Desc", m_paramDesc5);

            behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
            paramNode->setAttr("Type", typeStr5.c_str());

            if (this->m_param5.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param5.GetValue());
            }

            if (m_paramRangeValid5)
            {
                paramNode->setAttr("RangeMin", m_min5);
                paramNode->setAttr("RangeMax", m_max5);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType5, behaviac::Meta::IsEnum<ParamBaseType5>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName6);
            paramNode->setAttr("Desc", m_paramDesc6);

            behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
            paramNode->setAttr("Type", typeStr6.c_str());

            if (this->m_param6.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param6.GetValue());
            }

            if (m_paramRangeValid6)
            {
                paramNode->setAttr("RangeMin", m_min6);
                paramNode->setAttr("RangeMax", m_max6);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType6, behaviac::Meta::IsEnum<ParamBaseType6>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName7);
            paramNode->setAttr("Desc", m_paramDesc7);

            behaviac::string typeStr7 = ::GetTypeDescString<ParamType7>();
            paramNode->setAttr("Type", typeStr7.c_str());

            if (this->m_param7.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param7.GetValue());
            }

            if (m_paramRangeValid7)
            {
                paramNode->setAttr("RangeMin", m_min7);
                paramNode->setAttr("RangeMax", m_max7);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType7, behaviac::Meta::IsEnum<ParamBaseType7>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;

        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;

        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;

        }
        else if (m_paramDisplayName7.empty())
        {
            m_paramDisplayName7 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else if (m_paramDesc4.empty())
        {
            m_paramDesc4 = paramDesc;

        }
        else if (m_paramDesc5.empty())
        {
            m_paramDesc5 = paramDesc;

        }
        else if (m_paramDesc6.empty())
        {
            m_paramDesc6 = paramDesc;

        }
        else if (m_paramDesc7.empty())
        {
            m_paramDesc7 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }
    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;
            m_paramDesc4 = (paramDesc == 0 ? m_paramDisplayName4 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType4>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid4 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min4 = rangeMin;
                this->m_max4 = rangeMax;
            }
        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;
            m_paramDesc5 = (paramDesc == 0 ? m_paramDisplayName5 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType5>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid5 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min5 = rangeMin;
                this->m_max5 = rangeMax;
            }
        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;
            m_paramDesc6 = (paramDesc == 0 ? m_paramDisplayName6 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType6>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid6 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min6 = rangeMin;
                this->m_max6 = rangeMax;
            }
        }
        else if (m_paramDisplayName7.empty())
        {
            m_paramDisplayName7 = paramDisplayName;
            m_paramDesc7 = (paramDesc == 0 ? m_paramDisplayName7 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType7>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid7 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min7 = rangeMin;
                this->m_max7 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DEFAULTVALUE(int paramIndex, const DefaultParam_t& defaultParam)
    {
        if (paramIndex == 0)
        {
            DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;

        }
        else if (paramIndex == 1)
        {
            DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;

        }
        else if (paramIndex == 2)
        {
            DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);;

        }
        else if (paramIndex == 3)
        {
            DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);;

        }
        else if (paramIndex == 4)
        {
            DefaulParamSetValueSelector<StoredType5, ParamBaseType5, behaviac::Meta::IsRefType<ParamType5>::Result>::SetValue(this->m_param5, defaultParam);;

        }
        else if (paramIndex == 5)
        {
            DefaulParamSetValueSelector<StoredType6, ParamBaseType6, behaviac::Meta::IsRefType<ParamType6>::Result>::SetValue(this->m_param6, defaultParam);;

        }
        else if (paramIndex == 6)
        {
            DefaulParamSetValueSelector<StoredType7, ParamBaseType7, behaviac::Meta::IsRefType<ParamType7>::Result>::SetValue(this->m_param7, defaultParam);;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "not supported param");
        }

        return *this;
    }
    virtual int GetParams()
    {
        return 7;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            case 3:
            {
                behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
                return typeStr4;
            }

            case 4:
            {
                behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
                return typeStr5;
            }

            case 5:
            {
                behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
                return typeStr6;
            }

            case 6:
            {
                behaviac::string typeStr7 = ::GetTypeDescString<ParamType7>();
                return typeStr7;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4, const void* param5, const void* param6, const void* param7)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        (*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3, *(ParamPointerType4)param4, *(ParamPointerType5)param5, *(ParamPointerType6)param6, *(ParamPointerType7)param7);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic7_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6,
                (PARAM_CALLEDTYPE(ParamType7))v7);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 7);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6,
            (PARAM_CALLEDTYPE(ParamType7))p7);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(*m_methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;
    behaviac::wstring				m_paramDisplayName4;
    behaviac::wstring				m_paramDesc4;
    behaviac::wstring				m_paramDisplayName5;
    behaviac::wstring				m_paramDesc5;
    behaviac::wstring				m_paramDisplayName6;
    behaviac::wstring				m_paramDesc6;
    behaviac::wstring				m_paramDisplayName7;
    behaviac::wstring				m_paramDesc7;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;
    bool							m_paramRangeValid4;
    float							m_min4, m_max4;
    bool							m_paramRangeValid5;
    float							m_min5, m_max5;
    bool							m_paramRangeValid6;
    float							m_min6, m_max6;
    bool							m_paramRangeValid7;
    float							m_min7, m_max7;

    ParamVariable<StoredType1>	m_param1;
    ParamVariable<StoredType2>	m_param2;
    ParamVariable<StoredType3>	m_param3;
    ParamVariable<StoredType4>	m_param4;
    ParamVariable<StoredType5>	m_param5;
    ParamVariable<StoredType6>	m_param6;
    ParamVariable<StoredType7>	m_param7;
};

template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
class CGenericMethodStatic7 : public CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>
{
public:
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType1 StoredType1;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType2 StoredType2;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType3 StoredType3;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType4 StoredType4;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType5 StoredType5;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType6 StoredType6;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType7 StoredType7;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType6 ParamBaseType6;
    typedef typename CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType7 ParamBaseType7;

    CGenericMethodStatic7(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName) :
        CGenericMethodStatic7_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(methodPtr, className, propertyName)
    {}

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic7(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

            R returnValue = (*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                (PARAM_CALLEDTYPE(ParamType4))v4,
                                (PARAM_CALLEDTYPE(ParamType5))v5,
                                (PARAM_CALLEDTYPE(ParamType6))v6,
                                (PARAM_CALLEDTYPE(ParamType7))v7);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 7);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);

        R returnValue = (*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3,
                            (PARAM_CALLEDTYPE(ParamType4))p4,
                            (PARAM_CALLEDTYPE(ParamType5))p5,
                            (PARAM_CALLEDTYPE(ParamType6))p6,
                            (PARAM_CALLEDTYPE(ParamType7))p7);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
class CGenericMethodStatic7<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> : public CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>
{
public:
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType1 StoredType1;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType2 StoredType2;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType3 StoredType3;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType4 StoredType4;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType5 StoredType5;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType6 StoredType6;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType7 StoredType7;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType6 ParamBaseType6;
    typedef typename CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType7 ParamBaseType7;

    CGenericMethodStatic7(void(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName) :
        CGenericMethodStatic7_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic7(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6,
                (PARAM_CALLEDTYPE(ParamType7))v7);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_ASSERT(params.size() == 7);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6,
            (PARAM_CALLEDTYPE(ParamType7))p7);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethodStatic8_     ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
class CGenericMethodStatic8_ : public CMethodBase
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethodStatic8_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamBaseType			ParamBaseType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamBaseType			ParamBaseType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamBaseType			ParamBaseType6;
    typedef typename StoredTypeSelector<ParamType7>::ParamBaseType			ParamBaseType7;
    typedef typename StoredTypeSelector<ParamType8>::ParamBaseType			ParamBaseType8;

    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType			ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType			ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType			ParamPointerType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamPointerType			ParamPointerType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamPointerType			ParamPointerType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamPointerType			ParamPointerType6;
    typedef typename StoredTypeSelector<ParamType7>::ParamPointerType			ParamPointerType7;
    typedef typename StoredTypeSelector<ParamType8>::ParamPointerType			ParamPointerType8;

    typedef typename StoredTypeSelector<ParamType1>::StoredType		StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType		StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType		StoredType3;
    typedef typename StoredTypeSelector<ParamType4>::StoredType		StoredType4;
    typedef typename StoredTypeSelector<ParamType5>::StoredType		StoredType5;
    typedef typename StoredTypeSelector<ParamType6>::StoredType		StoredType6;
    typedef typename StoredTypeSelector<ParamType7>::StoredType		StoredType7;
    typedef typename StoredTypeSelector<ParamType8>::StoredType		StoredType8;

    CGenericMethodStatic8_(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr), m_param1(), m_param2(), m_param3(), m_param4(), m_param5(), m_param6(), m_param7(), m_param8(),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false), m_paramRangeValid6(false), m_paramRangeValid7(false), m_paramRangeValid8(false)
    {
        m_bStatic = true;
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min6 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max6 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min7 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max7 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_max8 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min8 = CMETHODBASE_RANGE_MIN_DEFAULT;
    }

    CGenericMethodStatic8_(const CGenericMethodStatic8_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;
        this->m_paramDisplayName4 = copy.m_paramDisplayName4;
        this->m_paramDisplayName5 = copy.m_paramDisplayName5;
        this->m_paramDisplayName6 = copy.m_paramDisplayName6;
        this->m_paramDisplayName7 = copy.m_paramDisplayName7;
        this->m_paramDisplayName8 = copy.m_paramDisplayName8;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_param4 = copy.m_param4;
        this->m_param5 = copy.m_param5;
        this->m_param6 = copy.m_param6;
        this->m_param7 = copy.m_param7;
        this->m_param8 = copy.m_param8;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
        this->m_paramRangeValid4 = copy.m_paramRangeValid4;
        this->m_paramRangeValid5 = copy.m_paramRangeValid5;
        this->m_paramRangeValid6 = copy.m_paramRangeValid6;
        this->m_paramRangeValid7 = copy.m_paramRangeValid7;
        this->m_paramRangeValid8 = copy.m_paramRangeValid8;
		this->m_min1 = copy.m_min1;
		this->m_max1 = copy.m_max1;
		this->m_min2 = copy.m_min2;
		this->m_max2 = copy.m_max2;
		this->m_min3 = copy.m_min3;
		this->m_max3 = copy.m_max3;
		this->m_min4 = copy.m_min4;
		this->m_max4 = copy.m_max4;
		this->m_min5 = copy.m_min5;
		this->m_max5 = copy.m_max5;
		this->m_min6 = copy.m_min6;
		this->m_max6 = copy.m_max6;
		this->m_min7 = copy.m_min7;
		this->m_max7 = copy.m_max7;
		this->m_max8 = copy.m_max8;
		this->m_min8 = copy.m_min8;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
        this->m_param4.LoadFromXML(parent, xmlNode, "param4");
        this->m_param5.LoadFromXML(parent, xmlNode, "param5");
        this->m_param6.LoadFromXML(parent, xmlNode, "param6");
        this->m_param7.LoadFromXML(parent, xmlNode, "param7");
        this->m_param8.LoadFromXML(parent, xmlNode, "param8");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
        this->m_param4.SaveToXML(parent, xmlNode, "param4");
        this->m_param5.SaveToXML(parent, xmlNode, "param5");
        this->m_param6.SaveToXML(parent, xmlNode, "param6");
        this->m_param7.SaveToXML(parent, xmlNode, "param7");
        this->m_param8.SaveToXML(parent, xmlNode, "param8");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName);
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName4);
            paramNode->setAttr("Desc", m_paramDesc4);

            behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
            paramNode->setAttr("Type", typeStr4.c_str());

            if (this->m_param4.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param4.GetValue());
            }

            if (m_paramRangeValid4)
            {
                paramNode->setAttr("RangeMin", m_min4);
                paramNode->setAttr("RangeMax", m_max4);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType4, behaviac::Meta::IsEnum<ParamBaseType4>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName5);
            paramNode->setAttr("Desc", m_paramDesc5);

            behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
            paramNode->setAttr("Type", typeStr5.c_str());

            if (this->m_param5.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param5.GetValue());
            }

            if (m_paramRangeValid5)
            {
                paramNode->setAttr("RangeMin", m_min5);
                paramNode->setAttr("RangeMax", m_max5);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType5, behaviac::Meta::IsEnum<ParamBaseType5>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName6);
            paramNode->setAttr("Desc", m_paramDesc6);

            behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
            paramNode->setAttr("Type", typeStr6.c_str());

            if (this->m_param6.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param6.GetValue());
            }

            if (m_paramRangeValid6)
            {
                paramNode->setAttr("RangeMin", m_min6);
                paramNode->setAttr("RangeMax", m_max6);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType6, behaviac::Meta::IsEnum<ParamBaseType6>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName7);
            paramNode->setAttr("Desc", m_paramDesc7);

            behaviac::string typeStr7 = ::GetTypeDescString<ParamType7>();
            paramNode->setAttr("Type", typeStr7.c_str());

            if (this->m_param7.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param7.GetValue());
            }

            if (m_paramRangeValid7)
            {
                paramNode->setAttr("RangeMin", m_min7);
                paramNode->setAttr("RangeMax", m_max7);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType7, behaviac::Meta::IsEnum<ParamBaseType7>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName8);
            paramNode->setAttr("Desc", m_paramDesc7);

            behaviac::string typeStr8 = ::GetTypeDescString<ParamType8>();
            paramNode->setAttr("Type", typeStr8.c_str());

            if (this->m_param8.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param8.GetValue());
            }

            if (m_paramRangeValid8)
            {
                paramNode->setAttr("RangeMin", m_min8);
                paramNode->setAttr("RangeMax", m_max8);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType8, behaviac::Meta::IsEnum<ParamBaseType8>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;

        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;

        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;

        }
        else if (m_paramDisplayName7.empty())
        {
            m_paramDisplayName7 = paramDisplayName;

        }
        else if (m_paramDisplayName8.empty())
        {
            m_paramDisplayName8 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else if (m_paramDesc4.empty())
        {
            m_paramDesc4 = paramDesc;

        }
        else if (m_paramDesc5.empty())
        {
            m_paramDesc5 = paramDesc;

        }
        else if (m_paramDesc6.empty())
        {
            m_paramDesc6 = paramDesc;

        }
        else if (m_paramDesc7.empty())
        {
            m_paramDesc7 = paramDesc;

        }
        else if (m_paramDesc8.empty())
        {
            m_paramDesc8 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }
    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;
            m_paramDesc4 = (paramDesc == 0 ? m_paramDisplayName4 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType4>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid4 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min4 = rangeMin;
                this->m_max4 = rangeMax;
            }
        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;
            m_paramDesc5 = (paramDesc == 0 ? m_paramDisplayName5 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType5>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid5 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min5 = rangeMin;
                this->m_max5 = rangeMax;
            }
        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;
            m_paramDesc6 = (paramDesc == 0 ? m_paramDisplayName6 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType6>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid6 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min6 = rangeMin;
                this->m_max6 = rangeMax;
            }
        }
        else if (m_paramDisplayName7.empty())
        {
            m_paramDisplayName7 = paramDisplayName;
            m_paramDesc7 = (paramDesc == 0 ? m_paramDisplayName7 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType7>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid7 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min7 = rangeMin;
                this->m_max7 = rangeMax;
            }
        }
        else if (m_paramDisplayName8.empty())
        {
            m_paramDisplayName8 = paramDisplayName;
            m_paramDesc8 = (paramDesc == 0 ? m_paramDisplayName8 : paramDesc);
            int typeId = GetClassTypeNumberId<ParamBaseType8>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid8 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min8 = rangeMin;
                this->m_max8 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DEFAULTVALUE(int paramIndex, const DefaultParam_t& defaultParam)
    {
        if (paramIndex == 0)
        {
            DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;

        }
        else if (paramIndex == 1)
        {
            DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;

        }
        else if (paramIndex == 2)
        {
            DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);;

        }
        else if (paramIndex == 3)
        {
            DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);;

        }
        else if (paramIndex == 4)
        {
            DefaulParamSetValueSelector<StoredType5, ParamBaseType5, behaviac::Meta::IsRefType<ParamType5>::Result>::SetValue(this->m_param5, defaultParam);;

        }
        else if (paramIndex == 5)
        {
            DefaulParamSetValueSelector<StoredType6, ParamBaseType6, behaviac::Meta::IsRefType<ParamType6>::Result>::SetValue(this->m_param6, defaultParam);;

        }
        else if (paramIndex == 6)
        {
            DefaulParamSetValueSelector<StoredType7, ParamBaseType7, behaviac::Meta::IsRefType<ParamType7>::Result>::SetValue(this->m_param7, defaultParam);;

        }
        else if (paramIndex == 7)
        {
            DefaulParamSetValueSelector<StoredType8, ParamBaseType8, behaviac::Meta::IsRefType<ParamType8>::Result>::SetValue(this->m_param8, defaultParam);;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "not supported param");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 8;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            case 3:
            {
                behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
                return typeStr4;
            }

            case 4:
            {
                behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
                return typeStr5;
            }

            case 5:
            {
                behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
                return typeStr6;
            }

            case 6:
            {
                behaviac::string typeStr7 = ::GetTypeDescString<ParamType7>();
                return typeStr7;
            }

            case 7:
            {
                behaviac::string typeStr8 = ::GetTypeDescString<ParamType8>();
                return typeStr8;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4, const void* param5, const void* param6, const void* param7, const void* param8)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        (*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3, *(ParamPointerType4)param4, *(ParamPointerType5)param5, *(ParamPointerType6)param6, *(ParamPointerType7)param7, *(ParamPointerType8)param8);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic8_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
                typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7,
                           (PARAM_CALLEDTYPE(ParamType8))v8);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
            typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6,
                (PARAM_CALLEDTYPE(ParamType7))v7,
                (PARAM_CALLEDTYPE(ParamType8))v8);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
            this->m_param8.SetVariableRegistry(parHolder, v8);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 8);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);
        PARAM_BASETYPE(ParamType8) p8;
        params[7]->GetValue(p8);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6,
            (PARAM_CALLEDTYPE(ParamType7))p7,
            (PARAM_CALLEDTYPE(ParamType8))p8);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(*m_methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;
    behaviac::wstring				m_paramDisplayName4;
    behaviac::wstring				m_paramDesc4;
    behaviac::wstring				m_paramDisplayName5;
    behaviac::wstring				m_paramDesc5;
    behaviac::wstring				m_paramDisplayName6;
    behaviac::wstring				m_paramDesc6;
    behaviac::wstring				m_paramDisplayName7;
    behaviac::wstring				m_paramDesc7;
    behaviac::wstring				m_paramDisplayName8;
    behaviac::wstring				m_paramDesc8;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;
    bool							m_paramRangeValid4;
    float							m_min4, m_max4;
    bool							m_paramRangeValid5;
    float							m_min5, m_max5;
    bool							m_paramRangeValid6;
    float							m_min6, m_max6;
    bool							m_paramRangeValid7;
    float							m_min7, m_max7;
    bool							m_paramRangeValid8;
    float							m_min8, m_max8;

    ParamVariable<StoredType1>	m_param1;
    ParamVariable<StoredType2>	m_param2;
    ParamVariable<StoredType3>	m_param3;
    ParamVariable<StoredType4>	m_param4;
    ParamVariable<StoredType5>	m_param5;
    ParamVariable<StoredType6>	m_param6;
    ParamVariable<StoredType7>	m_param7;
    ParamVariable<StoredType8>	m_param8;
};

template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
class CGenericMethodStatic8 : public CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>
{
public:
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType1 StoredType1;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType2 StoredType2;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType3 StoredType3;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType4 StoredType4;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType5 StoredType5;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType6 StoredType6;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType7 StoredType7;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType8 StoredType8;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType6 ParamBaseType6;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType7 ParamBaseType7;
    typedef typename CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType8 ParamBaseType8;

    CGenericMethodStatic8(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName) :
        CGenericMethodStatic8_<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(methodPtr, className, propertyName)
    {}

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic8(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
                typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7,
                           (PARAM_CALLEDTYPE(ParamType8))v8);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
            typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

            R returnValue = (*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                (PARAM_CALLEDTYPE(ParamType4))v4,
                                (PARAM_CALLEDTYPE(ParamType5))v5,
                                (PARAM_CALLEDTYPE(ParamType6))v6,
                                (PARAM_CALLEDTYPE(ParamType7))v7,
                                (PARAM_CALLEDTYPE(ParamType8))v8);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
            this->m_param8.SetVariableRegistry(parHolder, v8);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 7);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);
        PARAM_BASETYPE(ParamType8) p8;
        params[7]->GetValue(p8);

        R returnValue = (*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3,
                            (PARAM_CALLEDTYPE(ParamType4))p4,
                            (PARAM_CALLEDTYPE(ParamType5))p5,
                            (PARAM_CALLEDTYPE(ParamType6))p6,
                            (PARAM_CALLEDTYPE(ParamType7))p7,
                            (PARAM_CALLEDTYPE(ParamType8))p8);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
class CGenericMethodStatic8<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> : public CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>
{
public:
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType1 StoredType1;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType2 StoredType2;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType3 StoredType3;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType4 StoredType4;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType5 StoredType5;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType6 StoredType6;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType7 StoredType7;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType8 StoredType8;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType6 ParamBaseType6;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType7 ParamBaseType7;
    typedef typename CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType8 ParamBaseType8;

    CGenericMethodStatic8(void(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName) :
        CGenericMethodStatic8_<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(methodPtr, className, propertyName)
    {}

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethodStatic8(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

                }
                else
                {
                    nameTemp = this->m_propertyName;
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
                typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

                bHandled = behaviac::Network::GetInstance()->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7,
                           (PARAM_CALLEDTYPE(ParamType8))v8);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
            typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

            (*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6,
                (PARAM_CALLEDTYPE(ParamType7))v7,
                (PARAM_CALLEDTYPE(ParamType8))v8);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
            this->m_param8.SetVariableRegistry(parHolder, v8);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_ASSERT(params.size() == 8);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);
        PARAM_BASETYPE(ParamType8) p8;
        params[7]->GetValue(p8);

        (*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6,
            (PARAM_CALLEDTYPE(ParamType7))p7,
            (PARAM_CALLEDTYPE(ParamType8))p8);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_STATICMETHOD_H
