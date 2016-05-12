#include "behaviac/fsm/waitframesstate.h"
#include "behaviac/behaviortree/attachments/effector.h"
#include "behaviac/fsm/startcondition.h"
#include "behaviac/fsm/transitioncondition.h"
#include "behaviac/agent/agent.h"
#include "behaviac/behaviortree/nodes/conditions/condition.h"

namespace behaviac
{
	WaitFramesState::WaitFramesState() : m_frames_var(0), m_frames_method(0)
	{
	}

	WaitFramesState::~WaitFramesState()
	{
        BEHAVIAC_DELETE(this->m_frames_method);
	}

	void WaitFramesState::load(int version, const char* agentType, const properties_t& properties)
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

	int WaitFramesState::GetFrames(Agent* pAgent) const
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

	bool WaitFramesState::IsValid(Agent* pAgent, BehaviorTask* pTask) const
	{
		if (WaitFramesState::DynamicCast(pTask->GetNode()) == 0)
		{
			return false;
		}

		return super::IsValid(pAgent, pTask);
	}

	BehaviorTask* WaitFramesState::createTask() const
	{
		WaitFramesStateTask* pTask = BEHAVIAC_NEW WaitFramesStateTask();

		return pTask;
	}

	WaitFramesStateTask::WaitFramesStateTask() : StateTask(), m_start(0), m_frames(0)
	{
	}

	WaitFramesStateTask::~WaitFramesStateTask()
	{
	}

	void WaitFramesStateTask::copyto(BehaviorTask* target) const
	{
		super::copyto(target);

		BEHAVIAC_ASSERT(WaitFramesStateTask::DynamicCast(target));
		WaitFramesStateTask* ttask = (WaitFramesStateTask*)target;
		ttask->m_start = this->m_start;
		ttask->m_frames = this->m_frames;
	}

	void WaitFramesStateTask::save(ISerializableNode* node) const
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

	void WaitFramesStateTask::load(ISerializableNode* node)
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

	int WaitFramesStateTask::GetFrames(Agent* pAgent) const
	{
		BEHAVIAC_ASSERT(WaitFramesState::DynamicCast(this->GetNode()));
		const WaitFramesState* pWaitNode = (const WaitFramesState*)(this->GetNode());

		return pWaitNode ? pWaitNode->GetFrames(pAgent) : 0;
	}

	bool WaitFramesStateTask::onenter(Agent* pAgent)
	{
		BEHAVIAC_UNUSED_VAR(pAgent);

        this->m_nextStateId = -1;

		this->m_start = Workspace::GetInstance()->GetFrameSinceStartup();
		this->m_frames = this->GetFrames(pAgent);

		if (this->m_frames <= 0)
		{
			return false;
		}

		return true;
	}

	void WaitFramesStateTask::onexit(Agent* pAgent, EBTStatus s)
	{
		BEHAVIAC_UNUSED_VAR(pAgent);
		BEHAVIAC_UNUSED_VAR(s);
	}

	EBTStatus WaitFramesStateTask::update(Agent* pAgent, EBTStatus childStatus)
	{
		BEHAVIAC_UNUSED_VAR(pAgent);
		BEHAVIAC_UNUSED_VAR(childStatus);

		if (Workspace::GetInstance()->GetFrameSinceStartup() - this->m_start + 1 >= this->m_frames)
		{
			BEHAVIAC_ASSERT(WaitFramesState::DynamicCast(this->GetNode()) != 0, "node is not an WaitFramesState");
			WaitFramesState* pStateNode = (WaitFramesState*)(this->GetNode());

			pStateNode->Update(pAgent, this->m_nextStateId);

			return BT_SUCCESS;
		}

		return BT_RUNNING;
	}
}
