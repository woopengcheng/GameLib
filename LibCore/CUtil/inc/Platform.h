#ifndef __platform_h__
#define __platform_h__ 


#ifndef WIN32  
#include <uuid/uuid.h>
#include <unistd.h>
#else
//#include <ObjBase.h>
//#include <WinSock.h>
//#include <Windows.h>
#include <direct.h>
#endif

#ifdef WIN32
#pragma warning(disable : 4710)
#endif
 

#ifdef WIN32
typedef __int64   INT64;
typedef unsigned __int64    UINT64;
typedef char				SINT8;
typedef unsigned char       UINT8;
#else
typedef unsigned char       UINT8;
typedef char                SINT8;   //5 win32  下居然有了
typedef long long INT64;
typedef unsigned long long UINT64;
#endif


#ifdef WIN32   
// 
// #undef std_unordered_map
// #undef std_unordered_set
#define std_unordered_map std::tr1::unordered_map
#define std_unordered_set std::tr1::unordered_set
#define std_unordered_multimap std::tr1::unordered_multimap

// #define isinff(x) (_finite(x) == 0)
// #define isnanf(x) (_isnan(x))
// #define snprintf _snprintf

#else
#define std_unordered_map std::unordered_map
#define std_unordered_set std::unordered_set
#define std_unordered_multimap std::unordered_multimap 
#endif

#ifdef WIN32
#define STRCPY_SAFE strcpy_s
#else
#define STRCPY_SAFE strcpy
#endif

#ifdef DLL_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#else
#define DLL_EXPORT __declspec(dllimport)
#define DLL_IMPORT __declspec(dllimport)
#endif



#endif