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

#ifndef BEHAVIAC_COMPILER_H
#define BEHAVIAC_COMPILER_H

///////////////////////////////////////////////////////////////////////////////
// Identify the C++ compiler as macro BEHAVIAC_COMPILER_XXX

#if _MSC_VER
#if !defined(_CRT_SECURE_NO_WARNINGS)
	#define _CRT_SECURE_NO_WARNINGS
#endif

#define BEHAVIAC_COMPILER_MSVC 1
#define BEHAVIAC_BIGENDIAN			1

#if _MSC_VER >= 1900
#define BEHAVIAC_COMPILER_MSVC2015 1
#define BEHAVIAC_COMPILER_NAME "vs2015"
#elif _MSC_VER >= 1800
#define BEHAVIAC_COMPILER_MSVC2013 1
#define BEHAVIAC_COMPILER_NAME "vs2013"
#elif _MSC_VER >= 1600
#define BEHAVIAC_COMPILER_MSVC2010 1
#define BEHAVIAC_COMPILER_NAME "vs2010"
#elif _MSC_VER >= 1500
#define BEHAVIAC_COMPILER_MSVC2008 1
#define BEHAVIAC_COMPILER_NAME "vs2008"
#define nullptr 0
#elif _MSC_VER >= 1400
#define BEHAVIAC_COMPILER_MSVC2005 1
#define BEHAVIAC_COMPILER_NAME "vs2005"
#define nullptr 0
#elif _MSC_VER >= 1310
#define BEHAVIAC_COMPILER_MSVC2003 1
#define BEHAVIAC_COMPILER_NAME "vs2003"
#define nullptr 0
#else
#error Requires Visual C++ 2003 or above
#endif//_MSC_VER

#if _WIN64
#define BEHAVIAC_COMPILER_64BITS	1
#endif

#elif __APPLE_CC__
#include "TargetConditionals.h"

#define BEHAVIAC_COMPILER_APPLE 1
#define BEHAVIAC_BIGENDIAN			1
#define BEHAVIAC_COMPILER_NAME "gcc-apple"

#if defined(__LP64__)
#define BEHAVIAC_COMPILER_64BITS	1
#endif

#if TARGET_OS_IPHONE
#define BEHAVIAC_COMPILER_APPLE_IPHONE	1
#endif

#elif __ANDROID__ || ANDROID
#define BEHAVIAC_COMPILER_ANDROID 1
#define BEHAVIAC_BIGENDIAN			1
#define BEHAVIAC_COMPILER_NAME "gcc-android"

#if defined(__LP64__)
#define BEHAVIAC_COMPILER_64BITS	1
#endif

#ifndef BEHAVIAC_COMPILER_ANDROID_VER
#define BEHAVIAC_COMPILER_ANDROID_VER 9
#endif

#elif __CYGWIN__
#define BEHAVIAC_COMPILER_GCC_CYGWIN 1
#define BEHAVIAC_BIGENDIAN			1
#define BEHAVIAC_COMPILER_NAME "gcc-cygwin"
#if __GNUC__ < 4 || __GNUC_MINOR < 6
#define nullptr 0
#endif

#if defined(__LP64__)
#define BEHAVIAC_COMPILER_64BITS	1
#endif

#elif defined(__linux__)
#define BEHAVIAC_BIGENDIAN			1
#define BEHAVIAC_COMPILER_GCC_LINUX 1
#define BEHAVIAC_COMPILER_NAME "gcc-linux"

#if defined(__LP64__)
#define BEHAVIAC_COMPILER_64BITS	1
#endif
#else
#error Unsupported C++ compiler

//#define BEHAVIAC_BIGENDIAN			1

#endif // _MSC_VER

#if !defined(BEHAVIAC_BIGENDIAN)
	#error please define BEHAVIAC_BIGENDIAN 
#endif

#define INTRINSIC_PARAM(type) const type&

///////////////////////////////////////////////////////////////////////////////
// Visual C++

