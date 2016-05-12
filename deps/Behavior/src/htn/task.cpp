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
#include "behaviac/agent/agent.h"
#include "behaviac/behaviortree/nodes/conditions/condition.h"

#include "behaviac/htn/task.h"
#include "behaviac/behaviortree/nodes/actions/action.h"

#include "behaviac/htn/planner.h"
#include "behaviac/htn/plannertask.h"

namespace behaviac
{
    Task::Task() : m_task(0), m_bHTN(false)
    {
    }
    Task::~Task()
    {
		BEHAVIAC_DELETE(m_task);
	}

    void Task::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        //for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        {
            const property_t p = *it;

            if (strcmp(p.name, "Prototype") == 0)
            {
                if (!StringUtils::IsNullOrEmpty(p.value))
                {
                    CMethodBase* m = Action::LoadMethod(p.value);
                    this->m_task = (CTaskMethod*)m;
                }//if (p.value[0] != '\0')

            }
            else if (strcmp(p.name, "IsHTN") == 0)
            {
                if (strcmp(p.value, "true") == 0)
                {
                    this->m_bHTN = true;
                }
            }
        }
    }

    bool Task::decompose(BehaviorNode* node, PlannerTaskComplex* seqTask, int depth, Planner* planner)
    {
        bool bOk = false;
        Task* task = (Task*)node;
        PlannerTask* childTask = planner->decomposeTask(task, depth);

        if (childTask != NULL)
        {
            seqTask->AddChild(childTask);
            bOk = true;
        }

        return bOk;
    }

    bool Task::IsValid(Agent* pAgent, BehaviorTask* pTask) const
    {
        //if (!(pTask->GetNode() is Task))
        if (Task::DynamicCast(pTask->GetNode()) == 0)
        {
            return false;
        }

        return super::IsValid(pAgent, pTask);
    }
    BehaviorTask* Task::createTask() const
    {
        TaskTask* pTask = BEHAVIAC_NEW TaskTask();
        return pTask;
    }
    int Task::FindMethodIndex(Method* method)
    {
        for (uint32_t i = 0; i < this->GetChildrenCount(); ++i)
        {
            BehaviorNode* child = (BehaviorNode*) this->GetChild(i);

            if (child == method)
            {
                return i;
            }
        }

        return -1;
    }
    bool Task::IsHTN()
    {
        return this->m_bHTN;
    }

    TaskTask::TaskTask() : SequenceTask()
    {
        this->_planner = BEHAVIAC_NEW Planner();
    }

    TaskTask::~TaskTask()
    {
        BEHAVIAC_DELETE this->_planner;
    }

    void TaskTask::addChild(BehaviorTask* pBehavior)
    {
        super::addChild(pBehavior);
    }

    void TaskTask::Init(const BehaviorNode* node)
    {
        BEHAVIAC_ASSERT(Task::DynamicCast(node) != 0, "node is not an Method");
        Task* pTaskNode = (Task*)(node);

        if (pTaskNode->IsHTN())
        {
            BranchTask::Init(node);

        }
        else
        {
            super::Init(node);
        }
    }

    void TaskTask::copyto(BehaviorTask* taeget) const
    {
        super::copyto(taeget);
    }
    void TaskTask::save(ISerializableNode* node) const
    {
        super::save(node);
    }
    void TaskTask::load(ISerializableNode* node)
    {
        super::load(node);
    }
    bool TaskTask::onenter(Agent* pAgent)
    {
        this->m_activeChildIndex = CompositeTask::InvalidChildIndex;
        BEHAVIAC_ASSERT(this->m_activeChildIndex == CompositeTask::InvalidChildIndex);
        Task* pMethodNode = (Task*)(this->GetNode());

        _planner->Init(pAgent, pMethodNode);

        return super::onenter(pAgent);
    }
    void TaskTask::onexit(Agent* pAgent, EBTStatus s)
    {
        _planner->Uninit();
        super::onexit(pAgent, s);
    }

    EBTStatus TaskTask::update(Agent* pAgent, EBTStatus childStatus)
    {
		EBTStatus status = childStatus;

		if (childStatus == BT_RUNNING) {
			BEHAVIAC_ASSERT(Task::DynamicCast(this->GetNode()) != 0, "node is not an Method");
			Task* pTaskNode = (Task*)(this->GetNode());

			if (pTaskNode->IsHTN())
			{
				status = _planner->Update();
			}
			else
			{
				BEHAVIAC_ASSERT(this->m_children.size() == 1);
				BehaviorTask* c = this->m_children[0];
				status = c->exec(pAgent);
			}
		}
		else {
			BEHAVIAC_ASSERT(true);
		}

		return status;
    }
}
