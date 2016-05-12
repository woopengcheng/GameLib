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

#include "EmployeeParTestAgent.h"

float EmployeeParTestAgent::STV_F_0 = 0.0f;
behaviac::string EmployeeParTestAgent::STV_STR_0 = "";
behaviac::Agent* EmployeeParTestAgent::STV_AGENT_0 = NULL;
behaviac::vector<float> EmployeeParTestAgent::STV_LIST_F_0;
behaviac::vector<behaviac::string> EmployeeParTestAgent::STV_LIST_STR_0;
behaviac::vector<behaviac::Agent*> EmployeeParTestAgent::STV_LIST_AGENT_0;

EmployeeParTestAgent::EmployeeParTestAgent()
{
    TV_UINT_0 = 0;
    TV_ULONG_0 = 0L;
    TV_LL_0 = 0L;
    TV_ULL_0 = 0L;
    TV_F_0 = 0.0f;
    TV_D_0 = 0.0;
    TV_STR_0 = "";
    TV_AGENT_0 = NULL;
	TV_CSZSTR_0 = 0;
	TV_SZSTR_0 = 0;
}

EmployeeParTestAgent::~EmployeeParTestAgent()
{
}

void EmployeeParTestAgent::resetProperties()
{
    super::resetProperties();

    TV_UINT_0 = 0;
    TV_ULONG_0 = 0L;
    TV_F_0 = 0.0f;
    STV_F_0 = 0.0f;
    TV_D_0 = 0.0;
    TV_LL_0 = 0L;
    TV_ULL_0 = 0L;
    TV_STR_0 = "";
    TV_SZSTR_0 = NULL;
    TV_CSZSTR_0 = "TV_CSZSTR_0";
    STV_STR_0 = "";
    TV_AGENT_0 = NULL;
    STV_AGENT_0 = NULL;

    TV_LIST_F_0.clear();
    STV_LIST_F_0.clear();
    TV_LIST_STR_0.clear();
    STV_LIST_STR_0.clear();
    TV_LIST_AGENT_0.clear();
    STV_LIST_AGENT_0.clear();
}

