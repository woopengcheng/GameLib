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

#ifndef BEHAVIAC_ENGINESERVICES_CONTAINERMEMBER_H
#define BEHAVIAC_ENGINESERVICES_CONTAINERMEMBER_H

#include "behaviac/base/object/tagobject.h"
#include "behaviac/base/object/containertypehandler.h"

/////////////////////////////////////////////////////////
///////////////     CONTAINER MEMBER      ///////////////
/////////////////////////////////////////////////////////

#define REGISTER_CONTAINER_MEMBER(containerName, elementName, valueName, memberName, propertyFlags, UiDescriptor) \
    { \
        DECLARE_UIWRAPPER(UiDescriptor); \
        if( localWrapper )\
        { \
            localWrapper->SetMemberName(elementName); \
        } \
        CMemberBase* property = NULL; \
        if( strlen(containerName) == 0 ) \
        { \
            property = CContainerMemberFactory< GenericContainerHandler, TagVectorProvider, DefaultContainedTypeHandler, propertyFlags, false >::Create( \
                       &objectType::memberName, objectType::GetClassTypeName(), containerName, elementName, valueName, "", localWrapper); \
        } \
        else \
        { \
            property = CContainerMemberFactory< GenericContainerHandler, TagVectorProvider, DefaultContainedTypeHandler, propertyFlags, true >::Create( \
                       &objectType::memberName, objectType::GetClassTypeName(), containerName, elementName, valueName, "", localWrapper); \
        } \
        CTagObjectDescriptor::PushBackMember(ms_members, property); \
    }

template<class ObjectType, class ContainerType, class ContainerHandler, uint32_t PropertyFlags, bool ChildNodeCreate>
class CContainerMember : public CMemberBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CContainerMember);

    CContainerMember(ContainerType ObjectType::* memberPtr, const char* className, const char* propertyName, const char* elementName, const char* valueName, const char* idName, UiGenericType* uiWrapper)
        : CMemberBase(propertyName, className), m_memberPtr(memberPtr), m_uiWrapper(uiWrapper), m_handler(elementName, valueName, idName)
    {}

    CContainerMember(const CContainerMember& copy) : CMemberBase(copy), m_memberPtr(copy.m_memberPtr), m_uiWrapper(copy.m_uiWrapper), m_handler(copy.m_handler)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CContainerMember(*this);

        return p;
    }

    virtual void Load(CTagObject* parent, const behaviac::ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_Description_Load) && ObjectType::IsOfMyKind(parent))
        {
            const behaviac::ISerializableNode* childNode;

            if (ChildNodeCreate)
            {
                childNode = node->findChild(m_propertyID);

            }
            else
            {
                childNode = node;
            }

            if (childNode)
            {
                m_handler.Load(childNode, (ObjectType*)parent, ((ObjectType*)parent)->*m_memberPtr, this->m_classFullName);
            }
        }
    }

    virtual void Save(const CTagObject* parent, behaviac::ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_Description_Save) && ObjectType::IsOfMyKind(parent))
        {
            behaviac::ISerializableNode* childNode;

            if (ChildNodeCreate)
            {
                childNode = node->newChild(m_propertyID);

            }
            else
            {
                childNode = node;
            }

            m_handler.Save(childNode, (ObjectType*)parent, ((ObjectType*)parent)->*m_memberPtr, this->m_classFullName);
        }
    }

    virtual void LoadState(CTagObject* parent, const behaviac::ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_State_Load) && ObjectType::IsOfMyKind(parent))
        {
            const behaviac::ISerializableNode* childNode;

            if (ChildNodeCreate)
            {
                childNode = node->findChild(m_propertyID);

            }
            else
            {
                childNode = node;
            }

            if (childNode)
            {
                m_handler.LoadState(childNode, (ObjectType*)parent, ((ObjectType*)parent)->*m_memberPtr, this->m_classFullName);
            }
        }
    }

    virtual void SaveState(const CTagObject* parent, behaviac::ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_State_Save) && ObjectType::IsOfMyKind(parent))
        {
            behaviac::ISerializableNode* childNode;

            if (ChildNodeCreate)
            {
                childNode = node->newChild(m_propertyID);

            }
            else
            {
                childNode = node;
            }

            m_handler.SaveState(childNode, (ObjectType*)parent, ((ObjectType*)parent)->*m_memberPtr, this->m_classFullName);
        }
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        int readonlyFlag = this->READONLYFLAG();

        if ((PropertyFlags & EPersistenceType_UiInfo) && ObjectType::IsOfMyKind(parent))
        {
			behaviac::XmlNodeRef memberNode = xmlNode;

            if (ChildNodeCreate && types == NULL)
            {
                memberNode = xmlNode->newChild("Member");
                memberNode->setAttr("Name", m_propertyID.GetString());

                if (this->m_classFullName)
                {
                    memberNode->setAttr("Class", this->m_classFullName);
                }

                if (m_bStatic)
                {
                    memberNode->setAttr("Static", "true");
                }

                if (readonlyFlag & 0x1)
                {
                    memberNode->setAttr("Readonly", "true");
                }

                if (readonlyFlag & 0x2)
                {
                    memberNode->setAttr("Property", "true");
                }

                if (m_uiWrapper)
                {
                    m_uiWrapper->SaveDescription(memberNode);
                }
            }

            m_handler.GetUiInfo(types, memberNode, (ObjectType*)parent, ((ObjectType*)parent)->*m_memberPtr, m_bStatic, readonlyFlag, this->m_classFullName, m_propertyID, this->GetDisplayName(), this->GetDesc(), m_uiWrapper);
        }
    }

    virtual void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
		behaviac::XmlNodeRef childNode = xmlNode;

        if (types == NULL)
        {
            childNode = xmlNode->newChild(m_propertyID.GetString());
        }

        m_handler.GetMethodsDescription(types, childNode, (ObjectType*)parent, ((ObjectType*)parent)->*m_memberPtr, this->m_classFullName);
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
        BEHAVIAC_UNUSED_VAR(defaultValue);
        typedef PARAM_BASETYPE(ContainerType)				StoredMemberType;
        behaviac::Property* pProperty = behaviac::Property::Creator<StoredMemberType>(this, bConst);

        return pProperty;
    }
private:
    ContainerType ObjectType::* m_memberPtr;
    UiGenericType* m_uiWrapper;
    ContainerHandler m_handler;
};

template <template <class T1, template <class T2> class T3> class ContainerHandler, template <class T1, class T2> class ContainerProvider, template <class T> class ContainedTypeHandler, uint32_t PropertyFlags, bool ChildNodeCreate>
struct CContainerMemberFactory
{
    template<class ObjectType, class ContainerType>
    static CMemberBase* Create(ContainerType ObjectType::* memberPtr, const char* className, const char* propertyName, const char* elementName, const char* valueName, const char* idName, UiGenericType* uiWrapper)
    {
        typedef CContainerMember<ObjectType, ContainerType, ContainerHandler< ContainerProvider<ObjectType, ContainerType>, ContainedTypeHandler >, PropertyFlags, ChildNodeCreate> MemberType;
        return BEHAVIAC_NEW MemberType(memberPtr, className, propertyName, elementName, valueName, idName, uiWrapper);
    }
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_CONTAINERMEMBER_H
