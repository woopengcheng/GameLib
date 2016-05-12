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

#include "CustomPropertyAgent.h"

BEGIN_PROPERTIES_DESCRIPTION(UnityEngine::Vector3)
{
    REGISTER_PROPERTY(x);
    REGISTER_PROPERTY(y);
    REGISTER_PROPERTY(z);
}
END_PROPERTIES_DESCRIPTION()

int CustomPropertyAgent::IntMemberConst = 0;

CustomPropertyAgent::CustomPropertyAgent()
{
    init();
}

CustomPropertyAgent::~CustomPropertyAgent()
{
}

BEGIN_PROPERTIES_DESCRIPTION(CustomPropertyAgent)
{
    //CLASS_DISPLAYNAME(L"CustomPropertyAgent DisplayName")
    //CLASS_DESC(L"CustomPropertyAgent Desc")
    REGISTER_PROPERTY(IntProperty);
    REGISTER_PROPERTY(FloatPropertyReadonly);
    REGISTER_PROPERTY(BoolMemberReadonly);
    REGISTER_PROPERTY(IntMemberConst);
    REGISTER_PROPERTY(StringMemberReadonly);
    REGISTER_PROPERTY(Location);

    REGISTER_METHOD(FnWithOutParam);
}
END_PROPERTIES_DESCRIPTION()
