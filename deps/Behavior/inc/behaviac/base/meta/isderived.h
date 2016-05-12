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

#ifndef _BEHAVIAC_BASE_META_ISDERIVED_H_INCLUDED
#define _BEHAVIAC_BASE_META_ISDERIVED_H_INCLUDED

namespace behaviac
{
    namespace Meta
    {
        namespace Private
        {
            template <typename BASE, typename DERIVED>
            struct TypeResolver
            {
                template <typename TYPE>
                static Yes ResolvedType(DERIVED const volatile*, TYPE);
                static No  ResolvedType(BASE const volatile*, int);
            };

            template<typename BASE, typename DERIVED>
            struct IsDerivedInternal
            {
                struct TypeCaster
                {
                    operator BASE const volatile* () const;
                    operator DERIVED const volatile* ();
                };

                static const bool Result = (sizeof(TypeResolver<BASE, DERIVED>::ResolvedType(TypeCaster(), 0)) == sizeof(Yes));
            };
        }

        /// Lets you determine if a type derives from another one at compile time.
        /*! IsDerived<A, B>::Result will be true if type B inherits from type A.
         */
        template<typename BASE, typename DERIVED>
        struct IsDerived : public BooleanType<Private::IsDerivedInternal<BASE, DERIVED>::Result>
        {
        };
    }//namespace Meta
}//namespace behaviac

#endif//_BEHAVIAC_BASE_META_ISDERIVED_H_INCLUDED
