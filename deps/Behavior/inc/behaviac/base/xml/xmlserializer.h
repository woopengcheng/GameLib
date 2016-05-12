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

#ifndef BEHAVIAC_CORE_XMLSERIALIZER_H
#define BEHAVIAC_CORE_XMLSERIALIZER_H

#include "behaviac/base/xml/ixml.h"

template <class TXmlSerializer, typename T> void XmlSerializeAttr(const typename TXmlSerializer::NodeRef& xmlNode, const char* attr, T& value)
{
    TXmlSerializer serializer;
    serializer.SerializeAttr(xmlNode, attr, value);
}

template <class TXmlSerializer, typename T> void XmlSerializeEnum(const typename TXmlSerializer::NodeRef& xmlNode, const char* attr, T& value)
{
    TXmlSerializer serializer;
    serializer.SerializeEnum(xmlNode, attr, value);
}

#define XMLSERIALIZERBEHAVIAC_ARRAYSIZE "hidArraySize"
template <class TXmlSerializer, typename T> void XmlSerializeArray(const typename TXmlSerializer::NodeRef& xmlNode, const char* attr, T& array)
{
    TXmlSerializer serializer;
    serializer.SerializeArray(xmlNode, attr, array);
}

class XmlSerializerLoad
{
public:
    typedef XmlConstNodeRef NodeRef;
    static bool IsLoading()
    {
        return true;
    }
    template<typename T> void SerializeAttr(const NodeRef& xmlNode, const char* attr, T& value)
    {
        xmlNode->getAttr(attr, value);
    }
    template<typename T> void SerializeEnum(const NodeRef& xmlNode, const char* attr, T& value)
    {
        int enumValue;

        if (xmlNode->getAttr(attr, enumValue))
        {
            value = (T)enumValue;
        }
    }
    template <typename T> void SerializeArray(const NodeRef& xmlNode, const char* attr, behaviac::vector<T>& array)
    {
        int size = 0;
        xmlNode->getAttr(XMLSERIALIZERBEHAVIAC_ARRAYSIZE, size);
        array.resize(size);

        for (int i = 0; i < size; i++)
        {
            char tag[64];
            string_sprintf(tag, attr, i);
            T value;
            xmlNode->getAttr(tag, value);
            array[i] = value;
        }
    }
    static NodeRef NewChild(const NodeRef& xmlNode, const char*)
    {
        BEHAVIAC_ASSERT(0);
        return NULL;
    }
    typedef char TEnumValue[32];
    static void ListEnum(const NodeRef& xmlNode, const char* name, const TEnumValue* values, unsigned int nbValues)
    {
        // Nothing to do
    }
};

class XmlSerializerSave
{
public:
    typedef XmlNodeRef NodeRef;
    static bool IsLoading()
    {
        return false;
    }
    template<typename T> void SerializeAttr(const NodeRef& xmlNode, const char* attr, const T& value)
    {
        xmlNode->setAttr(attr, value);
    }
    template<typename T> void SerializeEnum(const NodeRef& xmlNode, const char* attr, const T& value)
    {
        xmlNode->setAttr(attr, (int)value);
    }
    template <typename T> void SerializeArray(const NodeRef& xmlNode, const char* attr, const behaviac::vector<T>& array)
    {
        int size = (int)array.size();
        xmlNode->setAttr(XMLSERIALIZERBEHAVIAC_ARRAYSIZE, size);

        for (int i = 0; i < size; i++)
        {
            char tag[64];
            string_sprintf(tag, attr, i);
            xmlNode->setAttr(tag, array[i]);
        }
    }
    static NodeRef NewChild(const NodeRef& xmlNode, const char* tag)
    {
        return xmlNode->newChild(tag);
    }
    typedef char TEnumValue[32];
    static void ListEnum(const NodeRef& xmlNode, const char* name, const TEnumValue* values, unsigned int nbValues)
    {
        NodeRef xmlEnum = xmlNode->newChild(name);

        for (unsigned int value = 0; value < nbValues; value++)
        {
            NodeRef xmlValue = xmlEnum->newChild("enum");
            xmlValue->setAttr("Value", values[value]);
        }
    }
};

class CResourceContainer;

#define XML_DECLARE_SERIALIZE_TEMPLATE() template <class TXmlSerializer> void XmlSerialize( const typename TXmlSerializer::NodeRef& xmlNode, CResourceContainer* resContainer = NULL )
#define XML_IMPLEMENT_SERIALIZE_TEMPLATE(ClassName) template <class TXmlSerializer> void ClassName::XmlSerialize( const typename TXmlSerializer::NodeRef& xmlNode, CResourceContainer* resContainer )

#define XML_DECLARE_VIRTUAL_SINGLE_SERIALIZE( serializer ) virtual void XmlSerialize( serializer&, const serializer::NodeRef& xmlNode, CResourceContainer* resContainer = NULL ) { XmlSerialize<serializer>(xmlNode,resContainer); }
#define XML_DECLARE_VIRTUAL_SERIALIZE() \
    XML_DECLARE_VIRTUAL_SINGLE_SERIALIZE( XmlSerializerLoad ); \
    XML_DECLARE_VIRTUAL_SINGLE_SERIALIZE( XmlSerializerSave );

#endif // #ifndef BEHAVIAC_CORE_XMLSERIALIZER_H
