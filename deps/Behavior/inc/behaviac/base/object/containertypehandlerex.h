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

#ifndef BEHAVIAC_ENGINESERVICES_CONTAINERTYPEHANDLEREX_H
#define BEHAVIAC_ENGINESERVICES_CONTAINERTYPEHANDLEREX_H

#include "behaviac/base/object/typehandler.h"
#include "behaviac/base/object/member.h"
//#include "behaviac/base/object/containertypehandler.h" // Inherit empty predicates

// Utility class to filter empty IDs. The default implementation compares
// the element to 0, but it can be specialized for other types.
template <class TParentType, class TContainerType, class TContainedType>
class CEmptyIdPredicate
{
public:
    static bool IsEmpty(TContainedType element)
    {
        return element == -1;
    }
};

// Example partial specialization for eastl::strings, the same can be done for any
// type, ideally specializing the parent type at the same time to avoid
// conflicts with other users (especially in blob where specializations have
// indefinite scope).
template <class TParentType, class TContainerType>
class CEmptyIdPredicate<TParentType, TContainerType, behaviac::string>
{
public:
    static bool IsEmpty(const behaviac::string& element)
    {
        return element.empty();
    }
};

template <class TParentType, class TContainerType>
class CEmptyIdPredicate<TParentType, TContainerType, behaviac::CStringID>
{
public:
    static bool IsEmpty(const behaviac::CStringID& element)
    {
        return !element.IsValid();
    }
};

template <class TParentType, class TContainerType>
class CEmptyIdPredicate<TParentType, TContainerType, behaviac::CNoCaseStringID>
{
public:
    static bool IsEmpty(const behaviac::CNoCaseStringID& element)
    {
        return !element.IsValid();
    }
};

// Similar to TagVectorProvider but for maps.
template<class TParentType, class TContainerType>
class TagMapProvider
{
    TagMapProvider& operator=(const TagMapProvider&);
public:
    typedef TParentType ParentType;
    typedef TContainerType ContainerType;
    typedef typename TContainerType::key_type IDType;
    typedef typename TContainerType::mapped_type contained_type;

    TagMapProvider(ParentType* parent, ContainerType& container, uint32_t /*persistenceFlags*/)
        : m_parent(parent), m_container(container), m_currentIterator(m_container.end())
    {}

    contained_type* GetFirstElement()
    {
        m_currentIterator = m_container.begin();
        return GetCurrentElement();
    }

    contained_type* GetNextElement()
    {
        ++m_currentIterator;
        return GetCurrentElement();
    }

    contained_type* GetElementByID(IDType id)
    {
        return &m_container[id];
    }

    IDType GetCurrentElementID()
    {
        return m_currentIterator->first;
    }

protected:
    contained_type* GetCurrentElement()
    {
        if (m_currentIterator != m_container.end())
        {
            return &(m_currentIterator->second);
        }

        return NULL;
    }

    ParentType* m_parent;
    ContainerType& m_container;

    typename TContainerType::iterator m_currentIterator;
};

// Extended provider class, same as TagMapProvider but adds an empty entry
// at the end and filters empty entries on Add.
template<class TParentType, class TContainerType>
class TagEmptyEntryMapProvider : public TagMapProvider<TParentType, TContainerType>
{
public:
    typedef TParentType ParentType;
    typedef TContainerType ContainerType;

    typedef TagMapProvider<TParentType, TContainerType> super;
    typedef typename super::contained_type contained_type;
    typedef typename TContainerType::key_type IDType;

    TagEmptyEntryMapProvider(ParentType* parent, ContainerType& container, uint32_t persistenceFlags)
        : super(parent, container, persistenceFlags), m_emptyElement(), m_emptyId()
    {}

    contained_type* GetFirstElement()
    {
        super::m_currentIterator = super::m_container.begin();
        return GetCurrentElement();
    }

    contained_type* GetNextElement()
    {
        if (super::m_currentIterator != super::m_container.end())
        {
            ++super::m_currentIterator;
            return GetCurrentElement();
        }

        return NULL;
    }

    IDType GetCurrentElementID()
    {
        if (super::m_currentIterator != super::m_container.end())
        {
            return super::m_currentIterator->first;
        }

        return m_emptyId;
    }

    contained_type* GetElementByID(IDType id)
    {
        if (!CEmptyIdPredicate<ParentType, ContainerType, IDType>::IsEmpty(id))
        {
            return &super::m_container[id];
        }

        return NULL;
    }

protected:
    contained_type* GetCurrentElement()
    {
        if (super::m_currentIterator != super::m_container.end())
        {
            return &(super::m_currentIterator->second);
        }

        return &m_emptyElement;
    }

