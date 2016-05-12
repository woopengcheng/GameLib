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

#ifndef _BEHAVIAC_BASE_META_META_H_INCLUDED
#define _BEHAVIAC_BASE_META_META_H_INCLUDED

#include "behaviac/base/base.h"
#include "behaviac/base/core/assert_t.h"
#include "behaviac/base/core/staticassert.h"

namespace behaviac
{
    // Meta programming and Traits tools

    namespace Meta
    {
        // Empty Class tag
        struct Empty
        {
            typedef Empty Type;
        };

        // Null Class behaviac
        struct Null
        {
            typedef Null Type;
        };

        template< typename T = Null >
        struct Identity
        {
            typedef T Type;
        };

        typedef uint8_t Yes;
        typedef struct
        {
            uint8_t padding[2];
        } No;

        struct TrueType
        {
            enum { Result = true };
        };

        struct FalseType
        {
            enum { Result = false };
        };

        template <bool val>
        struct BooleanType : public FalseType
        {};

        template <>
        struct BooleanType<true> : public TrueType
        {};
    }
}

#endif
