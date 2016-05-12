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

#ifndef BEHAVIAC_PS_THREAD__INCLUDED
#define BEHAVIAC_PS_THREAD__INCLUDED

#include "behaviac/base/core/compiler.h"

#if BEHAVIAC_COMPILER_MSVC
typedef unsigned int THREAD_ID_TYPE;
#define __STDCALL __stdcall
#elif BEHAVIAC_COMPILER_APPLE || BEHAVIAC_COMPILER_ANDROID || BEHAVIAC_COMPILER_GCC_LINUX
#include <sys/types.h>
#include <pthread.h>
typedef pthread_t* THREAD_ID_TYPE;
#define __STDCALL
#else
typedef __pthread_t* THREAD_ID_TYPE;
#define __STDCALL
#endif//BEHAVIAC_COMPILER_MSVC

// Invalid thread Id definition
#define BEHAVIAC_INVALID_TID        0xFFFFFFFF

namespace behaviac
{
    BEHAVIAC_API THREAD_ID_TYPE GetTID();

    namespace thread
    {
        typedef void*		ThreadHandle;
        typedef unsigned int (__STDCALL ThreadFunction)(void* arg);

        BEHAVIAC_API bool IsThreadTerminated(behaviac::thread::ThreadHandle hThread);
        BEHAVIAC_API ThreadHandle CreateAndStartThread(thread::ThreadFunction* function, void* arg, size_t stackSize);
        BEHAVIAC_API void StopThread(ThreadHandle th);
    }//namespace thread
}//namespace behaviac

#endif //BEHAVIAC_PS_THREAD__INCLUDED
