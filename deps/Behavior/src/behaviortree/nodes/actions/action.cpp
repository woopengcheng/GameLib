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

#include "behaviac/base/base.h"
#include "behaviac/property/property_t.h"
#include "behaviac/behaviortree/nodes/actions/action.h"

#include "behaviac/base/core/profiler/profiler.h"

namespace behaviac
{
    Action::Action() : m_method(0), m_resultOption(BT_INVALID), m_resultFunctor(0)
    {
    }

    Action::~Action()
    {
        BEHAVIAC_DELETE(m_method);
        BEHAVIAC_DELETE(m_resultFunctor);
    }

    const char* strrchr(const char* start, const char* end, char c)
    {
        while (end > start)
        {
            if (*end == c)
            {
                return end;
            }

            end--;
        }

        return 0;
    }

	static const size_t kNameLength = 256;
    const char* Action::ParseMethodNames(const char* fullName, char* agentIntanceName, char* agentClassName, char* methodName)
    {
        //Self.test_ns::AgentActionTest::Action2(0)
        string str;

        const char*  pClassBegin = strchr(fullName, '.');
        BEHAVIAC_ASSERT(pClassBegin);

        size_t posClass = pClassBegin - fullName;
        BEHAVIAC_ASSERT(posClass < kNameLength);
        string_ncpy(agentIntanceName, fullName, posClass);
        agentIntanceName[posClass] = '\0';

        const char* pBeginAgentClass = pClassBegin + 1;

        const char* pBeginP = strchr(pBeginAgentClass, '(');
        BEHAVIAC_ASSERT(pBeginP);

        //test_ns::AgentActionTest::Action2(0)
        const char* pBeginMethod = strrchr(pBeginAgentClass, pBeginP, ':');
        BEHAVIAC_ASSERT(pBeginMethod);
        //skip '::'
        BEHAVIAC_ASSERT(pBeginMethod[0] == ':' && pBeginMethod[-1] == ':');
        pBeginMethod += 1;

        size_t pos1 = pBeginP - pBeginMethod;
        BEHAVIAC_ASSERT(pos1 < kNameLength);

        string_ncpy(methodName, pBeginMethod, pos1);
        methodName[pos1] = '\0';

		size_t pos = pBeginMethod - 2 - pBeginAgentClass;
        BEHAVIAC_ASSERT(pos < kNameLength);

        string_ncpy(agentClassName, pBeginAgentClass, pos);
        agentClassName[pos] = '\0';

        return pBeginP;
    }

    //suppose params are seprated by ','
    static void ParseForParams(const behaviac::string& tsrc, behaviac::vector<behaviac::string>& params)
    {
        int tsrcLen = (int)tsrc.size();
        int startIndex = 0;
        int index = 0;
        int quoteDepth = 0;

        for (; index < tsrcLen; ++index)
        {
            if (tsrc[index] == '"')
            {
                quoteDepth++;

                //if (quoteDepth == 1)
                //{
                //	startIndex = index;
                //}

                if ((quoteDepth & 0x1) == 0)
                {
                    //closing quote
                    quoteDepth -= 2;
                    BEHAVIAC_ASSERT(quoteDepth >= 0);
                }
            }
            else if (quoteDepth == 0 && tsrc[index] == ',')
            {
                //skip ',' inside quotes, like "count, count"
                int lengthTemp = index - startIndex;
                behaviac::string strTemp = tsrc.substr(startIndex, lengthTemp);
                params.push_back(strTemp);
                startIndex = index + 1;
            }
        }//end for

        // the last param
        int lengthTemp = index - startIndex;

        if (lengthTemp > 0)
        {
            behaviac::string strTemp = tsrc.substr(startIndex, lengthTemp);
            params.push_back(strTemp);

            //params.push_back(strTemp);
        }
    }

    CMethodBase* Action::LoadMethod(const char* value_)
    {
        //Self.test_ns::AgentActionTest::Action2(0)
        char agentIntanceName[kNameLength];
        char agentClassName[kNameLength];
        char methodName[kNameLength];

        const char* pBeginP = ParseMethodNames(value_, agentIntanceName, agentClassName, methodName);

        //propertyName = FormatString("%s::%s", agentClassName, methodName);
        CStringID agentClassId(agentClassName);
        CStringID methodId(methodName);

        CMethodBase* method = Agent::CreateMethod(agentClassId, methodId);

        if (!method)
        {
            BEHAVIAC_LOGWARNING("No Method %s::%s registered\n", agentClassName, methodName);
            BEHAVIAC_ASSERT(0, "No Method %s::%s registered\n", agentClassName, methodName);
        }
        else
        {
            //if (Agent::IsInstanceNameRegistered(agentIntanceName))
            {
                method->SetInstanceNameString(agentIntanceName, PT_INSTANCE);
            }

            BEHAVIAC_ASSERT(method, "No Method %s::%s registered", agentClassName, methodName);
            const char* params = pBeginP;

            BEHAVIAC_ASSERT(params[0] == '(');

            behaviac::vector<behaviac::string> tokens;

            {
                size_t len = strlen(params);

                BEHAVIAC_ASSERT(params[len - 1] == ')');

                behaviac::string text = behaviac::string(params + 1, len - 2);
                //behaviac::StringUtils::SplitIntoArray(text, ",", tokens);
                ParseForParams(text, tokens);
            }

            if (tokens.size() > 0)
            {
                XmlNodeRef xmlNode = CreateXmlNode("Method");

                for (uint32_t i = 0; i < tokens.size(); ++i)
                {
                    const behaviac::string& token = tokens[i];
                    behaviac::string attriName = FormatString("param%d", i + 1);
                    xmlNode->setAttr(attriName.c_str(), token);
                }

                CTextNode node(xmlNode);
                method->LoadFromXML(0, node);
            }
        }

        return method;
    }

