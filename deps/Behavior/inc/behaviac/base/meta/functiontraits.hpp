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

#ifndef _BEHAVIAC_BASE_META_DETAILS_FUNCTIONTRAITS_HPP__INCLUDED
#define _BEHAVIAC_BASE_META_DETAILS_FUNCTIONTRAITS_HPP__INCLUDED

#include "behaviac/base/meta/issame.h"
#include "behaviac/base/meta/removeall.h"

namespace behaviac
{
    namespace Meta
    {
        namespace Details
        {
            template< typename Function >
            struct FunctionTraits;

            template< typename Return >
            struct FunctionTraits< Return(*)(void) >
            {
                enum { ParamCount = 0 };
                typedef Return ReturnType;

                template< typename T >
                struct ParamTypeCount
                {
                    enum { Result = 0 };
                };
            };

            template< typename Return, typename Arg0 >
            struct FunctionTraits< Return(*)(Arg0) >
            {
                enum { ParamCount = 1 };
                typedef Return ReturnType;
                typedef Arg0 ArgType0;

                template< typename T >
                struct ParamTypeCount
                {
                    enum { Result = IsSame< typename RemoveAll<ArgType0>::Result, T >::Result };
                };
            };

            template< typename Return, typename Arg0, typename Arg1 >
            struct FunctionTraits< Return(*)(Arg0, Arg1) >
            {
                enum { ParamCount = 2 };
                typedef Return ReturnType;
                typedef Arg0 ArgType0;
                typedef Arg1 ArgType1;

                template< typename T >
                struct ParamTypeCount
                {
                    enum
                    {
                        Result = IsSame< typename RemoveAll<ArgType0>::Result, T >::Result +
                        IsSame< typename RemoveAll<ArgType1>::Result, T >::Result
                    };
                };
            };

            template< typename Return, typename Arg0, typename Arg1, typename Arg2 >
            struct FunctionTraits< Return(*)(Arg0, Arg1, Arg2) >
            {
                enum { ParamCount = 3 };
                typedef Return ReturnType;
                typedef Arg0 ArgType0;
                typedef Arg1 ArgType1;
                typedef Arg2 ArgType2;

                template< typename T >
                struct ParamTypeCount
                {
                    enum
                    {
                        Result = IsSame< typename RemoveAll<ArgType0>::Result, T >::Result +
                        IsSame< typename RemoveAll<ArgType1>::Result, T >::Result +
                        IsSame< typename RemoveAll<ArgType2>::Result, T >::Result
                    };
                };
            };

            template< typename Return, typename Arg0, typename Arg1, typename Arg2, typename Arg3 >
            struct FunctionTraits< Return(*)(Arg0, Arg1, Arg2, Arg3) >
            {
                enum { ParamCount = 4 };
                typedef Return ReturnType;
                typedef Arg0 ArgType0;
                typedef Arg1 ArgType1;
                typedef Arg2 ArgType2;
                typedef Arg3 ArgType3;

                template< typename T >
                struct ParamTypeCount
                {
                    enum
                    {
                        Result = IsSame< typename RemoveAll<ArgType0>::Result, T >::Result +
                        IsSame< typename RemoveAll<ArgType1>::Result, T >::Result +
                        IsSame< typename RemoveAll<ArgType2>::Result, T >::Result +
                        IsSame< typename RemoveAll<ArgType3>::Result, T >::Result
                    };
                };
            };
        }
    }
}

#endif
