#ifndef __cutil_h__
#define __cutil_h__
#include "CUtil/inc/Common.h"

namespace Json
{
	class Value;
}
 
namespace CUtil
{ 
#ifndef __min
#define __min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#ifndef __max
#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

	extern CErrno  DLL_EXPORT  Init(std::string strLogFile = "CUtil");
	extern CErrno  DLL_EXPORT  Cleanup( void );


	///////////////////////////////通用函数///////////////////////////////////////////

	extern INT32  DLL_EXPORT  strcmp(const char* str1,const char* str2);
	extern INT32  DLL_EXPORT  stricmp(const char* str1,const char* str2);  //5 不区分大小写
	extern INT64  DLL_EXPORT  atoi(const char* str);
	extern INT64  DLL_EXPORT  atoi(const std::string & str);
	extern UINT64 DLL_EXPORT	atoull(const char* str);
	extern INT64	DLL_EXPORT  strtoll(const char* str, char** endptr = NULL, INT32 base = 10);
	extern UINT64	DLL_EXPORT strtoull(const char* str, char** endptr = NULL, INT32 base = 10);
	extern double DLL_EXPORT  atof(const char* str);
	extern double DLL_EXPORT  atof(const std::string & str);
	extern void   DLL_EXPORT  itoa(char* str,INT64 val);
	extern std::string  DLL_EXPORT  itoa(INT64 val); 
	extern void   DLL_EXPORT  ftoa(char* str,double val);
	extern bool  DLL_EXPORT  floatcmp(float value1,float value2);    //5 超过-e6的就不能这么比较了.
	extern char   DLL_EXPORT* strncpy(char* dst,size_t len,const char* src);
	extern INT64  DLL_EXPORT  rdtsc();
	extern INT64  DLL_EXPORT  random();
	extern INT32  DLL_EXPORT  random(INT32 start,INT32 end);
	extern float  DLL_EXPORT  random(float start,float end);
	extern std::string   DLL_EXPORT strupper(const char* szBuf);
	extern std::string   DLL_EXPORT strlower(const char* szBuf); 
	extern BOOL	  DLL_EXPORT  is_between(float start,float end,float value);
	extern BOOL	  DLL_EXPORT  is_between(INT64 start,INT64 end,INT64 value);
	extern BOOL	  DLL_EXPORT  is_between(INT32 start,INT32 end,INT32 value);
	extern float  DLL_EXPORT  clamp(float ho,float lo,float value);
	extern INT32  DLL_EXPORT  clamp(INT32 ho,INT32 lo,INT32 value);
	extern BOOL	  DLL_EXPORT  strtobool(const char* text);
	extern BOOL	  DLL_EXPORT  isNumber(const char* text);
	extern void   DLL_EXPORT  tohex(INT64 val,char* obuf,UINT32 len);
	extern void   DLL_EXPORT  tohex(const char* ibuf,UINT32 ilen,char* obuf,UINT32& olen);
	extern void	  DLL_EXPORT  create_token(char* buf,UINT32 len);
	extern void	  DLL_EXPORT	tokenize(const std::string& str, std::vector<std::string>& result,const std::string& delimiters, const std::string& delimiters_preserve = "",const std::string& quote = "\"", const std::string& esc = "\\");
	extern char   DLL_EXPORT* strncat(char* dest,const char* src,UINT32 destlen); 
	extern void   DLL_EXPORT  FastMemmove(void * dst , const void * src , INT64 unSize);
	extern INT64  DLL_EXPORT  ParseHumanSizeFromString(const std::string strSize);
	extern INT64  DLL_EXPORT  ParseHumanSizeFromJson(const Json::Value & value);
	extern UINT64 DLL_EXPORT  CityHash(const void* pBuf,INT64 len,INT64 seed); 
	extern INT64  DLL_EXPORT  BKDRHash(const char* pBuf);
	extern INT64  DLL_EXPORT  BKDRHashSum(const char* pBuf);
	extern UINT32 DLL_EXPORT  Compress(const char * pBuf , UINT32 unLength , std::string & strUncompressed);
	extern bool   DLL_EXPORT  Uncompress(const char * pBuf , UINT32 unLength , std::string & strUncompressed);
	extern bool	  DLL_EXPORT  Base64EncodeBySSL(std::string & strDst , const std::string & strSrc);
	extern bool	  DLL_EXPORT  Base64DecodeBySSL(std::string & strDst , const std::string & strSrc);

	extern void   DLL_EXPORT GenerateUUIDBySys( char* buf );
 	extern INT64  DLL_EXPORT GenerateUUIDByTime( void );
}

#endif
