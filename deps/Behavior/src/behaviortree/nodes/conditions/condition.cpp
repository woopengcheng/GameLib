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
#include "behaviac/behaviortree/nodes/conditions/condition.h"

#include "behaviac/base/core/profiler/profiler.h"
#include "behaviac/behaviortree/nodes/actions/action.h"

namespace behaviac
{
    Condition::Condition() : m_opl(0), m_opl_m(0), m_opr(0), m_opr_m(0), m_comparator(0)
    {
    }

    Condition::~Condition()
    {
        BEHAVIAC_DELETE(m_opl_m);
        BEHAVIAC_DELETE(m_opr_m);
        BEHAVIAC_DELETE(m_comparator);
    }


    Property* Condition::ParseProperty(const char* value, behaviac::string& typeName)
    {
        Property* opr = NULL;

        behaviac::vector<behaviac::string> tokens = StringUtils::SplitTokens(value);

        if (tokens[0] == "static")
        {
            //static int Property1
            typeName = tokens[1];

            if (tokens.size() == 3)
            {
                opr = Property::Create(typeName.c_str(), tokens[2].c_str(), true, NULL);
            }
            else
            {
                BEHAVIAC_ASSERT(tokens.size() == 4);
                opr = Property::Create(typeName.c_str(), tokens[2].c_str(), true, tokens[3].c_str());
            }
        }
        else
        {
            //int Property1
            typeName = tokens[0];

            if (tokens.size() == 2)
            {
                opr = Property::Create(typeName.c_str(), tokens[1].c_str(), false, NULL);
            }
            else
            {
                opr = Property::Create(typeName.c_str(), tokens[1].c_str(), false, tokens[2].c_str());
            }
        }

        return opr;
    }

    Property* Condition::LoadLeft(const char* value)
    {
        Property* opl = NULL;

        if (value[0] != '0')
        {
            behaviac::string typeName;
            opl = ParseProperty(value, typeName);
        }

        return opl;
    }

    Property* Condition::LoadRight(const char* value, behaviac::string& typeName)
    {
        Property* opr = 0;

        if (value && value[0] != '\0')
        {
            if (StringUtils::StartsWith(value, "const"))
            {
                //const Int32 0
                const int kConstLength = 5;
                const char* strRemaining = value + (kConstLength + 1);
                const char* p = StringUtils::FirstToken(strRemaining, ' ', typeName);

                const char* strVale = (p + 1);
                opr = Property::Create(typeName.c_str(), strVale);
            }
            else
            {
                opr = ParseProperty(value, typeName);
            }
        }

        return opr;
    }

    void Condition::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        behaviac::string typeName;
        behaviac::string comparatorName;

        for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        {
            const property_t& p = (*it);

            if (strcmp(p.name, "Operator") == 0)
            {
                comparatorName = p.value;
            }
            else if (strcmp(p.name, "Opl") == 0)
            {
                const char* pParenthesis = strchr(p.value, '(');

                if (pParenthesis == 0)
                {
                    this->m_opl = LoadLeft(p.value);
                }
                else
                {
                    this->m_opl_m = Action::LoadMethod(p.value);
                }
            }
            else if (strcmp(p.name, "Opr") == 0)
            {
                const char* pParenthesis = strchr(p.value, '(');

                if (pParenthesis == 0)
                {
                    this->m_opr = LoadRight(p.value, typeName);
                }
                else
                {
                    this->m_opr_m = Action::LoadMethod(p.value);

                    if (this->m_opr_m)
                    {
                        this->m_opr_m->GetReturnTypeName(typeName);
                    }
                }
            }
            else
            {
                //BEHAVIAC_ASSERT(0, "unrecognised property %s", p.name);
            }
        }

        if (!comparatorName.empty() && (this->m_opl || this->m_opl_m) && (this->m_opr || this->m_opr_m))
        {
            this->m_comparator = Condition::Create(typeName.c_str(), comparatorName.c_str(), this->m_opl, this->m_opl_m, this->m_opr, this->m_opr_m);
        }
    }

    bool Condition::IsValid(Agent* pAgent, BehaviorTask* pTask) const
    {
        if (!Condition::DynamicCast(pTask->GetNode()))
        {
            return false;
        }

        return super::IsValid(pAgent, pTask);
    }

    bool Condition::Evaluate(Agent* pAgent)
    {
        if (this->m_comparator != NULL)
        {
            return this->m_comparator->Execute(pAgent);
        }
        else
        {
            EBTStatus childStatus = BT_INVALID;
            EBTStatus result = this->update_impl((Agent*)pAgent, childStatus);
            return result == BT_SUCCESS;
        }
    }

    BehaviorTask* Condition::createTask() const
    {
        ConditionTask* pTask = BEHAVIAC_NEW ConditionTask();

        return pTask;
    }

    Condition::VariableComparators* Condition::ms_comparatorCreators;

    Condition::VariableComparators& Condition::ComparatorCreators()
    {
        if (!ms_comparatorCreators)
        {
            ms_comparatorCreators = BEHAVIAC_NEW Condition::VariableComparators;
        }

        return *ms_comparatorCreators;
    }

    void Condition::Cleanup()
    {
        if (ms_comparatorCreators)
        {
            ms_comparatorCreators->clear();
            BEHAVIAC_DELETE(ms_comparatorCreators);
            ms_comparatorCreators = 0;
        }
    }

    VariableComparator* Condition::Create(const char* typeName, const char* comparionOperator, Property* lhs, CMethodBase* lhs_m, Property* rhs, CMethodBase* rhs_m)
    {
        E_VariableComparisonType comparisonType = VariableComparator::ParseComparisonType(comparionOperator);

        bool bAgentPtr = false;

        //it might be par or the right value of condition/assignment
        if (Agent::IsAgentClassName(typeName))
        {
            bAgentPtr = true;
            typeName = "void*";
        }

        BEHAVIAC_UNUSED_VAR(bAgentPtr);

        VariableComparatorCreator* pCreator = ComparatorCreators()[typeName];

        if (pCreator)
        {
            VariableComparator* pComparator = (*pCreator)(comparisonType, lhs, lhs_m, rhs, rhs_m);
            return pComparator;
        }
        else
        {
            BEHAVIAC_LOGWARNING("please add Condition::Register<%s>(\"%s\") in your code\n", typeName, typeName, typeName);
            BEHAVIAC_ASSERT(0);
        }

        return 0;
    }

    void ConditionTask::copyto(BehaviorTask* target) const
    {
        super::copyto(target);
    }

    void ConditionTask::save(ISerializableNode* node) const
    {
        super::save(node);
    }

    void ConditionTask::load(ISerializableNode* node)
    {
        super::load(node);
    }

    bool ConditionTask::onenter(Agent* pAgent)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        return true;
    }

    void ConditionTask::onexit(Agent* pAgent, EBTStatus s)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_UNUSED_VAR(s);
    }

    EBTStatus ConditionTask::update(Agent* pAgent, EBTStatus childStatus)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_UNUSED_VAR(childStatus);

        // EBTStatus result = BT_FAILURE;

        BEHAVIAC_ASSERT(Condition::DynamicCast(this->GetNode()));
        Condition* pConditionNode = (Condition*)(this->GetNode());

        bool ret = pConditionNode->Evaluate(pAgent);

        return ret ? BT_SUCCESS : BT_FAILURE;
    }
}
