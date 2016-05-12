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

#ifndef BEHAVIAC_BEHAVIORTREE_ATTACHACTION_H
#define BEHAVIAC_BEHAVIORTREE_ATTACHACTION_H

#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/property/property.h"

namespace behaviac
{
    class VariableComparator;

    class BEHAVIAC_API  AttachAction : public BehaviorNode
    {
    public:
        enum EOperatorType
        {
            E_INVALID,
            E_ASSIGN,        // =
            E_ADD,           // +
            E_SUB,           // -
            E_MUL,           // *
            E_DIV,           // /
            E_EQUAL,         // ==
            E_NOTEQUAL,      // !=
            E_GREATER,       // >
            E_LESS,          // <
            E_GREATEREQUAL,  // >=
            E_LESSEQUAL      // <=
        };

        enum TransitionMode
        {
            TM_Condition,
            TM_Success,
            TM_Failure,
            TM_End
        };

        BEHAVIAC_DECLARE_DYNAMIC_TYPE(AttachAction, BehaviorNode);
        AttachAction();
        virtual ~AttachAction();
        class ActionConfig
        {
        public:
            TransitionMode	m_mode;
            behaviac::string m_typeName;
            Property*		m_opl;
            CMethodBase*	m_opl_m;
            Property*		m_opr1;
            CMethodBase*	m_opr1_m;
            EOperatorType	m_operator ;
            Property* 		m_opr2;
            CMethodBase* 	m_opr2_m;

        private:
            VariableComparator* m_comparator;

        protected:
            ActionConfig();
        public:
			BEHAVIAC_DECLARE_MEMORY_OPERATORS(ActionConfig);
			virtual ~ActionConfig();
            virtual bool load(const properties_t& properties);

        public:
            bool Execute(Agent* pAgent) const;
        };
    protected:
        ActionConfig* m_ActionConfig;
        virtual void load(int version, const char* agentType, const properties_t& properties);

    public:
        virtual bool Evaluate(Agent* pAgent);
    protected:
        virtual BehaviorTask* createTask() const;
    };
}

#endif //BEHAVIAC_BEHAVIORTREE_ATTACHACTION_H
