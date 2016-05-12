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

#ifndef BEHAVIAC_BEHAVIORTREE_CONDITION_H
#define BEHAVIAC_BEHAVIORTREE_CONDITION_H

#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/property/property.h"

#include "behaviac/property/comparator.h"
#include "behaviac/behaviortree/nodes/conditions/conditionbase.h"

namespace behaviac
{
    /*! \addtogroup treeNodes Behavior Tree
    * @{
    * \addtogroup Condition
    * @{ */

    /**
    Condition node can compare the value of right and left. return Failure or Success
    */

    class BEHAVIAC_API Condition : public ConditionBase
    {
    private:
        friend class TypeRegister;

        template<typename T>
        static bool Register(const char* typeName)
        {
            ComparatorCreators()[typeName] = &_Creator<T>;

            return true;
        }

        template<typename T>
        static void UnRegister(const char* typeName)
        {
            ComparatorCreators().erase(typeName);
        }
    public:
        static VariableComparator* Create(const char* typeName, const char* comparionOperator, Property* lhs, CMethodBase* lhs_m, Property* rhs, CMethodBase* rhs_m);

        static void Cleanup();

        static Property* LoadLeft(const char* value);
        static Property* LoadRight(const char* value, behaviac::string& typeName);
        static Property* ParseProperty(const char* value, behaviac::string& typeName);

    private:
        template<typename T>
        static VariableComparator* _Creator(E_VariableComparisonType comparisonType,
                                            const Property* lhs, const CMethodBase* lhs_m, const Property* rhs, const CMethodBase* rhs_m)
        {
            typedef VariableComparatorImpl<T> VariableComparatorType;
            VariableComparatorType* pComparator = BEHAVIAC_NEW VariableComparatorType;
            pComparator->SetType(comparisonType);
            pComparator->SetProperty(const_cast<Property*>(lhs), const_cast<CMethodBase*>(lhs_m), const_cast<Property*>(rhs), const_cast<CMethodBase*>(rhs_m));
            return pComparator;
        }

        typedef VariableComparator* VariableComparatorCreator(E_VariableComparisonType comparisonType, const Property* lhs, const CMethodBase* lhs_m, const Property* rhs, const CMethodBase* rhs_m);
        typedef behaviac::map<behaviac::string, VariableComparatorCreator*> VariableComparators;
        typedef VariableComparators::iterator VariableComparatorIterator;
        static VariableComparators* ms_comparatorCreators;
        static VariableComparators& ComparatorCreators();

    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(Condition, ConditionBase);

        Condition();
        virtual ~Condition();
        virtual void load(int version, const char* agentType, const properties_t& properties);
        virtual bool Evaluate(Agent* pAgent);

    protected:
        virtual bool IsValid(Agent* pAgent, BehaviorTask* pTask) const;

    private:
        virtual BehaviorTask* createTask() const;

    protected:
        Property*			m_opl;
        CMethodBase*		m_opl_m;
        Property*			m_opr;
        CMethodBase*		m_opr_m;
        VariableComparator* m_comparator;

        friend class ConditionTask;
    };

    class BEHAVIAC_API ConditionTask : public ConditionBaseTask
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(ConditionTask, ConditionBaseTask);

        ConditionTask() : ConditionBaseTask()
        {
        }

        virtual ~ConditionTask()
        {
        }

    protected:
        virtual void copyto(BehaviorTask* target) const;
        virtual void save(ISerializableNode* node) const;
        virtual void load(ISerializableNode* node);

        virtual bool onenter(Agent* pAgent);
        virtual void onexit(Agent* pAgent, EBTStatus s);
        virtual EBTStatus update(Agent* pAgent, EBTStatus childStatus);
    };
    /*! @} */
    /*! @} */
}

#endif//BEHAVIAC_BEHAVIORTREE_CONDITION_H
