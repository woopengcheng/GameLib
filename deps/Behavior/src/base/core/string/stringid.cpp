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

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/memory/memory.h"
#include "behaviac/base/core/logging/log.h"
#include "behaviac/base/core/string/stringid.h"
#include "behaviac/base/core/string/formatstring.h"
#include "behaviac/base/core/crc.h"
#include "behaviac/base/core/thread/mutex.h"
#include "behaviac/base/core/container/map_t.h"
#include "behaviac/base/string/pathid.h"
#include "behaviac/base/string/stringutils.h"

//#include <map>
#include <map>
////////////////////////////////////////
// CURRENT ENGINE STRING ID CONFIGURATION
#if !defined(BEHAVIAC_STRINGID_DEBUG) && defined(BEHAVIAC_STRINGID_USESTRINGCONTENT)
#pragma message ("WARNING!!! BEHAVIAC_STRINGID_USESTRINGCONTENT defined in other config than debug.\n")
#endif
//
////////////////////////////////////////

namespace behaviac
{
	const CStringID::IDType CStringID::InvalidID = 0xFFFFFFFF;

	namespace
	{
		typedef behaviac::map<CStringID::IDType, const char*> StringIdDictionary;
	}

	////////////////////////////////////////////////////////////////////////////////
	CStringID::CStringID()
		:
		m_value(static_cast<IDType>(InvalidID))
#if BEHAVIAC_STRINGID_USESTRINGCONTENT
		, m_content(NULL)
#endif
	{
#ifdef BEHAVIAC_STRINGID_DEBUG_CASE
		m_noCase = false;
#endif
	}

	CStringID::~CStringID()
	{
#if BEHAVIAC_STRINGID_USESTRINGCONTENT
		//BEHAVIAC_FREE((void*)m_content);
#endif//#if BEHAVIAC_STRINGID_USESTRINGCONTENT
	}

	////////////////////////////////////////////////////////////////////////////////
	bool CStringID::IsValid() const
	{
		return m_value != static_cast<IDType>(InvalidID);
	}

#if BEHAVIAC_STRINGID_USESTRINGCONTENT
	static StringIdDictionary* ms_dictionary;
	static StringIdDictionary& GetContentDictionary()
	{
		if (!ms_dictionary)
		{
			ms_dictionary = BEHAVIAC_NEW StringIdDictionary;
		}

		BEHAVIAC_ASSERT(ms_dictionary);

		return *ms_dictionary;
	}
#endif//#if BEHAVIAC_STRINGID_USESTRINGCONTENT

	////////////////////////////////////////////////////////////////////////////////
	void CStringID::SetContent(const char* content, bool noCase, bool resolve)
	{
		BEHAVIAC_UNUSED_VAR(resolve);

#ifdef BEHAVIAC_STRINGID_DEBUG_CASE
		m_noCase = noCase;
#endif
#if BEHAVIAC_STRINGID_USESTRINGCONTENT
		static behaviac::Mutex ms_critialsection;
#ifdef BEHAVIAC_STRINGID_RESOLVE_CONTENT

		if (resolve)
		{
			ms_critialsection.Lock();
			StringIdDictionary::iterator it = GetContentDictionary().find(m_value);

			if (it != GetContentDictionary().end())
			{
				m_content = (*it).second;
			}

			ms_critialsection.Unlock();
			return;
		}

#endif
#endif

		if (content == NULL || content[0] == '\0')
		{
			m_value = InvalidID;
#if BEHAVIAC_STRINGID_USESTRINGCONTENT
			m_content = NULL;
#endif

		}
		else
		{
			if (noCase)
			{
				BEHAVIAC_ASSERT(false);
				m_value = CRC32::CalcCRCNoCase(content);

			}
			else
			{
				m_value = CRC32::CalcCRC(content);
			}

#if BEHAVIAC_STRINGID_USESTRINGCONTENT
			ms_critialsection.Lock();
			StringIdDictionary::iterator it = GetContentDictionary().find(m_value);

			if (it != GetContentDictionary().end())
			{
				if (noCase)
				{
					if (string_icmp(content, (*it).second))
					{
						BEHAVIAC_LOG3(BEHAVIAC_LOG_INFO, "There is a conflict between two StringID for the CRC 0x%08X.\n%s\n%s\n\nThe result of that is unpredictable but will probably crash.", m_value, content, (*it).second);
					}
				}
				else
				{
					if (strcmp(content, (*it).second))
					{
						BEHAVIAC_LOG3(BEHAVIAC_LOG_INFO, "There is a conflict between two StringID for the CRC 0x%08X.\n%s\n%s\n\nThe result of that is unpredictable but will probably crash.", m_value, content, (*it).second);
					}
				}

				m_content = (*it).second;

			}
			else
			{
				size_t len = strlen(content);
				char* str = (char*)BEHAVIAC_MALLOC(sizeof(char) * (len + 1));
				string_cpy(str, content);

				if (noCase)
				{
					for (size_t i = 0; i < len; ++i)
					{
						str[i] = (char)tolower(str[i]);
					}
				}

				GetContentDictionary()[m_value] = str;
				m_content = str;
			}

			ms_critialsection.Unlock();
#endif
		}
	}

	////////////////////////////////////////////////////////////////////////////////
#if BEHAVIAC_STRINGID_USESTRINGCONTENT
	const char* CStringID::c_str() const
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

	////////////////////////////////////////////////////////////////////////////////
	const char* CStringID::LogStr() const
	{
		return c_str();
	}

#else

	////////////////////////////////////////////////////////////////////////////////
	const char* CStringID::LogStr() const
	{
		static char s_buf[32];
		string_sprintf(s_buf, "%d", m_value);
		return s_buf;
	}

#endif

	void CStringID::Cleanup()
	{
#if BEHAVIAC_STRINGID_USESTRINGCONTENT

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

#endif//#if BEHAVIAC_STRINGID_USESTRINGCONTENT

		CPathID::ClearDictionary();
	}

	void CStringID::SetContentWithExpectedCRC(const char* content, bool noCase, bool resolve, IDType crc)
	{
		SetContent(content, noCase, resolve);
		IDType computedCrc = GetUniqueID();

		if (crc != computedCrc)
		{
			BEHAVIAC_ASSERT(false, "C%sStringID(0x%08X, \"%s\") has wrong CRC (should be 0x%08X!)  RETAIL BUILDS WILL USE THE WRONG VALUE, THIS MUST BE FIXED P0!",
				noCase ? "NoCase" : "", crc, content, GetUniqueID());
		}
	}
}//namespace behaviac