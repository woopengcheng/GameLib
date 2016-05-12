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

#ifndef BEHAVIAC_BASE_THREAD_WRAPPER_H
#define BEHAVIAC_BASE_THREAD_WRAPPER_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"
#include "behaviac/base/core/staticassert.h"
#include "behaviac/base/core/container/fixed_hash.h"
#include "behaviac/base/core/thread/mutex.h"
#include "behaviac/base/core/system.h"

#if BEHAVIAC_COMPILER_MSVC
#include <intrin.h>

#define MemoryReadBarrier	_ReadBarrier
#define MemoryWriteBarrier	_WriteBarrier
#else
#define MemoryReadBarrier
#define MemoryWriteBarrier
#endif//BEHAVIAC_COMPILER_MSVC

#include <cassert>

namespace behaviac
{
    typedef long			Atomic32;

    namespace Thread
    {
        BEHAVIAC_API void Sleep(long millis);
    }

    template<typename T>
    inline T Load_Relaxed(const T& v)
    {
        T ret = v;
        return ret;
    }

    template<typename T>
    inline T Load_Acquire(const T& v)
    {
        T ret = v;
#if _MSC_VER >= 1500
        MemoryReadBarrier();
#endif//#if _MSC_VER >= 1400
        return ret;
    }
    template<typename T>
    inline void Store_Release(T& dst, T v)
    {
#if _MSC_VER >= 1500
        MemoryWriteBarrier();
#endif//#if _MSC_VER >= 1400
        dst = v;
    }

    // Returns a new value of i
    Atomic32 AtomicInc(volatile Atomic32& i);
    Atomic32 AtomicDec(volatile Atomic32& i);

    template<typename T>
    class ScopedInt
    {
        T* m_int;
    public:
        ScopedInt(T* i) : m_int(i)
        {
            ++(*m_int);
        }

        ~ScopedInt()
        {
            --(*m_int);
        }

        bool equal(long v) const
        {
            long vThis = m_int->value();
            return vThis == v;
        }
    };

#if BEHAVIAC_COMPILER_MSVC
    class BEHAVIAC_API ThreadInt
    {
        fixed_hash<long, 256* 2>	m_threadInt;
        behaviac::Mutex			m_csMemory;
        bool					m_inited;
    public:
        ThreadInt();
        ~ThreadInt();
        long value() const;
    private:
        void Init();
        //void set(long v);
        long operator++();
        void operator--();

        friend class ScopedInt<ThreadInt>;
    };
} // behaviac

#else
    class BEHAVIAC_API ThreadInt
    {
        long					m_value;
        behaviac::Mutex			m_csMemory;
        bool					m_inited;
    public:
        ThreadInt();
        ~ThreadInt();
        long value() const;
    private:
        void Init();
        //void set(long v);
        long operator++();
        void operator--();

        friend class ScopedInt<ThreadInt>;
    };
} // behaviac

#endif

typedef behaviac::ScopedInt<behaviac::ThreadInt> ScopedInt_t;

#endif//BEHAVIAC_BASE_THREAD_WRAPPER_H
