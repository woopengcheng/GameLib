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

#ifndef BEHAVIAC_EXT_TYPEHANDLEREXT_H
#define BEHAVIAC_EXT_TYPEHANDLEREXT_H

#include "range.h"

template <typename T>
struct GenericTypeHandler< TRange<T> >
{
    static bool Load(const ISerializableNode* node, TRange<T>& member, const char* className, const CSerializationID& propertyID)
    {
        return node->getAttr(CSerializationID(0x5EB87A8B, "Start"), member.start) && node->getAttr(CSerializationID(0x38B17551, "End"), member.end);
    }

    static void Save(ISerializableNode* node, const TRange<T>& member, const char* className, const CSerializationID& propertyID)
    {
        node->setAttr(CSerializationID(0x5EB87A8B, "Start"), member.start);
        node->setAttr(CSerializationID(0x38B17551, "End"), member.end);
    }

    static bool LoadState(const ISerializableNode* node, TRange<T>& member, const char* className, const CSerializationID& propertyID)
    {
        return Load(node, member, className, propertyID);
    }

    static void SaveState(ISerializableNode* node, const TRange<T>& member, const char* className, const CSerializationID& propertyID)
    {
        Save(node, member, className, propertyID);
    }

    static void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, const T& member, bool bStatic, const char* classFullName, const CSerializationID& propertyID, const behaviac::wstring& displayName, const behaviac::wstring& desc, UiGenericType* uiWrapper)
    {
        if (types == NULL)
        {
            XmlNodeRef memberNode = xmlNode->newChild("Member");
            memberNode->setAttr("Name", propertyID.GetString());
            memberNode->setAttr("DisplayName", displayName);
            memberNode->setAttr("Desc", desc);
            memberNode->setAttr("Type", "TRange");

            if (classFullName)
            {
                memberNode->setAttr("Class", classFullName);
            }

            if (bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (uiWrapper)
            {
                uiWrapper->SaveDescription(memberNode);
            }
        }
    }

    static void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, const TRange<T>& member, const char* propertyName)
    {
        BEHAVIAC_UNUSED_VAR(types);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(member);
        BEHAVIAC_UNUSED_VAR(propertyName);
    }
};

#endif//BEHAVIAC_EXT_TYPEHANDLEREXT_H
