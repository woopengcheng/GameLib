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

#ifndef BEHAVIAC_COMPARISOR_H
#define BEHAVIAC_COMPARISOR_H

#include "behaviac/property/property.h"
#include "behaviac/agent/agent.h"
#include "behaviac/property/property_t.h"
#include "behaviac/property/operators.inl"

#include <vector>
#include <string>

namespace behaviac
{
    //------------------------------------------------------------------------
    enum E_VariableComparisonType
    {
        VariableComparisonType_Assignment,      //( "Assignment (=)" )
        VariableComparisonType_Equal,           //( "Equal (==)" )
        VariableComparisonType_NotEqual,        //( "Not Equal (!=)" )
        VariableComparisonType_Greater,         //( "Greater (>)"  )
        VariableComparisonType_GreaterEqual,    //( "Greater Or Equal (>=)" )
        VariableComparisonType_Less,            //( "Lower (<)"  )
        VariableComparisonType_LessEqual,       //( "Lower Or Equal (<=)" )
        VariableComparisonType_And,             //( "Lower Or Equal (&&)" )
        VariableComparisonType_Or               //( "Lower Or Equal (||)" )
    };

    //------------------------------------------------------------------------
    class BEHAVIAC_API VariableComparator
    {
    public:
		VariableComparator() : m_lhs(0), m_lhs_m(0), m_rhs(0), m_rhs_m(0), m_comparisonType(VariableComparisonType_Assignment)
        {}

		VariableComparator(const VariableComparator& copy) : m_lhs(copy.m_lhs), m_lhs_m(copy.m_lhs_m), m_rhs(copy.m_rhs), m_rhs_m(copy.m_rhs_m), m_comparisonType(copy.m_comparisonType)
        {}

        virtual ~VariableComparator()
        {
            m_lhs = 0;
            m_lhs_m = 0;
            m_rhs = 0;
            m_rhs_m = 0;
        }

        virtual VariableComparator* clone() = 0;

        virtual bool Execute(const Agent* pAgent) const = 0;

        void SetProperty(Property* lhs, CMethodBase* lhs_m, Property* rhs, CMethodBase* rhs_m)
        {
            m_lhs = lhs;
            m_lhs_m = lhs_m;
            m_rhs = rhs;
            m_rhs_m = rhs_m;
        }

        static E_VariableComparisonType ParseComparisonType(const char* comparionOperator);

    protected:
        Property*					m_lhs;
        CMethodBase*				m_lhs_m;
        Property*					m_rhs;
        CMethodBase*				m_rhs_m;
        E_VariableComparisonType	m_comparisonType;
    };

    //------------------------------------------------------------------------
    template< typename VariableType >
    class VariableComparatorImpl_ : public VariableComparator
    {
    protected:
        VariableComparatorImpl_(const VariableComparatorImpl_& copy) : VariableComparator(copy), m_comparisonType(copy.m_comparisonType)
        {}

    public:
        VariableComparatorImpl_();
        virtual ~VariableComparatorImpl_() {}

        virtual VariableComparator* clone() = 0;

        virtual bool Execute(const Agent* pAgent) const = 0;

        void SetType(const E_VariableComparisonType type);
        E_VariableComparisonType GetType() const;

    protected:
        //The operator used in the comparison
        E_VariableComparisonType m_comparisonType;
    };

    //------------------------------------------------------------------------
    template< typename VariableType >
    VariableComparatorImpl_< VariableType >::VariableComparatorImpl_() : m_comparisonType(VariableComparisonType_Equal)
    {}

    //------------------------------------------------------------------------
    template< typename VariableType >
    inline void VariableComparatorImpl_< VariableType >::SetType(const E_VariableComparisonType type)
    {
        m_comparisonType = type;
    }

    //------------------------------------------------------------------------
    template< typename VariableType >
    inline E_VariableComparisonType VariableComparatorImpl_< VariableType >::GetType() const
    {
        return m_comparisonType;
    }

    template< typename VariableType >
    class VariableComparatorImpl : public VariableComparatorImpl_<VariableType>
    {
    protected:
        VariableComparatorImpl(const VariableComparatorImpl& copy) : VariableComparatorImpl_<VariableType>(copy)
        {}
    public:
        VariableComparatorImpl()
        {}

        virtual VariableComparator* clone()
        {
            return BEHAVIAC_NEW VariableComparatorImpl(*this);
        }

        virtual bool Execute(const Agent* pAgent) const;
    };

