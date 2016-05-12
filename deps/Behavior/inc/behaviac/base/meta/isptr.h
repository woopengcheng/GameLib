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

#ifndef _BEHAVIAC_BASE_META_ISPTR_H_INCLUDED
#define _BEHAVIAC_BASE_META_ISPTR_H_INCLUDED

#include "behaviac/base/meta/meta.h"

namespace behaviac
{
    namespace Meta
    {
        // Answer is true if the provided Type is a pointer
        //
        // typedef int * IntPtr;
        //
        // IsPtr< IntPtr >::Answer == true
        // IsPtr< int >::Answer == false
        //

        template< typename Type >
        struct IsPtr
        {
            enum { Result = 0 };
        };

        template< typename Type >
        struct IsPtr< Type* >
        {
            enum { Result = 1 };
        };

        template< typename Type >
        struct IsPtr< const Type* >
        {
            enum { Result = 1 };
        };

        template< typename Type >
        struct IsPtr < Type*& >
        {
            enum { Result = 1 };
        };

        template< typename Type >
        struct IsPtr < const Type*& >
        {
            enum { Result = 1 };
        };
    }
}

#endif//_BEHAVIAC_BASE_META_ISPTR_H_INCLUDED