BEGIN_PROPERTIES_DESCRIPTION(EmployeeParTestAgent)
{
    //CLASS_DISPLAY_INFO(L"EmployeeParTestAgent", L"")

    REGISTER_PROPERTY(TV_UINT_0).DISPLAY_INFO(L"2 # TV_UINT_0", L"A");
    REGISTER_PROPERTY(TV_ULONG_0).DISPLAYNAME(L"2 # TV_ULONG_0").DESC(L"A");
    REGISTER_PROPERTY(TV_LL_0).DISPLAYNAME(L"2 # TV_LL_0").DESC(L"A");
    REGISTER_PROPERTY(TV_ULL_0).DISPLAYNAME(L"2 # TV_ULL_0").DESC(L"A");
    REGISTER_PROPERTY(TV_F_0).DISPLAYNAME(L"2 # TV_F_0").DESC(L"A");
    REGISTER_PROPERTY(STV_F_0).DISPLAYNAME(L"2 # STV_F_0").DESC(L"A");;
    REGISTER_PROPERTY(TV_D_0).DISPLAYNAME(L"2 # TV_D_0").DESC(L"A");
    REGISTER_PROPERTY(TV_STR_0).DISPLAYNAME(L"2 # TV_STR_0").DESC(L"A");
    REGISTER_PROPERTY(TV_CSZSTR_0).DISPLAYNAME(L"2 # TV_CSZSTR_0").DESC(L"A");
    REGISTER_PROPERTY(TV_SZSTR_0).DISPLAYNAME(L"2 # TV_SZSTR_0").DESC(L"A");
    REGISTER_PROPERTY(STV_STR_0).DISPLAYNAME(L"2 # STV_STR_0").DESC(L"A");
    REGISTER_PROPERTY(TV_AGENT_0).DISPLAYNAME(L"2 # TV_AGENT_0").DESC(L"A");
    REGISTER_PROPERTY(STV_AGENT_0).DISPLAYNAME(L"2 # STV_AGENT_0").DESC(L"A");
    REGISTER_PROPERTY(TV_LIST_F_0).DISPLAYNAME(L"2 # TV_LIST_F_0").DESC(L"A");
    REGISTER_PROPERTY(STV_LIST_F_0).DISPLAYNAME(L"2 # STV_LIST_F_0").DESC(L"A");
    REGISTER_PROPERTY(TV_LIST_STR_0).DISPLAYNAME(L"2 # TV_LIST_STR_0").DESC(L"A");
    REGISTER_PROPERTY(STV_LIST_STR_0).DISPLAYNAME(L"2 # STV_LIST_STR_0").DESC(L"A");
    REGISTER_PROPERTY(TV_LIST_AGENT_0).DISPLAYNAME(L"2 # TV_LIST_AGENT_0").DESC(L"A");
    REGISTER_PROPERTY(STV_LIST_AGENT_0).DISPLAYNAME(L"2 # STV_LIST_AGENT_0").DESC(L"A");

    REGISTER_METHOD(Func_UIntRef).DISPLAYNAME(L"2 # UPR_UIntRef").DESC(L"A");
    REGISTER_METHOD(Func_ULongRef).DISPLAYNAME(L"2 # UPR_ULongRef").DESC(L"A");
    REGISTER_METHOD(Func_SingleRef).DISPLAYNAME(L"2 # UPR_SingleRef").DESC(L"A");
    REGISTER_METHOD(Func_DoubleRef).DISPLAYNAME(L"2 # UPR_DoubleRef").DESC(L"A");
    REGISTER_METHOD(Func_LongLongRef).DISPLAYNAME(L"2 # UPR_LongLongRef").DESC(L"A");
    REGISTER_METHOD(Func_ULongLongRef).DISPLAYNAME(L"2 # UPR_ULongLongRef").DESC(L"A");
    REGISTER_METHOD(Func_StringRef).DISPLAYNAME(L"2 # UPR_StringRef").DESC(L"A");
    REGISTER_METHOD(Func_SzStringRef).DISPLAYNAME(L"2 # UPR_SzStringRef").DESC(L"A");
    REGISTER_METHOD(Func_CSzStringRef).DISPLAYNAME(L"2 # UPR_CSzStringRef").DESC(L"A");
    REGISTER_METHOD(Func_AgentRef).DISPLAYNAME(L"2 # UPR_AgentRef").DESC(L"A");
    REGISTER_METHOD(Func_SingleListRef).DISPLAYNAME(L"2 # UPR_SingleListRef").DESC(L"A");
    REGISTER_METHOD(Func_StringListRef).DISPLAYNAME(L"2 # UPR_StringListRef").DESC(L"A");
    REGISTER_METHOD(Func_AgentListRef).DISPLAYNAME(L"2 # UPR_AgentListRef").DESC(L"A");

    REGISTER_METHOD(Func_UIntIR).DISPLAYNAME(L"2 # PIR_UIntIR").DESC(L"A");
    REGISTER_METHOD(Func_ULongIR).DISPLAYNAME(L"2 # PIR_ULongIR").DESC(L"A");
    REGISTER_METHOD(Func_SingleIR).DISPLAYNAME(L"2 # PIR_SingleIR").DESC(L"A");
    REGISTER_METHOD(Func_DoubleIR).DISPLAYNAME(L"2 # PIR_DoubleIR").DESC(L"A");
    REGISTER_METHOD(Func_LongLongIR).DISPLAYNAME(L"2 # PIR_LongLongIR").DESC(L"A");
    REGISTER_METHOD(Func_ULongLongIR).DISPLAYNAME(L"2 # PIR_ULongLongIR").DESC(L"A");
    REGISTER_METHOD(Func_StringIR).DISPLAYNAME(L"2 # PIR_StringIR").DESC(L"A");
    REGISTER_METHOD(Func_SzStringIR).DISPLAYNAME(L"2 # PIR_SzStringIR").DESC(L"A");
    REGISTER_METHOD(Func_CSzStringIR).DISPLAYNAME(L"2 # PIR_CSzStringIR").DESC(L"A");
    REGISTER_METHOD(Func_AgentIR).DISPLAYNAME(L"2 # PIR_AgentIR").DESC(L"A");
    REGISTER_METHOD(Func_ParTestAgentBaseIR).DISPLAYNAME(L"2 # PIR_ParTestAgentBaseIR").DESC(L"A");
    REGISTER_METHOD(Func_SingleListIR).DISPLAYNAME(L"2 # PIR_SingleListIR").DESC(L"A");
    REGISTER_METHOD(Func_StringListIR).DISPLAYNAME(L"2 # PIR_StringListIR").DESC(L"A");
    REGISTER_METHOD(Func_AgentListIR).DISPLAYNAME(L"2 # PIR_AgentListIR").DESC(L"A");
    REGISTER_METHOD(Func_ParTestAgentBaseListIR).DISPLAYNAME(L"2 # PIR_ParTestAgentBaseListIR").DESC(L"A");

    REGISTER_METHOD(Func_ULongSMF).DISPLAYNAME(L"2 # SMF_ULongSMF").DESC(L"A");
    REGISTER_METHOD(Func_StringSMF).DISPLAYNAME(L"2 # SMF_StringSMF").DESC(L"A");
    REGISTER_METHOD(Func_StringListSMF).DISPLAYNAME(L"2 # SMF_StringListSMF").DESC(L"A");
    REGISTER_METHOD(Func_AgentSMF).DISPLAYNAME(L"2 # SMF_AgentSMF").DESC(L"A");
    REGISTER_METHOD(Func_AgentListSMF).DISPLAYNAME(L"2 # SMF_AgentListSMF").DESC(L"A");
}
END_PROPERTIES_DESCRIPTION()
