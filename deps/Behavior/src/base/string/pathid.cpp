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

#include "behaviac/base/core/thread/mutex.h"

#include "behaviac/base/base.h"
#include "behaviac/base/string/pathid.h"
#include "behaviac/base/file/filemanager.h"
#include "behaviac/base/string/extensionconfig.h"
#include "behaviac/base/string/stringutils.h"
#include "behaviac/base/core/crc.h"

namespace behaviac
{
	const CPathID::IDType CPathID::InvalidID = 0xFFFFFFFF;

#if STRINGID_USESTRINGCONTENT
	namespace
	{
		//typedef behaviac::map<CPathID::IDType,const char*, stdext::hash_compare< CPathID::IDType, std::less< CPathID::IDType > >, CCritSection> StringIdDictionary;
		typedef behaviac::map<CPathID::IDType, const char*> StringIdDictionary;
		static StringIdDictionary* ms_dictionary;
		StringIdDictionary& GetDictionary()
		{
			if (!ms_dictionary)
			{
				ms_dictionary = BEHAVIAC_NEW StringIdDictionary;
			}

			BEHAVIAC_ASSERT(ms_dictionary);
			return *ms_dictionary;
		}

		void CleanupDictionary()
		{
			if (ms_dictionary)
			{
				StringIdDictionary* dictionary = ms_dictionary;

				for (StringIdDictionary::iterator it = dictionary->begin(); it != dictionary->end(); ++it)
				{
					const char* p = it->second;

					BEHAVIAC_FREE(p);
				}

				dictionary->clear();
				BEHAVIAC_DELETE(ms_dictionary);
				ms_dictionary = 0;
			}
		}
	}
#endif//#if STRINGID_USESTRINGCONTENT

	CPathID::CPathID() :
		m_value(InvalidID)
#if STRINGID_USESTRINGCONTENT
		, m_content(NULL)
#endif
	{
	}

	CPathID::CPathID(const char* path)
	{
		BEHAVIAC_ASSERT(path);
		SetContent(path);
	}

	CPathID::CPathID(IDType crc) :
		m_value(crc)
#if STRINGID_USESTRINGCONTENT
		, m_content(NULL)
#endif
	{
	}

	CPathID::CPathID(const char* path, bool doNotFromat)
	{
		BEHAVIAC_ASSERT(path);

		if (doNotFromat)
		{
			behaviac::string strPath(path);
			strPath = make_lower(strPath);
			SetContentPrivate(strPath.c_str());
#ifdef BEHAVIAC_ENABLE_ASSERTS
			CPathID pathId(path);
			BEHAVIAC_ASSERT(GetUniqueID() == pathId.GetUniqueID());
#endif // #ifdef BEHAVIAC_ENABLE_ASSERTS

		}
		else
		{
			SetContent(path);
		}
	}

#if STRINGID_USESTRINGCONTENT
	const char* CPathID::c_str() const
	{
		if (m_content == NULL)
		{
			return "";

		}
		else
		{
			return m_content;
		}
	}

	behaviac::string CPathID::LogStr() const
	{
		return behaviac::string(c_str());
	}

#else

	behaviac::string CPathID::LogStr() const
	{
		char buf[32];
		string_sprintf(buf, "%u", m_value);
		return buf;
	}

#endif

	void CPathID::SetContent(const char* path)
	{
		BEHAVIAC_ASSERT(path);   // no full paths allowed
		char filename[PATHID_MAX_PATH];
		filename[0] = '\0';
		behaviac::CFileManager::GetInstance()->FormatPath(path, filename);
#ifdef _DEBUG

		if (strchr(filename, ':'))
		{
			BEHAVIAC_LOGERROR("Building a path id with a full path is prohibited.\n%s", filename);
		}

#endif // #ifdef _DEBUG

		if (CExtensionConfig::GetInstance())
		{
			const CExtensionConfig::SConfigInfo* cfg = CExtensionConfig::GetInstance()->GetInfoFromSource(filename);

			if (cfg)
			{
				//BEHAVIAC_LOGWARNING("Resource ID for %s uses wrong extension (%s instead of %s).\nSupported, but should be fixed.", s.c_str(), cfg->m_sourceExt[0].c_str(), cfg->m_targetExt.c_str());
				const char* ext = behaviac::StringUtils::FindFullExtension(filename);
				string_cpy(const_cast<char*>(ext), cfg->m_targetExt.c_str() + 1);
			}
		}

		CPathID::SetContentPrivate(filename);
	}

	void CPathID::ClearDictionary()
	{
#if STRINGID_USESTRINGCONTENT
		CleanupDictionary();
#endif//#if STRINGID_USESTRINGCONTENT
	}

	//////////////////////////////////////////////////////////////////////////////
	void CPathID::SetContentPrivate(const char* content)
	{
		if (content == NULL || content[0] == '\0')
		{
			m_value = InvalidID;
#if STRINGID_USESTRINGCONTENT
			m_content = NULL;
#endif

		}
		else
		{
			m_value = CRC32::CalcCRCNoCase(content);
		}

#if STRINGID_USESTRINGCONTENT
		behaviac::Mutex s_cs;
		behaviac::ScopedLock lock(s_cs);
		StringIdDictionary& dictionary = GetDictionary();
		StringIdDictionary::iterator it = dictionary.find(m_value);

		if (it != dictionary.end())
		{
			if (string_icmp(content, (*it).second))
			{
				BEHAVIAC_ASSERT(0, "There is a conflict between two PathID for the CRC 0x%08X.\n%s\n%s\n\nThe result of that is unpredictable but will probably crash.", m_value, content, (*it).second);
			}

			m_content = (*it).second;

		}
		else
		{
			uint32_t len = strlen(content);
			char* str = (char*)BEHAVIAC_MALLOC_WITHTAG(len + 1, "PathID");
			string_cpy(str, content);
			dictionary[m_value] = str;
			m_content = str;
		}

#endif
	}
}//namespace behaviac