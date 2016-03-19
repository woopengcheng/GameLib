#include "CUtil/inc/CUtil.h"
#include "CUtil/inc/Random.h"
#include "CUtil/inc/UnitTest.h"
#include "LogLib/inc/Log.h"
#include "NetLib/inc/NetCommon.h" 
#include "Timer/inc/TimerHelp.h"
#include "MsgLib/inc/RpcCheckParams.h"
#include "json/value.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/hmac.h>
#include "CityHash/city.h"
#include "snappy.h"

#ifdef WIN32
#include <ObjBase.h>
#else
#include <uuid/uuid.h>
#endif

namespace CUtil
{
	void GenerateUUIDBySys(char* buf)
	{

#ifdef WIN32
		GUID FilterCLSID;
		CoCreateGuid(&FilterCLSID);

		sprintf(buf, "%08X_%04X_%04X_%02X%02X_%02X%02X%02X%02X%02X%02X",
			FilterCLSID.Data1, FilterCLSID.Data2, FilterCLSID.Data3,
			FilterCLSID.Data4[0], FilterCLSID.Data4[1], FilterCLSID.Data4[2], FilterCLSID.Data4[3], 
			FilterCLSID.Data4[4], FilterCLSID.Data4[5], FilterCLSID.Data4[6], FilterCLSID.Data4[7]
		);
#else
		uuid_t uid;
		uuid_generate(uid);
		uuid_unparse(uid,buf);
#endif 
	}

	INT64 GenerateUUIDByTime( void )
	{ 
		return (Timer::GetTickCount());
	}

	CErrno  Init(std::string strLogFile)
	{  
		Assert_ReF(Log::Init(strLogFile.c_str() , NULL , 0).IsSuccess()) ; 
		Assert_ReF(!Net::InitNet()); 

		UnitTestStart();
		char * p = ".";
		GoogleTestStart(1, &p);
		return CErrno::Success();
	}

	CErrno  Cleanup( void)
	{ 
		Assert_ReF(!Net::CleanNet());   
		Assert_ReF(Log::Cleanup().IsSuccess()); 

		return CErrno::Success();
	} 

	INT32   strcmp( const char* str1,const char* str2 )
	{
		return ::strcmp(str1,str2);
	}

	INT32   stricmp( const char* str1,const char* str2 )
	{
#ifdef WIN32
		return ::_stricmp(str1,str2);
#else 
		return ::strcasecmp(str1,str2);
#endif 
	}

	INT64   atoi( const char* str )
	{ 
		if(str == NULL)
			return 0LL;

#ifdef WIN32
		return _atoi64(str); 
#else
		return ::atoll(str);
#endif

	}

	INT64 atoi(const std::string & str)
	{
		return CUtil::atoi(str.c_str());
	}

	UINT64   atoull(const char* str)
	{
		if(str == NULL)
			return 0LL;

#ifdef WIN32
		return _strtoui64(str , NULL , 10); 
#else
		return ::strtouq(str);
#endif


	}
	double  atof( const char* str )
	{
		return ::atof(str);
	}

	double atof(const std::string & str)
	{
		return CUtil::atof(str.c_str());
	}

	std::string   itoa( INT64  val )
	{
		char buf[100] = "";
		itoa(buf,val);
		return std::string(buf);
	}

	void    itoa( char* str,INT64 val )
	{
#ifdef WIN32
		_i64toa(val,str,10);
#else
		sprintf(str,"%lld",val);
#endif
	}

	void    ftoa( char* str,double val )
	{
		sprintf(str,"%lf",val);
	}

	char*   strncpy( char* dst,size_t len,const char* src )
	{
#ifdef WIN32
		strcpy_s(dst,len , src);
		return dst;
#else
		return ::strncpy(dst,src,len);
#endif

	}

	bool   floatcmp( float value1,float value2 )
	{
		const float fEpsilon = 0.000001f;
		float fDelta = fabs(value1-value2);

		if (fDelta > fEpsilon)
			return true;
		else
			return false; 
	}

