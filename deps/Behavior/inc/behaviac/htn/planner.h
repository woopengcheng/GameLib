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

#ifndef BEHAVIAC_HTN_PLANNER_H
#define BEHAVIAC_HTN_PLANNER_H
#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/property/property.h"

namespace behaviac
{
    class PlannerTask;
    class DecoratorIterator;
    class PlannerTaskComplex;
    class Task;
    class ReferencedBehavior;
    class Planner
    {
    private:
		/// <summary>
		/// Gets or sets the agent script instance that taskner will be generating plans for
		/// </summary>
        Agent* agent;
        /// <summary>
        /// Gets or sets whether the planner will automatically perform periodic replanning
        /// </summary>
        bool AutoReplan;

        Task* m_rootTaskNode;
        PlannerTask* m_rootTask;

    public:
        void Init(Agent* pAgent, Task* rootTask);

        void Uninit();
        Planner() : agent(0), m_rootTaskNode(0), m_rootTask(0)
        {
            AutoReplan = true;
        }
    private:
        void OnDisable();

    public:
        EBTStatus Update();

        /// <summary>
        /// Generate a new task for the <paramref name="agent"/> based on the current world state as
        /// described by the <paramref name="agentState"/>.
        /// </summary>
        /// <param name="agent">The agent for which the task is being generated. This object instance must be
        /// of the same type as the type for which the Task was developed</param>
        /// <param name="agentState">The current world state required by the planner</param>
        /// <returns></returns>
    private:
        PlannerTask* GeneratePlan();
        bool canInterruptCurrentPlan();

        void doAutoPlanning();
        behaviac::string GetTickInfo(const behaviac::Agent* pAgent, const behaviac::BehaviorTask* b, const char* action);
    public:
        Agent* GetAgent()
        {
            return this->agent;
        }
        void LogPlanBegin(Agent* a, Task* root);

        void LogPlanEnd(Agent* a, Task* root);

        void LogPlanNodeBegin(Agent* a, BehaviorNode* n);
        void LogPlanNodePreconditionFailed(Agent* a, BehaviorNode* n);
        void LogPlanNodeEnd(Agent* a, BehaviorNode* n, const behaviac::string& result);

        void LogPlanReferenceTreeEnter(Agent* a, ReferencedBehavior* referencedNode);

        void LogPlanReferenceTreeExit(Agent* a, ReferencedBehavior* referencedNode);
        void LogPlanMethodBegin(Agent* a, BehaviorNode* m);

        void LogPlanMethodEnd(Agent* a, BehaviorNode* m, const behaviac::string& result);

        void LogPlanForEachBegin(Agent* a, DecoratorIterator* pForEach, int index, int count);

        void LogPlanForEachEnd(Agent* a, DecoratorIterator* pForEach, int index, int count, const behaviac::string& result);
        PlannerTask* BuildPlan(Task* root);

        PlannerTask* decomposeNode(BehaviorNode* node, int depth);
        bool decomposeComplex(BehaviorNode* node, PlannerTaskComplex* seqTask, int depth);

    public:
        PlannerTask* decomposeTask(Task* task, int depth);
    };
}

#endif
