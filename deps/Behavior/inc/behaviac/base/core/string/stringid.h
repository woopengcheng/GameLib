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

#ifndef BEHAVIAC_BASE_STRINGID_H
#define BEHAVIAC_BASE_STRINGID_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"

#if !BEHAVIAC_RELEASE
#define		BEHAVIAC_STRINGID_DEBUG 1
#endif

// Usage of behaviac::string content will eventually be removed in release!
#ifdef BEHAVIAC_STRINGID_DEBUG
#define	BEHAVIAC_STRINGID_USESTRINGCONTENT	1
#endif

#ifdef BEHAVIAC_STRINGID_DEBUG
#define	BEHAVIAC_STRINGID_DEBUG_CASE
#endif

#if BEHAVIAC_STRINGID_USESTRINGCONTENT
// WARNING: Only define if needed for debugging
// Note: resolving the content is a "best guess", if the behaviac::string wasn't registered before
//       the resolve will fail.
#define	BEHAVIAC_STRINGID_RESOLVE_CONTENT
#endif

#if !BEHAVIAC_STRINGID_USESTRINGCONTENT && !defined(BEHAVIAC_STRINGID_RESOLVE_CONTENT) && !defined(BEHAVIAC_STRINGID_DEBUG_CASE)
// This should only be defined if we have nothing but the IDType in the CStringID instance.  (i.e. a retail build)
#define BEHAVIAC_STRINGID_RETAIL_OPTIM
#endif


namespace behaviac
{
	class CNoCaseStringID;

	////////////////////////////////////////////////////////////////////////////////
	//! StringID Class Declaration : This is a INTEGER that acts like STRING
	/*!
	*   - in BEHAVIAC_STRINGID_USESTRINGCONTENT, we store the behaviac::string source
	*	- the ID comes from the hash function with very low collision rate
	*/
	class BEHAVIAC_API CStringID
	{
	public:
		// Type definitions.
		typedef uint32_t IDType;
	private:
		static const IDType InvalidID;
		static const uint32_t  NoCaseMaxStringSize = 256;
	public:
		friend class CNoCaseStringID;

		//! Creates an Invalid behaviac::string ID.
		CStringID();
		virtual ~CStringID();

		//! Copy constructor.
		CStringID(const CStringID& other);

		//! Create a behaviac::string ID out of a behaviac::string.
		explicit CStringID(const char* str);

		//! Create a behaviac::string ID out of its CRC.
		explicit CStringID(IDType crc);

		//! Create a behaviac::string ID out of its CRC, also providing a behaviac::string (to use for safety checks and debugging)
		explicit CStringID(IDType crc, const char* str);

		//! Returns the behaviac::string ID's CRC.
		const IDType& GetUniqueID() const;

		//! Sets the behaviac::string ID's CRC. Flushes any previous content
		void SetUniqueID(IDType crc);

		//! Set the content of the behaviac::string ID post-creation; case-sensitive.
		void SetContent(const char* content);

		//! Returns true if the behaviac::string ID is valid.
		bool IsValid() const;

		static void Cleanup();
	private:
		//! Sets the content of the behaviac::string ID post-creation.
		void SetContent(const char* content, bool noCase, bool resolve);
		void SetContentWithExpectedCRC(const char* content, bool noCase, bool resolve, IDType crc);

		//! The CRC value.
		IDType m_value;

#ifdef BEHAVIAC_STRINGID_DEBUG_CASE
		bool m_noCase;
#endif

#if BEHAVIAC_STRINGID_USESTRINGCONTENT
	private:
		const char* 	            m_content;

	public:
		//! Returns the character behaviac::string value of the behaviac::string ID.
		const char* c_str() const;
#endif //BEHAVIAC_STRINGID_USESTRINGCONTENT

