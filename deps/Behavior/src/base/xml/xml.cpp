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

#include "behaviac/base/xml/xml.h"
#include "behaviac/base/xml/xmlparser.h"

#ifdef I_WANT_TO_PUT_BACK_BINXML
#include "behaviac/base/xml/xml_binary.h"
#endif

#include "behaviac/base/file/file.h"
#include "behaviac/base/file/filemanager.h"
#include "behaviac/base/string/stringutils.h"
#include "behaviac/base/string/pathid.h"

/**
 ******************************************************************************
 * CXmlNode implementation.
 ******************************************************************************
 */

namespace behaviac
{
	XmlString::TableType XmlString::m_table;
	behaviac::Mutex XmlString::ms_critSection;

	XmlString::XmlString(XmlString const& string)
	{
		behaviac::ScopedLock lock(ms_critSection);
		Insert(string.m_str);
	}

	XmlString::XmlString(char const* string)
	{
		behaviac::ScopedLock lock(ms_critSection);
		Insert(string);
	}

	XmlString::~XmlString()
	{
		behaviac::ScopedLock lock(ms_critSection);
		Remove();
	}

	void XmlString::operator=(XmlString const& string)
	{
		behaviac::ScopedLock lock(ms_critSection);
		Remove();
		Insert(string.m_str);
	}

	void XmlString::Insert(char const* string)
	{
		if (string)
		{
			TableType::iterator i = m_table.find(string);

			if (i != m_table.end())
			{
				m_str = i->first;
				++i->second;

			}
			else
			{
				char* copy = (char*)BEHAVIAC_MALLOC_WITHTAG(strlen(string) + 1, "XmlString");
				string_cpy(copy, string);
				m_table.insert(TableType::value_type(copy, 1));
				m_str = copy;
			}
		}
		else
		{
			m_str = 0;
		}
	}

	void XmlString::Remove()
	{
		if (m_str)
		{
			TableType::iterator result = m_table.find(m_str);

			if (--result->second == 0)
			{
				m_table.erase(result);
				BEHAVIAC_FREE(const_cast<char*>(m_str));
			}

			m_str = 0;
		}
	}

	IXmlNode* CreateXmlNode(const char* tag)
	{
		return BEHAVIAC_NEW CXmlNode(tag);
	}

	const XmlConstNodeRef CXmlNode::m_invalidNode = BEHAVIAC_NEW CXmlNode("InvalidTag");

	CXmlNode::~CXmlNode()
	{
	}

	//! The only ctor and private, protect us from deriviation.
	CXmlNode::CXmlNode(const char* tag) : m_tag(tag), m_refCount(0) {}

	//////////////////////////////////////////////////////////////////////////
	bool CXmlNode::isTag(const char* tag) const
	{
		return string_icmp(tag, m_tag.c_str()) == 0;
	}

	const char* CXmlNode::getAttr(const char* key) const
	{
		XmlAttributes::const_iterator it = std::find(m_attributes.begin(), m_attributes.end(), key);

		if (it != m_attributes.end())
		{
			return it->GetValue();
		}

		return NULL;
	}

	//! Get XML Node child nodes.
	int CXmlNode::getAttrCount() const
	{
		return (int)m_attributes.size();
	}

	//! Get XML Node child nodes.
	const char* CXmlNode::getAttr(int index) const
	{
		if ((size_t)index < m_attributes.size())
		{
			return m_attributes[index].GetValue();
		}

		return 0;
	}

	//! Get XML Node child nodes.
	const char* CXmlNode::getAttrTag(int index) const
	{
		if ((uint32_t)index < m_attributes.size())
		{
			return m_attributes[index].GetKey();
		}

		return 0;
	}

	bool CXmlNode::haveAttr(const char* key) const
	{
		XmlAttributes::const_iterator it = std::find(m_attributes.begin(), m_attributes.end(), key);

		if (it != m_attributes.end())
		{
			return true;
		}

		return false;
	}

