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

#ifndef BEHAVIAC_BEHAVIORTREE_PARALLEL_H
#define BEHAVIAC_BEHAVIORTREE_PARALLEL_H

#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"

#include <vector>

namespace behaviac
{
    /*! \addtogroup treeNodes Behavior Tree
    * @{
    * \addtogroup Parallel
    * @{ */

    /// Enumerates the options for when a parallel node is considered to have failed.
    /**
    - FAIL_ON_ONE indicates that the node will return failure as soon as one of its children fails.
    - FAIL_ON_ALL indicates that all of the node's children must fail before it returns failure.

    If FAIL_ON_ONE and SUCEED_ON_ONE are both active and are both trigerred in the same time step, failure will take precedence.
    */
    enum FAILURE_POLICY
    {
        FAIL_ON_ONE,
        FAIL_ON_ALL
    };

    /// Enumerates the options for when a parallel node is considered to have succeeded.
    /**
    - SUCCEED_ON_ONE indicates that the node will return success as soon as one of its children succeeds.
    - SUCCEED_ON_ALL indicates that all of the node's children must succeed before it returns success.
    */
    enum SUCCESS_POLICY
    {
        SUCCEED_ON_ONE,
        SUCCEED_ON_ALL
    };

    /// Enumerates the options when a parallel node is exited
    /**
    - EXIT_NONE indicates that the parallel node just exit.
    - EXIT_ABORT_RUNNINGSIBLINGS indicates that the parallel node abort all other running siblings.
    */
    enum EXIT_POLICY
    {
        EXIT_NONE,
        EXIT_ABORT_RUNNINGSIBLINGS
    };

    /// Enumerates the options of what to do when a child finishes
    /**
    - CHILDFINISH_ONCE indicates that the child node just executes once.
    - CHILDFINISH_LOOP indicates that the child node run again and again.
    */
    enum CHILDFINISH_POLICY
    {
        CHILDFINISH_ONCE,
        CHILDFINISH_LOOP
    };

    class BEHAVIAC_API Parallel : public BehaviorNode
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(Parallel, BehaviorNode);

        Parallel();
        virtual ~Parallel();
        virtual bool decompose(BehaviorNode* node, PlannerTaskComplex* seqTask, int depth, Planner* planner);
        virtual void load(int version, const char* agentType, const properties_t& properties);
        EBTStatus ParallelUpdate(Agent* pAgent, behaviac::vector<BehaviorTask*>children);
        virtual bool IsManagingChildrenAsSubTrees() const;
    protected:
        virtual bool IsValid(Agent* pAgent, BehaviorTask* pTask) const;

        void SetPolicy(FAILURE_POLICY failPolicy = FAIL_ON_ALL, SUCCESS_POLICY successPolicy = SUCCEED_ON_ALL, EXIT_POLICY exitPolicty = EXIT_NONE)
        {
            m_failPolicy = failPolicy;
            m_succeedPolicy = successPolicy;
            m_exitPolicy = exitPolicty;
        }

    private:
        virtual BehaviorTask* createTask() const;

    protected:
        FAILURE_POLICY		m_failPolicy;
        SUCCESS_POLICY		m_succeedPolicy;
        EXIT_POLICY			m_exitPolicy;
        CHILDFINISH_POLICY	m_childFinishPolicy;

        friend class ParallelTask;
    };

    ///Execute behaviors in parallel
    /** There are two policies that control the flow of execution. The first is the policy for failure,
    and the second is the policy for success.

    For failure, the options are "fail when one child fails" and "fail when all children fail".
    For success, the options are similarly "complete when one child completes", and "complete when all children complete".
    */
    class BEHAVIAC_API ParallelTask : public CompositeTask
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(ParallelTask, CompositeTask);
        ParallelTask();
        virtual ~ParallelTask();

    protected:
        virtual void Init(const BehaviorNode* node);
        virtual void copyto(BehaviorTask* target) const;
        virtual void save(ISerializableNode* node) const;
        virtual void load(ISerializableNode* node);

        virtual bool onenter(Agent* pAgent);
        virtual void onexit(Agent* pAgent, EBTStatus s);
        virtual EBTStatus update(Agent* pAgent, EBTStatus childStatus);
        virtual EBTStatus update_current(Agent* pAgent, EBTStatus childStatus);
    };
    /*! @} */
    /*! @} */
}

#endif//BEHAVIAC_BEHAVIORTREE_PARALLEL_Hrap
