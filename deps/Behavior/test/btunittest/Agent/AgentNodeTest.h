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

#pragma once

#include "behaviac/base/base.h"
#include "behaviac/agent/agent.h"
#include "behaviac/agent/registermacros.h"
#include "Agent/UnitTestTypes.h"
#include "ext/extendreftype.h"
#include "ext/extendstruct.h"

using TNS::NE::NAT::eColor;
using TNS::NE::NAT::WHITE;
using TNS::NE::NAT::RED;
using TNS::NE::NAT::GREEN;
using TNS::NE::NAT::YELLOW;
using TNS::NE::NAT::BLUE;
using TNS::ST::PER::WRK::kEmployee;

namespace UnityEngine
{
    class GameObject
    {
    public:
        GameObject()
        {}

        virtual ~GameObject()
        {}

        behaviac::string name;

        DECLARE_BEHAVIAC_STRUCT(UnityEngine::GameObject, true);
    };
}

enum EnumTest
{
	EnumTest_One = 0,
	EnumTest_OneAfterOne = 1,
};

DECLARE_BEHAVIAC_OBJECT_ENUM(EnumTest, EnumTest);

class ChildNodeTest;
class AgentNodeTest : public behaviac::Agent
{
public:
    AgentNodeTest();
    virtual ~AgentNodeTest();

    DECLARE_BEHAVIAC_AGENT(AgentNodeTest, behaviac::Agent);

    int testVar_0;
    int testVar_1;
    float testVar_2;
    float testVar_3;
    int waiting_timeout_interval;
    behaviac::string testVar_str_0;
    behaviac::string testVar_str_1;

    int event_test_var_int;
    bool event_test_var_bool;
    float event_test_var_float;
	AgentNodeTest* event_test_var_agent;

    // enter action and exit action
    int action_0_enter_count;
    int action_0_exit_count;
    int action_1_enter_count;
    int action_1_exit_count;
    int action_2_enter_count;
    int action_2_exit_count;

	bool m_bCanSee;
	bool m_bTargetValid;

	EnumTest	testColor;

	TestNS::Float2	TestFloat2;
public:
    virtual void resetProperties();

    void init()
    {
        resetProperties();
    }

    void finl()
    {
    }

    void initChildAgentTest();

    template<class ChildAgentType>
    ChildAgentType* getChildAgent(int var_0, const char* strChildAgentName)
    {
        ChildAgentType* childAgent = behaviac::Agent::Create<ChildAgentType>(var_0,strChildAgentName,0,0);
        return childAgent;
    };

    void setEventVarInt(int var)
    {
        event_test_var_int = var;
    }

    void setEventVarBool(bool var)
    {
        event_test_var_bool = var;
    }

    void setEventVarFloat(float var)
    {
        event_test_var_float = var;
    }

    void setEventVarAgent(AgentNodeTest* agent)
    {
        event_test_var_agent = agent;
    }

    int getConstOne()
    {
        return 1;
    }

	int getConstThousand(int a, int b)
	{
		return a + b;
	}

    void setTestVar_0(int var)
    {
        testVar_0 = var;
    }

    void setTestVar_1(int var)
    {
        testVar_1 = var;
    }

    void setTestVar_2(float var)
    {
        testVar_2 = var;
    }

    void setTestVar_0_2(int var0, float var2)
    {
        testVar_0 = var0;
        testVar_2 = var2;
    }

    float setTestVar_R()
    {
        return (float)testVar_0 + testVar_2;
    }

    void setTestVar_3(float var)
    {
        testVar_3 = var;
    }

    UnityEngine::GameObject* createGameObject()
    {
        UnityEngine::GameObject* go = BEHAVIAC_NEW UnityEngine::GameObject();
        go->name = "HC";
        return go;
    }

    void testGameObject(UnityEngine::GameObject* go)
    {
        if (go != NULL)
			testVar_str_0 = go->name;
		else
			testVar_str_0 = "null";
    }

	TestNS::Node* createExtendedNode()
	{
		TestNS::Node* n = BEHAVIAC_NEW TestNS::Node();
		n->name = "NODE";
		return n;
	}

	void testExtendedRefType(TestNS::Node* go)
	{
		BEHAVIAC_ASSERT(go != NULL);
		testVar_str_1 = go->name;
	}

	void testExtendedStruct(TestNS::Float2& f)
	{
		f.x = 1.0f;
		f.y = 1.0f;
	}

	TestNS::Float2& getExtendedStruct()
	{
		return this->TestFloat2;
	}

	const TestNS::Float2& getConstExtendedStruct()
	{
		return this->TestFloat2;
	}

    behaviac::EBTStatus switchRef(const behaviac::string& refTree)
    {
        this->btreferencetree(refTree.c_str());

        return behaviac::BT_RUNNING;
    }

    bool enter_action_0()
    {
        action_0_enter_count++;
        return true;
    }

    void exit_action_0()
    {
        action_0_exit_count++;
    }

    bool enter_action_1(float f)
    {
        action_1_enter_count++;
        return true;
    }

    void exit_action_1(float f)
    {
        action_1_exit_count++;
    }

    bool enter_action_2(int i, behaviac::string str)
    {
        testVar_1 = i;
        testVar_str_0 = str;
        action_2_enter_count++;

        return true;
    }

    void exit_action_2(int i, behaviac::string str)
    {
        testVar_1 = i;
        testVar_str_0 = str;
        action_2_exit_count++;
    }

	void Stop()
	{
	}

	void SelectTarget()
	{
		m_bTargetValid = true;
	}

	bool IsTargetValid() const
	{
		return m_bTargetValid;
	}

	bool CanSeeEnemy() const
	{
		return m_bCanSee;
	}

	behaviac::EBTStatus Move()
	{
		return behaviac::BT_RUNNING;
	}

	behaviac::EBTStatus MoveToTarget()
	{
		return behaviac::BT_RUNNING;
	}

	const char* GetRefTree() const {
		return "node_test/reference_sub_0";
	}

    void initChildAgent();
};


class ChildNodeTest : public AgentNodeTest
{
public:
    ChildNodeTest(int var_0);
    virtual ~ChildNodeTest();

    DECLARE_BEHAVIAC_AGENT(ChildNodeTest, AgentNodeTest);

public:
	float GetConstFloatValue();
	double GetConstDoubleValue();
};