#if BEHAVIAC_COMPILER_MSVC
/// Define the force inline compiler indicator
//#define BEHAVIAC_FORCEINLINE __forceinline
#define BEHAVIAC_FORCEINLINE __inline
#define BEHAVIAC_FORCENOINLINE _declspec(noinline)

#if _MSC_VER >= 1400
/// Define the restrict compiler indicator
#define BEHAVIAC_RESTRICT __restrict
#else
#define BEHAVIAC_RESTRICT
#endif//#if _MSC_VER >= 1400

/// Define alignment on user-defined type declaration
/// You should use both the prefix and suffix when your decide to align
/// a type to make sure the alignment is respected on any compiler that we
/// currently support (some want a prefix, while other want a suffix)
/// You can use them this way:
///     class BEHAVIAC_ALIGN_PREFIX(16) A { ... } BEHAVIAC_ALIGN_SUFFIX(16);
#define BEHAVIAC_ALIGN_PREFIX(alignment)				__declspec(align(alignment))
#define BEHAVIAC_ALIGN_SUFFIX(alignment)

#ifdef BEHAVIAC_DLL
#define BEHAVIAC_DLL_ENTRY_IMPORT						__declspec(dllimport)
#define BEHAVIAC_DLL_ENTRY_EXPORT						__declspec(dllexport)
#else
#define BEHAVIAC_DLL_ENTRY_IMPORT
#define BEHAVIAC_DLL_ENTRY_EXPORT
#endif//BEHAVIAC_DLL

#elif BEHAVIAC_COMPILER_APPLE
/// Define the force inline compiler indicator
#define BEHAVIAC_FORCEINLINE inline

/// Define the restrict compiler indicator
#define BEHAVIAC_RESTRICT __restrict__

#define BEHAVIAC_ALIGN_PREFIX(alignment)
#define BEHAVIAC_ALIGN_SUFFIX(alignment)				__attribute__ ((aligned(n)))

#ifdef BEHAVIAC_DLL
#	define BEHAVIAC_DLL_ENTRY_IMPORT					
#	define BEHAVIAC_DLL_ENTRY_EXPORT					__attribute__((visibility("default")))
#else
#	define BEHAVIAC_DLL_ENTRY_IMPORT
#	define BEHAVIAC_DLL_ENTRY_EXPORT
#endif//BEHAVIAC_DLL

#else
//GCC
#pragma GCC diagnostic ignored "-Wmissing-braces"

/// Define the force inline compiler indicator
#define BEHAVIAC_FORCEINLINE inline

/// Define the restrict compiler indicator
#define BEHAVIAC_RESTRICT __restrict__

#define BEHAVIAC_ALIGN_PREFIX(alignment)
#define BEHAVIAC_ALIGN_SUFFIX(alignment)				__attribute__ ((aligned(n)))

#ifdef BEHAVIAC_DLL
#	define BEHAVIAC_DLL_ENTRY_IMPORT					
#	define BEHAVIAC_DLL_ENTRY_EXPORT					__attribute__((visibility("default")))
#else
#	define BEHAVIAC_DLL_ENTRY_IMPORT
#	define BEHAVIAC_DLL_ENTRY_EXPORT
#endif//BEHAVIAC_DLL

#endif // BEHAVIAC_COMPILER_GCC_CYGWIN

#ifdef BEHAVIACDLL_EXPORTS
#	define BEHAVIAC_API BEHAVIAC_DLL_ENTRY_EXPORT
#else
#	define BEHAVIAC_API BEHAVIAC_DLL_ENTRY_IMPORT
#endif//BEHAVIACDLL_EXPORTS

#if BEHAVIAC_COMPILER_MSVC
	//warning C4275: non dll-interface class 'stdext::exception' used as base for dll-interface class 'std::bad_cast'
	#pragma warning(disable : 4275)

	//warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
	#pragma warning(disable : 4530)

	//warning C4251: 'behaviac::VariableRegistry::m_proxyHolders' : class 'behaviac::vector<T>' needs to have dll-interface to be used
	//by clients of class 'behaviac::VariableRegistry'
	#pragma warning(disable : 4251)

	//unreferenced formal parameter
	#pragma warning(disable : 4100)

	#pragma warning(disable : 4127) // conditional expression is constant

	//warning C4702: unreachable code
	#pragma warning(disable : 4702)

	#pragma warning(disable : 4996) //'strcpy': This function or variable may be unsafe.
	//#if BEHAVIAC_COMPILER_MSVC2015
	//	#pragma warning(disable : 4714) //__forceinline
	//#endif

