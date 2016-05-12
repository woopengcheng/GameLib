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

#ifndef BEHAVIAC_CORE_XML_H
#define BEHAVIAC_CORE_XML_H

#include "behaviac/base/xml/ixml.h"

namespace behaviac
{
	/**
	******************************************************************************
	* CXmlNode class
	* Never use CXmlNode directly instead use reference counted XmlNodeRef.
	******************************************************************************
	*/
	class IFile;
	class CXmlNode : public IXmlNode
	{
	public:
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(CXmlNode)

			//! Constructor.
			CXmlNode(const char* tag);
		//! Destructor.
		~CXmlNode();

		//////////////////////////////////////////////////////////////////////////
		//! Reference counting.
		void AddRef() const
		{
			m_refCount++;
		};
		//! When ref count reach zero XML node dies.
		void Release() const
		{
			if (--m_refCount <= 0)
			{
				BEHAVIAC_DELETE(const_cast<CXmlNode*>(this));
			}
		};

		//! Get XML node tag.
		const char* getTag() const
		{
			return m_tag.c_str();
		};
		void setTag(const char* tag)
		{
			m_tag = tag;
		}

		//! Return true if given tag equal to node tag.
		bool isTag(const char* tag) const;

		//! Get XML Node attributes.
		virtual void copyAttributes(XmlConstNodeRef fromNode);

		//! Get the number of attribute
		virtual int	getAttrCount() const;
		//! Get an attribute value with his index
		virtual const char* getAttr(int index) const;
		//! Get an attribute value with his index
		virtual const char* getAttrTag(int index) const;

		//! Get XML Node attribute for specified key.
		const char* getAttr(const char* key) const;
		//! Check if attributes with specified key exist.
		bool haveAttr(const char* key) const;

		//! Adds new child node.
		void addChild(XmlNodeRef node);

		//! Creates new xml node and add it to childs list.
		XmlNodeRef newChild(const char* tagName);

		//! Remove child node.
		void removeChild(XmlNodeRef node);

		//! Remove all child nodes.
		void removeAllChilds();

		//! Swap child nodes
		void swapChilds(int child1, int child2);

		//! Get number of child XML nodes.
		int	getChildCount() const
		{
			return (int)m_childs.size();
		};
		int	getChildCount(const char* tag) const;

		//! Get XML Node child nodes.
		XmlNodeRef getChild(int i);
		XmlConstNodeRef getChild(int i) const;

		//! Find node with specified tag.
		XmlNodeRef findChild(const char* tag);
		XmlConstNodeRef findChild(const char* tag) const;

		//! Find node with specified tag. Create the new one if not found.
		XmlNodeRef findChildSafe(const char* tag);
		//! Find node with specified tag. Return the Invalid node if not found.
		XmlConstNodeRef findChildSafe(const char* tag) const;

		//! Returns content of this node.
		const char* getContent() const
		{
			return m_content.c_str();
		};
		void setContent(const char* str)
		{
			m_content = str;
		};
		void transferContent(behaviac::string& newContent)
		{
			m_content.swap(newContent);
		}

		XmlNodeRef	clone() const;

#ifdef _DEBUG
		//! Returns line number for XML tag.
		int getLine() const
		{
			return m_line;
		};
		//! Set line number in xml.
		void setLine(int line)
		{
			m_line = line;
		};
#endif //_DEBUG

		//! Returns XML of this node and sub nodes.
		void getXML(behaviac::string& xml, int level = 0) const;
		void getXML(behaviac::wstring& xml, int level = 0) const;
		bool saveToFile(const char* fileName) const;
		bool saveToFile(IFile* file) const;

		void ReserveAttr(int nCount);

		//! Set new XML Node attribute.
		virtual void setAttrText(const char* key, const char* value);
		virtual void setAttrText(const char* key, const wchar_t* text);

		//! Delete attrbute.
		void delAttr(const char* key);
		//! Remove all node attributes.
		void removeAllAttributes();

		const XmlConstNodeRef& getInvalidNode() const
		{
			return m_invalidNode;
		}

	private:

		static const XmlConstNodeRef m_invalidNode;

		typedef behaviac::vector<XmlNodeRef>	XmlNodes;
		XmlNodes m_childs;

		//! Xml node attributes.
		XmlAttributes m_attributes;

		//! Content of XML node.
		behaviac::string m_content;

		//! behaviac of XML node.
		XmlString m_tag;

		//! Ref count itself, its zeroed on node creation.
		mutable int m_refCount; // mutable as ref-counting as nothing to do with data access

		//! Line in XML file where this node firstly appeared (usefull for debuggin).
#ifdef _DEBUG
		int m_line;
#endif //_DEBUG
	};

}//namespace behaviac

/**
******************************************************************************
* CXmlNode class
* Never use CXmlNode directly instead use reference counted XmlNodeRef.
******************************************************************************
*/

#endif // #ifndef BEHAVIAC_CORE_XML_H
