#ifndef BEHAVIAC_FSM_H
#define BEHAVIAC_FSM_H
#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"

namespace behaviac
{
    // ============================================================================
    class BEHAVIAC_API FSM : public BehaviorNode
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(FSM, BehaviorNode);

    public:
        FSM();
        virtual ~FSM();

        void SetInitialId(int initialId);
        int GetInitialId() const;
    protected:
        virtual void load(int version, const char* agentType, const properties_t& properties);
        virtual BehaviorTask* createTask() const;
        virtual bool IsValid(Agent* pAgent, BehaviorTask* pTask) const;
    private:
        int		m_initialId;
    };

    class BEHAVIAC_API FSMTask : public CompositeTask
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(FSMTask, CompositeTask);

        FSMTask();
        virtual ~FSMTask();

    protected:
        virtual void copyto(BehaviorTask* target) const;
        virtual void save(ISerializableNode* node) const;
        virtual void load(ISerializableNode* node);

        virtual bool onenter(Agent* pAgent);
        virtual void onexit(Agent* pAgent, EBTStatus s);

        virtual EBTStatus update_current(Agent* pAgent, EBTStatus childStatus);
        virtual EBTStatus update(Agent* pAgent, EBTStatus childStatus);

        EBTStatus UpdateFSM(Agent* pAgent, EBTStatus childStatus);
    };

}
#endif
