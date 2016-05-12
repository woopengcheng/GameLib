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

#ifndef BEHAVIAC_BASE_THREAD_MUTEX_H
#define BEHAVIAC_BASE_THREAD_MUTEX_H
#include "behaviac/base/core/config.h"
#include "behaviac/base/core/compiler.h"
#include "behaviac/base/core/types.h"

#include "behaviac/base/core/assert_t.h"

namespace behaviac
{
    /// used for large wait times.
    class BEHAVIAC_API Mutex
    {
    public:
        Mutex();
        ~Mutex();

        ///Lock the section following the lock.
        void Lock();

        /// Unlock the section following the lock.
        /*! NOTE: It is a programming error to unlock a blocking lock
        twice in a row (i.e. to unlock an already unlocked blocking lock) and may
        result in a crash
        */
        void Unlock();

        ///Try to lock the section following the lock.
        /*! \return true if lock was locked successfully, false otherwise
        */
        bool TryLock();

    private:
        struct MutexImpl;
        struct MutexImpl* _impl;

        static const int kMutexShadowSize = 40;

        uint8_t        m_Shadow[kMutexShadowSize];
    };

    class BEHAVIAC_API ScopedLock
    {
        Mutex& m_mutex_;
        ScopedLock& operator=(ScopedLock& cs);
    public:
        ScopedLock(Mutex& cs) : m_mutex_(cs)
        {
            m_mutex_.Lock();
        }

        ~ScopedLock()
        {
            m_mutex_.Unlock();
        }
    };
}//namespace behaviac

#endif //BEHAVIAC_BASE_THREAD_MUTEX_H
