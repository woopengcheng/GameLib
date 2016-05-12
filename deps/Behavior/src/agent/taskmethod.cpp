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

#include "behaviac/agent/agent.h"
#include "behaviac/agent/taskmethod.h"
#include "behaviac/htn/task.h"

namespace behaviac
{
    CMethodBase* CTaskMethod::clone() const
    {
        return BEHAVIAC_NEW CTaskMethod(*this);
    }
    void CTaskMethod::SetTaskParams(behaviac::Agent* pAgent)
    {
        if (this->m_paramTypes.size() > 0)
        {
            BEHAVIAC_ASSERT(this->m_paramTypes.size() == this->m_params.size());
            const char* agentType = pAgent->GetObjectTypeName();

            AgentProperties* agentT = AgentProperties::Get(agentType);
			BEHAVIAC_ASSERT(agentT);

            for (uint32_t i = 0; i < this->m_paramTypes.size(); ++i)
            {
                behaviac::Property* valueProperty = this->m_params[i];
                BEHAVIAC_ASSERT(valueProperty);

                behaviac::string paramName = FormatString("%s%d", BEHAVIAC_LOCAL_TASK_PARAM_PRE, i);
                this->SetTaskParam(pAgent, agentT, paramName.c_str(), valueProperty);
            }
        }
    }


    void CTaskMethod::SetTaskParam(behaviac::Agent* pAgent, behaviac::AgentProperties* agentT, const char* paramName, const behaviac::Property* valueProperty)
    {
		BEHAVIAC_ASSERT(agentT);
        behaviac::Property* pProperty = agentT->GetLocal(paramName);
		BEHAVIAC_ASSERT(pProperty);
        pProperty->SetFrom(pAgent, valueProperty, pAgent);
    }
}
