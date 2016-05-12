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

#ifndef BEHAVIAC_BEHAVIORTREE_EVENT_H
#define BEHAVIAC_BEHAVIORTREE_EVENT_H

#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/property/property.h"

namespace behaviac
{
    class BEHAVIAC_API Event : public ConditionBase
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(Event, ConditionBase);

        Event();
        virtual ~Event();
        virtual void load(int version, const char* agentType, const properties_t& properties);

        const char*		GetEventName();
        bool			TriggeredOnce();
        TriggerMode		GetTriggerMode();
        void			switchTo(Agent* pAgent);
    protected:
        virtual bool IsValid(Agent* pAgent, BehaviorTask* pTask) const;

    private:
        virtual BehaviorTask* createTask() const;

    protected:
        CMethodBase*				m_event;

        behaviac::string			m_referencedBehaviorPath;

        TriggerMode					m_triggerMode;
        //an event can be configured to stop being checked if triggered
        bool						m_bTriggeredOnce;

        friend class EventetTask;
    };

    class BEHAVIAC_API EventetTask : public AttachmentTask
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(EventetTask, AttachmentTask);

        EventetTask();
        virtual ~EventetTask();

        bool TriggeredOnce() const;
        TriggerMode GetTriggerMode() const;

        const char* GetEventName() const;
    protected:
        virtual void copyto(BehaviorTask* target) const;
        virtual void save(ISerializableNode* node) const;
        virtual void load(ISerializableNode* node);

        virtual bool onenter(Agent* pAgent);
        virtual void onexit(Agent* pAgent, EBTStatus s);
        virtual EBTStatus update(Agent* pAgent, EBTStatus childStatus);

        //virtual bool NeedRestart() const;
    };
    /*! @} */
    /*! @} */
}

#endif//BEHAVIAC_BEHAVIORTREE_EVENT_H
