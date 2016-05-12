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

#ifndef BEHAVIAC_ENGINESERVICES_TYPEHANDLEREX_H
#define BEHAVIAC_ENGINESERVICES_TYPEHANDLEREX_H

// please use the derivation after this class to not have number of parameters errors
template<class MemberType, bool clearOnLoad>
struct BasicVectorHandlerWithClearOption
{
    template <bool State>
    static bool LoadHelper(const behaviac::ISerializableNode* node, MemberType& member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        const behaviac::ISerializableNode* childNode = node->findChild(propertyID);

        if (childNode)
        {
            uint32_t childCount = childNode->getChildCount();

            if (clearOnLoad)
            {
                member.resize(childCount);
            }

            for (uint32_t i = 0; i < childCount; i++)
            {
                const behaviac::ISerializableNode* elementNode = childNode->getChild(i);

                if (!State)
                {
                    CHandlerGuesser<GenericTypeHandler>::Load(elementNode, member[i], behaviac::CSerializationID(0xDCB67730, "Value"));

                }
                else
                {
                    CHandlerGuesser<GenericTypeHandler>::LoadState(elementNode, member[i], behaviac::CSerializationID(0xDCB67730, "Value"));
                }
            }

            return true;
        }

        return false;
    }

    template <bool State>
    static void SaveHelper(behaviac::ISerializableNode* node, const MemberType& member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        behaviac::ISerializableNode* childNode = node->newChild(propertyID);

        for (uint32_t i = 0; i < member.size(); i++)
        {
            behaviac::ISerializableNode* elementNode = childNode->newChild(behaviac::CSerializationID(0xE7CAC750, "Elem"));

            if (!State)
            {
                CHandlerGuesser<GenericTypeHandler>::Save(elementNode, member[i], behaviac::CSerializationID(0xDCB67730, "Value"));

            }
            else
            {
                CHandlerGuesser<GenericTypeHandler>::SaveState(elementNode, member[i], behaviac::CSerializationID(0xDCB67730, "Value"));
            }
        }
    }

    static bool Load(const behaviac::ISerializableNode* node, MemberType& member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        return LoadHelper< false >(node, member, className, propertyID);
    }

    static void Save(behaviac::ISerializableNode* node, const MemberType& member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        SaveHelper< false >(node, member, className, propertyID);
    }

    static bool LoadState(const behaviac::ISerializableNode* node, MemberType& member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        return LoadHelper< true >(node, member, className, propertyID);
    }

    static void SaveState(behaviac::ISerializableNode* node, const MemberType& member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        SaveHelper< true >(node, member, className, propertyID);
    }

    static void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, const MemberType& member, bool bStatic, int readonlyFlag, const char* classFullName, const behaviac::CSerializationID& propertyID, const behaviac::wstring& displayName, const behaviac::wstring& desc, UiGenericType* uiWrapper)
    {
        if (types == NULL)
        {
            behaviac::XmlNodeRef memberNode = xmlNode->newChild("Member");
            memberNode->setAttr("Name", propertyID.GetString());
            memberNode->setAttr("Type", "enum");

            if (classFullName)
            {
                memberNode->setAttr("Class", classFullName);
            }

            if (bStatic)
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

            if (uiWrapper)
            {
                uiWrapper->SaveDescription(memberNode);
            }
        }
    }

    static void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, const MemberType& member, const char* className, const char* propertyName)
    {
        BEHAVIAC_UNUSED_VAR(types);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(member);
        BEHAVIAC_UNUSED_VAR(className);
        BEHAVIAC_UNUSED_VAR(propertyName);
    }
};

template<class MemberType>
struct BasicVectorHandler : public BasicVectorHandlerWithClearOption<MemberType, true>
{
};

template<class MemberType>
struct BasicVectorHandlerNoClear : public BasicVectorHandlerWithClearOption<MemberType, false>
{
};

template<class MemberType>
struct BasicMapHandler
{
    static bool Load(const behaviac::ISerializableNode* node, MemberType& member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        const behaviac::ISerializableNode* childNode = node->findChild(propertyID);

        if (childNode)
        {
            uint32_t childCount = childNode->getChildCount();

            for (uint32_t i = 0; i < childCount; i++)
            {
                const behaviac::ISerializableNode* elementNode = childNode->getChild(i);
                typename MemberType::key_type key;
                typename MemberType::MappedType value;
                elementNode->getAttr(behaviac::CSerializationID(0xB2DDED49, "Key"), key);
                elementNode->getAttr(behaviac::CSerializationID(0xDCB67730, "Value"), value);
                member.insert(typename MemberType::value_type(key, value));
            }

            return true;
        }

        return false;
    }

    static void Save(behaviac::ISerializableNode* node, const MemberType& member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        behaviac::ISerializableNode* childNode = node->newChild(propertyID);
        typename MemberType::const_iterator it = member.begin();
        typename MemberType::const_iterator itEnd = member.end();

        for (; it != itEnd; ++it)
        {
            behaviac::ISerializableNode* elementNode = childNode->newChild(behaviac::CSerializationID(0xE7CAC750, "Elem"));
            elementNode->setAttr(behaviac::CSerializationID(0xB2DDED49, "Key"), (*it).first);
            elementNode->setAttr(behaviac::CSerializationID(0xDCB67730, "Value"), (*it).second);
        }
    }

    static bool LoadState(const behaviac::ISerializableNode* node, MemberType& member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        return Load(node, member, className, propertyID);
    }

    static void SaveState(behaviac::ISerializableNode* node, const MemberType& member, const char* className, const behaviac::CSerializationID& propertyID)
    {
        Save(node, member, className, propertyID);
    }

    static void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, const MemberType& member, bool bStatic, int readonlyFlag, const char* classFullName, const behaviac::CSerializationID& propertyID, const behaviac::wstring& displayName, const behaviac::wstring& desc, UiGenericType* uiWrapper)
    {
        if (types == NULL)
        {
            behaviac::XmlNodeRef memberNode = xmlNode->newChild("Member");
            memberNode->setAttr("Name", propertyID.GetString());
            memberNode->setAttr("Type", "behaviac::map");

            if (classFullName)
            {
                memberNode->setAttr("Class", classFullName);
            }

            if (bStatic)
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

            if (uiWrapper)
            {
                uiWrapper->SaveDescription(memberNode);
            }
        }
    }

    static void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const behaviac::XmlNodeRef& xmlNode, const MemberType& member, const char* className, const char* propertyName)
    {
        BEHAVIAC_UNUSED_VAR(types);
        BEHAVIAC_UNUSED_VAR(xmlNode);
        BEHAVIAC_UNUSED_VAR(member);
        BEHAVIAC_UNUSED_VAR(className);
        BEHAVIAC_UNUSED_VAR(propertyName);
    }
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_TYPEHANDLEREX_H
