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

#ifndef BEHAVIAC_AGENT_TASKMETHOD_H
#define BEHAVIAC_AGENT_TASKMETHOD_H

#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/base/custommethod.h"
#include "behaviac/htn/agentproperties.h"

namespace behaviac
{
    class BEHAVIAC_API CTaskMethod : public CNamedEvent
    {
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(CTaskMethod);
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(CTaskMethod, CNamedEvent);

        CTaskMethod(const char* className, const char* eventName) : CNamedEvent(className, eventName)
        {}
        virtual CMethodBase* clone() const;

        void SetTaskParams(behaviac::Agent* pAgent);

    private:
        CTaskMethod(const CTaskMethod& copy) : CNamedEvent(copy)
        {}
        static void SetTaskParam(behaviac::Agent* pAgent, behaviac::AgentProperties* agentT, const char* paramName, const behaviac::Property* paramValue);
    };
}

#endif//#ifndef BEHAVIAC_AGENT_NAMEDEVENT_H
