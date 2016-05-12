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

#include "behaviac/behaviortree/nodes/decorators/decoratoriterator.h"

#include "behaviac/behaviortree/nodes/conditions/condition.h"
#include "behaviac/behaviortree/nodes/actions/action.h"
#include "behaviac/property/property_t.h"

#include "behaviac/htn/planner.h"
#include "behaviac/htn/plannertask.h"

namespace behaviac
{
    DecoratorIterator::DecoratorIterator() : m_opl(0), m_opr(0), m_opr_m(0)
    {
    }
    DecoratorIterator::~DecoratorIterator()
    {
        BEHAVIAC_DELETE(m_opr_m);
    }

    void DecoratorIterator::load(int version, const char*  agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        behaviac::string typeName;
        behaviac::string propertyName;

        for (propertie_const_iterator_t p = properties.begin(); p != properties.end(); ++p)
        {
            if (strcmp(p->name, "Opl") == 0)
            {
                behaviac::string str(p->value);
                size_t pParenthesis = str.find_first_of('(');

				if (pParenthesis == (size_t)-1)
                {
                    this->m_opl = Condition::LoadLeft(p->value);

                }
                else
                {
                    BEHAVIAC_ASSERT(false);
                }
            }
            else if (strcmp(p->name, "Opr") == 0)
            {
                behaviac::string str(p->value);
                size_t pParenthesis = str.find_first_of('(');

				if (pParenthesis == (size_t)-1)
                {
                    this->m_opr = Condition::LoadRight(p->value, typeName);

                }
                else
                {
                    //method
                    this->m_opr_m = Action::LoadMethod(p->value);
                }
            }
            else
            {
                //BEHAVIAC_ASSERT(0, "unrecognised property %s", p->name);
            }
        }
    }

    bool DecoratorIterator::decompose(BehaviorNode* node, PlannerTaskComplex* seqTask, int depth, Planner* planner)
    {
        bool bOk = false;
        DecoratorIterator* pForEach = (DecoratorIterator*)node;
        int childCount = pForEach->GetChildrenCount();
        BEHAVIAC_ASSERT(childCount == 1);
        BEHAVIAC_UNUSED_VAR(childCount);
        BehaviorNode* childNode = (BehaviorNode*)pForEach->GetChild(0);

        bool bGoOn = true;
        int count = 0;
        int index = 0;

        while (bGoOn)
        {
            int depth2 = planner->GetAgent()->m_variables.Depth();
            BEHAVIAC_UNUSED_VAR(depth2);
            {
                AgentState::AgentStateScope scopedState(planner->GetAgent()->m_variables.Push(false));

                bGoOn = pForEach->IterateIt(planner->GetAgent(), index, count);

                if (bGoOn)
                {
                    planner->LogPlanForEachBegin(planner->GetAgent(), pForEach, index, count);
                    PlannerTask* childTask = planner->decomposeNode(childNode, depth);
                    planner->LogPlanForEachEnd(planner->GetAgent(), pForEach, index, count, childTask != NULL ? "success" : "failure");

                    if (childTask != NULL)
                    {
                        BEHAVIAC_ASSERT(PlannerTaskIterator::DynamicCast(seqTask) != 0);
                        PlannerTaskIterator* pForEachTask = (PlannerTaskIterator*)seqTask;
						pForEachTask->SetIndex(index);

                        seqTask->AddChild(childTask);
                        bOk = true;
                        break;
                    }

                    index++;
                }

                BEHAVIAC_ASSERT(planner->GetAgent()->m_variables.Depth() == depth2);
            }
        }

        return bOk;
    }

    bool DecoratorIterator::IsValid(Agent* pAgent, BehaviorTask* pTask) const
    {
        if (DecoratorIterator::DynamicCast(pTask->GetNode()) == 0)
        {
            return false;
        }

        return super::IsValid(pAgent, pTask);
    }

    bool DecoratorIterator::IterateIt(Agent* pAgent, int index, int& count)
    {
        BEHAVIAC_UNUSED_VAR(count);

        if (this->m_opr_m != NULL)
        {
            this->m_opr_m->Invoke(pAgent);
            Agent* pParentOpl = this->m_opl->GetParentAgent(pAgent);
            Agent* pParentR = this->m_opr_m->GetParentAgent(pAgent);

            this->m_opl->SetFrom(pParentR, this->m_opr_m, pParentOpl, index);

            return true;

        }
        else if (this->m_opr != NULL)
        {
            Agent* pParentL = this->m_opl->GetParentAgent(pAgent);
            Agent* pParentR = this->m_opr->GetParentAgent(pAgent);

            //m_opr is a vector
            this->m_opr->SetVectorElementTo(pParentR, index, this->m_opl, pParentL);

            return true;

        }
        else
        {
            BEHAVIAC_ASSERT(false);
        }

        return false;
    }

    BehaviorTask* DecoratorIterator::createTask() const
    {
        BEHAVIAC_ASSERT(false);
        return NULL;
    }
}
