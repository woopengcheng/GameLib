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

//------------------------------------------------------------------------
//------------------------------------------------------------------------

#include "behaviac/base/meta/isenum.h"
#include "behaviac/base/meta/isclass.h"
#include "behaviac/base/meta/isfunction.h"
#include "behaviac/base/meta/isinstanceof.h"
#include "behaviac/base/meta/issame.h"
#include "behaviac/base/meta/functiontraits.h"
#include "behaviac/base/meta/isconst.h"
#include "behaviac/base/meta/removeconst.h"
#include "behaviac/base/meta/removeref.h"
#include "behaviac/base/meta/removeptr.h"
#include "behaviac/base/meta/removeall.h"
#include "behaviac/base/meta/isabstract.h"

#include "test.h"

SUITE(behaviac)
{
    SUITE(Traits)
    {
        class Im_a_Class
        {
        public:
            Im_a_Class() : m_notEmpty(0)
            {
            }

        private:
            void fn_access_property()
            {
                m_notEmpty++;
            }

            int32_t m_notEmpty;
        };
        class Im_an_Empty_Class
        {
        };
        class Im_an_abstract_Class
        {
            virtual ~Im_an_abstract_Class() {}
            virtual void MyFunction() = 0;
        };
        enum Im_an_Enum
        {
            SureIs
        };
        class BaseClass
        {
        public:
        };
        class DerivedClass : public BaseClass
        {
        public:
        };
        typedef void            Im_a_Function();
        typedef Im_a_Class*     Im_a_Ptr;
        typedef Im_a_Class&     Im_a_Ref;
        typedef Im_a_Class      Im_an_Array[10];
        typedef uint8_t        Im_a_Fundamental;
        TEST(TestTrait, IsEnum)
        {
            CHECK(behaviac::Meta::IsEnum< Im_an_Enum >::Result);
        }
        TEST(TestTrait, IsNotEnum)
        {
            CHECK(!behaviac::Meta::IsEnum< Im_a_Class >::Result);
            CHECK(!behaviac::Meta::IsEnum< Im_an_Empty_Class >::Result);
            CHECK(!behaviac::Meta::IsEnum< Im_a_Function >::Result);
            CHECK(!behaviac::Meta::IsEnum< Im_a_Ptr >::Result);
            CHECK(!behaviac::Meta::IsEnum< Im_a_Ref >::Result);
            CHECK(!behaviac::Meta::IsEnum< Im_an_Array >::Result);
            CHECK(!behaviac::Meta::IsEnum< Im_a_Fundamental >::Result);
        }
        TEST(TestTrait, IsClass)
        {
            CHECK(behaviac::Meta::IsClass< Im_a_Class >::Result);
            CHECK(behaviac::Meta::IsClass< Im_an_Empty_Class >::Result);
        }
        TEST(TestTrait, IsNotClass)
        {
            CHECK(!behaviac::Meta::IsClass< Im_an_Enum >::Result);
            CHECK(!behaviac::Meta::IsClass< Im_a_Function >::Result);
            CHECK(!behaviac::Meta::IsClass< Im_a_Ptr >::Result);
            CHECK(!behaviac::Meta::IsClass< Im_a_Ref >::Result);
            CHECK(!behaviac::Meta::IsClass< Im_an_Array >::Result);
            CHECK(!behaviac::Meta::IsClass< Im_a_Fundamental >::Result);
        }
        TEST(TestTrait, IsFunction)
        {
            CHECK(behaviac::Meta::IsFunction< Im_a_Function >::Result);
        }
        TEST(TestTrait, IsNotFunction)
        {
            CHECK(!behaviac::Meta::IsFunction< Im_an_Enum >::Result);
            CHECK(!behaviac::Meta::IsFunction< Im_a_Class >::Result);
            CHECK(!behaviac::Meta::IsFunction< Im_a_Ptr >::Result);
            CHECK(!behaviac::Meta::IsFunction< Im_a_Ref >::Result);
            CHECK(!behaviac::Meta::IsFunction< Im_an_Array >::Result);
            CHECK(!behaviac::Meta::IsFunction< Im_a_Fundamental >::Result);
        }
        TEST(TestTrait, IsPtr)
        {
            CHECK(behaviac::Meta::IsPtr< Im_a_Ptr >::Result);
            CHECK(behaviac::Meta::IsPtr< const int* >::Result);
        }
        TEST(TestTrait, IsNotPtr)
        {
            CHECK(!behaviac::Meta::IsPtr< Im_a_Ref >::Result);
            CHECK(!behaviac::Meta::IsPtr< Im_an_Array >::Result);
            CHECK(!behaviac::Meta::IsPtr< Im_an_Enum >::Result);
            CHECK(!behaviac::Meta::IsPtr< Im_a_Class >::Result);
            CHECK(!behaviac::Meta::IsPtr< Im_a_Fundamental >::Result);
        }
        TEST(TestTrait, IsRef)
        {
            CHECK(behaviac::Meta::IsRef< Im_a_Ref >::Result);
            CHECK(behaviac::Meta::IsRef< const int& >::Result);
        }
        TEST(TestTrait, IsNotRef)
        {
            CHECK(!behaviac::Meta::IsRef< Im_a_Ptr >::Result);
            CHECK(!behaviac::Meta::IsRef< Im_an_Array >::Result);
            CHECK(!behaviac::Meta::IsRef< Im_an_Enum >::Result);
            CHECK(!behaviac::Meta::IsRef< Im_a_Class >::Result);
            CHECK(!behaviac::Meta::IsRef< Im_a_Fundamental >::Result);
        }
        TEST(TestTrait, IsArray)
        {
            CHECK(behaviac::Meta::IsArray< Im_an_Array >::Result);
        }
        TEST(TestTrait, IsNotArray)
        {
            CHECK(!behaviac::Meta::IsArray< Im_a_Ptr >::Result);
            CHECK(!behaviac::Meta::IsArray< Im_a_Ref >::Result);
            CHECK(!behaviac::Meta::IsArray< Im_an_Enum >::Result);
            CHECK(!behaviac::Meta::IsArray< Im_a_Class >::Result);
            CHECK(!behaviac::Meta::IsArray< Im_a_Fundamental >::Result);
        }
        TEST(TestTrait, IsCompound)
        {
            CHECK(behaviac::Meta::IsCompound< Im_a_Ptr >::Result);
            CHECK(behaviac::Meta::IsCompound< Im_a_Ref >::Result);
            CHECK(behaviac::Meta::IsCompound< Im_an_Array >::Result);
        }
        TEST(TestTrait, IsNotCompound)
        {
            CHECK(!behaviac::Meta::IsCompound< Im_an_Enum >::Result);
            CHECK(!behaviac::Meta::IsCompound< Im_a_Class >::Result);
            CHECK(!behaviac::Meta::IsCompound< Im_a_Fundamental >::Result);
        }
        TEST(TestTrait, IsFundamental)
        {
            CHECK(behaviac::Meta::IsFundamental< uint8_t >::Result);
            CHECK(behaviac::Meta::IsFundamental< uint16_t >::Result);
            CHECK(behaviac::Meta::IsFundamental< uint32_t >::Result);
            CHECK(behaviac::Meta::IsFundamental< int8_t >::Result);
            CHECK(behaviac::Meta::IsFundamental< int16_t >::Result);
            CHECK(behaviac::Meta::IsFundamental< int32_t >::Result);
            CHECK(behaviac::Meta::IsFundamental< float >::Result);
            CHECK(behaviac::Meta::IsFundamental< bool >::Result);
        }
        TEST(TestTrait, IsInstanceOf)
        {
            CHECK((behaviac::Meta::IsInstanceOf< BaseClass, DerivedClass >::Result));
            CHECK((behaviac::Meta::IsInstanceOf< BaseClass, BaseClass >::Result));
            CHECK((behaviac::Meta::IsInstanceOf< DerivedClass, DerivedClass >::Result));
        }
        TEST(TestTrait, IsNotInstanceOf)
        {
            CHECK((!behaviac::Meta::IsInstanceOf< BaseClass, int32_t >::Result));
            CHECK((!behaviac::Meta::IsInstanceOf< int32_t, DerivedClass >::Result));
            CHECK((!behaviac::Meta::IsInstanceOf< DerivedClass, BaseClass >::Result));
        }
        TEST(TestTrait, IsSame)
        {
            CHECK((!behaviac::Meta::IsSame< BaseClass, int32_t >::Result));
            CHECK((behaviac::Meta::IsSame< BaseClass, BaseClass >::Result));
        }
        TEST(TestTrait, FunctionTraitsParamCount)
        {
            CHECK(0 == behaviac::Meta::FunctionTraits< void() >::ParamCount);
            CHECK(1 == behaviac::Meta::FunctionTraits< void(int32_t) >::ParamCount);
            CHECK(2 == behaviac::Meta::FunctionTraits< void(int32_t, int32_t) >::ParamCount);
            CHECK(3 == behaviac::Meta::FunctionTraits< void(int32_t, int32_t, int32_t) >::ParamCount);
            CHECK(4 == behaviac::Meta::FunctionTraits< void(int32_t, int32_t, int32_t, int32_t) >::ParamCount);
        }
        TEST(TestTrait, FunctionTraitsParamCount_MixedCompound)
        {
            CHECK(0 == behaviac::Meta::FunctionTraits< void() >::ParamCount);
            CHECK(1 == behaviac::Meta::FunctionTraits< void(const int32_t) >::ParamCount);
            CHECK(2 == behaviac::Meta::FunctionTraits< void(const int32_t, int32_t) >::ParamCount);
            CHECK(3 == behaviac::Meta::FunctionTraits< void(const int32_t, int32_t*, int32_t) >::ParamCount);
            CHECK(4 == behaviac::Meta::FunctionTraits< void(const int32_t, int32_t*, int32_t&, int32_t) >::ParamCount);
        }
        TEST(TestTrait, FunctionTraitsParamType1)
        {
            typedef behaviac::Meta::FunctionTraits< void(int32_t) > Function;
            CHECK((behaviac::Meta::IsSame< int32_t, Function::ArgType0 >::Result));
        }
        TEST(TestTrait, FunctionTraitsParamType4)
        {
            typedef behaviac::Meta::FunctionTraits< void(int32_t, float, uint32_t, Im_a_Class) > Function;
            CHECK((behaviac::Meta::IsSame< int32_t, Function::ArgType0 >::Result));
            CHECK((behaviac::Meta::IsSame< float, Function::ArgType1 >::Result));
            CHECK((behaviac::Meta::IsSame< uint32_t, Function::ArgType2 >::Result));
            CHECK((behaviac::Meta::IsSame< Im_a_Class, Function::ArgType3 >::Result));
        }
        TEST(TestTrait, FunctionTraitsParamCount0)
        {
            typedef behaviac::Meta::FunctionTraits< void() > Function;
            CHECK(0 == Function::ParamTypeCount< uint32_t >::Result);
        }
        TEST(TestTrait, FunctionTraitsParamCount1)
        {
            typedef behaviac::Meta::FunctionTraits< void(uint32_t) > Function;
            CHECK(1 == Function::ParamTypeCount< uint32_t >::Result);
            CHECK(0 == Function::ParamTypeCount< float >::Result);
        }
        TEST(TestTrait, FunctionTraitsParamCount2)
        {
            typedef behaviac::Meta::FunctionTraits< void(uint32_t, uint32_t) > Function;
            CHECK(2 == Function::ParamTypeCount< uint32_t >::Result);
            CHECK(0 == Function::ParamTypeCount< float >::Result);
        }
        TEST(TestTrait, FunctionTraitsParamCount3)
        {
            typedef behaviac::Meta::FunctionTraits< void(uint32_t, uint32_t, uint32_t) > Function;
            CHECK(3 == Function::ParamTypeCount< uint32_t >::Result);
            CHECK(0 == Function::ParamTypeCount< float >::Result);
        }
        TEST(TestTrait, FunctionTraitsParamCount4)
        {
            typedef behaviac::Meta::FunctionTraits< void(uint32_t, uint32_t, uint32_t, uint32_t) > Function;
            CHECK(4 == Function::ParamTypeCount< uint32_t >::Result);
            CHECK(0 == Function::ParamTypeCount< float >::Result);
        }
        TEST(TestTrait, FunctionTraitsParamCount4DifferentParam)
        {
            typedef behaviac::Meta::FunctionTraits< void(uint32_t, uint32_t, int32_t, float) > Function;
            CHECK(2 == Function::ParamTypeCount< uint32_t >::Result);
            CHECK(1 == Function::ParamTypeCount< float >::Result);
            CHECK(1 == Function::ParamTypeCount< int32_t >::Result);
        }
        TEST(TestTrait, IsConst)
        {
            typedef const int ConstInt;
            CHECK((behaviac::Meta::IsConst< ConstInt >::Result));
            CHECK(!(behaviac::Meta::IsConst< int >::Result));
        }
        TEST(TestTrait, RemoveConst)
        {
            CHECK(!(behaviac::Meta::IsConst< behaviac::Meta::RemoveConst< const int >::Result >::Result));
            CHECK(!(behaviac::Meta::IsConst< behaviac::Meta::RemoveConst< const int* >::Result >::Result));
            CHECK(!(behaviac::Meta::IsConst< behaviac::Meta::RemoveConst< const int& >::Result >::Result));
        }
        TEST(TestTrait, RemoveRef)
        {
            CHECK(!(behaviac::Meta::IsRef< behaviac::Meta::RemoveRef< int& >::Result >::Result));
            CHECK(!(behaviac::Meta::IsRef< behaviac::Meta::RemoveRef< const int& >::Result >::Result));
        }
        TEST(TestTrait, RemovePtr)
        {
            CHECK(!(behaviac::Meta::IsPtr< behaviac::Meta::RemovePtr< int* >::Result >::Result));
            CHECK(!(behaviac::Meta::IsPtr< behaviac::Meta::RemovePtr< const int* >::Result >::Result));
        }
        TEST(TestTrait, RemoveAll)
        {
            CHECK((behaviac::Meta::IsSame< int, behaviac::Meta::RemoveAll< const int >::Result >::Result));
            CHECK((behaviac::Meta::IsSame< int, behaviac::Meta::RemoveAll< const int* >::Result >::Result));
            CHECK((behaviac::Meta::IsSame< int, behaviac::Meta::RemoveAll< const int& >::Result >::Result));
        }
        TEST(TestTrait, IsAbstract_return_1_if_abstract)
        {
            CHECK(behaviac::Meta::IsAbstract< Im_an_abstract_Class >::Result);
        }
        TEST(TestTrait, IsAbsctract_return_0_if_not_abstract)
        {
            CHECK(!behaviac::Meta::IsAbstract< Im_a_Class >::Result);
            CHECK(!behaviac::Meta::IsAbstract< int >::Result);
            CHECK(!behaviac::Meta::IsAbstract< int& >::Result);
            CHECK(!behaviac::Meta::IsAbstract< int* >::Result);
            CHECK(!behaviac::Meta::IsAbstract< Im_a_Function >::Result);
            CHECK(!behaviac::Meta::IsAbstract< Im_an_Enum >::Result);
        }
    }
}
