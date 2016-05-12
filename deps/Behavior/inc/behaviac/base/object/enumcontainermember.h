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

#ifndef BEHAVIAC_ENGINESERVICES_ENUMCONTAINERMEMBER_H
#define BEHAVIAC_ENGINESERVICES_ENUMCONTAINERMEMBER_H

#include "behaviac/base/object/containermember.h"
#include "behaviac/base/object/enummember.h"

/////////////////////////////////////////////////////////
////////////     ENUM CONTAINER MEMBER      /////////////
/////////////////////////////////////////////////////////

#define BEGIN_ENUM_CONTAINER_MEMBER(containerName, elementName, enumName, memberName, propertyFlags, containerProvider, UiDescriptor) \
    { \
        DECLARE_UIWRAPPER(UiDescriptor); \
        CEnumMember* enumMember; \
        CMemberBase* enumContainerMember = CEnumContainerMemberFactory<GenericContainerHandler, containerProvider, BasicTypeHandlerEnum, propertyFlags, true>::Create( \
                                           &objectType::memberName, containerName, elementName, "sel"enumName, "enum"enumName, localWrapper, enumMember); \
        CTagObjectDescriptor::PushBackMember(ms_members, enumContainerMember);

template<class ObjectType, class ContainerType, class ContainerHandler, uint32_t PropertyFlags, bool ChildNodeCreate>
class CEnumContainerMember : public CContainerMember<ObjectType, ContainerType, ContainerHandler, PropertyFlags, ChildNodeCreate>
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CEnumContainerMember);

public:
    typedef CContainerMember<ObjectType, ContainerType, ContainerHandler, PropertyFlags, ChildNodeCreate> super;

    CEnumContainerMember(ContainerType ObjectType::* memberPtr, const char* propertyName, const char* elementName, const char* selName, const char* enumName, UiGenericType* uiWrapper)
        : super(memberPtr, propertyName, elementName, selName, "", uiWrapper), m_enumMember(enumName), m_elementID(elementName)
    {
    }

    virtual void Save(const CTagObject* parent, ISerializableNode* node)
    {
        super::Save(parent, node);
        // Retrieve the child node if necessary
        ISerializableNode* childNode;

        if (ChildNodeCreate)
        {
            childNode = node->findChild(super::m_propertyID);

        }
        else
        {
            childNode = node;
        }

        // Add the enum definition to every element of the container
        SerializableNodeIt iter(childNode);

        for (ISerializableNode* elementNode = iter.first(m_elementID); elementNode; elementNode = iter.next(m_elementID))
        {
            m_enumMember.Save(parent, elementNode);
        }
    }

    CEnumMember* GetEnumMember()
    {
        return &m_enumMember;
    }

private:
    CEnumMember m_enumMember;
    CSerializationID m_elementID;
};

template <template <class T1, template <class T2> class T3> class ContainerHandler, template <class T1, class T2> class ContainerProvider, template <class T> class ContainedTypeHandler, uint32_t PropertyFlags, bool ChildNodeCreate>
struct CEnumContainerMemberFactory
{
    template<class ObjectType, class ContainerType>
    static CMemberBase* Create(ContainerType ObjectType::* memberPtr, const char* propertyName, const char* elementName, const char* selName, const char* enumName, UiGenericType* uiWrapper, CEnumMember*& enumMember)
    {
        typedef CEnumContainerMember<ObjectType, ContainerType, ContainerHandler< ContainerProvider< ObjectType, ContainerType >, ContainedTypeHandler >, PropertyFlags, ChildNodeCreate> MemberType;
        MembetType* enumContainerMember = BEHAVIAC_NEW MembetType(memberPtr, propertyName, elementName, selName, enumName, uiWrapper);
        enumMember = enumContainerMember->GetEnumMember();
        return enumContainerMember;
    }
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_ENUMCONTAINERMEMBER_H
