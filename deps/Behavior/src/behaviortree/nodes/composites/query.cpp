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
#include "behaviac/agent/agent.h"
#include "behaviac/behaviortree/nodes/composites/query.h"

#include "behaviac/agent/registermacros.h"
#include "behaviac/behaviortree/propertymember.h"

namespace behaviac
{
    Query::Query()
    {}

    Query::~Query()
    {
        this->m_descriptors.clear();
    }

    Property* LoadLeft(const char* value, behaviac::string& propertyName, const char* constValue);

    BEGIN_PROPERTIES_DESCRIPTION(Query::Descriptor_t)
    {
        REGISTER_PROPERTY(Attribute);
        //REGISTER_PROPERTY_V4(Attribute, "Attribute", EPersistenceType_Description | EPersistenceType_UiInfo,
        REGISTER_PROPERTY(Reference);
        REGISTER_PROPERTY(Weight);
    }
    END_PROPERTIES_DESCRIPTION()

    void Query::load(int version, const char* agentType, const properties_t& properties)
    {
        super::load(version, agentType, properties);

        if (properties.size() > 0)
        {
            for (propertie_const_iterator_t it = properties.begin(); it != properties.end(); ++it)
            {
                const property_t& p = (*it);

                if (strcmp(p.name, "Domain") == 0)
                {
                    m_domain = p.value;
                }
                else if (strcmp(p.name, "Descriptors") == 0)
                {
                    SetDescriptors(p.value);
                }
                else
                {
                    //BEHAVIAC_ASSERT(0, "unrecognised property %s", p.name);
                }
            }
        }
    }

    BehaviorTask* Query::createTask() const
    {
        QueryTask* pTask = BEHAVIAC_NEW QueryTask();

        return pTask;
    }

    const Query::Descriptors_t& Query::GetDescriptors() const
    {
        return this->m_descriptors;
    }

    void Query::SetDescriptors(const char* descriptors)
    {
        behaviac::StringUtils::FromString(descriptors, this->m_descriptors);

        //for (size_t i = 0; i < this->m_descriptors.size(); ++i)
        //{
        //    Descriptor_t& d = this->m_descriptors[i];
        //    d.Attribute->SetDefaultValue(d.Reference);
        //}
    }

    bool Query::PropertyFinder_t::operator()(const BehaviorTree::Descriptor_t& other)
    {
        if (tofind.Attribute->GetVariableId() == other.Descriptor->GetVariableId())
        {
            return true;
        }

        return false;
    }

    const Property* Query::FindProperty(const Query::Descriptor_t& q, const BehaviorTree::Descriptors_t& c)
    {
        BehaviorTree::Descriptors_t::const_iterator it = std::find_if(c.begin(), c.end(), PropertyFinder_t(q));

        if (it != c.end())
        {
            return it->Reference;
        }

        return 0;
    }

    float Query::ComputeSimilarity(const Query::Descriptors_t& q, const BehaviorTree::Descriptors_t& c) const
    {
        float similarity = 0.0f;

        for (size_t i = 0; i < q.size(); ++i)
        {
            const Descriptor_t& qi = q[i];

            const Property* ci = FindProperty(qi, c);

            if (ci)
            {
				float dp = qi.Attribute->DifferencePercentage(qi.Reference, ci);

                BEHAVIAC_ASSERT(dp >= 0.0f && dp <= 1.0f, "dp should be normalized to [0, 1], please check its scale");

                similarity += (1.0f - dp) * qi.Weight;
            }
        }

        return similarity;
    }

    QueryTask::QueryTask() : SingeChildTask()
    {
    }

    void QueryTask::Init(const BehaviorNode* node)
    {
        super::Init(node);
    }

    void QueryTask::copyto(BehaviorTask* target) const
    {
        super::copyto(target);

        // BEHAVIAC_ASSERT(QueryTask::DynamicCast(target));
        // QueryTask* ttask = (QueryTask*)target;
    }

    void QueryTask::save(ISerializableNode* node) const
    {
        super::save(node);
    }

    void QueryTask::load(ISerializableNode* node)
    {
        super::load(node);
    }

    QueryTask::~QueryTask()
    {
    }

    bool QueryTask::ReQuery(Agent* pAgent)
    {
        const Query* pQueryNode = Query::DynamicCast(this->GetNode());

        if (pQueryNode)
        {
            const Query::Descriptors_t& qd = pQueryNode->GetDescriptors();

            if (qd.size() > 0)
            {
                const Workspace::BehaviorTrees_t& bs = Workspace::GetInstance()->GetBehaviorTrees();

                BehaviorTree* btFound = 0;
                float similarityMax = -1.0f;

                for (Workspace::BehaviorTrees_t::const_iterator it = bs.begin();
                     it != bs.end(); ++it)
                {
                    BehaviorTree* bt = it->second;

                    const behaviac::string& domains = bt->GetDomains();

                    if (pQueryNode->m_domain.empty() || domains.find(pQueryNode->m_domain) != behaviac::string::npos)
                    {
                        const BehaviorTree::Descriptors_t& bd = bt->GetDescriptors();

                        float similarity = pQueryNode->ComputeSimilarity(qd, bd);

                        if (similarity > similarityMax)
                        {
                            similarityMax = similarity;
                            btFound = bt;
                        }
                    }
                }

                if (btFound)
                {
                    //BehaviorTreeTask* pAgentCurrentBT = pAgent->btgetcurrent();
                    //if (pAgentCurrentBT && pAgentCurrentBT->GetName() != btFound->GetName())
                    {
                        const char* pReferencedTree = btFound->GetName().c_str();
                        pAgent->btreferencetree(pReferencedTree);

                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool QueryTask::onenter(Agent* pAgent)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        //this->m_root = 0;

        if (this->ReQuery(pAgent))
        {
            return true;
        }

        //return false;
        return true;
    }

    void QueryTask::onexit(Agent* pAgent, EBTStatus s)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_UNUSED_VAR(s);
    }
    EBTStatus QueryTask::update(Agent* pAgent, EBTStatus childStatus)
    {
        BEHAVIAC_UNUSED_VAR(childStatus);
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_ASSERT(pAgent);

        //the selected tree is set as 'TM_Return', when the selected tree is finished, to use its exec status then
        return BT_RUNNING;
    }
}
