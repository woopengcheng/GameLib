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

#ifndef BEHAVIAC_BASE_CONFIG_H
#define BEHAVIAC_BASE_CONFIG_H

#include "behaviac/base/core/singleton.h"
#include "behaviac/base/core/string/stringid.h"

#include "behaviac/base/xml/ixml.h"

#include <map>
#include <string>

namespace behaviac
{
	class BEHAVIAC_API CConfig
	{
	public:
		BEHAVIAC_DELCARE_SINGLETON(CConfig);
		BEHAVIAC_DECLARE_MEMORY_OPERATORS(CConfig);
		CConfig();
		~CConfig();
	public:
		static const char*  Get(const char* section, const char* key);
		static void         Set(const char* section, const char* key, const char* value);

		static bool         Exists(const char* section, const char* key);
		static bool         SectionExists(const char* section);

		// overrideIfPresent : true=replace destination value with source value, false=keep destination value
		void MergeSections(const char* sourceSectionName, const char* destinationSectionName, bool overrideIfPresent);

		bool LoadConfig(const char* configfile);

		static const behaviac::map< CStringID, behaviac::string >& GetSection(const char* section);
		static XmlConstNodeRef                   GetSectionXmlInfo(const char* section);

	private:
		typedef behaviac::map< CStringID, behaviac::string > TSectionXmlInfoMap;
		typedef behaviac::map< CStringID, TSectionXmlInfoMap > TSectionsXmlInfoMap;
		TSectionsXmlInfoMap m_settings;
		behaviac::map< CStringID, XmlConstNodeRef > m_settingsXmlInfo;
	};
}//namespace behaviac

#endif // #ifndef BEHAVIAC_BASE_CONFIG_H