    IDType m_emptyId;
    contained_type m_emptyElement;
};

// This handler serializes an ID to match the elements.
// For this handler, TContainerProvider must implement:
// - typenames ParentType, ContainerType, IDType, contained_type
// - functions GetFirstElement, GetNextElement, GetElementByID, GetCurrentElementID
template <class TContainerProvider, template <class T> class TContainedTypeHandler>
class ByIDContainerHandler
{
public:
    typedef typename TContainerProvider::ParentType ParentType;
    typedef typename TContainerProvider::ContainerType ContainerType;
    typedef typename TContainerProvider::contained_type contained_type;
    typedef typename TContainerProvider::IDType IDType;
    typedef TContainedTypeHandler<contained_type> ContainedTypeHandler;
    typedef TContainerProvider Provider;

    ByIDContainerHandler(const char* elementName, const char* valueName, const char* idName)
        : m_elementID(elementName), m_valueID(valueName), m_idID(idName)
    {}

    void Load(const behaviac::ISerializableNode* node, ParentType* parent, ContainerType& container, const char* className)
    {
        Provider provider(parent, container, EPersistenceType_Description_Load);
		behaviac::ConstSerializableNodeIt iter(node);

        for (const behaviac::ISerializableNode* childNode = iter.first(m_elementID); childNode; childNode = iter.next(m_elementID))
        {
            IDType childID;

            if (GenericTypeHandler<IDType>::Load(childNode, childID, className, m_idID))
            {
                contained_type* child = provider.GetElementByID(childID);

                if (child)
                {
                    ContainedTypeHandler::Load(childNode, *child, className, m_valueID);
                }
            }
        }
    }

    void Save(behaviac::ISerializableNode* node, ParentType* parent, ContainerType& container, const char* className)
    {
        Provider provider(parent, container, EPersistenceType_Description_Save);
        contained_type* element = provider.GetFirstElement();

        while (element)
        {
            behaviac::ISerializableNode* childNode = node->newChild(m_elementID);
            ContainedTypeHandler::Save(childNode, *element, className, m_valueID);
            GenericTypeHandler<IDType>::Save(childNode, provider.GetCurrentElementID(), className, m_idID);
            element = provider.GetNextElement();
        }
    }

    void LoadState(const behaviac::ISerializableNode* node, ParentType* parent, ContainerType& container, const char* className)
    {
        Provider provider(parent, container, EPersistenceType_State_Load);
		behaviac::ConstSerializableNodeIt iter(node);

        for (const behaviac::ISerializableNode* childNode = iter.first(m_elementID); childNode; childNode = iter.next(m_elementID))
        {
            IDType childID;

            if (GenericTypeHandler<IDType>::Load(childNode, childID, className, m_idID))
            {
                contained_type* child = provider.GetElementByID(childID);

                if (child)
                {
                    ContainedTypeHandler::LoadState(childNode, *child, className, m_valueID);
                }
            }
        }
    }

    void SaveState(behaviac::ISerializableNode* node, ParentType* parent, ContainerType& container, const char* className)
    {
        Provider provider(parent, container, EPersistenceType_State_Save);
        contained_type* element = provider.GetFirstElement();

        while (element)
        {
            behaviac::ISerializableNode* childNode = node->newChild(m_elementID);
            ContainedTypeHandler::SaveState(childNode, *element, className, m_valueID);
            GenericTypeHandler<IDType>::Save(childNode, provider.GetCurrentElementID(), className, m_idID);
            element = provider.GetNextElement();
        }
    }

    void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, ParentType* parent, ContainerType& container, bool bStatic, int readonlyFlag, const char* classFullName, const behaviac::CSerializationID& propertyID, const behaviac::wstring& displayName, const behaviac::wstring& desc, UiGenericType* uiWrapper)
    {
        Provider provider(parent, container, EPersistenceType_UiInfo);
        contained_type* element = provider.GetFirstElement();

		behaviac::XmlNodeRef childNode = xmlNode;

        while (element)
        {
            if (types == NULL)
            {
                childNode = xmlNode->newChild("Member");
                childNode->setAttr("Name", m_elementID.GetString());
                childNode->setAttr("ContainerElement", true);

                if (classFullName)
                {
                    childNode->setAttr("Class", classFullName);
                }

                if (bStatic)
                {
                    childNode->setAttr("Static", "true");
                }

                if (readonlyFlag & 0x1)
                {
                    childNode->setAttr("Readonly", "true");
                }

                if (readonlyFlag & 0x2)
                {
                    childNode->setAttr("Property", "true");
                }
            }

            ContainedTypeHandler::GetUiInfo(types, childNode, *element, bStatic, readonlyFlag, classFullName, m_valueID, displayName, desc, NULL);
            GenericTypeHandler<IDType>::GetUiInfo(types, childNode, provider.GetCurrentElementID(), bStatic, readonlyFlag, classFullName, m_idID, displayName, desc, NULL);

            if (uiWrapper)
            {
                uiWrapper->SaveDescription(childNode);
            }

            element = provider.GetNextElement();
        }
    }

    void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, ParentType* parent, ContainerType& container, const char* className)
    {
        BEHAVIAC_UNUSED_VAR(types);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(container);
        BEHAVIAC_UNUSED_VAR(className);

        BEHAVIAC_LOGERROR("Not implemented");
    }

