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

#ifndef BEHAVIAC_MEMORY_H
#define BEHAVIAC_MEMORY_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"

#if BEHAVIAC_COMPILER_APPLE
//#include <sys/malloc.h>
#else
#include <malloc.h>
#endif//BEHAVIAC_COMPILER_APPLE

#include <memory.h>
//#include <string.h>
#include <stdlib.h>

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/thread/mutex.h"
#include "behaviac/base/core/memory/memalloc.h"

#include "behaviac/base/core/memory/mempool.h"
#include "behaviac/base/core/memory/mempoollinked.h"

/// http://msdn.microsoft.com/en-us/library/x98tx3cf.aspx
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

//_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//_CrtDumpMemoryLeaks();

#if _MSC_VER >= 1400
#include <crtdbg.h>
#endif//#if _MSC_VER >= 1400

namespace behaviac
{
    BEHAVIAC_API IMemAllocator& GetMemoryAllocator();

    /**
    you can call this to override/customize the memory allocator
    */
    BEHAVIAC_API void SetMemoryAllocator(IMemAllocator& allocator);
}

#define BEHAVIAC_MALLOC_WITHTAG(size, tag)							BEHAVIAC_ALLOCATOR_MALLOC_WITHTAG(&behaviac::GetMemoryAllocator(), size, tag)
#define BEHAVIAC_REALLOC_WITHTAG(ptr, size, tag)					BEHAVIAC_ALLOCATOR_REALLOC_WITHTAG(&behaviac::GetMemoryAllocator(), ((void*)ptr), size, tag)
#define BEHAVIAC_FREE_WITHTAG(ptr, tag)								BEHAVIAC_ALLOCATOR_FREE_WITHTAG(&behaviac::GetMemoryAllocator(), ((void*)ptr), tag)

#define BEHAVIAC_MALLOC(size)										BEHAVIAC_MALLOC_WITHTAG(size, "behaviac")
#define BEHAVIAC_REALLOC(ptr, size)									BEHAVIAC_REALLOC_WITHTAG(((void*)ptr), size, "behaviac")
#define BEHAVIAC_FREE(ptr)											BEHAVIAC_FREE_WITHTAG(((void*)ptr), "behaviac")

#define BEHAVIAC_MALLOCALIGNED_WITHTAG(size, alignment, tag)		BEHAVIAC_ALLOCATOR_MALLOCALIGNED_WITHTAG(&behaviac::GetMemoryAllocator(), size, alignment, tag)
#define BEHAVIAC_REALLOCALIGNED_WITHTAG(ptr, size, alignment, tag)	BEHAVIAC_ALLOCATOR_REALLOCALIGNED_WITHTAG(&behaviac::GetMemoryAllocator(), ptr, size, alignment, tag)
#define BEHAVIAC_FREEALIGNED_WITHTAG(ptr, alignment, tag)			BEHAVIAC_ALLOCATOR_FREEALIGNED_WITHTAG(&behaviac::GetMemoryAllocator(), ptr, alignment, tag)

#define BEHAVIAC_MALLOCALIGNED(size, alignment)						BEHAVIAC_MALLOCALIGNED_WITHTAG(size, alignment, "behaviac")
#define BEHAVIAC_REALLOCALIGNED(ptr, size, alignment)				BEHAVIAC_REALLOCALIGNED_WITHTAG(ptr, size, alignment, "behaviac")
#define BEHAVIAC_FREEALIGNED(ptr, alignment)						BEHAVIAC_FREEALIGNED_WITHTAG(ptr, alignment, "behaviac")

#if BEHAVIAC_COMPILER_MSVC
/// Allocates memory on the stack
#define BEHAVIAC_ALLOCA(s)  _alloca(s)
#else
/// Allocates memory on the stack
#define BEHAVIAC_ALLOCA(s)  alloca(s)
#endif

namespace behaviac
{
    template<typename T>
    behaviac::IMemAllocator* GetAllocator()
    {
        behaviac::IMemAllocator* pAllocator = &behaviac::GetMemoryAllocator();
        return pAllocator;
    }
}