    void Action::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        {
            const property_t& p = (*it);

            if (strcmp(p.name, "Method") == 0)
            {
                if (p.value[0] != '\0')
                {
                    this->m_method = Action::LoadMethod(p.value);
                }
            }
            else if (strcmp(p.name, "ResultOption") == 0)
            {
                if (strcmp(p.value, "BT_INVALID") == 0)
                {
                    m_resultOption = BT_INVALID;

                }
                else if (strcmp(p.value, "BT_FAILURE") == 0)
                {
                    m_resultOption = BT_FAILURE;

                }
                else if (strcmp(p.value, "BT_RUNNING") == 0)
                {
                    m_resultOption = BT_RUNNING;

                }
                else
                {
                    m_resultOption = BT_SUCCESS;
                }
            }
            else if (strcmp(p.name, "ResultFunctor") == 0)
            {
                if (p.value[0] != '\0')
                {
                    this->m_resultFunctor = LoadMethod(p.value);
                }
            }
            else
            {
                //BEHAVIAC_ASSERT(0, "unrecognised property %s", p.name);
            }
        }
    }

    bool Action::IsValid(Agent* pAgent, BehaviorTask* pTask) const
    {
        if (!Action::DynamicCast(pTask->GetNode()))
        {
            return false;
        }

        return super::IsValid(pAgent, pTask);
    }

    BehaviorTask* Action::createTask() const
    {
        ActionTask* pTask = BEHAVIAC_NEW ActionTask();

        return pTask;
    }

	uint32_t SetNodeId(uint32_t nodeId);
	void ClearNodeId(uint32_t slot);

    //Execute(Agent* pAgent)method hava be change to Execute(Agent* pAgent, EBTStatus childStatus)
    EBTStatus Action::Execute(const Agent* pAgent, EBTStatus childStatus)
    {
        EBTStatus result = BT_SUCCESS;

        if (this->m_method)
        {
            //#if BEHAVIAC_ENABLE_PROFILING
            //			BEHAVIAC_PROFILE("Node");
            //#endif
			uint32_t nodeId = this->GetId();

			uint32_t slot = SetNodeId(nodeId);
            BEHAVIAC_ASSERT(slot != (uint32_t)-1, "no empty slot found!");

            const Agent* pParent = this->m_method->GetParentAgent(pAgent);
            this->m_method->run(pParent, pAgent);

            if (this->m_resultOption != BT_INVALID)
            {
                result = this->m_resultOption;

            }
            else if (this->m_resultFunctor)
            {
                const Agent* pParentCheckResult = this->m_resultFunctor->GetParentAgent(pAgent);

                result = (EBTStatus)this->m_method->CheckReturn(pParent, pParentCheckResult, this->m_resultFunctor);

            }
            else
            {
                this->m_method->CheckReturn(pParent, result);
            }

            ClearNodeId(slot);
        }
        else
        {
            //#if BEHAVIAC_ENABLE_PROFILING
            //			BEHAVIAC_PROFILE("ActionGenerated");
            //#endif
            result = this->update_impl((Agent*)pAgent, childStatus);
        }

        return result;
    }

    ActionTask::ActionTask() : LeafTask()
    {
    }

    ActionTask::~ActionTask()
    {
    }

    void ActionTask::copyto(BehaviorTask* target) const
    {
        super::copyto(target);
    }

    void ActionTask::save(ISerializableNode* node) const
    {
        super::save(node);
    }

    void ActionTask::load(ISerializableNode* node)
    {
        super::load(node);
    }

    bool ActionTask::onenter(Agent* pAgent)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        return true;
    }

    void ActionTask::onexit(Agent* pAgent, EBTStatus s)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_UNUSED_VAR(s);
    }

    EBTStatus ActionTask::update(Agent* pAgent, EBTStatus childStatus)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_UNUSED_VAR(childStatus);

        BEHAVIAC_ASSERT(Action::DynamicCast(this->GetNode()));
        Action* pActionNode = (Action*)(this->GetNode());

        EBTStatus result = pActionNode->Execute(pAgent, childStatus);

        return result;
    }
}
