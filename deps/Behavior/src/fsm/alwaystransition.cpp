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
#include "behaviac/fsm/alwaystransition.h"
#include "behaviac/behaviortree/attachments/effector.h"

namespace behaviac
{
    bool AlwaysTransition::IsValid(Agent* pAgent, BehaviorTask* pTask) const
    {
        if (AlwaysTransition::DynamicCast(pTask->GetNode()) == 0)
        {
            return false;
        }

        return super::IsValid(pAgent, pTask);
    }

    void AlwaysTransition::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);
    }

	bool AlwaysTransition::Evaluate(Agent* pAgent)
	{
        BEHAVIAC_UNUSED_VAR(pAgent);
        
		return true;
	}
}
