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

#include "behaviac/base/serialization/textnode.h"
#include "behaviac/base/xml/xmlparser.h"
#include "behaviac/base/xml/xml.h"

#include "behaviac/base/file/file.h"

namespace behaviac
{
	SerializableNodeRef CTextNode::clone() const
	{
		return GetNodeRef(BEHAVIAC_NEW CTextNode(m_constXmlNode->clone()));
	}

	int32_t CTextNode::getChildCount() const
	{
		return (int32_t)m_children.size();
	}

	ISerializableNode* CTextNode::getChild(int32_t childIndex)
	{
		BEHAVIAC_ASSERT(childIndex < getChildCount());
		ChildrenContainer::iterator iter = m_children.begin();
		std::advance(iter, childIndex);
		return &*iter;
	}

	const ISerializableNode* CTextNode::getChild(int32_t childIndex) const
	{
		BEHAVIAC_ASSERT(childIndex < getChildCount());
		ChildrenContainer::const_iterator iter = m_children.begin();
		std::advance(iter, childIndex);
		return &*iter;
	}

	ISerializableNode* CTextNode::findChild(const CSerializationID& childID)
	{
		ChildrenContainer::iterator iter, end = m_children.end();

		for (iter = m_children.begin(); iter != end; ++iter)
		{
			ISerializableNode* currentChild = &*iter;

			if (currentChild->isTag(childID))
			{
				return currentChild;
			}
		}

		return NULL;
	}

	const ISerializableNode* CTextNode::findChild(const CSerializationID& childID) const
	{
		ChildrenContainer::const_iterator iter, end = m_children.end();

		for (iter = m_children.begin(); iter != end; ++iter)
		{
			const ISerializableNode* currentChild = &*iter;

			if (currentChild->isTag(childID))
			{
				return currentChild;
			}
		}

		return NULL;
	}

	CTextNode* CTextNode::newChild(const CSerializationID& childID)
	{
		XmlNodeRef newXmlChild = m_xmlNode->newChild(childID.GetString());
		m_children.push_back(CTextNode(newXmlChild));
		return &m_children.back();
	}

	void CTextNode::removeChild(ISerializableNode* child)
	{
		ChildrenContainer::iterator iter, end = m_children.end();

		for (iter = m_children.begin(); iter != end; ++iter)
		{
			if (&*iter == child)
			{
				m_xmlNode->removeChild(iter->m_xmlNode);
				m_children.erase(iter);
				return;
			}
		}
	}

	int32_t CTextNode::getAttributesCount() const
	{
		return m_constXmlNode->getAttrCount();
	}

	void CTextNode::RebuildChildrenList()
	{
		// If possible init the children with XmlNodeRef's
		m_children.clear();

		if (m_xmlNode)
		{
			XmlNodeIt childrenIter(m_xmlNode);

			for (XmlNodeRef child = childrenIter.first(); child; child = childrenIter.next())
			{
				m_children.push_back(CTextNode());
				m_children.back() = child;
			}
		}
		else
		{
			XmlConstNodeIt childrenIter(m_constXmlNode);

			for (XmlConstNodeRef child = childrenIter.first(); child; child = childrenIter.next())
			{
				m_children.push_back(CTextNode());
				m_children.back() = child;
			}
		}
	}

	void CTextNode::addChild(const CSerializationID& keyID, const ISerializableNode* child)
	{
		const CTextNode* textChild = static_cast<const CTextNode*>(child);
		XmlNodeRef cloneNode = textChild->m_constXmlNode->clone();
		cloneNode->setTag(keyID.GetString());
		m_xmlNode->addChild(cloneNode);
		RebuildChildrenList();
	}

	void CTextNode::addChild(XmlNodeRef xmlChild)
	{
		m_xmlNode->addChild(xmlChild);
		// This is a lot more effective than the previous addChild because we don't
		// need to rebuild the entire hierarchy
		m_children.push_back(CTextNode());
		m_children.back() = xmlChild;
	}

	bool CTextNode::LoadFromFile(const char* fileName)
	{
		XmlParser parser;
		m_xmlNode = parser.parse(fileName);
		m_constXmlNode = m_xmlNode;
		RebuildChildrenList();
		return (m_xmlNode != NULL);
	}

	bool CTextNode::SaveToFile(const char* fileName) const
	{
		return m_constXmlNode->saveToFile(fileName);
	}

	bool CTextNode::LoadFromFile(IFile* file)
	{
		XmlParser parser;
		m_xmlNode = parser.parse(file);
		m_constXmlNode = m_xmlNode;
		RebuildChildrenList();
		return (m_xmlNode != NULL);
	}

	bool CTextNode::SaveToFile(IFile* file) const
	{
		behaviac::string temp;
		m_constXmlNode->getXML(temp);

		file->Write(temp.c_str(), (uint32_t)temp.size());

		return true;
	}

	// This is a very unprecise approximation...
	int32_t CTextNode::GetMemUsage() const
	{
		int32_t memUsage = sizeof(CTextNode);
		memUsage += sizeof(CXmlNode);
		memUsage += m_constXmlNode->getAttrCount() * sizeof(XmlAttributes::value_type);
		ChildrenContainer::const_iterator iter, end = m_children.end();

		for (iter = m_children.begin(); iter != end; ++iter)
		{
			memUsage += iter->GetMemUsage();
		}

		return memUsage;
	}
}//namespace behaviac