	INT64   rdtsc()
	{
#ifdef WIN32
//		_asm rdtsc;
		// 		LARGE_INTEGER result = {0};
		// 		if(!QueryPerformanceCounter(&result))
		// 			assert(false && "System NoSupport QueryPerformanceCounter");
		// 		return result.LowPart;
		// return (int)GetTickCount();
#else
//		unsigned int lo;
//		unsigned int hi;
//		__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
//		return (((sint64)hi) << 32) + lo;
#endif 
		return 0;
	}

	INT64   random()
	{
		return g_objRandomGenerator.Generator(); 
	}

	INT32   random( INT32 start,INT32 end )
	{
		if(start == end)
			return start;
		unsigned int r = random() % 0xffffffff;
		return (r % (end - start)) + start; 
	}

	float   random( float start,float end )
	{
		unsigned int r = random() % 0xffffffff;
		double d = (double)r / (double)0xffffffff;
		return (float)(start + (end - start) * d); 
	}

	std::string  strlower(const  char* szBuf )
	{
		std::locale loc;
		std::string str = szBuf;

		for(size_t i = 0;i < str.size();++i)
		{ 
#ifdef WIN32
			str[i] = ::tolower(str[i]);
#else
			str[i] = std::tolower(str[i],loc);
#endif
		}
		return str; 
	}

	std::string strupper(const  char* szBuf )
	{
		std::locale loc;
		std::string str = szBuf; 

		for(size_t i = 0;i < str.size();++i)
		{  
#ifdef WIN32
			str[i] = ::toupper(str[i]);
#else
			str[i] = std::toupper(str[i],loc);
#endif
		}
		return str;
	}

	BOOL	   is_between( float start,float end,float value )
	{
		if(start > end)
			std::swap(start,end);
		return value >= start && value <= end;

	}

	BOOL	   is_between( INT64 start,INT64 end,INT64 value )
	{
		if(start > end)
			std::swap(start,end);
		return value >= start && value <= end; 
	}

	BOOL	   is_between( INT32 start,INT32 end,INT32 value )
	{ 
		if(start > end)
			std::swap(start,end);
		return value >= start && value <= end;
	}

	float   clamp( float ho,float lo,float value )
	{
		if(value < lo)
			return lo;
		else if(value >= ho)
			return ho;
		return value;  
	}

	INT32   clamp( INT32 ho,INT32 lo,INT32 value )
	{
		if(value < lo)
			return lo;
		else if(value >= ho)
			return ho;
		return value; 
	}

	BOOL	   strtobool( const char* text )
	{ 
		if(stricmp(text,"true") == 0)
			return true;
		if(stricmp(text,"false") == 0)
			return false;
		if(stricmp(text,"1") == 0)
			return true;
		if(stricmp(text,"0") == 0)
			return false;
		return atoi(text) != 0;
	}

	BOOL	   isNumber( const char* text )
	{
		size_t len = strlen(text);
		for(size_t i = 0; i < len; ++i)
		{
			if(!(text[i] >= '0' && text[i] <= '9'))
			{
				return false;
			}
		}
		return true;
	}

