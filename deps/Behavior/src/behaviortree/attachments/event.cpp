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

#include "behaviac/behaviortree/attachments/event.h"
#include "behaviac/behaviortree/nodes/actions/action.h"

namespace behaviac
{
    Event::Event() : m_event(0), m_triggerMode(TM_Transfer), m_bTriggeredOnce(false)
    {
    }

    Event::~Event()
    {
        BEHAVIAC_DELETE(m_event);
    }

    //CMethodBase* LoadMethod(const char* value);

    bool Event::IsValid(Agent* pAgent, BehaviorTask* pTask) const
    {
        if (!Event::DynamicCast(pTask->GetNode()))
        {
            return false;
        }

        return super::IsValid(pAgent, pTask);
    }

    void Event::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        behaviac::string typeName;
        behaviac::string propertyName;
        behaviac::string comparatorName;

        for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        {
            const property_t& p = (*it);

            if (strcmp(p.name, "Task") == 0)
            {
                //method
                this->m_event = Action::LoadMethod(p.value);
            }
            else if (strcmp(p.name, "ReferenceFilename") == 0)
            {
                this->m_referencedBehaviorPath = p.value;
            }
            else if (strcmp(p.name, "TriggeredOnce") == 0)
            {
                if (string_icmp(p.value, "true") == 0)
                {
                    this->m_bTriggeredOnce = true;
                }
            }
            else if (strcmp(p.name, "TriggerMode") == 0)
            {
                if (string_icmp(p.value, "Transfer") == 0)
                {
                    this->m_triggerMode = TM_Transfer;
                }
                else if (string_icmp(p.value, "Return") == 0)
                {
                    this->m_triggerMode = TM_Return;
                }
                else
                {
                    BEHAVIAC_ASSERT(0, "unrecognised trigger mode %s", p.value);
                }
            }
            else
            {
                //BEHAVIAC_ASSERT(0, "unrecognised property %s", p.name);
            }
        }
    }

    const char* Event::GetEventName()
    {
        if (this->m_event != NULL)
        {
            return this->m_event->GetName();
        }

        return NULL;
    }

    bool Event::TriggeredOnce()
    {
        return this->m_bTriggeredOnce;
    }

    TriggerMode Event::GetTriggerMode()
    {
        return m_triggerMode;
    }

    void Event::switchTo(Agent* pAgent)
    {
        if (!StringUtils::IsNullOrEmpty(this->m_referencedBehaviorPath.c_str()))
        {
            if (pAgent != NULL)
            {
                TriggerMode tm = this->GetTriggerMode();

                pAgent->bteventtree(this->m_referencedBehaviorPath.c_str(), tm);
                pAgent->btexec();
            }
        }
    }

    BehaviorTask* Event::createTask() const
    {
        EventetTask* pTask = BEHAVIAC_NEW EventetTask();

        return pTask;
    }

    EventetTask::EventetTask() : AttachmentTask()
    {
    }

    EventetTask::~EventetTask()
    {
    }

    //bool EventetTask::NeedRestart() const
    //{
    //	return true;
    //}

    void EventetTask::copyto(BehaviorTask* target) const
    {
        super::copyto(target);
    }

    void EventetTask::save(ISerializableNode* node) const
    {
        super::save(node);
    }

    void EventetTask::load(ISerializableNode* node)
    {
        super::load(node);
    }

    bool EventetTask::onenter(Agent* pAgent)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);

        return true;
    }

    void EventetTask::onexit(Agent* pAgent, EBTStatus s)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_UNUSED_VAR(s);
    }

    bool EventetTask::TriggeredOnce() const
    {
        const Event* pEventNode = Event::DynamicCast(this->GetNode());
        return pEventNode->m_bTriggeredOnce;
    }

    TriggerMode EventetTask::GetTriggerMode() const
    {
        const Event* pEventNode = Event::DynamicCast(this->GetNode());
        return pEventNode->m_triggerMode;
    }

    const char* EventetTask::GetEventName() const
    {
        const Event* pEventNode = Event::DynamicCast(this->GetNode());
        return pEventNode->m_event ? pEventNode->m_event->GetName() : NULL;
    }

    EBTStatus EventetTask::update(Agent* pAgent, EBTStatus childStatus)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_UNUSED_VAR(childStatus);

        BEHAVIAC_ASSERT(Event::DynamicCast(this->GetNode()));
        const Event* pEventNode = (const Event*)(this->GetNode());

        if (!pEventNode->m_referencedBehaviorPath.empty())
        {
            if (pAgent)
            {
                TriggerMode tm = this->GetTriggerMode();

                pAgent->bteventtree(pEventNode->m_referencedBehaviorPath.c_str(), tm);
                EBTStatus s = pAgent->btexec();
                BEHAVIAC_UNUSED_VAR(s);
            }
        }

        return BT_SUCCESS;
    }
}
