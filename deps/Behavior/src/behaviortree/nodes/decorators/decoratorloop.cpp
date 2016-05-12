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
#include "behaviac/behaviortree/nodes/decorators/decoratorloop.h"

#include "behaviac/htn/planner.h"
#include "behaviac/htn/plannertask.h"

namespace behaviac
{
    DecoratorLoop::DecoratorLoop()
    {
    }

    DecoratorLoop::~DecoratorLoop()
    {
    }

    void DecoratorLoop::load(int version, const char* agentType, const properties_t& properties)
    {
        DecoratorCount::load(version, agentType, properties);
    }

    bool DecoratorLoop::decompose(BehaviorNode* node, PlannerTaskComplex* seqTask, int depth, Planner* planner)
    {
        bool bOk = false;
        DecoratorLoop* loop = (DecoratorLoop*)node;
        int childCount = loop->GetChildrenCount();
        BEHAVIAC_UNUSED_VAR(childCount);
        BEHAVIAC_ASSERT(childCount == 1);
        BehaviorNode* childNode = (BehaviorNode*)loop->GetChild(0);
        PlannerTask* childTask = planner->decomposeNode(childNode, depth);

        if (childTask != NULL)
        {
            seqTask->AddChild(childTask);
            bOk = true;
        }

        return bOk;
    }

    bool DecoratorLoop::IsValid(Agent* pAgent, BehaviorTask* pTask) const
    {
        if (!DecoratorLoop::DynamicCast(pTask->GetNode()))
        {
            return false;
        }

        return super::IsValid(pAgent, pTask);
    }

    BehaviorTask* DecoratorLoop::createTask() const
    {
        DecoratorLoopTask* pTask = BEHAVIAC_NEW DecoratorLoopTask();

        return pTask;
    }

    DecoratorLoopTask::DecoratorLoopTask() : DecoratorCountTask()
    {
    }

    DecoratorLoopTask::~DecoratorLoopTask()
    {
    }

    void DecoratorLoopTask::copyto(BehaviorTask* target) const
    {
        super::copyto(target);
    }

    void DecoratorLoopTask::save(ISerializableNode* node) const
    {
        super::save(node);
    }

    void DecoratorLoopTask::load(ISerializableNode* node)
    {
        super::load(node);
    }

    EBTStatus DecoratorLoopTask::decorate(EBTStatus status)
    {
        BEHAVIAC_UNUSED_VAR(status);

        if (this->m_n > 0)
        {
            this->m_n--;

            if (this->m_n == 0)
            {
                return BT_SUCCESS;
            }

            return BT_RUNNING;
        }

        if (this->m_n == -1)
        {
            return BT_RUNNING;
        }

        BEHAVIAC_ASSERT(this->m_n == 0);

        return BT_SUCCESS;
    }
}//namespace behaviac
