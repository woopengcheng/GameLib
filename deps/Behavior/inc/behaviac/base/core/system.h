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

////////////////////////////////////////////////////////////////////////////////
//
// Description: System related services
//
////////////////////////////////////////////////////////////////////////////////
#ifndef BEHAVIAC_BASE_CORE_SYSTEM_H
#define BEHAVIAC_BASE_CORE_SYSTEM_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"

#define BEHAVIAC_HW_MEM_ALIGN								16

#define BEHAVIAC_SHADOW_SIZE_SPIN_LOCK						24
#define BEHAVIAC_SHADOW_SIZE_ADAPTIVE_LOCK					40

#define BEHAVIAC_CFG_THREADNAME_MAXLENGTH                   64
#define BEHAVIAC_CFG_FILENAME_MAXLENGTH                     255
#define BEHAVIAC_CFG_CLASSNAME_MAXLENGTH                    200
#define BEHAVIAC_CFG_SETEXENAME_BUF_SIZE					256

#if BEHAVIAC_COMPILER_MSVC
#else
#include <pthread.h>
#endif//BEHAVIAC_COMPILER_MSVC

namespace behaviac
{
#if BEHAVIAC_COMPILER_MSVC
    typedef uint32_t THREAD_ID_TYPE;
#elif BEHAVIAC_COMPILER_APPLE || BEHAVIAC_COMPILER_ANDROID || BEHAVIAC_COMPILER_GCC_LINUX
    typedef pthread_t* THREAD_ID_TYPE;
#else
    typedef __pthread_t* THREAD_ID_TYPE;
#endif//BEHAVIAC_COMPILER_MSVC
    // Invalid thread Id definition
#define BEHAVIAC_INVALID_TID        0xFFFFFFFF

    /// This is the Low Level System function set

    /// Sleep for a number of milliseconds.
    /// \param timeMS time in milliseconds
    BEHAVIAC_API void Sleep(uint32_t timeMS);

    /// Yield the current thread execution to another.
    BEHAVIAC_API void YieldCPU();

    /// Check if the given thread ID is valid.
    BEHAVIAC_API bool IsValidThread(uint32_t threadID);

    /// Get the current thread ID
    BEHAVIAC_API THREAD_ID_TYPE GetTID();

    /// Get the current module ID
    BEHAVIAC_API Address GetModuleID();

    /// Get the name of the executable.
    /// \param exeNameBuffer pointer to a buffer in which to store the executable name
    /// \param exeNameBufferSize size of the buffer in which the data will be stored
    BEHAVIAC_API const Char* GetExeName(Char* exeNameBuffer, uint32_t exeNameBufferSize);

    /// Set the name of the executable.
    /// \param exeNameBuffer pointer to the user provided buffer containing the exe name.
    BEHAVIAC_API void SetExeName(const Char* exeNameBuffer);

    /// Test a memory pointer for read access
    /// \param ptr the pointer to test
    /// \param memSize size of the memory pointed on by the pointer, in bytes (4 for an int, for example)
    /// Note that if the return value is true, your program should make a critical error and stop running,
    /// since behavior is undefined. for a discussion about this, see http://blogs.msdn.com/oldnewthing/archive/2006/09/27/773741.aspx
    BEHAVIAC_API bool IsBadReadPointer(void* ptr, uint32_t memSize);

    /// Test a memory pointer for write access
    /// \param ptr the pointer to test
    /// \param memSize size of the memory pointed on by the pointer, in bytes (4 for an int, for example)
    /// Note that if the return value is true, your program should make a critical error and stop running,
    /// since behavior is undefined. for a discussion about this, see http://blogs.msdn.com/oldnewthing/archive/2006/09/27/773741.aspx
    BEHAVIAC_API bool IsBadWritePointer(void* ptr, uint32_t memSize);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                      behaviac Zero-initializer templates
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // This template class initializes any type to 0 (ptrs, POD, classes, ...)
    // In the case of classes, the Default Constructor is called after zeroing the class members, so
    // the default initialization of the members is still performed.
    // Usage: TYPE variable = ZeroType<TYPE>();
    /// The ZeroType template initializes any type to 0 (ptrs, POD, classes, ...)
    template <typename TYPE>
    class ZeroType
    {
    public:
        ZeroType()
        {
            memset(shadow, 0, sizeof(TYPE));
            var = new(shadow)TYPE;
        }
        ZeroType(const ZeroType& source)
        {
            memcpy(shadow, source.shadow, sizeof(TYPE));
            var = unaliased_cast<TYPE*>(shadow);
        }
        operator TYPE& ()
        {
            return *var;
        }
        TYPE& operator= (const TYPE& source)
        {
            *var = source;
            return *var;
        }
    private:
        uint8_t shadow[sizeof(TYPE)];
        TYPE* var;
    };
}

#endif  // #ifndef BEHAVIAC_BASE_CORE_SYSTEM_H
