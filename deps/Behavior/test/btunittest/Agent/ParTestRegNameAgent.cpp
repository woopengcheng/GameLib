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

#include "ParTestRegNameAgent.h"

kEmployee ParTestRegNameAgent::STV_KEMPLOYEE_0;
behaviac::vector<signed char> ParTestRegNameAgent::STV_LIST_SBYTE_0;
behaviac::vector<kEmployee> ParTestRegNameAgent::STV_LIST_KEMPLOYEE_0;

int StaticAgent::sInt = 1;

BEGIN_PROPERTIES_DESCRIPTION(StaticAgent)
{

    //CLASS_DISPLAY_INFO(L"StaticAgent", L"")

    REGISTER_PROPERTY(sInt);
    REGISTER_METHOD(sAction);
}
END_PROPERTIES_DESCRIPTION()

ParTestRegNameAgent::ParTestRegNameAgent()
{
    TV_CHAR_0 = '\0';
    TV_BYTE_0 = 0;
    TV_SBYTE_0 = 0;
    TV_STR_0 = "";
    TV_AGENT_0 = NULL;
}

ParTestRegNameAgent::~ParTestRegNameAgent()
{
}

void ParTestRegNameAgent::resetProperties()
{
    TV_CHAR_0 = '\0';
    TV_BYTE_0 = 0;
    TV_SBYTE_0 = 0;
    TV_STR_0 = "";
    TV_AGENT_0 = NULL;
    TV_KEMPLOYEE_0.resetProperties();
    STV_KEMPLOYEE_0.resetProperties();

    TV_LIST_KEMPLOYEE_0.clear();
    STV_LIST_SBYTE_0.clear();
    STV_LIST_KEMPLOYEE_0.clear();
}

BEGIN_PROPERTIES_DESCRIPTION(ParTestRegNameAgent)
{
    //CLASS_DISPLAY_INFO(L"ParTestRegNameAgent", L"")

    REGISTER_PROPERTY(TV_CHAR_0).DISPLAY_INFO(L"3 # TV_CHAR_0", L"A");
    REGISTER_PROPERTY(TV_BYTE_0).DISPLAYNAME(L"3 # TV_BYTE_0").DESC(L"A");
    REGISTER_PROPERTY(TV_SBYTE_0).DISPLAYNAME(L"3 # TV_SBYTE_0").DESC(L"A");
    REGISTER_PROPERTY(TV_STR_0).DISPLAYNAME(L"3 # TV_STR_0").DESC(L"A");;
    REGISTER_PROPERTY(TV_AGENT_0).DISPLAYNAME(L"3 # TV_AGENT_0").DESC(L"A");
    REGISTER_PROPERTY(TV_KEMPLOYEE_0).DISPLAYNAME(L"3 # TV_KEMPLOYEE_0").DESC(L"A");
    REGISTER_PROPERTY(STV_KEMPLOYEE_0).DISPLAYNAME(L"3 # STV_KEMPLOYEE_0").DESC(L"A");
    REGISTER_PROPERTY(TV_LIST_KEMPLOYEE_0).DISPLAYNAME(L"3 # TV_LIST_KEMPLOYEE_0").DESC(L"A");
    REGISTER_PROPERTY(STV_LIST_SBYTE_0).DISPLAYNAME(L"3 # STV_LIST_SBYTE_0").DESC(L"A");
    REGISTER_PROPERTY(STV_LIST_KEMPLOYEE_0).DISPLAYNAME(L"3 # STV_LIST_KEMPLOYEE_0").DESC(L"A");

    REGISTER_METHOD(Func_CharIR).DISPLAYNAME(L"3 # PIR_Char").DESC(L"A");
    REGISTER_METHOD(Func_ByteIR).DISPLAYNAME(L"3 # PIR_Byte").DESC(L"A");
    REGISTER_METHOD(Func_SByteIR).DISPLAYNAME(L"3 # PIR_SByte").DESC(L"A");
    REGISTER_METHOD(Func_StringIR).DISPLAYNAME(L"3 # PIR_String").DESC(L"A");
    REGISTER_METHOD(Func_AgentIR).DISPLAYNAME(L"3 # PIR_Agent").DESC(L"A");
    REGISTER_METHOD(Func_kEmployeeIR).DISPLAYNAME(L"3 # PIR_kEmployee").DESC(L"A");
    REGISTER_METHOD(Func_SByteListIR).DISPLAYNAME(L"3 # PIR_SByteList").DESC(L"A");
    REGISTER_METHOD(Func_kEmployeeListIR).DISPLAYNAME(L"3 # PIR_kEmployeeList").DESC(L"A");
}
END_PROPERTIES_DESCRIPTION()
