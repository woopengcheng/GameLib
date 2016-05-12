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

#include "PropertyReadonlyAgent.h"

//#if !BEHAVIAC_COMPILER_MSVC
//const int PropertyReadonlyAgent::MemberReadonly;
//#endif

float PropertyReadonlyAgent::_s_float_member = 0.0f;

PropertyReadonlyAgent::PropertyReadonlyAgent() : MemberReadonly(2)
{
    _int_member = 0;
    _int_property_getteronly = 1;

    MemberReadonlyAs = 3;
}

PropertyReadonlyAgent::~PropertyReadonlyAgent()
{
}

BEGIN_PROPERTIES_DESCRIPTION(PropertyReadonlyAgent)
{
    //CLASS_DISPLAYNAME(L"PropertyReadonlyAgent DisplayName")
    //CLASS_DESC(L"PropertyReadonlyAgent Desc")

    REGISTER_PROPERTY("PropertyGetterSetter", PropertyGetterSetter_get, PropertyGetterSetter_set);
    REGISTER_PROPERTY("PropertyGetterOnly", PropertyGetterOnly_get);
    REGISTER_PROPERTY("StaticPropertyGetterSetter", StaticPropertyGetterSetter_get, StaticPropertyGetterSetter_set);
    REGISTER_PROPERTY(MemberReadonly);
    REGISTER_PROPERTY(MemberReadonlyAs).SETREADONLY();
    REGISTER_METHOD(FnWithOutParam);
    REGISTER_METHOD(PassInProperty);
}
END_PROPERTIES_DESCRIPTION()
