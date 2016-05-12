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

#ifndef BEHAVIAC_CORE_TEXTNODE_H
#define BEHAVIAC_CORE_TEXTNODE_H

#include "behaviac/base/serialization/serializablenode.h"
#include "behaviac/base/xml/ixml.h"

#include <list>

namespace behaviac
{
	class BEHAVIAC_API CTextNode : public ISerializableNode
	{
	public:
		CTextNode()
			: ISerializableNode(true),
			m_xmlNode(),
			m_constXmlNode()
		{}
		CTextNode(const behaviac::XmlNodeRef& xmlNode)
			: ISerializableNode(true),
			m_xmlNode(xmlNode),
			m_constXmlNode(xmlNode)
		{
			RebuildChildrenList();
		}
		CTextNode(const XmlConstNodeRef& xmlNode)
			: ISerializableNode(true),
			m_xmlNode(),
			m_constXmlNode(xmlNode)
		{
			RebuildChildrenList();
		}
		CTextNode(const CSerializationID& tag)
			: ISerializableNode(true)
		{
			m_xmlNode = CreateXmlNode(tag.GetString());
			m_constXmlNode = m_xmlNode;
			RebuildChildrenList();
		}
		CTextNode(const CTextNode& other)
			: ISerializableNode(true),
			m_xmlNode(other.m_xmlNode),
			m_constXmlNode(other.m_constXmlNode)
		{
			RebuildChildrenList();
		}

		CTextNode& operator=(IXmlNode* xmlNode)
		{
			m_xmlNode = xmlNode;
			m_constXmlNode = xmlNode;
			RebuildChildrenList();
			return *this;
		}
		CTextNode& operator=(const behaviac::XmlNodeRef& xmlNode)
		{
			m_xmlNode = xmlNode;
			m_constXmlNode = xmlNode;
			RebuildChildrenList();
			return *this;
		}
		CTextNode& operator=(const XmlConstNodeRef& xmlNode)
		{
			m_xmlNode = NULL;
			m_constXmlNode = xmlNode;
			RebuildChildrenList();
			return *this;
		}
		CTextNode& operator=(const CTextNode& other)
		{
			m_xmlNode = other.m_xmlNode;
			m_constXmlNode = other.m_constXmlNode;
			RebuildChildrenList();
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		// ISerializableNode interface
		virtual SerializableNodeRef clone() const;

		virtual int32_t getChildCount() const;
		virtual ISerializableNode* getChild(int32_t childIndex);
		virtual const ISerializableNode* getChild(int32_t childIndex) const;

		virtual ISerializableNode* findChild(const CSerializationID& childID);
		virtual const ISerializableNode* findChild(const CSerializationID& childID) const;

		virtual CTextNode* newChild(const CSerializationID& childID);

		virtual void removeChild(ISerializableNode* child);

		virtual bool isTag(const CSerializationID& tagID) const
		{
			return CSerializationID(m_constXmlNode->getTag()) == tagID;
		}
		virtual CSerializationID getTag() const
		{
			return CSerializationID(m_constXmlNode->getTag());
		}

		virtual int32_t getAttributesCount() const;

		virtual const char* getAttrRaw(const CSerializationID& keyID, int typeId = 0, int length = 0) const
		{
			BEHAVIAC_UNUSED_VAR(typeId);
			BEHAVIAC_UNUSED_VAR(length);

			return m_constXmlNode->getAttr(keyID.GetString());
		}

		virtual void setAttrRaw(const CSerializationID& keyID, const char* valueStr, int typeId = 0, int length = 0)
		{
			BEHAVIAC_UNUSED_VAR(typeId);
			BEHAVIAC_UNUSED_VAR(length);

			m_xmlNode->setAttrText(keyID.GetString(), valueStr);
		}

		virtual void addChild(const CSerializationID& keyID, const ISerializableNode* child);

		void addChild(XmlNodeRef xmlChild);

		virtual bool SaveToFile(const char* fileName) const;
		virtual bool LoadFromFile(const char* fileName);

		virtual bool SaveToFile(IFile* file) const;
		virtual bool LoadFromFile(IFile* file);

		virtual int32_t GetMemUsage() const;
	private:

		void RebuildChildrenList();

		typedef behaviac::list<CTextNode> ChildrenContainer;
		ChildrenContainer m_children;

		// If we're given a XmlConstNodeRef for init, only m_constXmlNode will be
		// valid; if we're given a XmlNodeRef for init, both will be valid.
		XmlNodeRef m_xmlNode;
		XmlConstNodeRef m_constXmlNode;
	};
}//namespace behaviac
#endif //BEHAVIAC_CORE_TEXTNODE_H
