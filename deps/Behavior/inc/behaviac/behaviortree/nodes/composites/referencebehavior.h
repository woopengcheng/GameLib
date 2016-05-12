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

#ifndef BEHAVIAC_BEHAVIORTREE_REFERENCENODE_H
#define BEHAVIAC_BEHAVIORTREE_REFERENCENODE_H

#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/base/object/method.h"

namespace behaviac
{
    /*! \addtogroup treeNodes Behavior Tree
    * @{
    * \addtogroup ReferencedBehavior
    * @{ */
    class Task;
    class CTaskMethod;
    class Transition;

    class BEHAVIAC_API ReferencedBehavior : public BehaviorNode
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(ReferencedBehavior, BehaviorNode);

        ReferencedBehavior();
        virtual ~ReferencedBehavior();
        virtual bool decompose(BehaviorNode* node, PlannerTaskComplex* seqTask, int depth, Planner* planner);
        virtual void load(int version, const char* agentType, const properties_t& properties);
        void SetTaskParams(Agent* pAgent);
		Task* RootTaskNode(Agent* pAgent);

		virtual const char* GetReferencedTree(const Agent* pAgent) const;
        virtual void Attach(BehaviorNode* pAttachment, bool bIsPrecondition, bool bIsEffector, bool bIsTransition);

    protected:
        virtual bool IsValid(Agent* pAgent, BehaviorTask* pTask) const;
    private:
        virtual BehaviorTask* createTask() const;
        Task*							m_taskNode;
    protected:
		Property*		m_referencedBehaviorPath_var;
		CMethodBase*	m_referencedBehaviorPath_m;

        CTaskMethod*					m_taskMethod;
        behaviac::vector<Transition*>*	m_transitions;

        friend class ReferencedBehaviorTask;
    };

    class BEHAVIAC_API ReferencedBehaviorTask : public SingeChildTask
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(ReferencedBehaviorTask, SingeChildTask);

        ReferencedBehaviorTask();
        ~ReferencedBehaviorTask();

        virtual void Init(const BehaviorNode* node);

    protected:
        virtual void copyto(BehaviorTask* target) const;
        virtual void save(ISerializableNode* node) const;
        virtual void load(ISerializableNode* node);

		virtual bool onevent(Agent* pAgent, const char* eventName);

        virtual bool onenter(Agent* pAgent);
        virtual void onexit(Agent* pAgent, EBTStatus s);

        virtual EBTStatus update(Agent* pAgent, EBTStatus childStatus);

        virtual int GetNextStateId() const;
    private:
        int						m_nextStateId;
        BehaviorTreeTask*		m_subTree;
    };
    /*! @} */
    /*! @} */
}

#endif//BEHAVIAC_BEHAVIORTREE_REFERENCENODE_H
