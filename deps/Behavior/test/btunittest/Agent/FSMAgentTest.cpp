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

#include "FSMAgentTest.h"

FSMAgentTest::FSMAgentTest()
{
    Message = Invalid;
    TestVar = -1;
}

FSMAgentTest::~FSMAgentTest()
{
}

BEGIN_ENUM_DESCRIPTION(FSMAgentTest::EMessage, EMessage)
{
    DEFINE_ENUM_VALUE(FSMAgentTest::Invalid, "Invalid");
    DEFINE_ENUM_VALUE(FSMAgentTest::Begin, "Begin");
    DEFINE_ENUM_VALUE(FSMAgentTest::End, "End");
    DEFINE_ENUM_VALUE(FSMAgentTest::Pause, "Pause");
    DEFINE_ENUM_VALUE(FSMAgentTest::Resume, "Resume");
    DEFINE_ENUM_VALUE(FSMAgentTest::Exit, "Exit");
}
END_ENUM_DESCRIPTION()

BEGIN_PROPERTIES_DESCRIPTION(FSMAgentTest)
{
    //CLASS_DISPLAYNAME(L"FSMAgentTest DisplayName")
    //CLASS_DESC(L"FSMAgentTest Desc")

    REGISTER_PROPERTY(Message);

    REGISTER_METHOD(inactive_update);
    REGISTER_METHOD(active_update);
    REGISTER_METHOD(pause_update);
    REGISTER_METHOD(exit_update);
}
END_PROPERTIES_DESCRIPTION()