#include "memory.inl"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BEHAVIAC_G_* handles alignment
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BEHAVIAC_G_NEW_WITHTAG(T, tag)							new(behaviac::Private::MemHelperAlloc(behaviac::GetAllocator<T>(), sizeof(T), BEHAVIAC_ALIGNOF(T), tag, __FILE__, __LINE__)) T
#define BEHAVIAC_G_DELETE_WITHTAG(object, tag)					behaviac::Private::MemHelperDelete(object, tag, __FILE__, __LINE__)
#define BEHAVIAC_G_NEW_ARRAY_WITHTAG(T, count, tag)				behaviac::Private::MemHelperAllocArray<T>(behaviac::GetAllocator<T>(), count, tag, __FILE__, __LINE__)
#define BEHAVIAC_G_DELETE_ARRAY_WITHTAG(pArray, tag)			behaviac::Private::MemHelperDeleteArray(pArray, tag, __FILE__, __LINE__)

#define BEHAVIAC_G_NEW(T)										BEHAVIAC_G_NEW_WITHTAG(T, 0)
#define BEHAVIAC_G_DELETE(object)								BEHAVIAC_G_DELETE_WITHTAG(object, 0)
#define BEHAVIAC_G_NEW_ARRAY(T, count)							BEHAVIAC_G_NEW_ARRAY_WITHTAG(T, count, 0)
#define BEHAVIAC_G_DELETE_ARRAY(pArray)							BEHAVIAC_G_DELETE_ARRAY_WITHTAG(pArray, 0)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BEHAVIAC_DECLARE_MEMORY_OPERATORS(CLASS) \
    BEHAVIAC_DECLARE_MEMORY_OPERATORS_(CLASS, BEHAVIAC_ALIGNOF(CLASS))

#define BEHAVIAC_DECLARE_MEMORY_OPERATORS_AGENT(CLASS) \
    BEHAVIAC_DECLARE_MEMORY_OPERATORS_AGENT_(CLASS, BEHAVIAC_ALIGNOF(CLASS))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
BEHAVIAC_G_* is more reliable and flexible, but, the following macros' syntax is more similiar to the system's new/delete

generally, the following macros should work together with BEHAVIAC_DECLARE_MEMORY_OPERATORS.

BEHAVIAC_NEW	type
BEHAVIAC_DELETE	type

if the 'type' doesn't include 'BEHAVIAC_DECLARE_MEMORY_OPERATORS', it has the following risks:

BEHAVIAC_DELETE calls behaviac::Private::MemHelperDeleteAlignment to handle alignment
while BEHAVIAC_NEW doesn't handle alignment(alignenmt is always 4).

so if type's alignment > 4, it will assert-fail and crash in _aligned_free(was allocated by malloc instead of _aligned_malloc)
please use BEHAVIAC_G_NEW/BEHAVIAC_G_DELETE instead or to include BEHAVIAC_DECLARE_MEMORY_OPERATORS

the other risk is BEHAVIAC_NEW uses behaviac::GetMemoryAllocator() as the allocator
while BEHAVIAC_DELETE uses behaviac::GetAllocator<T>() as the allocator,
so if these two allocators are not the same, it causes problems.
please use BEHAVIAC_G_NEW/BEHAVIAC_G_DELETE instead or to include BEHAVIAC_DECLARE_MEMORY_OPERATORS

also, BEHAVIAC_NEW_ARRAY/BEHAVIAC_DELETE_ARRAY MUST work together with BEHAVIAC_DECLARE_MEMORY_OPERATORS,
otherwise, it will assert-fail. this is when you need to new/delete an array,
either you should use BEHAVIAC_G_NEW_ARRAY/BEHAVIAC_G_DELETE_ARRAY or you should include BEHAVIAC_DECLARE_MEMORY_OPERATORS int the type.
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BEHAVIAC_NEW					new(behaviac::STagOperatorNewType::GetInstance(), "behaviac", __FILE__, __LINE__)
#define BEHAVIAC_DELETE					behaviac::STagOperatorNewType::GetInstance() ^

#define BEHAVIAC_NEW_ARRAY				new(behaviac::STagOperatorNewArrayType::GetInstance(), "behaviac", __FILE__, __LINE__)
#define BEHAVIAC_DELETE_ARRAY			behaviac::STagOperatorNewArrayType::GetInstance() ^

/*! @} */
/*! @} */

#endif//BEHAVIAC_MEMORY_H
