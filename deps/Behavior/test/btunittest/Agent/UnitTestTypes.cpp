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

#include "UnitTestTypes.h"
#include "behaviac/agent/registermacros.h"

//< eColor
BEGIN_ENUM_DESCRIPTION(TNS::NE::NAT::eColor, eColor)
{
    ENUMCLASS_DISPLAY_INFO(L"色彩枚举", L"eColor 是一个用于测试枚举使用的类型");

    DEFINE_ENUM_VALUE(TNS::NE::NAT::RED, "RED").DISPLAY_INFO(L"红色 Red", L"Red 是一种非常激进的色彩");
    DEFINE_ENUM_VALUE(TNS::NE::NAT::GREEN, "GREEN");
    DEFINE_ENUM_VALUE(TNS::NE::NAT::BLUE, "BLUE");
    DEFINE_ENUM_VALUE(TNS::NE::NAT::YELLOW, "YELLOW");
    DEFINE_ENUM_VALUE(TNS::NE::NAT::WHITE, "WHITE");
}
END_ENUM_DESCRIPTION()

//< Car
BEGIN_PROPERTIES_DESCRIPTION(TNS::ST::kCar)
{
    //CLASS_DISPLAYNAME(L"测试Struct")
    //CLASS_DESC(L"测试Struct的说明")

    REGISTER_PROPERTY(brand);
    REGISTER_PROPERTY(price);
    REGISTER_PROPERTY(color);
}
END_PROPERTIES_DESCRIPTION()

//< kEmployee
BEGIN_PROPERTIES_DESCRIPTION(TNS::ST::PER::WRK::kEmployee)
{
    //CLASS_DISPLAYNAME(L"测试Struct")
    //CLASS_DESC(L"测试Struct的说明")

    REGISTER_PROPERTY(id);
    REGISTER_PROPERTY(name);
    REGISTER_PROPERTY(code);
    REGISTER_PROPERTY(weight);
    REGISTER_PROPERTY(isMale);
    REGISTER_PROPERTY(skinColor);
    REGISTER_PROPERTY(car);
    REGISTER_PROPERTY(boss);
}
END_PROPERTIES_DESCRIPTION()
