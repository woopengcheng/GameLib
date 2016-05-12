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

#ifndef BEHAVIAC_ENGINESERVICES_PROPERTYLISTMEMBER_H
#define BEHAVIAC_ENGINESERVICES_PROPERTYLISTMEMBER_H

#include "behaviac/base/object/tagobject.h"
#include "behaviac/base/object/member.h"

#include "behaviac/base/object/propertylisttypehandler.h"

#include "engineservices/property/propertylistdescriptor.h"
#include "engineservices/property/propertydescriptor.h"

/////////////////////////////////////////////////////////
///////////////     CONTAINER MEMBER      ///////////////
/////////////////////////////////////////////////////////

#define REGISTER_PROPERTYLIST_MEMBER(propertyName, memberName, propertyFlags, UiDescriptor) \
    { \
        DECLARE_UIWRAPPER(UiDescriptor); \
        if( localWrapper )\
        { \
            localWrapper->SetMemberName(propertyName); \
        } \
        CMemberBase* property = NULL; \
        property = CPropertlyListMemberFactory< propertyFlags>::Create( &objectType::memberName, propertyName, localWrapper); \
        CTagObjectDescriptor::PushBackMember(ms_members, property); \
    }

template<class ObjectType, uint32_t PropertyFlags>
class CPropertlyListMember : public CMemberBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CPropertlyListMember);

    CPropertlyListMember(CPropertyList * ObjectType::* memberPtr, const char* className, const char* propertyName, UiGenericType* uiWrapper)
        : CMemberBase(propertyName, className), m_memberPtr(memberPtr), m_uiWrapper(uiWrapper)
    {}

    CPropertlyListMember(const CPropertlyListMember& copy) : CMemberBase(copy), m_memberPtr(copy.m_memberPtr), m_uiWrapper(copy.m_uiWrapper)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CPropertlyListMember(*this);

        return p;
    }

    virtual void Load(CTagObject* parent, const ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_Description_Load) && ObjectType::IsOfMyKind(parent))
        {
            CPropertyList* propList = ((ObjectType*)parent)->*m_memberPtr;

            if (propList)
            {
                propList->Load(node);
            }
        }
    }

    virtual void Save(const CTagObject* parent, ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_Description_Save) && ObjectType::IsOfMyKind(parent))
        {
            CPropertyList* propList = ((ObjectType*)parent)->*m_memberPtr;

            if (propList)
            {
                propList->Save(node);
            }
        }
    }

    virtual void LoadState(CTagObject* parent, const ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_State_Load) && ObjectType::IsOfMyKind(parent))
        {
            CPropertyList* propList = ((ObjectType*)parent)->*m_memberPtr;
            propList->Load(node);
        }
    }

    virtual void SaveState(const CTagObject* parent, ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_State_Save) && ObjectType::IsOfMyKind(parent))
        {
            CPropertyList* propList = ((ObjectType*)parent)->*m_memberPtr;
            propList->Save(node);
        }
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        if (types == NULL && (PropertyFlags & EPersistenceType_UiInfo) && ObjectType::IsOfMyKind(parent))
        {
            const CPropertyListDescriptor& descriptor = (((ObjectType*)parent)->*m_memberPtr)->GetDescriptor();

            for (ndSizeT i = 0; i < descriptor.NumProperties(); i++)
            {
                const CPropertyDescriptor& propDescriptor = descriptor.GetDescriptor(i);
                XmlNodeRef childNode = xmlNode->newChild("Member");
                childNode->setAttr("Name", propDescriptor.GetID());

                if (this->m_classFullName)
                {
                    memberNode->setAttr("Class", this->m_classFullName);
                }

                if (m_bStatic)
                {
                    memberNode->setAttr("Static", "true");
                }

                childNode->setAttr("ContainerElement", true);

                if (m_uiWrapper)
                {
                    m_uiWrapper->SaveDescription(childNode);
                }
            }
        }
    }

    virtual void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        XmlNodeRef childNode = xmlNode;

        if (types == NULL)
        {
            childNode = xmlNode->newChild(m_propertyID.GetString());
        }

        m_handler.GetMethodsDescription(types, childNode, (ObjectType*)parent, ((ObjectType*)parent)->*m_memberPtr);
    }

private:
    CPropertyList* ObjectType::* m_memberPtr;
    UiGenericType* m_uiWrapper;
};

template <uint32_t PropertyFlags>
struct CPropertlyListMemberFactory
{
    template<class ObjectType>
    static CMemberBase* Create(CPropertyList * ObjectType::* memberPtr, const char* propertyName, UiGenericType* uiWrapper)
    {
        typedef CPropertlyListMember<ObjectType, PropertyFlags> MemberType;
        return BEHAVIAC_NEW MemberType(memberPtr, propertyName, uiWrapper);
    }
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_PROPERTYLISTMEMBER_H
