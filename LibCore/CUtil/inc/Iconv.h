#ifndef __cutil_iconv_h__
#define __cutil_iconv_h__
#include "CUtil/inc/Common.h"

namespace CUtil
{
	std::string UTF8ToGBK(const std::string * strInput);
	std::string UTF8ToGBK(const std::string & strInput);
	std::string GBKToUTF8(const std::string * strInput);
	std::string GBKToUTF8(const std::string & strInput);
}


#endif // __cutil_iconv_h__