	public:
		//#if _MSC_VER >= 1400
		//    typedef eastl::fixed_string<char, 511> LogStringType;
		//#else
		//	typedef char LogStringType[511];
		//#endif//#if _MSC_VER >= 1400
		const char* LogStr() const;
	};

#if !BEHAVIAC_COMPILER_MSVC
//#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif//
	//! Compare two behaviac::string ID CRCs
	inline bool operator==(const CStringID& r, const CStringID& l)
	{
		const CStringID* pR = (const CStringID*)&r;
		const CStringID* pL = (const CStringID*)&l;

		return pR->GetUniqueID() == pL->GetUniqueID();
	}
#if !BEHAVIAC_COMPILER_MSVC
//#pragma GCC diagnostic pop
#endif

	inline bool operator==(const CStringID& r, const CStringID::IDType l)
	{
		return r.GetUniqueID() == l;
	}

	inline bool operator==(const CStringID::IDType r, const CStringID& l)
	{
		return r == l.GetUniqueID();
	}

	inline bool operator!=(const CStringID& r, const CStringID& l)
	{
		return r.GetUniqueID() != l.GetUniqueID();
	}

	inline bool operator!=(const CStringID& r, const CStringID::IDType l)
	{
		return r.GetUniqueID() != l;
	}

	inline bool operator!=(const CStringID::IDType r, const CStringID& l)
	{
		return r != l.GetUniqueID();
	}

	inline bool operator<(const CStringID& r, const CStringID& l)
	{
		return r.GetUniqueID() < l.GetUniqueID();
	}

	inline bool operator>(const CStringID& r, const CStringID& l)
	{
		return r.GetUniqueID() > l.GetUniqueID();
	}

	inline bool operator<=(const CStringID& r, const CStringID& l)
	{
		return r.GetUniqueID() <= l.GetUniqueID();
	}

	inline bool operator>=(const CStringID& r, const CStringID& l)
	{
		return r.GetUniqueID() >= l.GetUniqueID();
	}

	///////////////////////////////////////////////////////////////////////////////
	//! Defines a behaviac::string ID that defaults it's operation on a case-insensitive basis.
	class BEHAVIAC_API CNoCaseStringID : public CStringID
	{
	public:

		//! Default empty constructor.
		CNoCaseStringID();

		//! Create CNoCaseStringID out of another CStringID
		explicit CNoCaseStringID(const CStringID& other);

		//! Build behaviac::string ID from behaviac::string. The behaviac::string will be lower-case.
		explicit CNoCaseStringID(const char* str);

		//! Build behaviac::string ID from CRC.
		explicit CNoCaseStringID(IDType crc);

		//! Build behaviac::string ID from CRC, also providing a behaviac::string (to use for safety checks
		//  and debugging).  The behaviac::string will be lower-case.
		explicit CNoCaseStringID(IDType crc, const char* str);

		//! Set the content of the behaviac::string ID post-creation; case-insensitive.
		void SetContent(const char* str);
	};

	////////////////////////////////////////////////////////////////////////////////
	inline CStringID::CStringID(const CStringID& other)
	{
		m_value = other.m_value;
#ifdef BEHAVIAC_STRINGID_DEBUG_CASE
		m_noCase = other.m_noCase;
#endif
#if BEHAVIAC_STRINGID_USESTRINGCONTENT
		m_content = other.m_content;
#endif
#ifdef BEHAVIAC_STRINGID_RESOLVE_CONTENT

		if (IsValid() && m_content == NULL)
		{
			SetContent(NULL, false, true);
		}

#endif
	}

	////////////////////////////////////////////////////////////////////////////////
	inline CStringID::CStringID(const char* str)
	{
		SetContent(str, false, false);
	}

	////////////////////////////////////////////////////////////////////////////////
	inline CStringID::CStringID(IDType crc)
		:
		m_value(crc)
#if BEHAVIAC_STRINGID_USESTRINGCONTENT
		, m_content(NULL)
#endif
	{
#ifdef BEHAVIAC_STRINGID_DEBUG_CASE
		m_noCase = false;
#endif
#ifdef BEHAVIAC_STRINGID_RESOLVE_CONTENT
		SetContent(NULL, false, true);
#endif
	}

	////////////////////////////////////////////////////////////////////////////////
#if defined(BEHAVIAC_STRINGID_RETAIL_OPTIM)

