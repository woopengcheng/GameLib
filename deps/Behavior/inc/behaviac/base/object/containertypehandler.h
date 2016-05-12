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

#ifndef BEHAVIAC_ENGINESERVICES_CONTAINERTYPEHANDLER_H
#define BEHAVIAC_ENGINESERVICES_CONTAINERTYPEHANDLER_H

#include "behaviac/base/object/typehandler.h"
#include "behaviac/base/object/member.h"
#include "behaviac/base/string/pathid.h"

#define DefaultContainerProvider TagVectorProvider

// We generally want to take care of the children nodes creation
#define DefaultContainedTypeHandler NoChildTypeHandler

// Utility class to filter empty elements. The default implementation compares
// the element to 0, but it can be specialized for other types.
template <class TParentType, class TContainerType, class TContainedType>
class CEmptyPredicate
{
public:
    static bool IsEmpty(TContainedType element)
    {
        return element == 0;
    }
};

// Example partial specialization for eastl::strings, the same can be done for any
// type, ideally specializing the parent type at the same time to avoid
// conflicts with other users (especially in blob where specializations have
// indefinite scope).
template <class TParentType, class TContainerType>
class CEmptyPredicate<TParentType, TContainerType, behaviac::string>
{
public:
    static bool IsEmpty(const behaviac::string& element)
    {
        return element.empty();
    }
};

template <class TParentType, class TContainerType>
class CEmptyPredicate<TParentType, TContainerType, behaviac::CStringID>
{
public:
    static bool IsEmpty(const behaviac::CStringID& element)
    {
        return !element.IsValid();
    }
};

template <class TParentType, class TContainerType>
class CEmptyPredicate<TParentType, TContainerType, behaviac::CNoCaseStringID>
{
public:
    static bool IsEmpty(const behaviac::CStringID& element)
    {
        return !element.IsValid();
    }
};

template <class TParentType, class TContainerType>
class CEmptyPredicate<TParentType, TContainerType, behaviac::CPathID>
{
public:
    static bool IsEmpty(const behaviac::CPathID& element)
    {
        return !element.IsValid();
    }
};

// Example provider class, very basic, uses the element index as its ID. Can be
// used with any class that define ValueType and implements Size and operator[]
// (i.e. ndContainers).
template<class TParentType, class TContainerType>
class TagVectorProvider
{
public:
    typedef TParentType ParentType;
    typedef TContainerType ContainerType;
    typedef typename TContainerType::value_type contained_type;

    TagVectorProvider(ParentType* parent, ContainerType& container, uint32_t /*persistenceFlags*/)
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

    void AddElement(const contained_type& element)
    {
        m_container.push_back(element);
    }

    // These are used to simulate entries indexed by ID
    typedef uint32_t IDType;
    contained_type* GetElementByID(IDType id)
    {
        if (id < m_container.size())
        {
            return &m_container[id];
        }

        return NULL;
    }

    IDType GetCurrentElementID()
    {
        return std::distance(m_container.begin(), m_currentIterator);
    }

protected:
    contained_type* GetCurrentElement()
    {
        if (m_currentIterator != m_container.end())
        {
            return &*(m_currentIterator);
        }

        return NULL;
    }

    ParentType* m_parent;
    ContainerType& m_container;

    typename TContainerType::iterator m_currentIterator;

    TagVectorProvider& operator=(const TagVectorProvider&);
};

// Extended provider class, same as TagVectorProvider but adds an empty entry
// at the end and filters empty entries on Add.
template<class TParentType, class TContainerType>
class TagEmptyEntryVectorProvider : public TagVectorProvider<TParentType, TContainerType>
{
public:
    typedef TParentType ParentType;
    typedef TContainerType ContainerType;

    typedef TagVectorProvider<TParentType, TContainerType> super;
    typedef typename super::contained_type contained_type;

    TagEmptyEntryVectorProvider(ParentType* parent, ContainerType& container, uint32_t persistenceFlags)
        : super(parent, container, persistenceFlags), m_emptyElement()
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

