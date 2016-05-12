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
#include "behaviac/behaviortree/nodes/composites/referencebehavior.h"
#include "behaviac/agent/agent.h"
#include "behaviac/agent/taskmethod.h"
#include "behaviac/behaviortree/nodes/actions/action.h"

#include "behaviac/htn/planner.h"
#include "behaviac/htn/plannertask.h"
#include "behaviac/htn/task.h"
#include "behaviac/fsm/state.h"
#include "behaviac/fsm/transitioncondition.h"
#include "behaviac/behaviortree/nodes/conditions/condition.h"

namespace behaviac
{
	ReferencedBehavior::ReferencedBehavior() : m_taskNode(0), m_referencedBehaviorPath_var(0), m_referencedBehaviorPath_m(0), m_taskMethod(0), m_transitions(0)
    {}

    ReferencedBehavior::~ReferencedBehavior()
    {
		BEHAVIAC_DELETE(m_referencedBehaviorPath_m);
		BEHAVIAC_DELETE(m_taskMethod);
		BEHAVIAC_DELETE this->m_transitions;
    }

    void ReferencedBehavior::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        {
            const property_t& p = (*it);

			if (StringUtils::StrEqual(p.name, "ReferenceBehavior"))
			{
				if (StringUtils::IsValidString(p.value))
				{
					const char* pParenthesis = strchr(p.value, '(');

					if (pParenthesis == 0)
					{
						behaviac::string typeName;
						this->m_referencedBehaviorPath_var = Condition::LoadRight(p.value, typeName);
					}
					else
					{
						this->m_referencedBehaviorPath_m = Action::LoadMethod(p.value);
					}

					const char* szTreePath = this->GetReferencedTree(0);

					//conservatively make it true
					bool bHasEvents = true;

					if (!StringUtils::IsNullOrEmpty(szTreePath))
					{
						//it has a const tree path, so as to load the tree and check if that tree has events
						BehaviorTree* behaviorTree = Workspace::GetInstance()->LoadBehaviorTree(szTreePath);

						if (behaviorTree) {
							bHasEvents = behaviorTree->HasEvents();
						}
					}

					this->m_bHasEvents |= bHasEvents;
				}
			}
            else if (strcmp(p.name, "Task") == 0)
            {
                BEHAVIAC_ASSERT(!StringUtils::IsNullOrEmpty(p.value));
                CMethodBase* m = Action::LoadMethod(p.value);
                //BEHAVIAC_ASSERT(m is CTaskMethod);

                this->m_taskMethod = (CTaskMethod*)m;
            }
            else
            {
                //BEHAVIAC_ASSERT(0, "unrecognised property %s", p.name);
            }
        }
    }

    bool ReferencedBehavior::decompose(BehaviorNode* node, PlannerTaskComplex* seqTask, int depth, Planner* planner)
    {
        bool bOk = false;
        ReferencedBehavior* taskSubTree = (ReferencedBehavior*)node;// as ReferencedBehavior;
        BEHAVIAC_ASSERT(taskSubTree != 0);
        int depth2 = planner->GetAgent()->m_variables.Depth();
        BEHAVIAC_UNUSED_VAR(depth2);
        {
            AgentState::AgentStateScope scopedState(planner->GetAgent()->m_variables.Push(false));
            //planner.agent.Variables.Log(planner.agent, true);
            taskSubTree->SetTaskParams(planner->GetAgent());

			Task* task = taskSubTree->RootTaskNode(planner->GetAgent());

            if (task != 0)
            {
                planner->LogPlanReferenceTreeEnter(planner->GetAgent(), taskSubTree);

                const BehaviorNode* tree = task->GetParent();
                tree->InstantiatePars(planner->GetAgent());

                PlannerTask* childTask = planner->decomposeNode(task, depth);

                if (childTask != 0)
                {
                    seqTask->AddChild(childTask);
                    bOk = true;
                }

                tree->UnInstantiatePars(planner->GetAgent());
                planner->LogPlanReferenceTreeExit(planner->GetAgent(), taskSubTree);
                BEHAVIAC_ASSERT(true);
            }
        }

        BEHAVIAC_ASSERT(planner->GetAgent()->m_variables.Depth() == depth2);
        return bOk;
    }
	
	void ReferencedBehavior::Attach(BehaviorNode* pAttachment, bool bIsPrecondition, bool bIsEffector, bool bIsTransition)
	{
		if (bIsTransition)
		{
			BEHAVIAC_ASSERT(!bIsEffector && !bIsPrecondition);

			if (this->m_transitions == 0)
			{
				this->m_transitions = BEHAVIAC_NEW behaviac::vector<Transition*>();
			}

			BEHAVIAC_ASSERT(Transition::DynamicCast(pAttachment) != 0);
			Transition* pTransition = (Transition*)pAttachment;
			this->m_transitions->push_back(pTransition);

			return;
		}

		BEHAVIAC_ASSERT(bIsTransition == false);
		super::Attach(pAttachment, bIsPrecondition, bIsEffector, bIsTransition);
	}


    bool ReferencedBehavior::IsValid(Agent* pAgent, BehaviorTask* pTask) const
    {
        if (!ReferencedBehavior::DynamicCast(pTask->GetNode()))
        {
            return false;
        }

        return super::IsValid(pAgent, pTask);
    }

    BehaviorTask* ReferencedBehavior::createTask() const
    {
        ReferencedBehaviorTask* pTask = BEHAVIAC_NEW ReferencedBehaviorTask();

        return pTask;
    }

    void ReferencedBehavior::SetTaskParams(Agent* pAgent)
    {
        if (this->m_taskMethod != 0)
        {
            this->m_taskMethod->SetTaskParams(pAgent);
        }
    }

	Task* ReferencedBehavior::RootTaskNode(Agent* pAgent)
    {
        if (this->m_taskNode == 0)
        {
			BehaviorTree* bt = Workspace::GetInstance()->LoadBehaviorTree(this->GetReferencedTree(pAgent));

            if (bt != 0 && bt->GetChildrenCount() == 1)
            {
                BehaviorNode* root = (BehaviorNode*)bt->GetChild(0);
                this->m_taskNode = (Task*)root;
            }
        }

        return this->m_taskNode;
    }

	const char* ReferencedBehavior::GetReferencedTree(const Agent* pAgent) const
    {
		if (this->m_referencedBehaviorPath_var)
		{
			TProperty<behaviac::string>* pTProperty = (TProperty<behaviac::string>*)this->m_referencedBehaviorPath_var;
			const behaviac::string& str = pTProperty->GetValue(pAgent);
			return str.c_str();
		}
		else
		{
			BEHAVIAC_ASSERT(this->m_referencedBehaviorPath_m);
			if (this->m_referencedBehaviorPath_m)
			{
				this->m_referencedBehaviorPath_m->Invoke(pAgent);
				return this->m_referencedBehaviorPath_m->GetReturnString(pAgent);
			}
		}

		return 0;
    }

	ReferencedBehaviorTask::ReferencedBehaviorTask() : SingeChildTask(), m_nextStateId(-1), m_subTree(0)
    {
    }

    ReferencedBehaviorTask::~ReferencedBehaviorTask()
    {
    }

    void ReferencedBehaviorTask::copyto(BehaviorTask* target) const
    {
        super::copyto(target);

        // BEHAVIAC_ASSERT(ReferencedBehaviorTask::DynamicCast(target));
        // ReferencedBehaviorTask* ttask = (ReferencedBehaviorTask*)target;
    }

    void ReferencedBehaviorTask::save(ISerializableNode* node) const
    {
        super::save(node);
    }

    void ReferencedBehaviorTask::load(ISerializableNode* node)
    {
        super::load(node);
    }

    void ReferencedBehaviorTask::Init(const BehaviorNode* node)
    {
        super::Init(node);
    }

	bool ReferencedBehaviorTask::onevent(Agent* pAgent, const char* eventName)
	{
		if (this->m_status == BT_RUNNING && this->m_node->HasEvents())
        {
			BEHAVIAC_ASSERT(this->m_subTree);
            if (!this->m_subTree->onevent(pAgent, eventName))
            {
                return false;
            }
		}

		return true;
	}

    bool ReferencedBehaviorTask::onenter(Agent* pAgent)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
		BEHAVIAC_ASSERT(ReferencedBehavior::DynamicCast(this->m_node) != 0);
		ReferencedBehavior* pNode = (ReferencedBehavior*)this->m_node;

		BEHAVIAC_ASSERT(pNode != 0);

		this->m_nextStateId = -1;

		const char* szTreePath = pNode->GetReferencedTree(pAgent);
		this->m_subTree = Workspace::GetInstance()->CreateBehaviorTreeTask(szTreePath);
		pNode->SetTaskParams(pAgent);

        return true;
    }

    void ReferencedBehaviorTask::onexit(Agent* pAgent, EBTStatus s)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_UNUSED_VAR(s);
    }

	int ReferencedBehaviorTask::GetNextStateId() const
	{
		return this->m_nextStateId;
	}

    EBTStatus ReferencedBehaviorTask::update(Agent* pAgent, EBTStatus childStatus)
    {
        BEHAVIAC_UNUSED_VAR(childStatus);
		BEHAVIAC_ASSERT(ReferencedBehavior::DynamicCast(this->GetNode()));
		const ReferencedBehavior* pNode = (const ReferencedBehavior*)this->m_node;
		BEHAVIAC_ASSERT(pNode);

#if !BEHAVIAC_RELEASE
		pAgent->m_debug_count++;
		if (pAgent->m_debug_count > 20)
		{
			BEHAVIAC_LOGWARNING("%s might be in a recurrsive inter calling of trees\n", pAgent->GetName().c_str());
			BEHAVIAC_ASSERT(false);
		}
#endif//#if !BEHAVIAC_RELEASE

		EBTStatus result = this->m_subTree->exec(pAgent);

		bool bTransitioned = State::UpdateTransitions(pAgent, pNode, pNode->m_transitions, this->m_nextStateId);

		if (bTransitioned)
		{
			result = BT_SUCCESS;
		}

		return result;
    }
}//namespace behaviac
