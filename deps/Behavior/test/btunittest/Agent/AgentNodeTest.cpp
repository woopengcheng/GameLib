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

#include "AgentNodeTest.h"


BEGIN_ENUM_DESCRIPTION(EnumTest, EnumTest)
{
	DEFINE_ENUM_VALUE(EnumTest_One, "EnumTest_One");
	DEFINE_ENUM_VALUE(EnumTest_OneAfterOne, "EnumTest_OneAfterOne");
}
END_ENUM_DESCRIPTION()

AgentNodeTest::AgentNodeTest()
{
}

AgentNodeTest::~AgentNodeTest()
{
}

void AgentNodeTest::resetProperties()
{
    testVar_0 = -1;
    testVar_1 = -1;
    testVar_2 = -1.0f;
    testVar_3 = -1.0f;
    event_test_var_int = -1;
    event_test_var_bool = false;
    event_test_var_float = -1.0f;
	event_test_var_agent = NULL;
    waiting_timeout_interval = 0;

    action_0_enter_count = 0;
    action_0_exit_count = 0;
    action_1_enter_count = 0;
    action_1_exit_count = 0;
    action_2_enter_count = 0;
    action_2_exit_count = 0;

    testVar_str_0 = "";
	testColor = EnumTest_One;

	m_bCanSee = false;
	m_bTargetValid = false;

	TestFloat2.x = 2.0f;
	TestFloat2.y = 2.0f;
}

namespace UnityEngine
{
    BEGIN_PROPERTIES_DESCRIPTION(GameObject)
    {
        REGISTER_PROPERTY(name);
    }
    END_PROPERTIES_DESCRIPTION()
}

BEGIN_PROPERTIES_DESCRIPTION(AgentNodeTest)
{
    //CLASS_DISPLAYNAME(L"²âÊÔbehaviac::Agent")
    //CLASS_DESC(L"²âÊÔbehaviac::AgentµÄËµÃ÷")
    REGISTER_PROPERTY(testVar_0);
    REGISTER_PROPERTY(testVar_1).DISPLAYNAME(L"testVar_1").DESC(L"testVar_1 property").RANGE(100);
    REGISTER_PROPERTY(testVar_2);
    REGISTER_PROPERTY(testVar_3);
    REGISTER_PROPERTY(waiting_timeout_interval);
    REGISTER_PROPERTY(testVar_str_0);
	REGISTER_PROPERTY(testColor);

    REGISTER_METHOD(setEventVarInt);
    REGISTER_METHOD(setEventVarBool);
    REGISTER_METHOD(setEventVarFloat);
    REGISTER_METHOD(setEventVarAgent);
	REGISTER_METHOD(getConstOne);
	REGISTER_METHOD(getConstThousand);
    REGISTER_METHOD(setTestVar_0);
    REGISTER_METHOD(setTestVar_1);
    REGISTER_METHOD(setTestVar_2);
    REGISTER_METHOD(setTestVar_0_2);
    REGISTER_METHOD(setTestVar_R);
    REGISTER_METHOD(setTestVar_3);
    REGISTER_METHOD(enter_action_0);
    REGISTER_METHOD(exit_action_0);
    REGISTER_METHOD(enter_action_1);
    REGISTER_METHOD(exit_action_1);
    REGISTER_METHOD(enter_action_2);
    REGISTER_METHOD(exit_action_2);
    REGISTER_METHOD(createGameObject);
    REGISTER_METHOD(testGameObject);
	REGISTER_METHOD(createExtendedNode);
	REGISTER_METHOD(testExtendedRefType);
	REGISTER_METHOD(testExtendedStruct);
	REGISTER_METHOD(getExtendedStruct);
	REGISTER_METHOD(getConstExtendedStruct);
    REGISTER_METHOD(switchRef);

	REGISTER_METHOD(Stop);
	REGISTER_METHOD(SelectTarget);

	REGISTER_METHOD(IsTargetValid);
	REGISTER_METHOD(CanSeeEnemy);
	REGISTER_METHOD(Move);
	REGISTER_METHOD(MoveToTarget);
	REGISTER_METHOD(GetRefTree);
	REGISTER_METHOD(initChildAgent);
	REGISTER_METHOD(initChildAgentTest);
}
END_PROPERTIES_DESCRIPTION()

void AgentNodeTest::initChildAgentTest()
{
	ChildNodeTest* testChildAgent = this->getChildAgent<ChildNodeTest>(1, "par_child_agent_1");
	this->SetVariable("par_child_agent_1", testChildAgent);
}

void AgentNodeTest::initChildAgent()
{
    ChildNodeTest* test = this->GetVariable<ChildNodeTest*>("par_child_agent_1");

    test->resetProperties();
    test->testVar_1 = 888;
}

ChildNodeTest::ChildNodeTest(int var_0)
{
	testVar_0 = var_0;
}

ChildNodeTest::~ChildNodeTest()
{
}

float ChildNodeTest::GetConstFloatValue()
{
	return 1000.0f;
}

double ChildNodeTest::GetConstDoubleValue()
{
	return 1000.0;
}

BEGIN_PROPERTIES_DESCRIPTION(ChildNodeTest)
{
	REGISTER_METHOD(GetConstFloatValue);
	REGISTER_METHOD(GetConstDoubleValue);
}
END_PROPERTIES_DESCRIPTION()
