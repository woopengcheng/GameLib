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

#ifndef BEHAVIAC_CORE_PATHID_H
#define BEHAVIAC_CORE_PATHID_H

#include "behaviac/base/base.h"
#include "behaviac/base/core/container/string_t.h"
#include "behaviac/base/core/string/stringid.h"

#include <string>

namespace behaviac
{
	class BEHAVIAC_API CPathID
	{
	public:

		// Type definitions.
		typedef uint32_t IDType;
		const static IDType InvalidID;
		static const int PATHID_MAX_PATH = 260;

		CPathID();
		explicit CPathID(const char* szPath);
		explicit CPathID(const char* szPath, bool doNotFromat);
		explicit CPathID(IDType crc);

		//! Casts the behaviac::string ID into an integer containing its CRC.
		operator IDType() const
		{
			return m_value;
		}

		void SetContent(const char* content);

		//! Returns the behaviac::string ID's CRC.
		IDType GetUniqueID() const
		{
			return m_value;
		}

#if STRINGID_USESTRINGCONTENT
		//! Returns the character behaviac::string value of the behaviac::string ID.
		const char* c_str() const;
#endif

		//! Returns a valid behaviac::string that contains the character behaviac::string value of the behaviac::string ID.
		behaviac::string LogStr() const;

		//! Returns true if the behaviac::string ID is valid.
		bool IsValid() const
		{
			return m_value != InvalidID;
		}

		////////////////////////////////////////////////////////////////////////////////
		inline bool operator==(const CPathID& r)const
		{
			return GetUniqueID() == r.GetUniqueID();
		}

		////////////////////////////////////////////////////////////////////////////////
		inline bool operator!=(const CPathID& r)const
		{
			return GetUniqueID() != r.GetUniqueID();
		}

		////////////////////////////////////////////////////////////////////////////////
		inline bool operator<(const CPathID& r)const
		{
			return GetUniqueID() < r.GetUniqueID();
		}

		inline CPathID& operator=(const CPathID& other)
		{
			m_value = other.m_value;
#if STRINGID_USESTRINGCONTENT

			if (other.m_content)
			{
				SetContent(other.m_content);

			}
			else
			{
				m_content = NULL;
			}

#endif
			return *this;
		}

		inline CPathID(const CPathID& other)
		{
			m_value = other.m_value;
#if STRINGID_USESTRINGCONTENT

			if (other.m_content)
			{
				SetContent(other.m_content);

			}
			else
			{
				m_content = NULL;
			}

#endif
		}

		static void ClearDictionary();

	private:
		void SetContentPrivate(const char* content);

		//! The CRC value.
		IDType m_value;
#if STRINGID_USESTRINGCONTENT
		const char* m_content;
#endif //STRINGID_USESTRINGCONTENT
	};
	
}//namespace behaviac

#endif //BEHAVIAC_CORE_PATHID_H
