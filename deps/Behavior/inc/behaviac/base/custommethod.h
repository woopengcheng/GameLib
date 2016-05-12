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

#ifndef BEHAVIAC_CUSTOMMETHOED
#define BEHAVIAC_CUSTOMMETHOED

#include "behaviac/base/base.h"
#include "behaviac/agent/agent.h"
#include "behaviac/base/object/method.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/base/object/method.h"

//////////////////////////////////////////////////////////////////
// CUSTOM METHOD
//////////////////////////////////////////////////////////////////
namespace behaviac
{
    class BEHAVIAC_API CCustomMethod : public CMethodBase, public CDynamicType
    {
    protected:
        const char* m_className;
        behaviac::vector<behaviac::string> m_paramTypes;
        behaviac::vector<behaviac::Property*> m_params;
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(CCustomMethod);
        BEHAVIAC_DECLARE_ROOT_DYNAMIC_TYPE(CCustomMethod, CDynamicType);

        CCustomMethod(const char* className, const char* eventName)
            : CMethodBase(eventName, className)
        {
            m_className = className;
        }

        virtual ~CCustomMethod();
    protected:
        CCustomMethod(const CCustomMethod& copy)
            : CMethodBase(copy)
        {
            m_className = copy.m_className;
            m_paramTypes = copy.m_paramTypes;
            m_params = copy.m_params;
        }

        Property* LoadFromXML(CTagObject* parent, const ISerializableNode& xmlNode, const char* typeName, const char* paramName);

        virtual void LoadFromXML(CTagObject* parent, const ISerializableNode& xmlNode);
        virtual void SaveToXML(const CTagObject* parent, ISerializableNode& xmlNode)
        {
            BEHAVIAC_UNUSED_VAR(parent);
            BEHAVIAC_UNUSED_VAR(xmlNode);
            BEHAVIAC_ASSERT(0);
        }

        virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
        {
            BEHAVIAC_UNUSED_VAR(types);
            BEHAVIAC_UNUSED_VAR(parent);
            BEHAVIAC_UNUSED_VAR(xmlNode);
            BEHAVIAC_ASSERT(0);
        }

    public:
        void AddParamType(const char* typeName);
        void AddParams(const behaviac::vector<behaviac::string>& params);
        const char* GetClassNameString();
    };

    class BEHAVIAC_API CNamedEvent : public CCustomMethod
    {
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(CNamedEvent);
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(CNamedEvent, CCustomMethod);

        CNamedEvent(const char* className, const char* eventName)
            : CCustomMethod(className, eventName), m_bFired(false)
        {
        }

    protected:
        CNamedEvent(const CNamedEvent& copy)
            : CCustomMethod(copy), m_bFired(copy.m_bFired)
        {
        }

        bool m_bFired;
    public:
        virtual CMethodBase* clone() const
        {
            return BEHAVIAC_NEW CNamedEvent(*this);
        }

        virtual void run(const CTagObject* parent, const CTagObject* parHolder)
        {
            BEHAVIAC_UNUSED_VAR(parent);
            BEHAVIAC_UNUSED_VAR(parHolder);
        }

        virtual bool IsNamedEvent() const
        {
            return true;
        }

        bool IsFired() const
        {
            return this->m_bFired;
        }

        void SetFired(Agent* pAgent, bool bFired);

        template<typename ParamType>
		void SetParam(Agent* pAgent, const ParamType& param);

        template<typename ParamType1, typename ParamType2>
		void SetParam(Agent* pAgent, const ParamType1& param1, const ParamType2& param2);

        template<typename ParamType1, typename ParamType2, typename ParamType3>
		void SetParam(Agent* pAgent, const ParamType1& param1, const ParamType2& param2, const ParamType3& param3);
    };
}
#endif // BEHAVIAC_CUSTOMMETHOED
