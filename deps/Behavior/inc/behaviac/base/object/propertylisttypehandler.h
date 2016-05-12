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

#ifndef BEHAVIAC_ENGINESERVICES_PROPERTYLISTTYPEHANDLER_H
#define BEHAVIAC_ENGINESERVICES_PROPERTYLISTTYPEHANDLER_H

#include "behaviac/base/object/typehandler.h"
#include "behaviac/base/object/member.h"
#include "engineservices/property/propertylist.h"

/////////////////////////////////////////////////////////
//////////     PROPERTY LIST TYPE HANDLER      ///////////
/////////////////////////////////////////////////////////

template< typename ParentType>
class PropertyListTypeHandler
{
public:

    PropertyListTypeHandler()
    {}

    void Load(const ISerializableNode* node, ParentType* parent, CPropertyList& propertyList)
    {
        //container.clear(); // Clear elements
        //Provider provider(parent, container, EPersistenceType_Description_Load);
        //ConstSerializableNodeIt iter(node);
        //if (CContainerReserver<ContainerType>::CanReserve())
        //{
        //    // Cheap way to count children...
        //    uint32_t numChildren = 0;
        //    for (const ISerializableNode* childNode = iter.first(m_elementID); childNode; childNode = iter.next(m_elementID))
        //    {
        //        ++numChildren;
        //    }
        //    CContainerReserver<ContainerType>::Reserve(container, numChildren);
        //}
        //for (const ISerializableNode* childNode = iter.first(m_elementID); childNode; childNode = iter.next(m_elementID))
        //{
        //    contained_type tempObject;
        //    ContainedTypeHandler::Load(childNode, tempObject, m_valueID);
        //    provider.AddElement(tempObject);
        //}
    }

    void Save(ISerializableNode* node, ParentType* parent, CPropertyList& propertyList)
    {
        //Provider provider(parent, container, EPersistenceType_Description_Save);
        //contained_type* element = provider.GetFirstElement();
        //while (element)
        //{
        //    ISerializableNode* childNode = node->newChild(m_elementID);
        //    ContainedTypeHandler::Save(childNode, *element, m_valueID);
        //    element = provider.GetNextElement();
        //}
    }

    void LoadState(const ISerializableNode* node, ParentType* parent, CPropertyList& propertyList)
    {
        //container.clear(); // Clear elements
        //Provider provider(parent, container, EPersistenceType_State_Load);
        //ConstSerializableNodeIt iter(node);
        //for (const ISerializableNode* childNode = iter.first(m_elementID); childNode; childNode = iter.next(m_elementID))
        //{
        //    contained_type tempObject;
        //    ContainedTypeHandler::LoadState(childNode, tempObject, m_valueID);
        //    provider.AddElement(tempObject);
        //}
    }

    void SaveState(ISerializableNode* node, ParentType* parent, CPropertyList& propertyList)
    {
        //Provider provider(parent, container, EPersistenceType_State_Save);
        //contained_type* element = provider.GetFirstElement();
        //while (element)
        //{
        //    ISerializableNode* childNode = node->newChild(m_elementID);
        //    ContainedTypeHandler::SaveState(childNode, *element, m_valueID);
        //    element = provider.GetNextElement();
        //}
    }

    void GetUiInfo(const behaviac::XmlNodeRef& xmlNode, const ParentType* parent, const CPropertyList& propertyList, const CSerializationID& propertyID, UiGenericType* uiWrapper)
    {
        //Provider provider(const_cast<ParentType*>(parent), const_cast<ContainerType&>(container), EPersistenceType_UiInfo);
        //contained_type* element = provider.GetFirstElement();
        //while (element)
        //{
        //          XmlNodeRef childNode = xmlNode->newChild("Member");
        //          childNode->setAttr("Name",m_elementID.GetString());
        //          childNode->setAttr("ContainerElement", true);
        //          ContainedTypeHandler::GetUiInfo(childNode, *element, m_valueID, NULL);
        //          if (uiWrapper)
        //          {
        //              uiWrapper->SaveDescription(childNode);
        //          }
        //          element = provider.GetNextElement();
        //}
    }

    void GetMethodsDescription(const behaviac::XmlNodeRef& xmlNode, const ParentType* parent, const ContainerType& container)
    {
        BEHAVIAC_LOGERROR("Not implemented");
    }

protected:
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_PROPERTYLISTTYPEHANDLER_H
