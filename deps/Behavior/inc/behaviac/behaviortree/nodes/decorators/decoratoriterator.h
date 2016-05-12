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

#ifndef BEHAVIAC_BEHAVIORTREE_DECORATORINTERATOR_H
#define BEHAVIAC_BEHAVIORTREE_DECORATORINTERATOR_H

#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"

#include "behaviac/behaviortree/nodes/conditions/condition.h"
#include "behaviac/behaviortree/nodes/actions/action.h"

namespace behaviac
{
    class BEHAVIAC_API DecoratorIterator : public DecoratorNode
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorIterator, DecoratorNode);

        DecoratorIterator();
        virtual ~DecoratorIterator();

    protected:
        Property* m_opl;
        Property* m_opr;
        CMethodBase* m_opr_m;
    public:
        virtual bool decompose(BehaviorNode* node, PlannerTaskComplex* seqTask, int depth, Planner* planner);

        virtual void load(int version, const char* agentType, const properties_t& properties);

        virtual bool IsValid(Agent* pAgent, BehaviorTask* pTask) const;

        bool IterateIt(Agent* pAgent, int index, int& count);
    protected:
        virtual BehaviorTask* createTask() const;
    };
}

#endif//BEHAVIAC_BEHAVIORTREE_DECORATORWEIGHT_H
