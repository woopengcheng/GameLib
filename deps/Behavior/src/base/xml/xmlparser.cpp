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
#include "behaviac/base/file/file.h"
#include "behaviac/base/file/filemanager.h"
#include "behaviac/base/core/rapidxml/rapidxml.hpp"

//******************************************************************************
//* XmlParserImp class.
//******************************************************************************
using namespace rapidxml;

namespace behaviac
{
	class XmlParserImp
	{
	public:
		XmlParserImp();
		~XmlParserImp();

		XmlNodeRef parse(char* buffer, int bufLen, const char* rootNodeName, behaviac::string& errorString, bool isFinal);

	protected:
		XmlNodeRef m_root;

		xml_document<>	m_parser;
		char*			m_buffer;
	};

	//******************************************************************************
	//* XmlParserImp
	//******************************************************************************
	XmlParserImp::XmlParserImp() : m_root(0), m_buffer(0)
	{
	}

	XmlParserImp::~XmlParserImp()
	{
		BEHAVIAC_FREE(m_buffer);
	}

	XmlNodeRef cloneXmlNodeFrom(xml_node<>* xmlnode)
	{
		XmlNodeRef node = CreateXmlNode(xmlnode->name());

		for (xml_attribute<>* attr = xmlnode->first_attribute();
			attr; attr = attr->next_attribute())
		{
			node->setAttrText(attr->name(), attr->value());
		}

		for (xml_node<>* subNode = xmlnode->first_node(); subNode; subNode = subNode->next_sibling())
		{
			XmlNodeRef sub = cloneXmlNodeFrom(subNode);
			node->addChild(sub);
		}

		return node;
	}

	XmlNodeRef XmlParserImp::parse(char* buffer, int bufLen, const char* rootNodeName, behaviac::string& errorString, bool isFinal)
	{
		BEHAVIAC_UNUSED_VAR(bufLen);
		BEHAVIAC_UNUSED_VAR(errorString);
		BEHAVIAC_UNUSED_VAR(isFinal);

		m_parser.parse<0>(buffer);

		xml_node<>* xmlnode = m_parser.first_node(rootNodeName);

		XmlNodeRef node = cloneXmlNodeFrom(xmlnode);

		return node;
	}

	XmlNodeRef XmlParser::parse(const char* fileName, const char* rootNodeName, const char* suffix)
	{
		m_errorString.clear();
		behaviac::IFile* file = behaviac::CFileManager::GetInstance()->FileOpen(fileName, behaviac::CFileSystem::EOpenAccess_Read);

		if (file)
		{
			XmlNodeRef xml = this->parse(file, rootNodeName, suffix, false);
			behaviac::CFileManager::GetInstance()->FileClose(file);

			if (!m_errorString.empty())
			{
				BEHAVIAC_LOGWARNING("Error while parsing file : %s\n\n%s", fileName, m_errorString.c_str());
			}

			return xml;

		}
		else
		{
			BEHAVIAC_ASSERT(0, "Cannot open XML file : %s\n", fileName);
			return XmlNodeRef();
		}
	}

	XmlNodeRef XmlParser::parse(behaviac::IFile* file, const char* rootNodeName, const char* suffix, bool handleError)
	{
		BEHAVIAC_UNUSED_VAR(suffix);
		BEHAVIAC_UNUSED_VAR(handleError);

		m_errorString.clear();
		XmlParserImp xml;

		if (file)
		{
			int iSize = (int)file->GetSize() - (int)file->Seek(0, behaviac::CFileSystem::ESeekMoveMode_Cur);

			if (iSize != 0)
			{
				static const int32_t ReadBlockSize = 64 * 1024;
				char* buf = (char*)BEHAVIAC_MALLOC_WITHTAG(ReadBlockSize, "XML");
				XmlNodeRef ref;

				for (int32_t i = 0; i <= iSize / (ReadBlockSize); ++i)
				{
					int32_t bufSize = file->Read(buf, ReadBlockSize);
					{
						buf[bufSize] = '\0';
						ref = xml.parse(buf, bufSize, rootNodeName, m_errorString, i == iSize / (ReadBlockSize));
					}
				}

				BEHAVIAC_FREE(buf);

				if (handleError && !m_errorString.empty())
				{
					BEHAVIAC_LOGWARNING("Error while parsing file\n\n%s", m_errorString.c_str());
				}

				return ref;

			}
			else
			{
				return XmlNodeRef();
			}
		}
		else
		{
			BEHAVIAC_ASSERT(0, "XmlParse(behaviac::IFile*) - Invalid file\n");
			return XmlNodeRef();
		}
	}

	//! Parse xml from null terminated buffer.
	XmlNodeRef XmlParser::parseBuffer(const char* buffer, const char* rootNodeName)
	{
		size_t bufLen = strlen(buffer);
		char* temp = (char*)BEHAVIAC_MALLOC_WITHTAG(bufLen + 1, "XmlParserImp::parse");
		memcpy(temp, buffer, bufLen);
		BEHAVIAC_ASSERT(temp[bufLen] == '\0');
		XmlNodeRef result = this->parseBuffer(temp, (int)bufLen, rootNodeName);
		BEHAVIAC_FREE(temp);
		return result;
	}

	//! Parse xml from memory buffer with specific size
	XmlNodeRef XmlParser::parseBuffer(char* buffer, int size, const char* rootNodeName)
	{
		m_errorString = "";
		XmlParserImp xml;
		XmlNodeRef ref = xml.parse(buffer, size, rootNodeName, m_errorString, true);

		if (!m_errorString.empty())
		{
			BEHAVIAC_LOGWARNING("Error while parsing XML file : \n\n%s", m_errorString.c_str());
		}

		return ref;
	}
}//namespace behaviac