    void AddElement(const contained_type& element)
    {
        if (!CEmptyPredicate<ParentType, ContainerType, contained_type>::IsEmpty(element))
        {
            super::AddElement(element);
        }
    }

protected:
    contained_type* GetCurrentElement()
    {
        if (super::m_currentIterator != super::m_container.end())
        {
            return &*(super::m_currentIterator);
        }

        return &m_emptyElement;
    }

    contained_type m_emptyElement;
};

// Specialize this to do nothing if your TContainerType does not support Reserve
template <typename TContainerType>
class CContainerReserver
{
public:
    static bool CanReserve()
    {
        return true;
    }
    static void Reserve(TContainerType& container, uint32_t size)
    {
        container.reserve(size);
    }
};

template <typename Type, typename Alloc>
class CContainerReserver< behaviac::list<Type, Alloc> >
{
public:
    static bool CanReserve()
    {
        return false;
    }
    static void Reserve(behaviac::list<Type, Alloc>&, uint32_t) {} // do nothing
};

/////////////////////////////////////////////////////////
//////////     GENERIC CONTAINER HANDLER      ///////////
/////////////////////////////////////////////////////////

// This handler's loading requests a new object from the provider to serialize
// it. Saving is performed by iterating through the provider's objects.
// For this handler, TContainerProvider must implement:
// - typenames ParentType, ContainerType, contained_type
// - functions GetFirstElement, GetNextElement, AddElement
template <class TContainerProvider, template <class T> class TContainedTypeHandler>
class GenericContainerHandler
{
public:
    typedef typename TContainerProvider::ParentType ParentType;
    typedef typename TContainerProvider::ContainerType ContainerType;
    typedef typename TContainerProvider::contained_type contained_type;
    typedef TContainedTypeHandler<contained_type> ContainedTypeHandler;
    typedef TContainerProvider Provider;

    GenericContainerHandler(const char* elementName, const char* valueName, const char* /*idName*/)
        : m_elementID(elementName), m_valueID(valueName)
    {}

    void Load(const behaviac::ISerializableNode* node, ParentType* parent, ContainerType& container, const char* className)
    {
        container.clear(); // Clear elements
        Provider provider(parent, container, EPersistenceType_Description_Load);
        behaviac::ConstSerializableNodeIt iter(node);

        if (CContainerReserver<ContainerType>::CanReserve())
        {
            // Cheap way to count children...
            uint32_t numChildren = 0;

            for (const behaviac::ISerializableNode* childNode = iter.first(m_elementID); childNode; childNode = iter.next(m_elementID))
            {
                ++numChildren;
            }

            CContainerReserver<ContainerType>::Reserve(container, numChildren);
        }

        for (const behaviac::ISerializableNode* childNode = iter.first(m_elementID); childNode; childNode = iter.next(m_elementID))
        {
            contained_type tempObject;
            ContainedTypeHandler::Load(childNode, tempObject, className, m_valueID);
            provider.AddElement(tempObject);
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
            element = provider.GetNextElement();
        }
    }

    void LoadState(const behaviac::ISerializableNode* node, ParentType* parent, ContainerType& container, const char* className)
    {
        container.clear(); // Clear elements
        Provider provider(parent, container, EPersistenceType_State_Load);
		behaviac::ConstSerializableNodeIt iter(node);

        for (const behaviac::ISerializableNode* childNode = iter.first(m_elementID); childNode; childNode = iter.next(m_elementID))
        {
            contained_type tempObject;
            ContainedTypeHandler::LoadState(childNode, tempObject, className, m_valueID);
            provider.AddElement(tempObject);
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
            element = provider.GetNextElement();
        }
    }

	void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, const ParentType* parent, const ContainerType& container, bool bStatic, int readonlyFlag, const char* classFullName, const behaviac::CSerializationID& propertyID, const behaviac::wstring& displayName, const behaviac::wstring& desc, UiGenericType* uiWrapper)
    {
        BEHAVIAC_UNUSED_VAR(propertyID);
        Provider provider(const_cast<ParentType*>(parent), const_cast<ContainerType&>(container), EPersistenceType_Description_UiInfo);
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

            if (uiWrapper)
            {
                uiWrapper->SaveDescription(childNode);
            }

            element = provider.GetNextElement();
        }
    }

    void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, const ParentType* parent, const ContainerType& container, const char* className)
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
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_CONTAINERTYPEHANDLER_H
