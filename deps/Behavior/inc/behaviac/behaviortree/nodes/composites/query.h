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

#ifndef BEHAVIAC_BEHAVIORTREE_QUERY_H
#define BEHAVIAC_BEHAVIORTREE_QUERY_H

#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"

namespace behaviac
{
    /*! \addtogroup treeNodes Behavior Tree
    * @{
    * \addtogroup Query
    * @{ */

    /**
    Sub trees are queried
    */
    class BEHAVIAC_API Query : public BehaviorNode
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(Query, BehaviorNode);

        Query();
        virtual ~Query();
        virtual void load(int version, const char* agentType, const properties_t& properties);

    private:
        virtual BehaviorTask* createTask() const;

    protected:
        behaviac::string			m_domain;

        struct Descriptor_t
        {
            Property*	Attribute;
            Property*	Reference;
            float		Weight;

            Descriptor_t() : Attribute(0), Reference(0), Weight(0.0f)
            {}

            Descriptor_t(const Descriptor_t& copy) : Attribute(copy.Attribute), Reference(copy.Reference), Weight(copy.Weight)
            {}

            ~Descriptor_t()
            {
            }

            DECLARE_BEHAVIAC_STRUCT(Query::Descriptor_t);
        };

        typedef behaviac::vector<Descriptor_t> Descriptors_t;

        Descriptors_t				m_descriptors;

        struct PropertyFinder_t
        {
            PropertyFinder_t& operator=(const PropertyFinder_t&);
            const Descriptor_t& tofind;

            PropertyFinder_t(const Descriptor_t& q) : tofind(q)
            {}

            bool operator()(const BehaviorTree::Descriptor_t& other);
        };

        static const Property* FindProperty(const Descriptor_t& q, const BehaviorTree::Descriptors_t& c);

        const Descriptors_t& GetDescriptors() const;
        void SetDescriptors(const char* descriptors);

        float ComputeSimilarity(const Descriptors_t& q, const BehaviorTree::Descriptors_t& c) const;

        friend class QueryTask;
    };

    class BEHAVIAC_API QueryTask : public SingeChildTask
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(QueryTask, SingeChildTask);

        QueryTask();
        virtual void Init(const BehaviorNode* node);

    protected:
        virtual ~QueryTask();

        virtual void copyto(BehaviorTask* target) const;
        virtual void save(ISerializableNode* node) const;
        virtual void load(ISerializableNode* node);

        virtual bool onenter(Agent* pAgent);
        virtual void onexit(Agent* pAgent, EBTStatus s);
        virtual EBTStatus update(Agent* pAgent, EBTStatus childStatus);

    private:

        bool ReQuery(Agent* pAgent);
    };
    /*! @} */
    /*! @} */
}

#endif//BEHAVIAC_BEHAVIORTREE_QUERY_H
