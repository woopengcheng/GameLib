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

#ifndef BEHAVIAC_BEHAVIORTREE_DECORATORCOUNT_H
#define BEHAVIAC_BEHAVIORTREE_DECORATORCOUNT_H

#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"

namespace behaviac
{
    class BEHAVIAC_API DecoratorCount : public DecoratorNode
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorCount, DecoratorNode);

        DecoratorCount();
        virtual ~DecoratorCount();

        virtual void load(int version, const char* agentType, const properties_t& properties);

        virtual int GetCount(Agent* pAgent) const;

    protected:
        Property*	m_count_var;

        friend class DecoratorCountTask;
    };

    class BEHAVIAC_API DecoratorCountTask : public DecoratorTask
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorCountTask, DecoratorTask);

        DecoratorCountTask();

    protected:
        virtual ~DecoratorCountTask();

        virtual void copyto(BehaviorTask* target) const;
        virtual void save(ISerializableNode* node) const;
        virtual void load(ISerializableNode* node);

        virtual bool onenter(Agent* pAgent);

        int GetCount(Agent* pAgent) const;

    protected:
        int			m_n;
    };
    /*! @} */
    /*! @} */
}

#endif//BEHAVIAC_BEHAVIORTREE_DECORATORCOUNT_H
