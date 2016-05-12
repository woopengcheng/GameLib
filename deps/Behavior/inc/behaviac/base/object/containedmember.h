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

#ifndef BEHAVIAC_ENGINESERVICES_CONTAINEDMEMBER_H
#define BEHAVIAC_ENGINESERVICES_CONTAINEDMEMBER_H

#include "behaviac/base/object/member.h"

/////////////////////////////////////////////////////////
///////////////     CONTAINED MEMBER      ///////////////
/////////////////////////////////////////////////////////

#define REGISTER_CONTAINED_MEMBER(propertyName, memberName, propertyFlags, UiDescriptor)           \
    {                                                                                                  \
        DECLARE_UIWRAPPER(UiDescriptor);                                                               \
        CMemberBase* property = CContainedMemberFactory<propertyFlags, CreateChildNode>::Create(       \
                                propertyName, &objectType::Load##memberName, &objectType::Save##memberName, localWrapper); \
        CTagObjectDescriptor::PushBackMember(ms_members, property);                                  \
    }

template<class ObjectType, uint32_t PropertyFlags, bool ChildNodeCreate>
class CContainedMember : public CMemberBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CContainedMember);

public:
    CContainedMember(void (ObjectType::*Load)(const XmlConstNodeRef&, uint32_t), void (ObjectType::*Save)(const behaviac::XmlNodeRef&, uint32_t) const, const char* className, const char* propertyName, UiGenericType* uiWrapper)
        : CMemberBase(propertyName, className), m_load(Load), m_save(Save), m_uiWrapper(uiWrapper)
    {}

    CContainedMember(const CContainedMember& copy) : :
        CMemberBase(copy), m_memberPtr(memberPtr), m_uiWrapper(copy.m_uiWrapper)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CContainedMember(*this);

        return p;
    }

    virtual void Load(CTagObject* parent, const ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_Description_Load))
        {
            if (ChildNodeCreate)
            {
                XmlConstNodeRef childNode;

                if (node->getAttr(m_propertyID, childNode))
                {
                    (((ObjectType*)parent)->*m_load)(childNode, EPersistenceType_Description_Load);
                }
            }
            else
            {
                BEHAVIAC_LOGERROR("Unsupported");
            }
        }
    }

    virtual void Save(const CTagObject* parent, ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_Description_Save))
        {
            if (ChildNodeCreate)
            {
                XmlNodeRef childNode = CreateXmlNode(m_propertyID.GetString());
                (((ObjectType*)parent)->*m_save)(childNode, EPersistenceType_Description_Save);
                node->setAttr(m_propertyID, (XmlConstNodeRef)childNode);

            }
            else
            {
                BEHAVIAC_LOGERROR("Unsupported");
            }
        }
    }

    virtual void LoadState(CTagObject* parent, const ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_State_Load))
        {
            if (ChildNodeCreate)
            {
                XmlConstNodeRef childNode;

                if (node->getAttr(m_propertyID, childNode))
                {
                    (((ObjectType*)parent)->*m_load)(childNode, EPersistenceType_State_Load);
                }
            }
            else
            {
                BEHAVIAC_LOGERROR("Unsupported");
            }
        }
    }

    virtual void SaveState(const CTagObject* parent, ISerializableNode* node)
    {
        if ((PropertyFlags & EPersistenceType_State_Save))
        {
            if (ChildNodeCreate)
            {
                XmlNodeRef childNode = CreateXmlNode(m_propertyID.GetString());
                (((ObjectType*)parent)->*m_save)(childNode, EPersistenceType_State_Save);
                node->setAttr(m_propertyID, (XmlConstNodeRef)childNode);

            }
            else
            {
                BEHAVIAC_LOGERROR("Unsupported");
            }
        }
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        if ((PropertyFlags & EPersistenceType_UiInfo))
        {
            if (ChildNodeCreate)
            {
                if (types == NULL)
                {
                    XmlNodeRef memberNode = xmlNode->newChild("Member");
                    memberNode->setAttr("Name", m_propertyID.GetString());
                    memberNode->setAttr("ContainerElement", true);

                    if (this->m_classFullName)
                    {
                        memberNode->setAttr("Class", this->m_classFullName);
                    }

                    if (m_bStatic)
                    {
                        memberNode->setAttr("Static", "true");
                    }

                    bool readonlyFlag = this->READONLYFLAG();

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

                    (((ObjectType*)parent)->*m_save)(memberNode, EPersistenceType_UiInfo);
                }
            }
            else
            {
                BEHAVIAC_LOGERROR("Unsupported");
            }
        }
    }

    virtual void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(types);
    }

    void (ObjectType::*m_load)(const XmlConstNodeRef&, uint32_t);
    void (ObjectType::*m_save)(const behaviac::XmlNodeRef&, uint32_t) const;

    UiGenericType* m_uiWrapper;
};

template<uint32_t PropertyFlags, bool ChildNodeCreate>
struct CContainedMemberFactory
{
    template<class ObjectType>
    static CMemberBase* Create(const char* propertyName, void (ObjectType::*Load)(const XmlConstNodeRef&, uint32_t), void (ObjectType::*Save)(const behaviac::XmlNodeRef&, uint32_t) const, UiGenericType* uiWrapper)
    {
        typedef CContainedMember<ObjectType, PropertyFlags, ChildNodeCreate> MemberType;
        return BEHAVIAC_NEW MemberType(Load, Save, propertyName, uiWrapper);
    }
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_CONTAINEDMEMBER_H
