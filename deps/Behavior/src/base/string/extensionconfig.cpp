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

#include "behaviac/base/string/extensionconfig.h"
#include "behaviac/base/string/stringutils.h"
#include "behaviac/base/string/pathid.h"

#include "behaviac/base/file/filemanager.h"
#include "behaviac/base/file/textfile.h"
#include "behaviac/base/xml/xmlparser.h"

namespace behaviac
{
	BEHAVIAC_IMPLEMNT_SINGLETON(CExtensionConfig);

	CExtensionConfig::CExtensionConfig()
	{
		LoadFromXML("engine/ResourceConfig.xml");
	}

	CExtensionConfig::~CExtensionConfig()
	{
	}

	void CExtensionConfig::LoadFromXML(const char* filepath)
	{
		if (behaviac::CFileManager::GetInstance()->FileExists(filepath))
		{
			XmlNodeRef xml = XmlParser().parse(filepath);
			LoadFromXML(xml);
		}
	}

	void CExtensionConfig::LoadFromXML(behaviac::XmlNodeRef& xml)
	{
		if (xml)
		{
			XmlNodeIt resNode(xml);

			for (XmlNodeRef res = resNode.first("Resource"); res; res = resNode.next("Resource"))
			{
				m_resInfos.push_back(SConfigInfo());
				m_resInfos.back().LoadFromXML(res);
			}
		}
	}

	const CExtensionConfig::SConfigInfo* CExtensionConfig::GetInfoFromSource(const char* source)
	{
		const char* str = behaviac::StringUtils::FindFullExtension(source);

		if (str && str[0])
		{
			const char* ext = str - 1;
			size_t resSize = m_resInfos.size();

			for (size_t i = 0; i < resSize; ++i)
			{
				SConfigInfo& cfg = m_resInfos[i];
				size_t extSize = cfg.m_sourceExt.size();

				for (size_t j = 0; j < extSize; ++j)
				{
					if (strcmp(cfg.m_sourceExt[j].c_str(), ext) == 0)
					{
						return &cfg;
					}
				}
			}

			if (m_resInfos.size() == 0)
			{
				BEHAVIAC_LOGERROR("Be sure that XML file \"engine/ResourceConfig.xml\" exist.\n");
			}
		}

		return NULL;
	}

	const CExtensionConfig::SConfigInfo* CExtensionConfig::GetInfoFromTarget(const char* target)
	{
		const char* str = behaviac::StringUtils::FindFullExtension(target);

		if (str && str[0])
		{
			const char* ext = str - 1;

			for (unsigned int i = 0; i < m_resInfos.size(); ++i)
			{
				if (m_resInfos[i].m_targetExt == ext)
				{
					return &m_resInfos[i];
				}
			}

			if (m_resInfos.size() == 0)
			{
				BEHAVIAC_LOGERROR("Be sure that XML file \"engine/ResourceConfig.xml\" exist.\n");
			}
		}

		return NULL;
	}

	const CExtensionConfig::SConfigInfo* CExtensionConfig::GetInfoFromDependency(const char* file)
	{
		const char* str = behaviac::StringUtils::FindFullExtension(file);

		if (str && str[0])
		{
			const char* ext = str - 1;

			for (unsigned int i = 0; i < m_resInfos.size(); ++i)
			{
				for (unsigned int j = 0; j < m_resInfos[i].m_compileDependencies.size(); ++j)
				{
					if (m_resInfos[i].m_compileDependencies[j] == ext)
					{
						return &m_resInfos[i];
					}
				}
			}

			if (m_resInfos.size() == 0)
			{
				BEHAVIAC_LOGERROR("Be sure that XML file \"engine/ResourceConfig.xml\" exist.\n");
			}
		}

		return NULL;
	}

	const CExtensionConfig::SConfigInfo* CExtensionConfig::GetInfoFromClassID(CStringID classID)
	{
		for (unsigned int i = 0; i < m_resInfos.size(); ++i)
		{
			if (m_resInfos[i].m_classID == classID)
			{
				return &m_resInfos[i];
			}
		}

		if (m_resInfos.size() == 0)
		{
			BEHAVIAC_LOGERROR("Be sure that XML file \"engine/ResourceConfig.xml\" exist.\n");
		}

		return NULL;
	}

	void CExtensionConfig::SConfigInfo::LoadFromXML(behaviac::XmlNodeRef& xml)
	{
		if (xml)
		{
			xml->getAttr("Class", this->m_className);
			m_classID.SetContent(m_className.c_str());
			xml->getAttr("TargetExt", m_targetExt);
			behaviac::string sourceExt;
			xml->getAttr("SourceExt", sourceExt);
			behaviac::StringUtils::SplitIntoArray(sourceExt, ";", m_sourceExt);
			behaviac::string deps;
			xml->getAttr("CompileDependency", deps);
			behaviac::StringUtils::SplitIntoArray(deps, ";", m_compileDependencies);
		}
	}
}//namespace behaviac