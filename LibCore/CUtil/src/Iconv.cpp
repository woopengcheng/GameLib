#include "CUtil/inc/Iconv.h"
#include "CUtil/inc/ThreadSpecific.h"
#include "iconv.h"

namespace CUtil
{
	class DLL_EXPORT IConvEncoding
	{
	public:
		IConvEncoding()
		{
			m_hIconvGBKToUTF8 = iconv_open("UTF-8", "GB2312");
			m_hIconvUTF8ToGBK = iconv_open("GB2312", "UTF-8");
		}
		~IConvEncoding()
		{
			iconv_close(m_hIconvGBKToUTF8);
			iconv_close(m_hIconvUTF8ToGBK);
		}

	public:
		std::string UTF8ToGBK(const std::string & strInput)
		{
			return Convert(m_hIconvUTF8ToGBK, strInput);
		}
		std::string GBKToUTF8(const std::string & strInput)
		{
			return Convert(m_hIconvGBKToUTF8, strInput);
		}

	private:
		std::string Convert(iconv_t ic, const std::string & strInput)
		{
			if (strInput.empty())
			{
				return std::string();
			}

			size_t ilen = strInput.length();
			const char* ibuf = strInput.c_str();

			size_t olen = (ilen+1) * 4;
			size_t nlen = olen;
			char* obuf = (char*)alloca(olen);//&buf[0];
			memset(obuf, 0, olen);
			char* obuf2 = obuf;

#ifdef _MSC_VER
			iconv(ic, (char**)(&ibuf), &ilen, &obuf2, &olen);
#else
			iconv(ic, (char**)&ibuf, &ilen, &obuf2, &olen);
#endif

			return std::string(obuf, nlen - olen);
		}

	private:
		iconv_t m_hIconvGBKToUTF8;
		iconv_t m_hIconvUTF8ToGBK;

	};

	ThreadSpecific<IConvEncoding> g_objIconvEncoding;

	IConvEncoding * GetCurrentEncoding()
	{
		IConvEncoding* result = g_objIconvEncoding.get();
		if (result != NULL)
			return result;
		result = new IConvEncoding();
		g_objIconvEncoding.set(result);
		return result;
	}
	std::string UTF8ToGBK(const std::string * strInput)
	{
		return GetCurrentEncoding()->UTF8ToGBK(*strInput);
	}

	std::string UTF8ToGBK(const std::string & strInput)
	{
		return GetCurrentEncoding()->UTF8ToGBK(strInput);
	}

	std::string GBKToUTF8(const std::string * strInput)
	{
		return GetCurrentEncoding()->GBKToUTF8(*strInput);
	}

	std::string GBKToUTF8(const std::string & strInput)
	{
		return GetCurrentEncoding()->GBKToUTF8(strInput);
	}
	
}