    //------------------------------------------------------------------------
    template< typename VariableType >
    inline bool VariableComparatorImpl< VariableType >::Execute(const Agent* pAgent) const
    {
        TProperty<VariableType>* pTP_lhs = NULL;
        VariableType lhs;
        Agent* agentL = (Agent*)pAgent;

        if (this->m_lhs)
        {
            agentL = this->m_lhs->GetParentAgent(agentL);
            pTP_lhs = (TProperty<VariableType>*)this->m_lhs;
            lhs = pTP_lhs->GetValue(agentL);
        }
        else
        {
            BEHAVIAC_ASSERT(this->m_lhs_m);
            this->m_lhs_m->Invoke(agentL);
			lhs = this->m_lhs_m->template GetReturnValue<VariableType>(agentL);
        }

        VariableType rhs;
        Agent* agentR = (Agent*)pAgent;

        if (this->m_rhs)
        {
            agentR = this->m_rhs->GetParentAgent(agentR);
            TProperty<VariableType>* pTP_rhs = (TProperty<VariableType>*)this->m_rhs;
            rhs = pTP_rhs->GetValue(agentR);
        }
        else
        {
            BEHAVIAC_ASSERT(this->m_rhs_m);
            this->m_rhs_m->Invoke(agentR);
			rhs = this->m_rhs_m->template GetReturnValue<VariableType>(agentR);
        }

        switch (this->m_comparisonType)
        {
            case VariableComparisonType_Assignment:
                BEHAVIAC_ASSERT(this->m_lhs && pTP_lhs);
				if (this->m_lhs && pTP_lhs)
				{
					pTP_lhs->SetValue(agentL, rhs);
				}

                break;

            case VariableComparisonType_Equal:
                return Details::Equal(lhs, rhs);

            case VariableComparisonType_NotEqual:
                return !Details::Equal(lhs, rhs);

            case VariableComparisonType_Greater:
                return Details::Greater(lhs, rhs);

            case VariableComparisonType_GreaterEqual:
                return Details::GreaterEqual(lhs, rhs);

            case VariableComparisonType_Less:
                return Details::Less(lhs, rhs);

            case VariableComparisonType_LessEqual:
                return Details::LessEqual(lhs, rhs);

            default:
                BEHAVIAC_ASSERT(0, "Unsupported comparison type");
                break;
        }

        return false;
    }

    template<>
    class VariableComparatorImpl<bool> : public VariableComparatorImpl_<bool>
    {
    protected:
        VariableComparatorImpl(const VariableComparatorImpl& copy) : VariableComparatorImpl_<bool>(copy)
        {}
    public:
        VariableComparatorImpl()
        {}

        virtual VariableComparator* clone()
        {
            return BEHAVIAC_NEW VariableComparatorImpl(*this);
        }

        virtual bool Execute(const Agent* pAgent) const
        {
            TProperty<bool>* pTP_lhs = NULL;
            bool lhs = false;
            Agent* agentL = (Agent*)pAgent;

            if (this->m_lhs)
            {
                agentL = this->m_lhs->GetParentAgent(agentL);
                pTP_lhs = (TProperty<bool>*)this->m_lhs;
                lhs = pTP_lhs->GetValue(agentL);
            }
            else
            {
                BEHAVIAC_ASSERT(this->m_lhs_m);
                this->m_lhs_m->Invoke(agentL);
                lhs = this->m_lhs_m->GetReturnValue<bool>(agentL);
            }

            bool rhs = false;
            Agent* agentR = (Agent*)pAgent;

            if (this->m_rhs)
            {
                agentR = this->m_rhs->GetParentAgent(agentR);
                TProperty<bool>* pTP_rhs = (TProperty<bool>*)this->m_rhs;
                rhs = pTP_rhs->GetValue(agentR);
            }
            else
            {
                BEHAVIAC_ASSERT(this->m_rhs_m);
                this->m_rhs_m->Invoke(agentR);
				rhs = this->m_rhs_m->GetReturnValue<bool>(agentR);
            }

            switch (this->m_comparisonType)
            {
                case VariableComparisonType_Assignment:
                    BEHAVIAC_ASSERT(this->m_lhs && pTP_lhs);
                    pTP_lhs->SetValue(agentL, rhs);
                    break;

                case VariableComparisonType_Equal:
                    return Details::Equal(lhs, rhs);

                case VariableComparisonType_NotEqual:
                    return !Details::Equal(lhs, rhs);

                case VariableComparisonType_Greater:
                    return Details::Greater(lhs, rhs);

                case VariableComparisonType_GreaterEqual:
                    return Details::GreaterEqual(lhs, rhs);

                case VariableComparisonType_Less:
                    return Details::Less(lhs, rhs);

                case VariableComparisonType_LessEqual:
                    return Details::LessEqual(lhs, rhs);

                case VariableComparisonType_And:
                    return (lhs && rhs);

                case VariableComparisonType_Or:
                    return (lhs || rhs);

                default:
                    BEHAVIAC_ASSERT(0, "Unsupported comparison type");
                    break;
            }

            return false;
        }
    };
}

#endif//BEHAVIAC_COMPARISOR_H
