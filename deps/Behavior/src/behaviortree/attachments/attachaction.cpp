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

#include "behaviac/behaviortree/attachments/attachaction.h"

#include "behaviac/behaviortree/nodes/conditions/condition.h"
#include "behaviac/behaviortree/nodes/actions/action.h"
#include "behaviac/behaviortree/nodes/actions/assignment.h"
#include "behaviac/behaviortree/nodes/actions/compute.h"
#include "behaviac/property/comparator.h"

namespace behaviac
{
    AttachAction::ActionConfig::ActionConfig() : m_mode(TM_Condition), m_opl(0), m_opl_m(0), m_opr1(0), m_opr1_m(0), m_opr2(0), m_opr2_m(0)
    {
        m_operator = E_INVALID;
		m_comparator = 0;
    }
 
	AttachAction::ActionConfig::~ActionConfig() {
		BEHAVIAC_DELETE(m_opl_m);
		BEHAVIAC_DELETE(m_opr1_m);
		BEHAVIAC_DELETE(m_opr2_m);
	}

    bool AttachAction::ActionConfig::load(const properties_t& properties)
    {
        string propertyName = "";
        string comparatorName = "";

        for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        {
            const property_t& p = *it;
            if (StringUtils::StrEqual(p.name, "Mode"))
            {
                if (StringUtils::StrEqual(p.value, "Condition"))
                {
                    this->m_mode = TM_Condition;
                }
                else if (StringUtils::StrEqual(p.value, "Success"))
                {
                    this->m_mode = TM_Success;
                }
                else if (StringUtils::StrEqual(p.value, "Failure"))
                {
                    this->m_mode = TM_Failure;
                }
                else if (StringUtils::StrEqual(p.value, "End"))
                {
                    this->m_mode = TM_End;
                }
            }
			else if (StringUtils::StrEqual(p.name, "Opl"))
            {
                if (StringUtils::IsValidString(p.value))
                {
					const char* pParenthesis = StringUtils::StrFind(p.value, '(');

                    if (pParenthesis == 0)
                    {
                        this->m_opl = Condition::LoadRight(p.value, this->m_typeName);
                    }
                    else
                    {
                        this->m_opl_m = Action::LoadMethod(p.value);

                        if (this->m_opl_m)
                        {
                            this->m_opl_m->GetReturnTypeName(this->m_typeName);
                        }
                    }
                }
            }
			else if (StringUtils::StrEqual(p.name, "Opr1"))
            {
                if (StringUtils::IsValidString(p.value))
                {
					const char* pParenthesis = StringUtils::StrFind(p.value, '(');
                    if (pParenthesis == 0)
                    {
                        this->m_opr1 = Condition::LoadRight(p.value, this->m_typeName);
                    }
                    else
                    {
                        this->m_opr1_m = Action::LoadMethod(p.value);

                        if (this->m_opr1_m)
                        {
                            this->m_opr1_m->GetReturnTypeName(this->m_typeName);
                        }
                    }
                }
            }
			else if (StringUtils::StrEqual(p.name, "Operator"))
            {
                comparatorName = p.value;

                if (StringUtils::StrEqual(p.value, "Invalid"))
                {
                    this->m_operator = E_INVALID;
                }
				else if (StringUtils::StrEqual(p.value, "Assign"))
                {
                    this->m_operator = E_ASSIGN;
                }
				else if (StringUtils::StrEqual(p.value, "Add"))
                {
                    this->m_operator = E_ADD;
                }
				else if (StringUtils::StrEqual(p.value, "Sub"))
                {
                    this->m_operator = E_SUB;
                }
				else if (StringUtils::StrEqual(p.value, "Mul"))
                {
                    this->m_operator = E_MUL;
                }
				else if (StringUtils::StrEqual(p.value, "Div"))
                {
                    this->m_operator = E_DIV;
                }
				else if (StringUtils::StrEqual(p.value, "Equal"))
                {
                    this->m_operator = E_EQUAL;
                }
				else if (StringUtils::StrEqual(p.value, "NotEqual"))
                {
                    this->m_operator = E_NOTEQUAL;
                }
				else if (StringUtils::StrEqual(p.value, "Greater"))
                {
                    this->m_operator = E_GREATER;
                }
				else if (StringUtils::StrEqual(p.value, "Less"))
                {
                    this->m_operator = E_LESS;
                }
				else if (StringUtils::StrEqual(p.value, "GreaterEqual"))
                {
                    this->m_operator = E_GREATEREQUAL;
                }
				else if (StringUtils::StrEqual(p.value, "LessEqual"))
                {
                    this->m_operator = E_LESSEQUAL;
                }
            }
			else if (StringUtils::StrEqual(p.name, "Opr2"))
            {
                if (StringUtils::IsValidString(p.value))
                {
					const char* pParenthesis = StringUtils::StrFind(p.value, '(');
                    if (pParenthesis == 0)
                    {
                        this->m_opr2 = Condition::LoadRight(p.value, this->m_typeName);
                    }
                    else
                    {
                        this->m_opr2_m = Action::LoadMethod(p.value);

                        if (this->m_opr2_m)
                        {
                            this->m_opr2_m->GetReturnTypeName(this->m_typeName);
                        }
                    }
                }
            }
            else
            {
                //BEHAVIAC_ASSERT(0, "unrecognised property %s", p.name);
            }
        }

        // compare
        if (this->m_operator >= E_EQUAL && this->m_operator <= E_LESSEQUAL)
        {
            if (comparatorName.length() > 0 && (this->m_opl != NULL || this->m_opl_m != NULL) &&
                (this->m_opr2 != NULL || this->m_opr2_m != NULL))
            {
                this->m_comparator = Condition::Create(this->m_typeName.c_str(), comparatorName.c_str(), this->m_opl, this->m_opl_m, this->m_opr2, this->m_opr2_m);
            }
        }

        return this->m_opl != NULL;
    }

