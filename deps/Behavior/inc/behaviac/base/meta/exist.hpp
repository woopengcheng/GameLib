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

#ifndef _BEHAVIAC_BASE_META_DETAILS_EXIST_HPP__INCLUDED
#define _BEHAVIAC_BASE_META_DETAILS_EXIST_HPP__INCLUDED

#include "behaviac/base/meta/collection.h"

namespace behaviac
{
    namespace Meta
    {
        namespace Details
        {
            template< typename LookingFor, typename HeadType, typename TailType >
            struct ExistImpl
            {
                enum
                {
                    Result = ExistImpl< LookingFor, typename TailType::Head, typename TailType::Tail >::Result
                };
            };

            template< typename LookingFor, typename TailType >
            struct ExistImpl< LookingFor, LookingFor, TailType >
            {
                enum
                {
                    Result = 1
                };
            };

            template< typename LookingFor, typename HeadType >
            struct ExistImpl< LookingFor, HeadType, Meta::Null >
            {
                enum
                {
                    Result = 0
                };
            };

            template< typename LookingFor >
            struct ExistImpl< LookingFor, LookingFor, Meta::Null >
            {
                enum
                {
                    Result = 1
                };
            };
        }

        template< typename LookingFor, typename VectorType >
        struct Exist
        {
            typedef typename VectorType::Type SequenceType;
            enum
            {
                Result = Details::ExistImpl< LookingFor, typename SequenceType::Head, typename SequenceType::Tail >::Result
            };
        };

        template< typename LookingFor >
        struct Exist< LookingFor, Meta::Null >
        {
            enum
            {
                Result = 0
            };
        };
    }
}

#endif
