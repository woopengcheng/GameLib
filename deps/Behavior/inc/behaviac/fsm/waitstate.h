#ifndef BEHAVIAC_FSM_WAITSTATE_H
#define BEHAVIAC_FSM_WAITSTATE_H
#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/property/property.h"
#include "behaviac/fsm/state.h"

namespace behaviac
{
    class Transition;
    // ============================================================================
    class BEHAVIAC_API WaitState : public State
    {
    public:
		BEHAVIAC_DECLARE_DYNAMIC_TYPE(WaitState, State);

		WaitState();
		virtual ~WaitState();

    protected:
		virtual void load(int version, const char* agentType, const properties_t& properties);
        virtual BehaviorTask* createTask() const;

    public:
        virtual bool IsValid(Agent* pAgent, BehaviorTask* pTask) const;

		virtual double GetTime(Agent* pAgent) const;

    protected:
		Property*		m_time_var;
		CMethodBase*	m_time_m;

		friend class WaitStateTask;
    };

	class WaitStateTask : public StateTask
    {
    public:
		WaitStateTask();

		virtual ~WaitStateTask();
        virtual void copyto(BehaviorTask* target) const;
        virtual void save(ISerializableNode* node) const;
        virtual void load(ISerializableNode* node);

    protected:
        virtual bool onenter(Agent* pAgent);
        virtual void onexit(Agent* pAgent, EBTStatus s);
        virtual EBTStatus update(Agent* pAgent, EBTStatus childStatus);

		double	GetTime(Agent* pAgent) const;

		double	m_start;
		double	m_time;
    };
}
#endif//BEHAVIAC_FSM_WAITSTATE_H
