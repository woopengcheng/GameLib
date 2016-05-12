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

#ifndef _BEHAVIAC_BASE_META_DETAILS_ISINSTANCEOF_HPP__INCLUDED
#define _BEHAVIAC_BASE_META_DETAILS_ISINSTANCEOF_HPP__INCLUDED

namespace behaviac
{
    namespace Meta
    {
        namespace Details
        {
            template< typename BaseType, typename DerivedType >
            struct IsInstanceOfImpl
            {
                static Yes Test(BaseType);
                static No Test(...);
                static DerivedType Make();

                enum
                {
                    Result = sizeof(Test(Make())) == sizeof(Yes)
                };
            };
        }
    }
}

#endif
