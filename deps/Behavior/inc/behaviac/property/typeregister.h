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

#ifndef BEHAVIAC_PROPERTYREGISTER_H
#define BEHAVIAC_PROPERTYREGISTER_H

#include "behaviac/property/property.h"
#include "behaviac/behaviortree/nodes/conditions/condition.h"

namespace behaviac
{
    class BEHAVIAC_API TypeRegister
    {
    public:
        template<typename T>
        static bool Register(const char* typeName)
        {
			RegisterSelector<T, behaviac::Meta::IsRefType<T>::Result>::Register(typeName);
            return true;
        }

        template<typename T>
        static void UnRegister(const char* typeName)
        {
			RegisterSelector<T, behaviac::Meta::IsRefType<T>::Result>::UnRegister(typeName);
        }

	private:
		template<typename T, bool bRefType>
		struct RegisterSelector
		{
			static void Register(const char* typeName)
			{
				behaviac::Property::Register<T>(typeName);
				behaviac::Condition::Register<T>(typeName);

				const char* szVectorType = FormatString("vector<%s>", typeName);
				behaviac::Property::Register<behaviac::vector<T> >(szVectorType);
				behaviac::Condition::Register<behaviac::vector<T> >(szVectorType);
			}

			static void UnRegister(const char* typeName)
			{
				behaviac::Property::UnRegister<T>(typeName);
				behaviac::Condition::UnRegister<T>(typeName);

				const char* szVectorType = FormatString("vector<%s>", typeName);
				behaviac::Property::UnRegister<behaviac::vector<T> >(szVectorType);
				behaviac::Condition::UnRegister<behaviac::vector<T> >(szVectorType);
			}
		};

		template<typename T>
		struct RegisterSelector<T, true>
		{
			typedef REAL_BASETYPE(T) BaseType;
			typedef BaseType* PointerType;

			static void Register(const char* typeName)
			{
				RegisterSelector<PointerType, false>::Register(typeName);
			}

			static void UnRegister(const char* typeName)
			{
				RegisterSelector<PointerType, false>::UnRegister(typeName);
			}
		};

    };
}
#endif//BEHAVIAC_PROPERTYREGISTER_H