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

#ifndef BEHAVIAC_COMPUTER_H
#define BEHAVIAC_COMPUTER_H

#include "behaviac/base/core/config.h"
#include "behaviac/property/property.h"
#include "behaviac/agent/agent.h"
#include "behaviac/property/property_t.h"
#include "behaviac/property/operators.inl"

#include <vector>
#include <string>

namespace behaviac
{
    //------------------------------------------------------------------------
    class BEHAVIAC_API VariableComputer
    {
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(VariableComputer);
        VariableComputer()
        {}

        VariableComputer(const VariableComputer& copy)
        {
            BEHAVIAC_UNUSED_VAR(copy);
        }

        virtual ~VariableComputer()
        {
        }

        virtual VariableComputer* clone() = 0;

        virtual bool Execute(Agent* pSelf,
                             Property* opl,
                             Property* opr1, CMethodBase* opr1_m,
                             EComputeOperator computeOperator,
                             Property* opr2, CMethodBase* opr2_m) const = 0;
    };

    template< typename VariableType >
    class VariableComputerImpl : public VariableComputer
    {
    protected:
        VariableComputerImpl(const VariableComputerImpl& copy) : VariableComputer(copy)
        {}
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(VariableComputerImpl);
        VariableComputerImpl()
        {}

        virtual VariableComputer* clone()
        {
            return BEHAVIAC_NEW VariableComputerImpl(*this);
        }

        virtual bool Execute(Agent* pSelf,
                             Property* opl,
                             Property* opr1, CMethodBase* opr1_m,
                             EComputeOperator computeOperator,
                             Property* opr2, CMethodBase* opr2_m) const;
    };

    //------------------------------------------------------------------------
    template< typename VariableType >
    bool VariableComputerImpl< VariableType >::Execute(Agent* pSelf,
                                                       Property* _opl,
                                                       Property* _opr1, CMethodBase* _opr1_m,
                                                       EComputeOperator computeOperator,
                                                       Property* _opr2, CMethodBase* _opr2_m) const
    {
        Agent* agentL = _opl->GetParentAgent(pSelf);

        TProperty<VariableType>* pTP_opl = (TProperty<VariableType>*)_opl;

        VariableType opr1;

        if (_opr1)
        {
            TProperty<VariableType>* pTP_opr1 = (TProperty<VariableType>*)_opr1;
            opr1 = pTP_opr1->GetValue(pSelf);
        }
        else
        {
			const Agent* pParent1 = _opr1_m->GetParentAgent(pSelf);
			_opr1_m->run(pParent1, pSelf);
			opr1 = _opr1_m->GetReturnValue<VariableType>(pSelf);
        }

        VariableType opr2;

        if (_opr2)
        {
            TProperty<VariableType>* pTP_opr2 = (TProperty<VariableType>*)_opr2;
            opr2 = pTP_opr2->GetValue(pSelf);
        }
        else
        {
			const Agent* pParent2 = _opr2_m->GetParentAgent(pSelf);
			_opr2_m->run(pParent2, pSelf);
			opr2 = _opr2_m->GetReturnValue<VariableType>(pSelf);
        }

        VariableType result;

        switch (computeOperator)
        {
            case ECO_ADD:
                result = opr1 + opr2;
                pTP_opl->SetValue(agentL, result);
                return true;

            case ECO_SUB:
                result = opr1 - opr2;
                pTP_opl->SetValue(agentL, result);
                return true;

            case ECO_MUL:
                result = opr1 * opr2;
                pTP_opl->SetValue(agentL, result);
                return true;

            case ECO_DIV:
                result = opr1 / opr2;
                pTP_opl->SetValue(agentL, result);
                return true;

            default:
                BEHAVIAC_ASSERT(0, "Unsupported computer type");
                break;
        }

        return false;
    }
}

#endif//BEHAVIAC_COMPUTER_H
