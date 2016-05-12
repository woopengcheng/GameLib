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
#include "behaviac/behaviortree/nodes/decorators/decoratorweight.h"
#include "behaviac/behaviortree/nodes/conditions/condition.h"

namespace behaviac
{
    DecoratorWeight::DecoratorWeight() : m_weight_var(0)
    {}

    DecoratorWeight::~DecoratorWeight()
    {
        BEHAVIAC_DELETE(m_weight_var);
    }

    //Property* LoadRight(const char* value, const behaviac::string& propertyName, behaviac::string& typeName);

    void DecoratorWeight::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        {
            const property_t& p = (*it);

            if (strcmp(p.name, "Weight") == 0)
            {
                behaviac::string typeName;
                behaviac::string propertyName;
                this->m_weight_var = Condition::LoadRight(p.value, typeName);
            }
        }
    }

    int DecoratorWeight::GetWeight(behaviac::Agent* pAgent) const
    {
        if (this->m_weight_var)
        {
            BEHAVIAC_ASSERT(this->m_weight_var);
            TProperty<int>* pP = (TProperty<int>*)this->m_weight_var;
            uint64_t count = pP->GetValue(pAgent);

            return (count == ((uint64_t) - 1) ? -1 : (int)count);
        }

        return 0;
    }

	bool DecoratorWeight::IsManagingChildrenAsSubTrees() const
	{
		return false;
	}

    BehaviorTask* DecoratorWeight::createTask() const
    {
        DecoratorWeightTask* pTask = BEHAVIAC_NEW DecoratorWeightTask();

        return pTask;
    }

    void DecoratorWeightTask::copyto(BehaviorTask* target) const
    {
        super::copyto(target);
    }

    void DecoratorWeightTask::save(ISerializableNode* node) const
    {
        super::save(node);
    }

    void DecoratorWeightTask::load(ISerializableNode* node)
    {
        super::load(node);
    }

    int DecoratorWeightTask::GetWeight(behaviac::Agent* pAgent) const
    {
        BEHAVIAC_ASSERT(DecoratorWeight::DynamicCast(this->GetNode()));
        const DecoratorWeight* pNode = (const DecoratorWeight*)(this->GetNode());

        return pNode ? pNode->GetWeight(pAgent) : 0;
    }

    EBTStatus DecoratorWeightTask::decorate(EBTStatus status)
    {
        return status;
    }
}//namespace behaviac