#endif//BEHAVIAC_COMPILER_MSVC

#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#define BEHAVIAC_ARRAY_LENGTH(s) (sizeof(s) / sizeof(s[0]))

// declare the variable as used (to remove unused variable warning)
#define BEHAVIAC_UNUSED_VAR(var)         ((void) &var)


// A macro to mark variables or types as unused, avoiding compiler warnings.
#if BEHAVIAC_COMPILER_MSVC
# define BEHAVIAC_UNUSED
# define BEHAVIAC_ALIAS
#else
# define BEHAVIAC_UNUSED __attribute__((unused))
# define BEHAVIAC_ALIAS __attribute__((__may_alias__))
#endif


#define _BEHAVIAC_OFFSETOF_BASE_ 0x01000000
#define BEHAVIAC_OFFSETOF(TYPE, MEMBER) (size_t)((unsigned char*)(&(((TYPE*)_BEHAVIAC_OFFSETOF_BASE_)->MEMBER)) - (unsigned char*)(TYPE*)_BEHAVIAC_OFFSETOF_BASE_)

//#define BEHAVIAC_OFFSETOF_POD(TYPE, MEMBER) offsetof(TYPE, MEMBER)
#define BEHAVIAC_OFFSETOF_POD(TYPE, MEMBER) BEHAVIAC_OFFSETOF(TYPE, MEMBER)

#if BEHAVIAC_COMPILER_MSVC
#define string_cpy strcpy
#define string_ncpy strncpy
#define string_icmp _stricmp
#define string_nicmp _strnicmp
#define string_snprintf _snprintf
#define string_vnprintf vsnprintf
#define string_vnwprintf _vsnwprintf_s
#define string_sprintf(s, fmt, ...) BEHAVIAC_ASSERT(BEHAVIAC_ARRAY_LENGTH(s) > 0); _snprintf(s, BEHAVIAC_ARRAY_LENGTH(s), fmt, __VA_ARGS__); s[BEHAVIAC_ARRAY_LENGTH(s) - 1] = '\0'
#elif BEHAVIAC_COMPILER_APPLE || BEHAVIAC_COMPILER_ANDROID || BEHAVIAC_COMPILER_GCC_LINUX || BEHAVIAC_COMPILER_GCC_CYGWIN
#define string_cpy strcpy
#define string_ncpy strncpy
#define string_icmp strcasecmp
#define string_nicmp strncasecmp
#define string_snprintf snprintf
#define string_vnprintf vsnprintf
#define string_vnwprintf vswprintf
#define string_sprintf(s, fmt, ...) BEHAVIAC_ASSERT(BEHAVIAC_ARRAY_LENGTH(s) > 0); snprintf(s, BEHAVIAC_ARRAY_LENGTH(s), fmt, __VA_ARGS__); s[BEHAVIAC_ARRAY_LENGTH(s) - 1] = '\0'
#else
#define string_cpy strcpy
#define string_ncpy strncpy
#define string_icmp stricmp
#define string_nicmp strnicmp
#define string_snprintf snprintf
#define string_vnprintf vsnprintf
#define string_vnwprintf vswprintf
#define string_sprintf(s, fmt, ...) BEHAVIAC_ASSERT(BEHAVIAC_ARRAY_LENGTH(s) > 0); snprintf(s, BEHAVIAC_ARRAY_LENGTH(s), fmt, __VA_ARGS__); s[BEHAVIAC_ARRAY_LENGTH(s) - 1] = '\0'
#endif//BEHAVIAC_COMPILER_MSVC

#endif // BEHAVIAC_COMPILER_H
