#ifndef BEHAVIAC_FSM_WAITFRAMESSTATE_H
#define BEHAVIAC_FSM_WAITFRAMESSTATE_H
#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/property/property.h"
#include "behaviac/fsm/state.h"

namespace behaviac
{
	class Transition;
	// ============================================================================
	class BEHAVIAC_API WaitFramesState : public State
	{
	public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(WaitFramesState, State);

		WaitFramesState();
		virtual ~WaitFramesState();
	protected:
		virtual void load(int version, const char* agentType, const properties_t& properties);
		virtual BehaviorTask* createTask() const;
	public:
		virtual bool IsValid(Agent* pAgent, BehaviorTask* pTask) const;

		virtual int GetFrames(Agent* pAgent) const;
	protected:
		Property*		m_frames_var;
		CMethodBase*	m_frames_method;

		friend class WaitFramesStateTask;
	};

	class WaitFramesStateTask : public StateTask
	{
	public:
		WaitFramesStateTask();

		virtual ~WaitFramesStateTask();
		virtual void copyto(BehaviorTask* target) const;
		virtual void save(ISerializableNode* node) const;
		virtual void load(ISerializableNode* node);
	protected:
		virtual bool onenter(Agent* pAgent);
		virtual void onexit(Agent* pAgent, EBTStatus s);
		virtual EBTStatus update(Agent* pAgent, EBTStatus childStatus);

		int GetFrames(Agent* pAgent) const;

		int		m_start;
		int		m_frames;
	};

}
#endif//BEHAVIAC_FSM_WAITFRAMESSTATE_H
