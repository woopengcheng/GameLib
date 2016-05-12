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

#ifndef BEHAVIAC_ENGINESERVICES_ENUMMEMBER_H
#define BEHAVIAC_ENGINESERVICES_ENUMMEMBER_H

#include "behaviac/base/object/member.h"

/////////////////////////////////////////////////////////
///////////////     ENUM MEMBER      ///////////////
/////////////////////////////////////////////////////////

// Easiest way to serialize enums.
// - enumName: human name for the enum, prefixes ("sel" & "enum") will be added.
// - enumValueName: int member containing the enum's current value
// - propertyFlags: flags determining when enumValueName will be serialized
#define BEGIN_ENUM_MEMBER(enumName, enumValueName, propertyFlags) \
    { \
        DECLARE_UIWRAPPER(DefaultUiInfo);\
        CMemberBase* property_ = CMemberFactory::Create<BasicTypeHandlerEnum, propertyFlags>(objectType::GetClassTypeName(), enumName, (uint32_t objectType::*)&objectType::enumValueName, localWrapper); \
        CTagObjectDescriptor::PushBackMember(ms_members, property_); \
        CEnumMember* enumMember = BEHAVIAC_NEW CEnumMember(enumName, objectType::GetClassTypeName()); \
        enumMember->SetEnumTypeName(&objectType::enumValueName);\
        CTagObjectDescriptor::PushBackMember(ms_members, enumMember);

#define REGISTER_ENUM_VALUE(enumValue) \
    enumMember->AddValue(enumValue);

#define REGISTER_CUSTOM_ENUM_VALUE(enumValue, customValue) \
    enumMember->AddValue(enumValue, customValue);

#define ENUM_REGISTRAR(registrarMethod) \
    const behaviac::vector<const char*>& enumNames = registrarMethod(); \
    behaviac::vector<const char*>::const_iterator end = enumNames.end(); \
    for (behaviac::vector<const char*>::const_iterator iter = enumNames.begin(); iter != end; ++iter) \
    {                                  \
        enumMember->AddValue((*iter)); \
    }

#define ENUM_REGISTRAR_CUSTOM(registrarMethod) \
    const behaviac::vector< std::pair< const char*, int32_t > >& enumNames = registrarMethod(); \
    behaviac::vector< std::pair< const char*, int32_t > >::const_iterator end = enumNames.end(); \
    for (behaviac::vector< std::pair< const char*, int32_t > >::const_iterator iter = enumNames.begin(); iter != end; ++iter) \
    {                                  \
        enumMember->AddValue((*iter).first, (*iter).second); \
    }

#define END_ENUM_MEMBER() \
    }

class CEnumMember : public CMemberBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CEnumMember);

public:
    // put u32 as type or enum name ?
    CEnumMember(const char* enumName, const char* className) : CMemberBase(enumName, className)
    {
    }

    CEnumMember(const CEnumMember& copy) : CMemberBase(copy), m_values(copy.m_values), m_enumTypeName(copy.m_enumTypeName)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CEnumMember(*this);

        return p;
    }

    virtual void Load(CTagObject* parent, const behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(node);
        // DO NOTHING
    }

    virtual void Save(const CTagObject* parent, behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(node);

        // DO NOTHING
    }

    virtual void LoadState(CTagObject* parent, const behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(node);

        // DO NOTHING
    }

    virtual void SaveState(const CTagObject* parent, behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(node);

        // DO NOTHING
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(types);
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Member");
            memberNode->setAttr("Name", this->m_propertyID.GetString());
            memberNode->setAttr("Type", this->m_enumTypeName.c_str());

            if (this->m_classFullName)
            {
                memberNode->setAttr("Class", this->m_classFullName);
            }

            if (this->m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }
        }
        else
        {
            for (behaviac::vector<SEnumValue>::const_iterator iter = m_values.begin(); iter != m_values.end(); ++iter)
            {
                const SEnumValue& enumValue = *iter;
                behaviac::XmlNodeRef enumNodeElement = memberNode->newChild("enum");
                enumNodeElement->setAttr("Value", enumValue.m_valueName);

                if (enumValue.m_hasCustomValue)
                {
                    enumNodeElement->setAttr("CustomValue", enumValue.m_customValue);
                }
            }
        }
    }

    virtual void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(types);

        //TODO;
    }

    void AddValue(const char* value)
    {
        m_values.push_back(SEnumValue(value));
    }
    void AddValue(const char* value, int32_t customValue)
    {
        m_values.push_back(SEnumValue(value, customValue));
    }

    template <typename TOBJ, typename TMEMBER>
    void SetEnumTypeName(TMEMBER TOBJ::* member)
    {
        this->m_enumTypeName = ::GetClassTypeName((TMEMBER*)0);
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
        //typedef PARAM_BASETYPE(int)				StoredMemberType;
        //behaviac::Property* pProperty = behaviac::Property::Creator<StoredMemberType>(defaultValue, this);
        behaviac::Property* pProperty = behaviac::Property::Creator<int>(defaultValue, this, bConst);

        return pProperty;
    }

    virtual float DifferencePercentage(const behaviac::Property* l, const behaviac::Property* r)
    {
        return CMemberBase::TDifferencePercentage<unsigned int>(l, r);
    }

private:
    struct SEnumValue
    {
        SEnumValue(const char* valueName)
			: m_valueName(valueName), m_hasCustomValue(false), m_customValue(0)
        {}

        SEnumValue(const char* valueName, int32_t customValue)
            : m_valueName(valueName), m_hasCustomValue(true), m_customValue(customValue)
        {}

        const char* m_valueName;
        bool m_hasCustomValue;
        int32_t m_customValue;
    };

    behaviac::vector<SEnumValue> m_values;
    behaviac::string m_enumTypeName;
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_ENUMMEMBER_H
