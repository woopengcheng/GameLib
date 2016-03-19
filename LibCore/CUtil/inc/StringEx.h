#ifndef __cutil_string_ex_h__
#define __cutil_string_ex_h__
#include "CUtil/inc/Common.h"
#include "CUtil/inc/CUtil.h"
 
namespace  CUtil
{
	class StringExBase 
	{
	public:
		virtual void  Init(const char * pStr , UINT32 unLength) = 0;
		virtual const char * c_str() const = 0 ;
		virtual const UINT32 length() const = 0;
	};

	template<UINT32 L>
	class StringEx : public StringExBase
	{
	public:
		StringEx(const char * pStr , UINT32 unLength)
		{
			memset(m_szString , 0 , sizeof(m_szString));
			Init(pStr , unLength);
		}
		StringEx(const char * pStr)
		{
			memset(m_szString , 0 , sizeof(m_szString));
			Init(pStr , (UINT32)strlen(pStr)); //5 故意少拷贝一个.结尾时自动加上.
		}

		virtual void  Init(const char * pStr , UINT32 unLength) override
		{
			INT32 nSize = __min(unLength , L); 
			strncpy(m_szString , L , pStr);
			m_szString[nSize] = 0;
		}

		virtual const UINT32 length() const override
		{
			return (UINT32)strlen(m_szString);
		}

		virtual const char * c_str() const override
		{
			return m_szString;
		}

		StringEx<L> & operator = (const char * pStr)
		{
			Init(pStr , (UINT32)strlen(pStr));

			return * this;
		}

		operator const char * ()
		{
			return m_szString;
		}

		operator char * () const
		{
			return m_szString;
		}

		bool operator==(const char* val) const
		{
			return CUtil::strcmp(val,m_szString) == 0;
		}

		bool operator!=(const char* val) const
		{
			return CUtil::strcmp(val,m_szString) != 0;
		}
	protected:
		char m_szString[L];
	};
}


#endif