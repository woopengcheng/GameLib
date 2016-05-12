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

#ifndef BEHAVIAC_CONTEXT_H
#define BEHAVIAC_CONTEXT_H

#include "behaviac/base/base.h"

#include "behaviac/base/dynamictype.h"
#include "behaviac/base/core/factory.h"
#include "behaviac/base/object/tagobject.h"
#include "behaviac/base/core/string/stringid.h"
#include "behaviac/base/string/stringutils.h"

#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/agent/state.h"
#include "behaviac/agent/context.h"
namespace behaviac
{
    class Property;
    class Agent;
    class BehaviorTreeTask;
    class Variables;
    class State_t;
    class CNamedEvent;

    /*! \addtogroup Agent
    * @{
    * \addtogroup Context
    * @{ */

    /// The Context class
    /*!
    I don't know what is this!
    */
    class BEHAVIAC_API Context
    {
    private:
        typedef behaviac::map<int, Context*> Contexts_t;

        static Contexts_t* ms_contexts;

        void LogCurrentState();

    public:
        static void execAgents(int contextId);
        static Context& GetContext(int contextId);

		void AddAgent(Agent* pAgent);
		void RemoveAgent(Agent* pAgent);

		bool IsExecuting();

        template<typename VariableType>
        const VariableType* GetStaticVariable(const char* staticClassName, uint32_t variableId)
        {
            BEHAVIAC_ASSERT(!StringUtils::IsNullOrEmpty(staticClassName));

            if (m_static_variables.find(staticClassName) == m_static_variables.end())
            {
                m_static_variables[staticClassName] = Variables();
            }

            const Variables& variables = m_static_variables[staticClassName];
            return variables.Get<VariableType>(NULL, false, NULL, variableId);
        }

        /**
        to cleanup the specified context.

        by default, contextId = -1, it cleans up all the contexts
        */
        static void Cleanup(int contextId = -1);

        static void LogCurrentStates(int contextId);

        virtual ~Context();

        void ResetChangedVariables();

        /**
        log changed static variables(propery) for the specified agent class or all agent classes

        @param agentClassName
        if null, it logs for all the agent class
        */
        void LogStaticVariables(const char* agentClassName = 0);

        /**
        if staticClassName is no null, it is for static variable
        */
        template<typename VariableType>
        void SetStaticVariable(const CMemberBase* pMember, const char* variableName, const VariableType& value, const char* staticClassName, uint32_t varableId);

        const CNamedEvent* FindEventStatic(const char* eventName, const char* className);
        void InsertEventGlobal(const char* className, CNamedEvent* pEvent);
        CNamedEvent* FindNamedEventTemplate(const CTagObject::MethodsContainer& methods, const char* eventName);

        /**
        bind 'agentInstanceName' to 'pAgentInstance'.
        'agentInstanceName' should have been registered to the class of 'pAgentInstance' or its parent class.

        @sa RegisterInstanceName
        */
        bool BindInstance(const char* agentInstanceName, Agent* pAgentInstance);

        /**
        unbind 'agentInstanceName' from 'pAgentInstance'.
        'agentInstanceName' should have been bound to 'pAgentInstance'.

        @sa RegisterInstanceName, BindInstance, CreateInstance
        */
        bool UnbindInstance(const char* agentInstanceName);

        Agent* GetInstance(const char* agentInstanceName);

        bool Save(States_t& states);
        bool Load(const States_t& states);

        typedef behaviac::map<int, Agent*> Agents_t;
        struct HeapItem_t
        {
            int priority;
            Agents_t agents;
        };
        behaviac::vector<HeapItem_t> m_agents;
        void SetAgents(behaviac::vector<HeapItem_t> value);
        behaviac::vector<HeapItem_t> GetAgents();

        struct HeapFinder_t
        {
            int priority;
            HeapFinder_t(int p) : priority(p)
            {}

            bool operator()(const HeapItem_t& item) const
            {
                return item.priority == priority;
            }
        };

        struct  HeapCompare_t
        {
            bool operator()(const HeapItem_t& lhs, const HeapItem_t& rhs) const
            {
                return lhs.priority < rhs.priority;
            }
        };

    protected:
        Context(int contextId);

        void CleanupStaticVariables();
        void CleanupInstances();

        void execAgents_();

    private:
		void DelayProcessingAgents();
		void addAgent_(Agent* pAgent);
		void removeAgent_(Agent* pAgent);

		behaviac::vector<Agent*> delayAddedAgents;
		behaviac::vector<Agent*> delayRemovedAgents;

        typedef behaviac::map<behaviac::string, Agent*> NamedAgents_t;
        NamedAgents_t m_namedAgents;

        typedef behaviac::map<behaviac::string, Variables> AgentTypeStaticVariables_t;
        AgentTypeStaticVariables_t	m_static_variables;

        typedef behaviac::map<CStringID, CNamedEvent*> AgentEvents_t;
        typedef behaviac::map<behaviac::string, AgentEvents_t> AgentStaticEvents_t;
        AgentStaticEvents_t	ms_eventInfosGlobal;

        int     m_context_id;
        bool    m_bCreatedByMe;
		bool	m_IsExecuting;
    };
    /*! @} */
    /*! @} */
}

#include "context.inl"

#endif//#ifndef BEHAVIAC_CONTEXT_H
