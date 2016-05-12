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

#ifndef BEHAVIAC_BEHAVIAC_SYSTEM_COMPONENT_DETAILS_TYPEMAPPING_H_INCLUDED
#define BEHAVIAC_BEHAVIAC_SYSTEM_COMPONENT_DETAILS_TYPEMAPPING_H_INCLUDED

namespace behaviac
{
    namespace Meta
    {
        /// TypeMapping utilities

        /// Mainly use for type deduction in template method
        ///

        template< typename T >
        struct Mapping
        {
            typedef Mapping< T > Type;
            typedef T ValueType;
        };
    }
}

#endif
