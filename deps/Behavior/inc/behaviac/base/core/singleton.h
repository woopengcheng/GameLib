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

#ifndef BEHAVIAC_BASE_CORE_SINGLETON_H
#define BEHAVIAC_BASE_CORE_SINGLETON_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"

#include "behaviac/base/core/memory/memory.h"

/*
class CSequenceManager
{
BEHAVIAC_DELCARE_SINGLETON(CSequenceManager);
//...
};

in cpp:
BEHAVIAC_IMPLEMNT_SINGLETON(CSequenceManager);
*/

#define BEHAVIAC_DELCARE_SINGLETON(T)									\
    protected:															\
    static T* ms_singleton;												\
    static T* GetInstance_();											\
    static void SetInstance_(T*);										\
    T(const T&);														\
    const T& operator=(const T&);										\
    public:																\
    static inline T* GetInstance()										\
    {																	\
        if (!GetInstance_())											\
        {																\
            CreateInstance();											\
        }																\
        \
        return GetInstance_();											\
    }																	\
    \
    static T* CreateInstance()											\
    {																	\
        if (!GetInstance_())											\
        {																\
            T* p = BEHAVIAC_NEW T;										\
            SetInstance_(p);											\
        }																\
        \
        return GetInstance_();											\
    }																	\
    \
    static void DestroyInstance()										\
    {																	\
        T* p = GetInstance_();											\
        if (p)															\
        {																\
            BEHAVIAC_DELETE(p);											\
            SetInstance_(0);											\
        }																\
    }

#define BEHAVIAC_IMPLEMNT_SINGLETON(T)				\
    T* T::ms_singleton = 0;							\
    T* T::GetInstance_()							\
    {												\
        return ms_singleton;						\
    }												\
    void T::SetInstance_(T* p)						\
    {												\
        ms_singleton = p;							\
    }

#endif//BEHAVIAC_BASE_CORE_SINGLETON_H
