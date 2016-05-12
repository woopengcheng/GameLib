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
#include "behaviac/property/property_t.h"
#include "behaviac/behaviortree/nodes/actions/compute.h"

#include "behaviac/base/core/profiler/profiler.h"
#include "behaviac/behaviortree/nodes/actions/action.h"
#include "behaviac/behaviortree/nodes/conditions/condition.h"

namespace behaviac
{
    Compute::VariableComputers* Compute::ms_computers;

    Compute::VariableComputers& Compute::Computers()
    {
        if (!ms_computers)
        {
            ms_computers = BEHAVIAC_NEW Compute::VariableComputers;
        }

        return *ms_computers;
    }

    void Compute::Cleanup()
    {
        if (ms_computers)
        {
            ms_computers->clear();
            BEHAVIAC_DELETE(ms_computers);
            ms_computers = 0;
        }
    }

    void Compute::RegisterBasicTypes()
    {
#undef BEHAVIAC_DECLARE_PRIMITE_TYPE
#define BEHAVIAC_DECLARE_PRIMITE_TYPE(type, typeName)				\
    Register<type>(#typeName);

        M_PRIMITIVE_NUMBER_TYPES();
    }

    void Compute::UnRegisterBasicTypes()
    {
#undef BEHAVIAC_DECLARE_PRIMITE_TYPE
#define BEHAVIAC_DECLARE_PRIMITE_TYPE(type, typeName)				\
    UnRegister<type>(#typeName);

        M_PRIMITIVE_NUMBER_TYPES();
    }

    Compute::Compute() : m_opl(0), m_opr1(0), m_opr1_m(0), m_opr2(0), m_opr2_m(0), m_operator(ECO_INVALID)
    {
    }

    Compute::~Compute()
    {
        BEHAVIAC_DELETE(m_opr1_m);
        BEHAVIAC_DELETE(m_opr2_m);
    }

    ////CMethodBase* LoadMethod(const char* value);
    //Property* LoadLeft(const char* value, behaviac::string& propertyName, const char* constValue);
    //Property* LoadRight(const char* value, const behaviac::string& propertyName, behaviac::string& typeName);

    void Compute::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        behaviac::string typeName;
        behaviac::string propertyName;

        for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        {
            const property_t& p = (*it);

            if (strcmp(p.name, "Opl") == 0)
            {
                this->m_opl = Condition::LoadLeft(p.value);
            }
            else if (strcmp(p.name, "Operator") == 0)
            {
                if (strcmp(p.value, "Add") == 0)
                {
                    this->m_operator = ECO_ADD;
                }
                else if (strcmp(p.value, "Sub") == 0)
                {
                    this->m_operator = ECO_SUB;
                }
                else if (strcmp(p.value, "Mul") == 0)
                {
                    this->m_operator = ECO_MUL;
                }
                else if (strcmp(p.value, "Div") == 0)
                {
                    this->m_operator = ECO_DIV;
                }
                else
                {
                    BEHAVIAC_ASSERT(0);
                }
            }
            else if (strcmp(p.name, "Opr1") == 0)
            {
                const char* pParenthesis = strchr(p.value, '(');

                if (pParenthesis == 0)
                {
                    this->m_opr1 = Condition::LoadRight(p.value, typeName);
                }
                else
                {
                    //method
                    this->m_opr1_m = Action::LoadMethod(p.value);
                }
            }
            else if (strcmp(p.name, "Opr2") == 0)
            {
                const char* pParenthesis = strchr(p.value, '(');

                if (pParenthesis == 0)
                {
                    this->m_opr2 = Condition::LoadRight(p.value, typeName);
                }
                else
                {
                    //method
                    this->m_opr2_m = Action::LoadMethod(p.value);
                }
            }
            else
            {
                //BEHAVIAC_ASSERT(0, "unrecognised property %s", p.name);
            }
        }

        BEHAVIAC_ASSERT(this->m_operator != ECO_INVALID);
        this->m_typeName = typeName;
    }

    bool Compute::EvaluteCompute(Agent* pAgent, const behaviac::string& typeName, Property* opl, Property* opr1, CMethodBase* opr1_m, EComputeOperator computeOperator, Property* opr2, CMethodBase* opr2_m)
    {
        bool bValid = false;

        if (typeName != "")
        {
            VariableComputer* pComputer = Computers()[typeName];

            if (pComputer)
            {
                bValid = pComputer->Execute(pAgent, opl, opr1, opr1_m, computeOperator, opr2, opr2_m);
            }
        }

        return bValid;
    }

    bool Compute::IsValid(Agent* pAgent, BehaviorTask* pTask) const
    {
        if (!Compute::DynamicCast(pTask->GetNode()))
        {
            return false;
        }

        return super::IsValid(pAgent, pTask);
    }

    BehaviorTask* Compute::createTask() const
    {
        ComputeTask* pTask = BEHAVIAC_NEW ComputeTask();

        return pTask;
    }

    ComputeTask::ComputeTask() : LeafTask()
    {
    }

    ComputeTask::~ComputeTask()
    {
    }

    void ComputeTask::copyto(BehaviorTask* target) const
    {
        super::copyto(target);
    }

    void ComputeTask::save(ISerializableNode* node) const
    {
        super::save(node);
    }

    void ComputeTask::load(ISerializableNode* node)
    {
        super::load(node);
    }


    bool ComputeTask::onenter(Agent* pAgent)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        return true;
    }

    void ComputeTask::onexit(Agent* pAgent, EBTStatus s)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_UNUSED_VAR(s);
    }
    EBTStatus ComputeTask::update(Agent* pAgent, EBTStatus childStatus)
    {
        BEHAVIAC_ASSERT(childStatus == BT_RUNNING);

        EBTStatus result = BT_SUCCESS;

        BEHAVIAC_ASSERT(Compute::DynamicCast(this->GetNode()));
        Compute* pComputeNode = (Compute*)(this->GetNode());

        bool bValid = Compute::EvaluteCompute(pAgent, pComputeNode->m_typeName, pComputeNode->m_opl, pComputeNode->m_opr1, pComputeNode->m_opr1_m,
                                              pComputeNode->m_operator, pComputeNode->m_opr2, pComputeNode->m_opr2_m);

        if (!bValid)
        {
            result = pComputeNode->update_impl(pAgent, childStatus);
        }

        return result;
    }
}