	inline CStringID::CStringID(IDType crc, const char*) : m_value(crc) { }

	// Retail: local vars should not be don't be static, it would increase both code
	// and data size (branches and guard vars).  Its better to just initialize the 4-byte object!
	// TODO wgarvin: remove the static once crcs are checked in
#define Declare_Local_CStringID static const CStringID

#else

	// debugging version (initialize from behaviac::string, and then compare CRC to make sure it matches)
	inline CStringID::CStringID(IDType crc, const char* str)
	{
		SetContentWithExpectedCRC(str, false, false, crc);
		//SetContent(str, false, false);
	}

	// debugging version: Locals should be static to avoid computing CRC at run-time more than once
#define Declare_Local_CStringID static const CStringID

#endif

	////////////////////////////////////////////////////////////////////////////////
	BEHAVIAC_FORCEINLINE const CStringID::IDType& CStringID::GetUniqueID() const
	{
		return m_value;
	}

	////////////////////////////////////////////////////////////////////////////////
	inline void CStringID::SetUniqueID(IDType crc)
	{
		m_value = crc;
#if BEHAVIAC_STRINGID_USESTRINGCONTENT
		m_content = NULL;
#endif
#ifdef BEHAVIAC_STRINGID_DEBUG_CASE
		m_noCase = false;
#endif
#ifdef BEHAVIAC_STRINGID_RESOLVE_CONTENT
		SetContent(NULL, false, true);
#endif
	}

	////////////////////////////////////////////////////////////////////////////////
	inline void CStringID::SetContent(const char* str)
	{
		SetContent(str, false, false);
	}

	////////////////////////////////////////////////////////////////////////////////
	inline CNoCaseStringID::CNoCaseStringID() : CStringID()
	{
#ifdef BEHAVIAC_STRINGID_DEBUG_CASE
		m_noCase = true;
#endif
	}

	////////////////////////////////////////////////////////////////////////////////
	inline CNoCaseStringID::CNoCaseStringID(const CStringID& other) : CStringID(other)
	{
#ifdef BEHAVIAC_STRINGID_DEBUG_CASE
		BEHAVIAC_ASSERT(!"Trying to force a case-sensitive CStringID into a non-case-sensitive one." && other.m_noCase == true);
		m_noCase = other.m_noCase;
#endif
	}

	////////////////////////////////////////////////////////////////////////////////
	inline CNoCaseStringID::CNoCaseStringID(const char* str)
	{
		SetContent(str);
	}

	////////////////////////////////////////////////////////////////////////////////
	inline CNoCaseStringID::CNoCaseStringID(IDType crc)
		:
		CStringID(crc)
	{
#ifdef BEHAVIAC_STRINGID_DEBUG_CASE
		m_noCase = true;
#endif
	}

	////////////////////////////////////////////////////////////////////////////////
#if defined(BEHAVIAC_STRINGID_RETAIL_OPTIM)

	inline CNoCaseStringID::CNoCaseStringID(IDType crc, const char*) : CStringID(crc) { }

#else

	// debugging version (initialize from behaviac::string, and then compare CRC to make sure it matches)
	inline CNoCaseStringID::CNoCaseStringID(IDType crc, const char* str)
	{
		CStringID::SetContentWithExpectedCRC(str, true, false, crc);
		//CStringID::SetContent(str, true, false);
	}

#endif

	////////////////////////////////////////////////////////////////////////////////
	inline void CNoCaseStringID::SetContent(const char* str)
	{
		CStringID::SetContent(str, true, false);
	}
}//namespace behaviac

namespace stdext
{
	template<class Keyval> size_t hash_value(const Keyval& k);

	inline size_t hash_value(const behaviac::CNoCaseStringID& id)
	{
		return stdext::hash_value<behaviac::CNoCaseStringID::IDType>(id.GetUniqueID());
	}

	inline size_t hash_value(const behaviac::CStringID& id)
	{
		return stdext::hash_value<behaviac::CStringID::IDType>(id.GetUniqueID());
	}
}


#endif //#ifndef BEHAVIAC_BASE_STRINGID_H
