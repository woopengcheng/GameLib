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
#include "behaviac/behaviortree/nodes/decorators/decoratorcount.h"
#include "behaviac/behaviortree/nodes/conditions/condition.h"

namespace behaviac
{
    DecoratorCount::DecoratorCount() : m_count_var(0)
    {}

    DecoratorCount::~DecoratorCount()
    {
    }

    //Property* LoadRight(const char* value, const behaviac::string& propertyName, behaviac::string& typeName);

    void DecoratorCount::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
        {
            const property_t& p = (*it);

            if (strcmp(p.name, "Count") == 0)
            {
                behaviac::string typeName;
                behaviac::string  propertyName;
                this->m_count_var = Condition::LoadRight(p.value, typeName);
            }
        }
    }

    int DecoratorCount::GetCount(Agent* pAgent) const
    {
        if (this->m_count_var)
        {
            BEHAVIAC_ASSERT(this->m_count_var);
            TProperty<int>* pP = (TProperty<int>*)this->m_count_var;
            uint64_t count = pP->GetValue(pAgent);

            return (count == ((uint64_t) - 1) ? -1 : (int)count);
        }

        return 0;
    }

    DecoratorCountTask::DecoratorCountTask() : DecoratorTask(), m_n(0)
    {
    }

    DecoratorCountTask::~DecoratorCountTask()
    {
    }

    int DecoratorCountTask::GetCount(Agent* pAgent) const
    {
        BEHAVIAC_ASSERT(DecoratorCount::DynamicCast(this->GetNode()));
        const DecoratorCount* pDecoratorCountNode = (const DecoratorCount*)(this->GetNode());

        return pDecoratorCountNode ? pDecoratorCountNode->GetCount(pAgent) : 0;
    }

    void DecoratorCountTask::copyto(BehaviorTask* target) const
    {
        super::copyto(target);

        BEHAVIAC_ASSERT(DecoratorCountTask::DynamicCast(target));
        DecoratorCountTask* ttask = (DecoratorCountTask*)target;

        ttask->m_n = this->m_n;
    }

    void DecoratorCountTask::save(ISerializableNode* node) const
    {
        super::save(node);

        if (this->m_status != BT_INVALID)
        {
            CSerializationID  countId("count");
            node->setAttr(countId, this->m_n);
        }
    }

    void DecoratorCountTask::load(ISerializableNode* node)
    {
        super::load(node);

        if (this->m_status != BT_INVALID)
        {
            CSerializationID  countId("count");
            behaviac::string attrStr;
            node->getAttr(countId, attrStr);
            StringUtils::FromString(attrStr.c_str(), this->m_n);
        }
    }

    bool DecoratorCountTask::onenter(Agent* pAgent)
    {
        super::onenter(pAgent);

        {
            int count = this->GetCount(pAgent);

            if (count == 0)
            {
                return false;
            }

            this->m_n = count;

        }

        return true;
    }
}//namespace behaviac