    bool AttachAction::ActionConfig::Execute(Agent* pAgent) const
    {
        bool bValid = false;

        // action
        if (this->m_opl_m != NULL && this->m_operator == E_INVALID)
        {
            bValid = true;
            this->m_opl_m->Invoke(pAgent);
        }
        // assign
        else if (this->m_operator == E_ASSIGN)
        {
            bValid = Assignment::EvaluteAssignment(pAgent, this->m_opl, this->m_opr2, this->m_opr2_m);
        }
        // compute
        else if (this->m_operator >= E_ADD && this->m_operator <= E_DIV)
        {
            EComputeOperator computeOperator = (EComputeOperator)(ECO_ADD + (this->m_operator - E_ADD));
            bValid = Compute::EvaluteCompute(pAgent, this->m_typeName, this->m_opl, this->m_opr1, this->m_opr1_m, computeOperator, this->m_opr2, this->m_opr2_m);
        }
        // compare
        else if (this->m_operator >= E_EQUAL && this->m_operator <= E_LESSEQUAL)
        {
            if (this->m_comparator)
            {
                bValid = this->m_comparator->Execute(pAgent);
            }
        }

        return bValid;
    }

    //implement the methods of AttachAction
	AttachAction::AttachAction() : m_ActionConfig(0)
    {
    }

    AttachAction::~AttachAction()
    {
    }

    void AttachAction::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        this->m_ActionConfig->load(properties);
    }

    bool AttachAction::Evaluate(Agent* pAgent)
    {
        bool bValid = this->m_ActionConfig->Execute((Agent*)pAgent);

        if (!bValid)
        {
            EBTStatus childStatus = BT_INVALID;
            bValid = (BT_SUCCESS == this->update_impl((Agent*)pAgent, childStatus));
        }

        return bValid;
    }

    BehaviorTask* AttachAction::createTask() const
    {
        BEHAVIAC_ASSERT(false);
        return NULL;
    }
}