	void CXmlNode::delAttr(const char* key)
	{
		XmlAttributes::iterator it = std::find(m_attributes.begin(), m_attributes.end(), key);

		if (it != m_attributes.end())
		{
			m_attributes.erase(it);
		}
	}

	void CXmlNode::removeAllAttributes()
	{
		m_attributes.clear();
	}

	void CXmlNode::ReserveAttr(int nCount)
	{
		m_attributes.reserve(nCount);
	}

	void CXmlNode::setAttrText(const char* key, const char* text)
	{
		XmlAttributes::iterator it = std::find(m_attributes.begin(), m_attributes.end(), key);

		if (it == m_attributes.end())
		{
			m_attributes.resize(m_attributes.size() + 1);
			m_attributes.back().SetKey(key);
			m_attributes.back().SetValue(text);

		}
		else
		{
			it->SetValue(text);
		}
	}

	void CXmlNode::setAttrText(const char* key, const wchar_t* text)
	{
		XmlAttributes::iterator it = std::find(m_attributes.begin(), m_attributes.end(), key);

		if (it == m_attributes.end())
		{
			m_attributes.resize(m_attributes.size() + 1);
			m_attributes.back().SetKey(key);
			m_attributes.back().SetValue(text);

		}
		else
		{
			it->SetValue(text);
		}
	}