protected:
    behaviac::CSerializationID m_elementID;
    behaviac::CSerializationID m_valueID;
    behaviac::CSerializationID m_idID;
};

// For this handler, TContainerProvider must implement:
// - typenames ParentType, ContainerType, contained_type
// - functions GetFirstElement, GetNextElement, GetElementByID, GetCurrentElementID
// - IDType MUST be behaviac::CSerializationID
template <class TContainerProvider, template <class T> class TContainedTypeHandler>
class UseTagAsIDContainerHandler
{
public:
    typedef typename TContainerProvider::ParentType ParentType;
    typedef typename TContainerProvider::ContainerType ContainerType;
    typedef typename TContainerProvider::contained_type contained_type;
    typedef TContainedTypeHandler<contained_type> ContainedTypeHandler;
    typedef TContainerProvider Provider;

    UseTagAsIDContainerHandler(const char* /*elementName*/, const char* valueName, const char* /*idName*/)
        : m_valueID(valueName)
    {}

    void Load(const behaviac::ISerializableNode* node, ParentType* parent, ContainerType& container, const char* className)
    {
        Provider provider(parent, container, EPersistenceType_Description_Load);
		behaviac::ConstSerializableNodeIt iter(node);

        for (const behaviac::ISerializableNode* childNode = iter.first(); childNode; childNode = iter.next())
        {
            behaviac::CSerializationID childID = childNode->getTag();
            contained_type* child = provider.GetElementByID(childID);

            if (child)
            {
                ContainedTypeHandler::Load(childNode, *child, m_valueID);
            }
        }
    }

    void Save(behaviac::ISerializableNode* node, ParentType* parent, ContainerType& container, const char* className)
    {
        Provider provider(parent, container, EPersistenceType_Description_Save);
        contained_type* element = provider.GetFirstElement();

        while (element)
        {
            behaviac::ISerializableNode* childNode = node->newChild(provider.GetCurrentElementID());
            ContainedTypeHandler::Save(childNode, *element, m_valueID);
            element = provider.GetNextElement();
        }
    }

    void LoadState(const behaviac::ISerializableNode* node, ParentType* parent, ContainerType& container, const char* className)
    {
        Provider provider(parent, container, EPersistenceType_State_Load);
		behaviac::ConstSerializableNodeIt iter(node);

        for (const behaviac::ISerializableNode* childNode = iter.first(); childNode; childNode = iter.next())
        {
            behaviac::CSerializationID childID = childNode->getTag();
            contained_type* child = provider.GetElementByID(childID);

            if (child)
            {
                ContainedTypeHandler::LoadState(childNode, *child, m_valueID);
            }
        }
    }

    void SaveState(behaviac::ISerializableNode* node, ParentType* parent, ContainerType& container, const char* className)
    {
        Provider provider(parent, container, EPersistenceType_State_Save);
        contained_type* element = provider.GetFirstElement();

        while (element)
        {
            behaviac::ISerializableNode* childNode = node->newChild(provider.GetCurrentElementID());
            ContainedTypeHandler::SaveState(childNode, *element, m_valueID);
            element = provider.GetNextElement();
        }
    }

    void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, ParentType* parent, ContainerType& container, const char* className)
    {
        Provider provider(parent, container, EPersistenceType_UiInfo);
        contained_type* element = provider.GetFirstElement();

        while (element)
        {
            ContainedTypeHandler::GetUiInfo(types, xmlNode, *element, provider.GetCurrentElementID(), NULL);
            element = provider.GetNextElement();
        }
    }

    void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, ParentType* parent, ContainerType& container, const char* className)
    {
        BEHAVIAC_UNUSED_VAR(types);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(container);
        BEHAVIAC_UNUSED_VAR(className);

        BEHAVIAC_LOGERROR("Not implemented");
    }

protected:
    behaviac::CSerializationID m_valueID;
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_CONTAINERTYPEHANDLEREX_H
