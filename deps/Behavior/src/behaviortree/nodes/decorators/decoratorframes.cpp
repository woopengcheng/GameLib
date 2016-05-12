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
#include "behaviac/behaviortree/nodes/decorators/decoratorframes.h"
#include "behaviac/agent/agent.h"
#include "behaviac/behaviortree/nodes/actions/action.h"
#include "behaviac/behaviortree/nodes/conditions/condition.h"

namespace behaviac
{
	DecoratorFrames::DecoratorFrames() : m_frames_var(0), m_frames_method(0)
    {
    }

    DecoratorFrames::~DecoratorFrames()
    {
		BEHAVIAC_DELETE(this->m_frames_method);
	}

    void DecoratorFrames::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        {
			const property_t& p = (*it);

			if (!strcmp(p.name, "Frames"))
			{
				const char* pParenthesis = strchr(p.value, '(');

				if (pParenthesis == 0)
				{
					behaviac::string typeName;
					behaviac::string propertyName;
					this->m_frames_var = Condition::LoadRight(p.value, typeName);
				}
				else
				{
					//method
					this->m_frames_method = Action::LoadMethod(p.value);
				}
			}
        }
    }

    int DecoratorFrames::GetFrames(Agent* pAgent) const
    {
		if (this->m_frames_var)
		{
			BEHAVIAC_ASSERT(this->m_frames_var);
			TProperty<int>* pP = (TProperty<int>*)this->m_frames_var;
			uint64_t frames = pP->GetValue(pAgent);

			return (frames == ((uint64_t)-1) ? -1 : (int)frames);

		}
		else if (this->m_frames_method)
		{
			Agent* pParent = Agent::GetInstance(pAgent, this->m_frames_method->GetInstanceNameString());
			BEHAVIAC_ASSERT(pParent);

			this->m_frames_method->run(pParent, pAgent);

			int frames = this->m_frames_method->GetReturnValue<int>(pParent);

			return frames;
		}

        return 0;
    }

    BehaviorTask* DecoratorFrames::createTask() const
    {
        DecoratorFramesTask* pTask = BEHAVIAC_NEW DecoratorFramesTask();

        return pTask;
    }

    DecoratorFramesTask::DecoratorFramesTask() : DecoratorTask(), m_start(0), m_frames(0)
    {
    }

    DecoratorFramesTask::~DecoratorFramesTask()
    {
    }

    int DecoratorFramesTask::GetFrames(Agent* pAgent) const
    {
        BEHAVIAC_ASSERT(DecoratorFrames::DynamicCast(this->GetNode()));
        const DecoratorFrames* pNode = (const DecoratorFrames*)(this->GetNode());

        return pNode ? pNode->GetFrames(pAgent) : 0;
    }

    void DecoratorFramesTask::copyto(BehaviorTask* target) const
    {
        super::copyto(target);

        BEHAVIAC_ASSERT(DecoratorFramesTask::DynamicCast(target));
        DecoratorFramesTask* ttask = (DecoratorFramesTask*)target;

        ttask->m_start = this->m_start;
        ttask->m_frames = this->m_frames;
    }

    void DecoratorFramesTask::save(ISerializableNode* node) const
    {
        super::save(node);

        if (this->m_status != BT_INVALID)
        {
            CSerializationID  startId("start");
            node->setAttr(startId, this->m_start);

            CSerializationID  framesId("frames");
            node->setAttr(framesId, this->m_frames);
        }
    }

    void DecoratorFramesTask::load(ISerializableNode* node)
    {
        super::load(node);

        if (this->m_status != BT_INVALID)
        {
            CSerializationID  startId("start");
            behaviac::string attrStr;
            node->getAttr(startId, attrStr);
            StringUtils::FromString(attrStr.c_str(), this->m_start);

            CSerializationID  framesId("frames");
            node->getAttr(framesId, attrStr);
            StringUtils::FromString(attrStr.c_str(), this->m_frames);
        }
    }

    bool DecoratorFramesTask::onenter(Agent* pAgent)
    {
        super::onenter(pAgent);

        this->m_start = Workspace::GetInstance()->GetFrameSinceStartup();
        this->m_frames = this->GetFrames(pAgent);

        return (this->m_frames > 0);
    }

    EBTStatus DecoratorFramesTask::decorate(EBTStatus status)
    {
        BEHAVIAC_UNUSED_VAR(status);

		if (Workspace::GetInstance()->GetFrameSinceStartup() - this->m_start + 1 >= this->m_frames)
		{
			return BT_SUCCESS;
		}

        return BT_RUNNING;
    }
}//namespace behaviac