	const char __HEX[16] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};

	void    tohex( INT64 val,char* obuf,UINT32 len )
	{
		std::stringstream ss;
		ss << std::hex << val << std::dec;
		strncpy(obuf,len,ss.str().c_str());
	}

	void    tohex( const char* ibuf,UINT32 ilen,char* obuf,UINT32 & olen )
	{
		UINT32 outlen = 0;
		for (UINT32 i = 0; i < ilen && outlen < olen; ++i) {
			int t = (unsigned char)ibuf[i];
			int a = t / 16;
			int b = t % 16;
			obuf[outlen] = __HEX[a];
			++outlen;
			obuf[outlen] = __HEX[b];
			++outlen;
		}
		obuf[outlen] = 0;
		olen = outlen; 
	}

	void	   create_token( char* buf,UINT32 len )
	{
		static const char s_objTokenPool[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-=+_";

		for(UINT32 i = 0; i < (len - 1); ++i)
		{
			int r = random(0,_countof(s_objTokenPool));
			buf[i] = s_objTokenPool[r];
		}
		buf[len - 1] = '\0';

	}

	/*Copyright (c) 2006 Joerg Wiedenmann*/
	void tokenize(const std::string& str, std::vector<std::string>& result, const std::string& delimiters, const std::string& delimiters_preserve /*= ""*/, const std::string& quote /*= "\""*/, const std::string& esc /*= "\\"*/)
	{
		// clear the std::vector
		if (false == result.empty())
		{
			result.clear();
		}

		std::string::size_type pos = 0; // the current position (char) in the std::string
		char ch = 0; // buffer for the current character
		char delimiter = 0;    // the buffer for the delimiter char which
							   // will be added to the tokens if the delimiter
							   // is preserved
		char current_quote = 0; // the char of the current open quote
		bool quoted = false; // indicator if there is an open quote
		std::string token;  // std::string buffer for the token
		bool token_complete = false; // indicates if the current token is
									 // read to be added to the result std::vector
		std::string::size_type len = str.length();  // length of the input-std::string

													// for every char in the input-std::string
		while (len > pos)
		{
			// get the character of the std::string and reset the delimiter buffer
			ch = str.at(pos);
			delimiter = 0;

			// assume ch isn't a delimiter
			bool add_char = true;

			// check ...

			// ... if the delimiter is an escaped character
			bool escaped = false; // indicates if the next char is protected
			if (false == esc.empty()) // check if esc-chars are  provided
			{
				if (std::string::npos != esc.find_first_of(ch))
				{
					// get the escaped char
					++pos;
					if (pos < len) // if there are more chars left
					{
						// get the next one
						ch = str.at(pos);

						// add the escaped character to the token
						add_char = true;
					}
					else // cannot get any more characters
					{
						// don't add the esc-char
						add_char = false;
					}

					// ignore the remaining delimiter checks
					escaped = true;
				}
			}

			// ... if the delimiter is a quote
			if (false == quote.empty() && false == escaped)
			{
				// if quote chars are provided and the char isn't protected
				if (std::string::npos != quote.find_first_of(ch))
				{
					// if not quoted, set state to open quote and set
					// the quote character
					if (false == quoted)
					{
						quoted = true;
						current_quote = ch;

						// don't add the quote-char to the token
						add_char = false;
					}
					else // if quote is open already
					{
						// check if it is the matching character to close it
						if (current_quote == ch)
						{
							if (token.length() == 0)
								token.push_back('\0');

							// close quote and reset the quote character
							quoted = false;
							current_quote = 0;

							// don't add the quote-char to the token
							add_char = false;
						}
					} // else
				}
			}

			// ... if the delimiter isn't preserved
			if (false == delimiters.empty() && false == escaped &&
				false == quoted)
			{
				// if a delimiter is provided and the char isn't protected by
				// quote or escape char
				if (std::string::npos != delimiters.find_first_of(ch))
				{
					// if ch is a delimiter and the token std::string isn't empty
					// the token is complete
					if (false == token.empty()) // BUGFIX: 2006-03-04
					{
						token_complete = true;
					}

					// don't add the delimiter to the token
					add_char = false;
				}
			}

			// ... if the delimiter is preserved - add it as a token
			bool add_delimiter = false;
			if (false == delimiters_preserve.empty() && false == escaped &&
				false == quoted)
			{
				// if a delimiter which will be preserved is provided and the
				// char isn't protected by quote or escape char
				if (std::string::npos != delimiters_preserve.find_first_of(ch))
				{
					// if ch is a delimiter and the token std::string isn't empty
					// the token is complete
					if (false == token.empty()) // BUGFIX: 2006-03-04
					{
						token_complete = true;
					}

					// don't add the delimiter to the token
					add_char = false;

					// add the delimiter
					delimiter = ch;
					add_delimiter = true;
				}
			}


			// add the character to the token
			if (true == add_char)
			{
				// add the current char
				token.push_back(ch);
			}

			// add the token if it is complete
			if (true == token_complete && false == token.empty())
			{
				// add the token std::string
				result.push_back(token);

				// clear the contents
				token.clear();

				// build the next token
				token_complete = false;
			}

			// add the delimiter
			if (true == add_delimiter)
			{
				// the next token is the delimiter
				std::string delim_token;
				delim_token.push_back(delimiter);
				result.push_back(delim_token);

				// REMOVED: 2006-03-04, Bugfix
			}

			// repeat for the next character
			++pos;
		} // while

		  // add the final token
		if (false == token.empty())
		{
			result.push_back(token);
		}
	}

	char*   strncat(char* dest, const char* src, UINT32 destlen)
	{
		return ::strncat(dest,src,destlen); 
	}

	void  FastMemmove(void * dst , const void * src , INT64 unSize)
	{
		char * pDst = (char *)dst;
		char * pSrc = (char *)src;

		if ((pSrc < pDst && pSrc + unSize >= pDst) || (pSrc > pDst) && pDst + unSize >= pSrc )
		{
			memmove(dst , src , unSize);
		}
		else
		{
			memcpy(dst , src , unSize);
		} 
	} 

	INT64  ParseHumanSizeFromString(const std::string strSize)
	{  
		INT64 llFactor = 1;
		switch(strSize[strSize.length() - 1])
		{
		case 'G':
		case 'g':
			llFactor = 1024*1024*1024;
			break;
		case 'M':
		case 'm':
			llFactor = 1024*1024;
			break;
		case 'k':
		case 'K':
			llFactor = 1024;
			break;
		}  

		return  atoull(strSize.c_str()) * llFactor;  //5 这里遇到字符串会自动截取掉后面的.
	}

	INT64  ParseHumanSizeFromJson(const Json::Value & objValue)
	{
		if(objValue.isInt() || objValue.isUInt() || objValue.isNumeric())
			return objValue.asInt();
		
		if(objValue.isString())
		{
			return ParseHumanSizeFromString(objValue.asString());
		}

		gErrorStream (" invalid value:" << objValue.toStyledString());
		
		return -1;
	}

	UINT64 CityHash(const void* pBuf,INT64 len,INT64 seed)
	{
		return (INT64)CityHash64WithSeed((const char*)pBuf,len,seed); 
	}

	UINT32 Compress(const char * pBuf , UINT32 unLength , std::string & strUncompressed)
	{
		return (UINT32)snappy::Compress(pBuf,unLength,&strUncompressed);
	}

	bool Uncompress(const char * pBuf , UINT32 unLength , std::string & strUncompressed)
	{
		return snappy::Uncompress(pBuf , unLength , &strUncompressed);
	}

	INT64 strtoll(const char* str, char** endptr, INT32 base)
	{
 		return _strtoui64(str, endptr, base);
	}

	UINT64 strtoull(const char* str, char** endptr, INT32 base)
	{
		return _strtoui64(str , endptr , base); 
	}

	INT64 BKDRHash(const char* pBuf)
	{
		unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
		unsigned int hash = 0;

		while (*pBuf)
		{
			hash = hash * seed + (*pBuf++);
		}

		return (hash & 0x7FFFFFFF); 
	}

	INT64 BKDRHashSum(const char* pBuf)
	{
		INT64 sum = 0;
		unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
		unsigned int hash = 0;

		while (*pBuf)
		{
			sum += *pBuf;
			hash = hash * seed + (*pBuf++);
		}

		return (sum << 32) + (hash & 0x7FFFFFFF); 
	}

	bool	Base64EncodeBySSL(std::string & strDst, const std::string & strSrc)
	{
		return false;
	}

	bool	Base64DecodeBySSL(std::string & strDst, const std::string & strSrc)
	{
		char * pDst = NULL;
		bool bFlag[2] = { false , true };
		for (int i = 0;i < 2;++i)
		{
			BIO * b64 = BIO_new(BIO_f_base64());
			if (bFlag[i])
			{
				BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
			}

			BIO * pMemery = BIO_new_mem_buf((void*)strSrc.c_str(), (INT32)strSrc.size());
			pMemery = BIO_push(b64, pMemery);
			size_t lExpireSize = BIO_read(pMemery , (void*)pDst , (INT32)(strSrc.size()));
			BIO_free_all(pMemery);
			if (lExpireSize > 0)
			{
				strDst.resize(lExpireSize);
				strDst.copy(pDst , lExpireSize);
				return true;
			}
		}

		return false;
	}

}