	XmlNodeRef CXmlNode::findChild(const char* tag)
	{
		for (XmlNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
		{
			if ((*it)->isTag(tag))
			{
				return *it;
			}
		}

		const char* i = strchr(tag, '/');

		if (i)
		{
			behaviac::string id(tag, i - tag);
			XmlNodeRef node = findChild(id.c_str());

			if (node)
			{
				return node->findChild(i + 1);
			}
		}

		return XmlNodeRef(0);
	}

	XmlConstNodeRef CXmlNode::findChild(const char* tag) const
	{
		for (XmlNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
		{
			if ((*it)->isTag(tag))
			{
				return *it;
			}
		}

		const char* i = strchr(tag, '/');

		if (i)
		{
			behaviac::string id(tag, i - tag);
			XmlConstNodeRef node = findChild(id.c_str());

			if (node)
			{
				return node->findChild(i + 1);
			}
		}

		return XmlConstNodeRef(0);
	}

	XmlNodeRef CXmlNode::findChildSafe(const char* tag)
	{
		XmlNodeRef node = findChild(tag);

		if (!node)
		{
			return newChild(tag);
		}

		return node;
	}

	XmlConstNodeRef CXmlNode::findChildSafe(const char* tag) const
	{
		XmlConstNodeRef node = findChild(tag);

		if (!node)
		{
			return getInvalidNode();
		}

		return node;
	}

	//! Adds new child node.
	void CXmlNode::addChild(XmlNodeRef node)
	{
		BEHAVIAC_ASSERT(node != 0);
		m_childs.push_back(node);
	};

	XmlNodeRef CXmlNode::newChild(const char* tagName)
	{
		XmlNodeRef node = CreateXmlNode(tagName);
		addChild(node);
		return node;
	}

	void CXmlNode::removeChild(XmlNodeRef node)
	{
		XmlNodes::iterator it = std::find(m_childs.begin(), m_childs.end(), (IXmlNode*)node);

		if (it != m_childs.end())
		{
			m_childs.erase(it);
		}
	}

	void CXmlNode::removeAllChilds()
	{
		m_childs.clear();
	}

	void CXmlNode::swapChilds(int child1, int child2)
	{
		BEHAVIAC_ASSERT(child1 >= 0 && child1 < static_cast<int>(m_childs.size()));
		BEHAVIAC_ASSERT(child2 >= 0 && child2 < static_cast<int>(m_childs.size()));
		std::swap(m_childs[child1], m_childs[child2]);
	}

	//! Get number of child XML nodes.
	int	CXmlNode::getChildCount(const char* tag) const
	{
		if (!tag)
		{
			return 0;
		}

		int count = 0;

		for (XmlNodes::const_iterator it = m_childs.begin(), itEnd = m_childs.end();
			it != itEnd;
			++it)
		{
			if ((*it)->isTag(tag))
			{
				++count;
			}
		}

		return count;
	}

	//! Get XML Node child nodes.
	XmlNodeRef CXmlNode::getChild(int i)
	{
		BEHAVIAC_ASSERT(i >= 0 && i < (int)m_childs.size());
		return m_childs[i];
	}

	XmlConstNodeRef CXmlNode::getChild(int i) const
	{
		BEHAVIAC_ASSERT(i >= 0 && i < (int)m_childs.size());
		return m_childs[i];
	}

	//////////////////////////////////////////////////////////////////////////
	void CXmlNode::copyAttributes(XmlConstNodeRef fromNode)
	{
		int32_t count = fromNode->getAttrCount();

		for (int idx = 0; idx < count; idx++)
		{
			setAttr(fromNode->getAttrTag(idx), fromNode->getAttr(idx));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	XmlNodeRef CXmlNode::clone() const
	{
		XmlNodeRef newNode = CreateXmlNode(getTag());
		newNode->copyAttributes(this);
		newNode->setContent(getContent());

		for (int i = 0; i < getChildCount(); ++i)
		{
			newNode->addChild(getChild(i)->clone());
		}

		return newNode;
	}

	void PushAndConvertToXmlString(const char* inputString, behaviac::string& outputString)
	{
		if (inputString)
		{
			for (int32_t c = 0; inputString[c] != '\0'; c++)
			{
				switch ((int32_t)inputString[c])
				{
				case '<':
					outputString += "&lt;";
					break;

				case '>':
					outputString += "&gt;";
					break;

				case '&':
					outputString += "&amp;";
					break;

				case '\"':
					outputString += "&quot;";
					break;

				case '\'':
					outputString += "&apos;";
					break;

					// Convert false white space
				case 0xFF:
					outputString += " ";
					break;

				default:
					outputString.push_back(inputString[c]);
					break;
				}
			}
		}
	}

	void CXmlNode::getXML(behaviac::string& xml, int level) const
	{
		// Add tabs.
		for (int i = 0; i < level; i++)
		{
			xml += "\t";
		}

		// Begin behaviac
		bool AddSpace = false;

		if (m_attributes.empty())
		{
			xml += "<";
			xml += m_tag.c_str();
			AddSpace = true;

		}
		else
		{
			xml += "<";
			xml += m_tag.c_str();

			// Put attributes.
			for (XmlAttributes::const_iterator it = m_attributes.begin(); it != m_attributes.end(); ++it)
			{
				xml += " ";
				xml += it->GetKey();
				xml += "=\"";
				PushAndConvertToXmlString(it->GetValue(), xml);
				xml += "\"";
			}
		}

		if (m_content.empty() && m_childs.empty())
		{
			// Compact tag form.
			if (AddSpace)
			{
				xml += " />\r\n";

			}
			else
			{
				xml += " />\r\n";
			}

			return;
		}

		xml += ">";
		// Put node content.
		PushAndConvertToXmlString(m_content.c_str(), xml);

		if (!m_childs.empty())
		{
			xml += "\r\n";

			// Put sub nodes.
			for (XmlNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
			{
				(*it)->getXML(xml, level + 1);
			}

			// End tag.

			// Add tabs.
			for (int i = 0; i < level; i++)
			{
				xml += "\t";
			}
		}

		xml += "</";
		xml += m_tag.c_str();
		xml += ">\r\n";
	}

	void CXmlNode::getXML(behaviac::wstring& xml, int level) const
	{
		// Add tabs.
		for (int i = 0; i < level; i++)
		{
			xml += L"\t";
		}

		// Begin behaviac
		bool AddSpace = false;

		if (m_attributes.empty())
		{
			xml += L"<";
			xml += behaviac::StringUtils::Char2Wide(m_tag.c_str());
			AddSpace = true;

		}
		else
		{
			xml += L"<";
			xml += behaviac::StringUtils::Char2Wide(m_tag.c_str());

			// Put attributes.
			for (XmlAttributes::const_iterator it = m_attributes.begin(); it != m_attributes.end(); ++it)
			{
				xml += L" ";
				xml += behaviac::StringUtils::Char2Wide(it->GetKey());
				xml += L"=\"";

				if (it->IsWide())
				{
					xml += it->GetValueWide();

				}
				else
				{
					behaviac::string t;
					PushAndConvertToXmlString(it->GetValue(), t);

					xml += behaviac::StringUtils::Char2Wide(t);
				}

				xml += L"\"";
			}
		}

		if (m_content.empty() && m_childs.empty())
		{
			// Compact tag form.
			if (AddSpace)
			{
				xml += L" />\r\n";

			}
			else
			{
				xml += L" />\r\n";
			}

			return;
		}

		xml += L">";
		// Put node content.
		{
			behaviac::string t;
			PushAndConvertToXmlString(m_content.c_str(), t);

			xml += behaviac::StringUtils::Char2Wide(t);
		}

		if (!m_childs.empty())
		{
			xml += L"\r\n";

			// Put sub nodes.
			for (XmlNodes::const_iterator it = m_childs.begin(); it != m_childs.end(); ++it)
			{
				(*it)->getXML(xml, level + 1);
			}

			// End tag.

			// Add tabs.
			for (int i = 0; i < level; i++)
			{
				xml += L"\t";
			}
		}

		xml += L"</";
		xml += behaviac::StringUtils::Char2Wide(m_tag.c_str());
		xml += L">\r\n";
	}

	//bool CXmlNode::saveToFile(const char* fileName) const
	//{
	//    FILE* file = fopen(fileName, "w+,ccs=UTF-8");
	//
	//    if (file)
	//    {
	//		behaviac::string out;
	//
	//		this->getXML(out);
	//
	//		int utf_size = out.size();
	//		BEHAVIAC_UNUSED_VAR(utf_size);
	//
	//		behaviac::wstring temp;
	//
	//		bool bOk = behaviac::StringUtils::MBSToWCS(temp, out);
	//		BEHAVIAC_ASSERT(bOk);
	//
	//		int wide_size = temp.size();
	//		BEHAVIAC_UNUSED_VAR(wide_size);
	//
	//		int sizeoft = sizeof(behaviac::wstring::value_type);
	//		BEHAVIAC_ASSERT(sizeoft == 2);
	//
	//		fwrite(temp.c_str(), temp.size(), sizeoft, file);
	//
	//        fclose(file);
	//
	//        return true;
	//    }
	//    else
	//    {
	//        BEHAVIAC_ASSERT(0,  "CXmlNode::SaveLevel, cannot open for write %s, xml won't be saved\n", fileName);
	//    }
	//
	//    return false;
	//}

	bool CXmlNode::saveToFile(const char* fileName) const
	{
		IFile* file = behaviac::CFileManager::GetInstance()->FileOpen(fileName, CFileSystem::EOpenAccess_Write);

		if (file)
		{
			this->saveToFile(file);
			behaviac::CFileManager::GetInstance()->FileClose(file);
			return true;

		}
		else
		{
			BEHAVIAC_ASSERT(0, "CXmlNode::SaveLevel, cannot open for write %s, xml won't be saved\n", fileName);
		}

		return false;
	}

	bool CXmlNode::saveToFile(IFile* file) const
	{
		if (file)
		{
			//bool result = getXML(file);
			behaviac::wstring temp;

			this->getXML(temp);

			behaviac::string out;
			behaviac::StringUtils::Wide2Char(out, temp);
			file->Write(out.c_str(), (uint32_t)out.size());

			return true;
		}

		return false;
	}
}//namespace behaviac