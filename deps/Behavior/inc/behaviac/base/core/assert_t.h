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

#ifndef BEHAVIAC_BASE_ASSERT_H
#define BEHAVIAC_BASE_ASSERT_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/compiler.h"
#include "behaviac/base/core/types.h"

//#include <assert.h>
//_CRTDBG_MAP_ALLOC predefined in the project files
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#if _MSC_VER
	#include <crtdbg.h>
#else
	#include <assert.h>
	#define _ASSERT(exp) assert(exp)
#endif//_MSC_VER

#if (defined(_DEBUG) || defined(DEBUG))
#define BEHAVIAC_ENABLE_ASSERTS	1
#endif//#ifdef _DEBUG

#if	BEHAVIAC_ENABLE_ASSERTS
namespace behaviac
{
    BEHAVIAC_API bool IsAssertEnabled();
}//namespace behaviac

// notice: do not remove zz_ at the beginning of doAssert. It's to put static variables at the end of CW debugger watches.
#define _BEHAVIAC_ASSERT_GROUP_MESSAGE_(exp, message) \
    do { \
        static bool zz_doAssert = true; \
        if (::behaviac::IsAssertEnabled() && zz_doAssert) \
        { \
            bool eval=!(exp); \
            if (eval) \
            { \
                /*assert(0);*/\
                /*_ASSERT_EXPR(0, _CRT_WIDE(message));*/\
                _ASSERT(0);\
            } \
        } \
    } while ( false )

#define BEHAVIAC_ASSERT_GROUP_MESSAGE(exp, ...) _BEHAVIAC_ASSERT_GROUP_MESSAGE_(exp, FormatString(__VA_ARGS__))

#define BEHAVIAC_DEBUGCODE(code) code
#define BEHAVIAC_VERIFYCODE(code) \
    { \
        bool __TAGVERIFYCODE_testValue = code ? true : false; \
        BEHAVIAC_ASSERT(__TAGVERIFYCODE_testValue); \
    }

#else // #ifdef BEHAVIAC_ENABLE_ASSERTS
#define BEHAVIAC_ASSERT_GROUP_MESSAGE(exp, ...)
#define BEHAVIAC_DEBUGCODE(code) void(0)
#define BEHAVIAC_VERIFYCODE(code) code
#endif // #ifdef BEHAVIAC_ENABLE_ASSERTS

#define BEHAVIAC_ASSERT(exp, ...) BEHAVIAC_ASSERT_GROUP_MESSAGE( exp, ##__VA_ARGS__ )

#endif//BEHAVIAC_BASE_ASSERT_H
