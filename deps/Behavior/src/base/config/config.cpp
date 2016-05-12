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

#include "behaviac/base/xml/ixml.h"
#include "behaviac/base/xml/xmlparser.h"
#include "behaviac/base/config/config.h"
#include "behaviac/base/file/filemanager.h"
#include "behaviac/base/string/valuetostring.h"

#include "behaviac/base/core/crc.h"

namespace behaviac
{
	BEHAVIAC_IMPLEMNT_SINGLETON(CConfig);

	const char* CConfig::Get(const char* section, const char* key)
	{
		CStringID sectionid(section);
		CStringID id(key);
		behaviac::map< CStringID, behaviac::string >& category = GetInstance()->m_settings[sectionid];
		return category[id].c_str();
	}

	const behaviac::map< CStringID, behaviac::string >& CConfig::GetSection(const char* section)
	{
		CStringID sectionid(section);
		return GetInstance()->m_settings[sectionid];
	}

	void CConfig::Set(const char* section, const char* key, const char* value)
	{
		CStringID sectionid(section);
		CStringID id(key);
		behaviac::map< CStringID, behaviac::string >& category = GetInstance()->m_settings[sectionid];
		category[id] = value;
	}

	bool CConfig::Exists(const char* section, const char* key)
	{
		CStringID sectionid(section);
		CStringID id(key);
		TSectionsXmlInfoMap& settingsMap = GetInstance()->m_settings;
		TSectionsXmlInfoMap::iterator settingsIter = settingsMap.find(sectionid);

		if (settingsIter == settingsMap.end())
		{
			return false;
		}

		TSectionXmlInfoMap& sectionMap = (*settingsIter).second;
		TSectionXmlInfoMap::iterator sectionIter = sectionMap.find(id);

		if (sectionIter == sectionMap.end())
		{
			return false;
		}

		return true;
	}

	bool CConfig::SectionExists(const char* section)
	{
		CStringID sectionid(section);
		TSectionsXmlInfoMap& settingsMap = GetInstance()->m_settings;
		TSectionsXmlInfoMap::iterator settingsIter = settingsMap.find(sectionid);

		if (settingsIter == settingsMap.end())
		{
			return false;
		}

		return true;
	}

	XmlConstNodeRef CConfig::GetSectionXmlInfo(const char* section)
	{
		behaviac::map< CStringID, XmlConstNodeRef >::iterator it = GetInstance()->m_settingsXmlInfo.find(CStringID(section));

		if (it != GetInstance()->m_settingsXmlInfo.end())
		{
			return (*it).second;
		}

		return XmlConstNodeRef();
	}

	CConfig::CConfig(void)
	{
	}

	void CConfig::MergeSections(const char* sourceSectionName, const char* destinationSectionName, bool overrideIfPresent)
	{
		const CStringID sourceSectionID(sourceSectionName);
		const CStringID destinationSectionID(destinationSectionName);
		const TSectionXmlInfoMap& sourceSection = m_settings[sourceSectionID];
		TSectionXmlInfoMap& destinationSection = m_settings[destinationSectionID];
		TSectionXmlInfoMap::const_iterator it = sourceSection.begin();
		TSectionXmlInfoMap::const_iterator itEnd = sourceSection.end();

		for (; it != itEnd; ++it)
		{
			TSectionXmlInfoMap::iterator itDest = destinationSection.find((*it).first);

			if (itDest == destinationSection.end())
			{
				destinationSection.insert(*it);

			}
			else if (overrideIfPresent)
			{
				(*itDest).second = (*it).second;
			}
		}
	}

	bool CConfig::LoadConfig(const char* Config)
	{
		if (!behaviac::CFileManager::GetInstance()->FileExists(Config))
		{
			return false;
		}

		behaviac::string configfile(Config);
		XmlParser parser;
		XmlConstNodeRef root = parser.parse(configfile.c_str(), "config");

		if (root)
		{
			for (int i = 0; i < root->getChildCount(); i++)
			{
				XmlConstNodeRef section = root->getChild(i);

				if (section)
				{
					int generatedId = 0;
					CStringID sectionId(section->getTag());

					for (int j = 0; j < section->getChildCount(); j++)
					{
						XmlConstNodeRef setting = section->getChild(j);

						if (string_icmp(setting->getTag(), "XmlInfo") != 0)
						{
							behaviac::string id, value, console;
							CStringID sid;

							if (setting->getAttr("id", id))
							{
								sid.SetContent(id.c_str());

							}
							else
							{
								sid.SetContent(behaviac::StringUtils::ToString(generatedId++).c_str());
							}

							setting->getAttr("string", value);
							m_settings[sectionId][sid] = value;

						}
						else
						{
							// store the xml node for this section
							m_settingsXmlInfo[sectionId] = setting;
						}
					}
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	CConfig::~CConfig(void)
	{
	}
}//namespace behaviac