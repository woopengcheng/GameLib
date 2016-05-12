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

#ifndef BEHAVIORTREE_PROPERTYMEMMBER_H
#define BEHAVIORTREE_PROPERTYMEMMBER_H

template<>
struct GenericTypeHandler<behaviac::Property*>
{
    static void Load(const behaviac::ISerializableNode* node, behaviac::Property* member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        BEHAVIAC_UNUSED_VAR(node);
        BEHAVIAC_UNUSED_VAR(member);
        BEHAVIAC_UNUSED_VAR(className);
        BEHAVIAC_UNUSED_VAR(propertyID);

        BEHAVIAC_ASSERT(0, "not implemented");
    }

    static void Save(behaviac::ISerializableNode* node, behaviac::Property* member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        BEHAVIAC_UNUSED_VAR(node);
        BEHAVIAC_UNUSED_VAR(member);
        BEHAVIAC_UNUSED_VAR(className);
        BEHAVIAC_UNUSED_VAR(propertyID);

        BEHAVIAC_ASSERT(0, "not implemented");
    }

    static void LoadState(const behaviac::ISerializableNode* node, behaviac::Property* member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        BEHAVIAC_UNUSED_VAR(node);
        BEHAVIAC_UNUSED_VAR(member);
        BEHAVIAC_UNUSED_VAR(className);
        BEHAVIAC_UNUSED_VAR(propertyID);

        BEHAVIAC_ASSERT(0, "not implemented");
    }

    static void SaveState(behaviac::ISerializableNode* node, behaviac::Property* member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        BEHAVIAC_UNUSED_VAR(node);
        BEHAVIAC_UNUSED_VAR(member);
        BEHAVIAC_UNUSED_VAR(className);
        BEHAVIAC_UNUSED_VAR(propertyID);

        BEHAVIAC_ASSERT(0, "not implemented");
    }

    static void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, behaviac::Property* const& member, bool bStatic, int readonlyFlag, const char* classFullName, const behaviac::CSerializationID& propertyID, UiGenericType* uiWrapper)
    {
        BEHAVIAC_UNUSED_VAR(types);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(member);
        BEHAVIAC_UNUSED_VAR(bStatic);
        BEHAVIAC_UNUSED_VAR(classFullName);
        BEHAVIAC_UNUSED_VAR(propertyID);
        BEHAVIAC_UNUSED_VAR(uiWrapper);
        BEHAVIAC_UNUSED_VAR(readonlyFlag);

        BEHAVIAC_ASSERT(0, "not implemented");
    }

    static void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, behaviac::Property* const& member, const char* className, const char* propertyName)
    {
        BEHAVIAC_UNUSED_VAR(types);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(member);
        BEHAVIAC_UNUSED_VAR(className);
        BEHAVIAC_UNUSED_VAR(propertyName);

        BEHAVIAC_ASSERT(0, "not implemented");
    }
};

//behaviac::Property* LoadRight(const char* value, const behaviac::string& propertyName, behaviac::string& typeName);
inline behaviac::Property* LoadProperty(const char* value)
{
    behaviac::string typeName;
    behaviac::string propertyName;
    behaviac::Property* opr = behaviac::Condition::LoadRight(value, typeName);

    return opr;
}

template<class ObjectType, class MemberHandler, uint32_t PropertyFlags>
class CGenericMember<ObjectType, behaviac::Property*, MemberHandler, PropertyFlags> : public CMemberBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMember);

    CGenericMember(behaviac::Property * ObjectType::* memberPtr, const char* className, const char* propertyName, UiGenericType* uiWrapper) : CMemberBase(propertyName, className), m_memberPtr(memberPtr), m_uiWrapper(uiWrapper)
    {
    }

    CGenericMember(const CGenericMember& copy) : CMemberBase(copy), m_memberPtr(copy.m_memberPtr), m_uiWrapper(copy.m_uiWrapper)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CGenericMember(*this);

        return p;
    }

    virtual void Load(CTagObject* parent, const behaviac::ISerializableNode* node)
    {
        if (PropertyFlags & EPersistenceType_Description_Load)
        {
            const char* valueStr = node->getAttrRaw(m_propertyID);

            behaviac::string propertyName;
            ((ObjectType*)parent)->*m_memberPtr = LoadProperty(valueStr);
        }
    }

    virtual void Save(const CTagObject* parent, behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(node);

        if (PropertyFlags & EPersistenceType_Description_Save)
        {
            BEHAVIAC_ASSERT(0, "not implemented");
        }
    }

    virtual void LoadState(CTagObject* parent, const behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(node);

        if (PropertyFlags & EPersistenceType_State_Load)
        {
            BEHAVIAC_ASSERT(0, "not implemented");
        }
    }

    virtual void SaveState(const CTagObject* parent, behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(node);

        if (PropertyFlags & EPersistenceType_State_Save)
        {
            BEHAVIAC_ASSERT(0, "not implemented");
        }
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(types);

        if (PropertyFlags & EPersistenceType_UiInfo)
        {
            BEHAVIAC_ASSERT(0, "not implemented");
        }
    }

    virtual void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(types);

        BEHAVIAC_ASSERT(0, "not implemented");
    }

    virtual void Set(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(value);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(0, "not implemented");
    }

    virtual const void* Get(const CTagObject* parent, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(0, "not implemented");
        return 0;
    }

    virtual void        SetVariable(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(value);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(0, "not implemented");
    }

    virtual void*       GetVariable(const CTagObject* parent, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(0, "not implemented");
        return NULL;
    };

    virtual int GetTypeId() const
    {
        BEHAVIAC_ASSERT(0, "not implemented");
        return 0;
    }

    virtual void SetFromProperty(CTagObject* parent, const CTagObject* parHolder, const behaviac::Property* pProperty) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_UNUSED_VAR(pProperty);
        BEHAVIAC_ASSERT(0, "not implemented");
    }

    virtual bool       Equal(const CTagObject* lhs, const CTagObject* rhs) const
    {
        BEHAVIAC_UNUSED_VAR(lhs);
        BEHAVIAC_UNUSED_VAR(rhs);

        BEHAVIAC_ASSERT(0, "not implemented");
        return false;
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
        BEHAVIAC_UNUSED_VAR(defaultValue);
        BEHAVIAC_UNUSED_VAR(bConst);

        BEHAVIAC_ASSERT(0, "not implemented");
        return 0;
    }

private:
    behaviac::Property* ObjectType::*    m_memberPtr;
    UiGenericType*						 m_uiWrapper;
};

#endif//BEHAVIORTREE_PROPERTYMEMMBER_H
