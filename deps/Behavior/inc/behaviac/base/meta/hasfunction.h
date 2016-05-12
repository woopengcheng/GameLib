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

#ifndef _BEHAVIAC_BASE_META_HASFUNCTION_H_INCLUDED
#define _BEHAVIAC_BASE_META_HASFUNCTION_H_INCLUDED

#include "behaviac/base/base.h"
#include "behaviac/base/core/assert_t.h"
#include "behaviac/base/core/staticassert.h"

#include "behaviac/base/meta/meta.h"

namespace behaviac
{
    namespace Meta
    {
		template<typename Type>
		struct HasToString
		{
		private:

			template<typename U, behaviac::string(U::*)() const> struct SFINAE {};

			template< typename U >
			static Yes Tester(SFINAE<U, &U::ToString>*);

			template<typename U>
			static No Tester(...);

		public:

			enum
			{
				Result = sizeof(Tester<Type>(0)) == sizeof(Yes)
			};
		};

        template<typename Type>
        struct HasFromString
        {
        private:

            template<typename U, bool (U::*)(const char*)> struct SFINAE {};

            template< typename U >
            static Yes Tester(SFINAE<U, &U::FromString>*);

            template<typename U>
            static No Tester(...);

        public:

            enum
            {
                Result = sizeof(Tester<Type>(0)) == sizeof(Yes)
            };
        };
    }
}


#endif//_BEHAVIAC_BASE_META_HASFUNCTION_H_INCLUDED
