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
namespace behaviac
{
    template<typename TAGENT>
    BEHAVIAC_FORCEINLINE bool Agent::IsRegistered()
    {
        const char* agentClassName = TAGENT::GetClassTypeName();
        return IsRegistered(agentClassName);
    }

	template<typename TAGENT>
    BEHAVIAC_FORCEINLINE TAGENT* Agent::GetAgentInstance(const char* agentInstanceName, int contextId, bool& bToBind)
    {
        TAGENT* pA = 0;
        bToBind = false;

        if (Agent::IsInstanceNameRegistered(agentInstanceName))
        {
            Context& c = Context::GetContext(contextId);

            Agent* a = c.GetInstance(agentInstanceName);

            if (a)
            {
                BEHAVIAC_ASSERT(TAGENT::DynamicCast(a) != 0);
                pA = (TAGENT*)a;
            }
            else
            {
                //instance will be created so that it need to bind it to the name
                bToBind = true;
            }
        }

		return pA;
	}

	BEHAVIAC_FORCEINLINE void Agent::InitAgent(Agent* pAgent, const char* agentInstanceName, const char* agentInstanceNameAny, bool bToBind, int contextId, short priority)
	{
		const char* szAgentInstanceName = (agentInstanceName || bToBind) ? agentInstanceNameAny : agentInstanceName;

		Init_(contextId, pAgent, priority, szAgentInstanceName);

		if (bToBind)
		{
			Context& c = Context::GetContext(contextId);
			c.BindInstance(agentInstanceNameAny, pAgent);
		}
	}

	template<typename TAGENT>
    BEHAVIAC_FORCEINLINE TAGENT* Agent::Create(const char* agentInstanceName, int contextId, short priority)
    {
        const char* agentInstanceNameAny = agentInstanceName;
        if (!agentInstanceName)
        {
            agentInstanceNameAny = TAGENT::GetClassTypeName();
        }

		bool bToBind = false;
		TAGENT* pA = GetAgentInstance<TAGENT>(agentInstanceNameAny, contextId, bToBind);

        if (pA == 0)
        {
            //TAGENT should be derived from Agent
            Agent* pAgent = BEHAVIAC_NEW TAGENT();
            BEHAVIAC_ASSERT(TAGENT::DynamicCast(pAgent));

			pA = (TAGENT*)pAgent;
			InitAgent(pA, agentInstanceName, agentInstanceNameAny, bToBind, contextId, priority);
        }

        return pA;
    }

	template<typename TAGENT, typename T1>
    BEHAVIAC_FORCEINLINE TAGENT* Agent::Create(T1 p1, const char* agentInstanceName, int contextId, short priority)
    {
        const char* agentInstanceNameAny = agentInstanceName;
        if (!agentInstanceName)
        {
            agentInstanceNameAny = TAGENT::GetClassTypeName();
        }

		bool bToBind = false;
		TAGENT* pA = GetAgentInstance<TAGENT>(agentInstanceNameAny, contextId, bToBind);

        if (pA == 0)
        {
            //TAGENT should be derived from Agent
            Agent* pAgent = BEHAVIAC_NEW TAGENT(p1);
            BEHAVIAC_ASSERT(TAGENT::DynamicCast(pAgent));

			pA = (TAGENT*)pAgent;
			InitAgent(pA, agentInstanceName, agentInstanceNameAny, bToBind, contextId, priority);
        }

        return pA;
    }

	template<typename TAGENT, typename T1, typename T2>
    BEHAVIAC_FORCEINLINE TAGENT* Agent::Create(T1 p1, T2 p2, const char* agentInstanceName, int contextId, short priority)
    {
        const char* agentInstanceNameAny = agentInstanceName;
        if (!agentInstanceName)
        {
            agentInstanceNameAny = TAGENT::GetClassTypeName();
        }

		bool bToBind = false;
		TAGENT* pA = GetAgentInstance<TAGENT>(agentInstanceNameAny, contextId, bToBind);

        if (pA == 0)
        {
            //TAGENT should be derived from Agent
            Agent* pAgent = BEHAVIAC_NEW TAGENT(p1, p2);
            BEHAVIAC_ASSERT(TAGENT::DynamicCast(pAgent));

			pA = (TAGENT*)pAgent;
			InitAgent(pA, agentInstanceName, agentInstanceNameAny, bToBind, contextId, priority);
        }

        return pA;
    }

	template<typename TAGENT, typename T1, typename T2, typename T3>
    BEHAVIAC_FORCEINLINE TAGENT* Agent::Create(T1 p1, T2 p2, T3 p3, const char* agentInstanceName, int contextId, short priority)
    {
        const char* agentInstanceNameAny = agentInstanceName;
        if (!agentInstanceName)
        {
            agentInstanceNameAny = TAGENT::GetClassTypeName();
        }

		bool bToBind = false;
		TAGENT* pA = GetAgentInstance<TAGENT>(agentInstanceNameAny, contextId, bToBind);

        if (pA == 0)
        {
            //TAGENT should be derived from Agent
            Agent* pAgent = BEHAVIAC_NEW TAGENT(p1, p2, p3);
            BEHAVIAC_ASSERT(TAGENT::DynamicCast(pAgent));

			pA = (TAGENT*)pAgent;
			InitAgent(pA, agentInstanceName, agentInstanceNameAny, bToBind, contextId, priority);
        }

        return pA;
    }

	template<typename TAGENT, typename T1, typename T2, typename T3, typename T4>
    BEHAVIAC_FORCEINLINE TAGENT* Agent::Create(T1 p1, T2 p2, T3 p3, T4 p4, const char* agentInstanceName, int contextId, short priority)
    {
        const char* agentInstanceNameAny = agentInstanceName;
        if (!agentInstanceName)
        {
            agentInstanceNameAny = TAGENT::GetClassTypeName();
        }

		bool bToBind = false;
		TAGENT* pA = GetAgentInstance<TAGENT>(agentInstanceNameAny, contextId, bToBind);

        if (pA == 0)
        {
            //TAGENT should be derived from Agent
            Agent* pAgent = BEHAVIAC_NEW TAGENT(p1, p2, p3, p4);
            BEHAVIAC_ASSERT(TAGENT::DynamicCast(pAgent));

			pA = (TAGENT*)pAgent;
			InitAgent(pA, agentInstanceName, agentInstanceNameAny, bToBind, contextId, priority);
        }

        return pA;
    }

    BEHAVIAC_FORCEINLINE void Agent::Destroy(Agent* pAgent)
    {
        if (pAgent)
        {
            const char* agentInstanceNameAny = pAgent->GetName().c_str();

            int contextId = pAgent->GetContextId();
            Context& c = Context::GetContext(contextId);
            Agent* a = c.GetInstance(agentInstanceNameAny);

            if (a && a == pAgent)
            {
                BEHAVIAC_ASSERT(Agent::IsInstanceNameRegistered(agentInstanceNameAny), "Don't UnRegisterInstanceName before Destory!");
                c.UnbindInstance(agentInstanceNameAny);
            }

            BEHAVIAC_DELETE(pAgent);
        }
    }

    template<typename TAGENT>
    BEHAVIAC_FORCEINLINE TAGENT* Agent::GetInstance(const char* agentInstanceName, int contextId)
    {
        const char* agentInstanceNameAny = agentInstanceName;

        if (!agentInstanceNameAny)
        {
            agentInstanceNameAny = TAGENT::GetClassTypeName();
        }

        Agent* a = Agent::GetInstance(agentInstanceNameAny, contextId);

        BEHAVIAC_ASSERT(!a || TAGENT::DynamicCast(a));
        TAGENT* pA = (TAGENT*)a;
        return pA;
    }

    BEHAVIAC_FORCEINLINE void Agent::FireEvent(Agent* pAgent, const char* eventName)
    {
        if (pAgent)
        {
            CNamedEvent* pEvent = pAgent->findEvent(eventName);

            if (!pEvent)
            {
                int contextId = pAgent->GetContextId();
                const CTagObjectDescriptor& meta = pAgent->GetDescriptor();
                pEvent = findNamedEventTemplate(meta.ms_methods, eventName, contextId);
            }

            if (pEvent)
            {
                pEvent->SetFired(pAgent, true);
            }
            else
            {
                BEHAVIAC_ASSERT(0, "unregistered event %s", eventName);
            }
        }
    }

    template<class ParamType1>
    BEHAVIAC_FORCEINLINE void Agent::FireEvent(Agent* pAgent, const char* eventName, const ParamType1& param1)
    {
        if (pAgent)
        {
            CNamedEvent* pEvent = pAgent->findEvent(eventName);

            if (!pEvent)
            {
                int contextId = pAgent->GetContextId();
                const CTagObjectDescriptor& meta = pAgent->GetDescriptor();
                pEvent = findNamedEventTemplate(meta.ms_methods, eventName, contextId);
            }

            if (pEvent)
            {
                if (pEvent)
                {
                    pEvent->SetParam(pAgent, param1);
                }
                else
                {
                    BEHAVIAC_ASSERT(0, "unregistered parameters %s", eventName);
                }

                pEvent->SetFired(pAgent, true);
            }
            else
            {
                BEHAVIAC_ASSERT(0, "unregistered event %s", eventName);
            }
        }
    }

    template<class ParamType1, class ParamType2>
    BEHAVIAC_FORCEINLINE void Agent::FireEvent(Agent* pAgent, const char* eventName, const ParamType1& param1, const ParamType2& param2)
    {
        if (pAgent)
        {
            CNamedEvent* pEvent = pAgent->findEvent(eventName);

            if (!pEvent)
            {
                int contextId = pAgent->GetContextId();
                const CTagObjectDescriptor& meta = pAgent->GetDescriptor();
                pEvent = findNamedEventTemplate(meta.ms_methods, eventName, contextId);
            }

            if (pEvent)
            {
                if (pEvent)
                {
                    pEvent->SetParam(pAgent, param1, param2);
                }
                else
                {
                    BEHAVIAC_ASSERT(0, "unregistered parameters %s", eventName);
                }

                pEvent->SetFired(pAgent, true);
            }
            else
            {
                BEHAVIAC_ASSERT(0, "unregistered event %s", eventName);
            }
        }
    }

    template<class ParamType1, class ParamType2, class ParamType3>
    BEHAVIAC_FORCEINLINE void Agent::FireEvent(Agent* pAgent, const char* eventName, const ParamType1& param1, const ParamType2& param2, const ParamType3& param3)
    {
        if (pAgent)
        {
            CNamedEvent* pEvent = pAgent->findEvent(eventName);

            if (!pEvent)
            {
                int contextId = pAgent->GetContextId();
                const CTagObjectDescriptor& meta = pAgent->GetDescriptor();
                pEvent = findNamedEventTemplate(meta.ms_methods, eventName, contextId);
            }

            if (pEvent)
            {
                if (pEvent)
                {
                    pEvent->SetParam(pAgent, param1, param2, param3);
                }
                else
                {
                    BEHAVIAC_ASSERT(0, "unregistered parameters %s", eventName);
                }

                pEvent->SetFired(pAgent, true);
            }
            else
            {
                BEHAVIAC_ASSERT(0, "unregistered event %s", eventName);
            }
        }
    }

    BEHAVIAC_FORCEINLINE void Agent::FireEvent(const char* eventName)
    {
        Agent::FireEvent(this, eventName);
    }

    template<class ParamType1>
    BEHAVIAC_FORCEINLINE void Agent::FireEvent(const char* eventName, const ParamType1& param1)
    {
        Agent::FireEvent(this, eventName, param1);
    }

    template<class ParamType1, class ParamType2>
    BEHAVIAC_FORCEINLINE void Agent::FireEvent(const char* eventName, const ParamType1& param1, const ParamType2& param2)
    {
        Agent::FireEvent(this, eventName, param1, param2);
    }

    template<class ParamType1, class ParamType2, class ParamType3>
    BEHAVIAC_FORCEINLINE void Agent::FireEvent(const char* eventName, const ParamType1& param1, const ParamType2& param2, const ParamType3& param3)
    {
        Agent::FireEvent(this, eventName, param1, param2, param3);
    }

    BEHAVIAC_API bool IsParVar(const char* variableName);

    BEHAVIAC_FORCEINLINE bool Agent::IsVariableExisting(const char* variableName) const
    {
        uint32_t variableId = MakeVariableId(variableName);

        return m_variables.IsExisting(variableId);
    }

    template<typename VariableType, bool bRefType>
    struct VariableGettterDispatcher
    {
        static const VariableType* Get(const AgentState& variables, const Agent* pAgent, Property* pProperty, uint32_t variableId)
        {
            const CMemberBase* pMember = pProperty != 0 ? pProperty->GetMember() : 0;
            return variables.Get<VariableType>(pAgent, true, pMember, variableId);
        }
    };

	template<typename VariableType, bool bAgentType>
	struct AgentVariableGettterDispatcher
	{
		static void Assert(const VariableType* pp)
		{
		}
	};

	template<typename VariableType>
	struct AgentVariableGettterDispatcher<VariableType, true>
	{
		static void Assert(const VariableType* pp)
		{
			if (pp && *pp)
			{
				VariableType p = *pp;
				typedef PARAM_BASETYPE(VariableType)	BaseAgentType;
				const char* nameBaseAgentType = GetClassTypeName((BaseAgentType*)0);

				BEHAVIAC_ASSERT(p->IsAKindOf(CStringID(nameBaseAgentType)));
				BEHAVIAC_UNUSED_VAR(p);
				BEHAVIAC_UNUSED_VAR(nameBaseAgentType);
				//BEHAVIAC_ASSERT(BaseAgentType::DynamicCast(*p));

				BEHAVIAC_ASSERT(Agent::DynamicCast(p));
			}
		}
	};

    template<typename VariableType>
    struct VariableGettterDispatcher<VariableType, true>
    {
        static const VariableType* Get(const AgentState& variables, const Agent* pAgent, Property* pProperty, uint32_t variableId)
        {
            const VariableType* pV = variables.Get<VariableType>(pAgent, true, pProperty->GetMember(), variableId);
            const VariableType* pp = pV;

			AgentVariableGettterDispatcher<VariableType, behaviac::Meta::IsAgent<VariableType>::Result>::Assert(pp);
            return pp;
        }
    };

    template<typename VariableType>
    BEHAVIAC_FORCEINLINE const VariableType& Agent::GetVariable(const char* variableName) const
    {
        //return *m_variables.Get<VariableType>(this, variableName);
        uint32_t variableId = MakeVariableId(variableName);
        return GetVariable<VariableType>(variableId);
    }

    template<typename VariableType>
    BEHAVIAC_FORCEINLINE const VariableType& Agent::GetVariable(uint32_t variableId) const
    {
        Property* pProperty = AgentProperties::GetProperty(this->GetObjectTypeName(), variableId);

        if (pProperty && pProperty->m_bIsStatic)
        {
            int contextId = this->GetContextId();
            Context& c = Context::GetContext(contextId);

            const char* staticClassName = this->GetObjectTypeName();
            const VariableType* pVal = c.GetStaticVariable<VariableType>(staticClassName, variableId);
            return *pVal;
        }

		const VariableType* pVal1 = VariableGettterDispatcher<VariableType, behaviac::Meta::IsRefType<VariableType>::Result>::Get(this->m_variables, this, pProperty, variableId);
		if (pVal1)
		{
			return *pVal1;
		}
		else
		{
			static VariableType s = VariableType();
			BEHAVIAC_ASSERT(false);
			return s;
		}
    }

    template<typename VariableType, bool bRefType>
    struct VariableSettterDispatcher
    {
        static void Set(AgentState& variables, bool bMemberSet, Agent* pAgent, bool bLocal, const CMemberBase* pMember, const char* variableName, const VariableType& value, uint32_t variableId)
        {
            variables.Set(bMemberSet, pAgent, bLocal, pMember, variableName, value, variableId);
        }
    };

    template<typename VariableType>
    struct VariableSettterDispatcher<VariableType, true>
    {
        static void Set(AgentState& variables, bool bMemberSet, Agent* pAgent, bool bLocal, const CMemberBase* pMember, const char* variableName, const VariableType& value, uint32_t variableId)
        {
            variables.Set(bMemberSet, pAgent, bLocal, pMember, variableName, (void*)value, variableId);
        }
    };

    template<typename VariableType>
    BEHAVIAC_FORCEINLINE void Agent::SetVariable(const char* variableName, const VariableType& value)
    {
        uint32_t variableId = MakeVariableId(variableName);

        this->SetVariable(variableName, value, variableId);
    }



    BEHAVIAC_FORCEINLINE void Agent::SetVariableFromString(const char* variableName, const char* valueStr)
    {
        m_variables.SetFromString(this, variableName, valueStr);
    }

    template<typename VariableType>
    BEHAVIAC_FORCEINLINE void Agent::Instantiate(const VariableType& value, const Property* property_)
    {
        m_variables.Instantiate(property_, value);
    }

    template<typename VariableType>
    BEHAVIAC_FORCEINLINE void Agent::UnInstantiate(const char* variableName)
    {
        m_variables.UnInstantiate<VariableType>(variableName);
    }

    template<typename VariableType>
    BEHAVIAC_FORCEINLINE void Agent::UnLoad(const char* variableName)
    {
        m_variables.UnLoad<VariableType>(variableName);
    }

    BEHAVIAC_FORCEINLINE bool Agent::Invoke(Agent* pAgent, const char* methodName)
    {
        const CMethodBase* pMethod = Agent::FindMethodBase(methodName);

        if (pMethod)
        {
            const_cast<CMethodBase*>(pMethod)->vCall(pAgent);
            return true;
        }

        return false;
    }

    template <typename P1>
    BEHAVIAC_FORCEINLINE bool Agent::Invoke(Agent* pAgent, const char* methodName, P1 p1)
    {
        const CMethodBase* pMethod = Agent::FindMethodBase(methodName);

        if (pMethod)
        {
            const_cast<CMethodBase*>(pMethod)->vCall(pAgent, &p1);
            return true;
        }

        return false;
    }

    template <typename P1, typename P2>
    BEHAVIAC_FORCEINLINE bool Agent::Invoke(Agent* pAgent, const char* methodName, P1 p1, P2 p2)
    {
        const CMethodBase* pMethod = Agent::FindMethodBase(methodName);

        if (pMethod)
        {
            const_cast<CMethodBase*>(pMethod)->vCall(pAgent, &p1, &p2);
            return true;
        }

        return false;
    }

    template <typename P1, typename P2, typename P3>
    BEHAVIAC_FORCEINLINE bool Agent::Invoke(Agent* pAgent, const char* methodName, P1 p1, P2 p2, P3 p3)
    {
        const CMethodBase* pMethod = Agent::FindMethodBase(methodName);

        if (pMethod)
        {
            const_cast<CMethodBase*>(pMethod)->vCall(pAgent, &p1, &p2, &p3);
            return true;
        }

        return false;
    }

    template <typename R>
    bool Agent::GetInvokeReturn(Agent* pAgent, const char* methodName, R& returnValue)
    {
        const CMethodBase* pMethod = Agent::FindMethodBase(methodName);

        if (pMethod)
        {
            const_cast<CMethodBase*>(pMethod)->GetReturnValue(pAgent, returnValue);
            return true;
        }

        return false;
    }

    template<typename TAGENT>
    BEHAVIAC_FORCEINLINE void Agent::RegisterTypeToMetas(bool bInternal)
    {
        //register the super
        AgentSuperRegister<typename TAGENT::super, Meta::IsAgent<typename TAGENT::super>::Result>::Register();

        const char* typeFullName = TAGENT::GetClassTypeName();

        AgentMetas_t& metas = Metas();

        CStringID typeId(typeFullName);

        AgentMetas_t::const_iterator it = metas.find(typeId);

        if (it != metas.end())
        {
            MetaInfo_t& meta = metas[typeId];

            //if registered as the base class and now is registered directly, then set it as non-internal
            if (meta.bInternal && !bInternal)
            {
                meta.bInternal = bInternal;
            }
        }
        else
        {
            const char* baseTypeFullName = TAGENT::super::GetClassTypeName();

            //filter out CTagObject
            if (string_icmp(baseTypeFullName, "CTagObject") == 0)
            {
                baseTypeFullName = 0;
            }

            metas[typeId] = MetaInfo_t(&TAGENT::GetObjectDescriptor(), typeFullName, baseTypeFullName, bInternal);
        }
    }

    template<typename TAGENT>
    BEHAVIAC_FORCEINLINE bool Agent::Register()
    {
        TAGENT::RegisterProperties();

        //after TAGENT::RegisterProperties()
        RegisterTypeToMetas<TAGENT>(false);

        return true;
    }

    template<typename TAGENT>
    BEHAVIAC_FORCEINLINE void Agent::UnRegister()
    {
        const char* typeName = TAGENT::GetClassTypeName();

        AgentMetas_t& metas = Metas();
        CStringID typeId(typeName);
        metas.erase(typeId);

        //Agent::UnRegisterProperties(meta);
        //handled in CleanupMetas
    }

    template<typename TAGENT>
    BEHAVIAC_FORCEINLINE bool Agent::RegisterInstanceName(const char* agentInstanceName, const wchar_t* displayName, const wchar_t* desc)
    {
		const char* arrStr[] = { " " };
		BEHAVIAC_ASSERT(StringUtils::FindString(agentInstanceName, arrStr, 1) == -1);
		BEHAVIAC_UNUSED_VAR(arrStr);

        const char* agentInstanceNameAny = agentInstanceName;

        if (!agentInstanceNameAny)
        {
            agentInstanceNameAny = TAGENT::GetClassTypeName();
        }

        AgentNames_t::iterator it = Agent::Names().find(agentInstanceNameAny);

        if (it == Agent::Names().end())
        {
            const char* classFullName = TAGENT::GetClassTypeName();

            Agent::Names()[agentInstanceNameAny] = AgentName_t(agentInstanceNameAny, classFullName, displayName, desc);

            return true;
        }

        return false;
    }

    template<typename TAGENT>
    BEHAVIAC_FORCEINLINE void Agent::UnRegisterInstanceName(const char* agentInstanceName)
    {
        const char* agentInstanceNameAny = agentInstanceName;

        if (!agentInstanceNameAny)
        {
            agentInstanceNameAny = TAGENT::GetClassTypeName();
        }

        AgentNames_t::iterator it = Agent::Names().find(agentInstanceNameAny);

        if (it != Agent::Names().end())
        {
            Agent::Names().erase(it);
        }
    }

    template<typename VariableType>
    BEHAVIAC_FORCEINLINE void Agent::SetVariableRegistry(bool bLocal, const CMemberBase* pMember, const char* variableName, const VariableType& value, const char* staticClassName, uint32_t varableId)
    {
        bool bValidName = variableName && variableName[0] != '\0';

        if (bValidName)
        {
            if (staticClassName)
            {
                BEHAVIAC_ASSERT(!bLocal);
                int contextId = this->GetContextId();
                Context& c = Context::GetContext(contextId);

                c.SetStaticVariable(pMember, variableName, value, staticClassName, varableId);
            }
            else
            {
				VariableSettterDispatcher<VariableType, behaviac::Meta::IsRefType<VariableType>::Result>::Set(this->m_variables, true, this, bLocal, pMember, variableName, value, varableId);
            }
        }
    }

    template <typename VariableType>
    BEHAVIAC_FORCEINLINE const VariableType* Agent::GetVariableRegistry(const char* staticClassName, const CMemberBase* pMember, uint32_t variableId) const
    {
        const VariableType* val = NULL;

        bool bValidName = !StringUtils::IsNullOrEmpty(staticClassName);

        if (bValidName)
        {
            int contextId = this->GetContextId();
            Context c = Context::GetContext(contextId);

            val = c.GetStaticVariable<VariableType>(staticClassName, variableId);
        }
        else
        {
            val = this->m_variables.Get<VariableType>(this, true, pMember, variableId);
        }

        return val;
    }

    template<typename VariableType>
    void TVariable<VariableType>::Log(const Agent* pAgent)
    {
        //BEHAVIAC_ASSERT(this->m_changed);

        behaviac::string valueStr = StringUtils::ToString(this->m_value);
        behaviac::string typeName = ::GetClassTypeName((VariableType*)0);

        behaviac::string full_name = this->m_name;

        if (this->m_pMember)
        {
            full_name = FormatString("%s::%s", this->m_pMember->GetClassNameString(), this->m_name.c_str());
        }

        LogManager::GetInstance()->Log(pAgent, typeName.c_str(), full_name.c_str(), valueStr.c_str());
#if !BEHAVIAC_RELEASE
        this->m_changed = false;
#endif
    }

    BEHAVIAC_FORCEINLINE void Variables::SetFromString(Agent* pAgent, const char* variableName, const char* valueStr)
    {
        BEHAVIAC_ASSERT(variableName && variableName[0] != '\0');

        //to skip class name
        const char* variableNameOnly = GetNameWithoutClassName(variableName);

        const CMemberBase* pMember = pAgent->FindMember(variableNameOnly);

        uint32_t varId = MakeVariableId(variableNameOnly);
        Variables_t::iterator it = this->m_variables.find(varId);

        if (it != this->m_variables.end())
        {
            IVariable* pVar = (IVariable*)it->second;

            pVar->SetFromString(pAgent, pMember, valueStr);
        }
    }

    template<typename VariableType>
    void Variables::Set(bool bMemberSet, Agent* pAgent, bool bLocal, const CMemberBase* pMember, const char* variableName, const VariableType& value, uint32_t varId)
    {
        BEHAVIAC_UNUSED_VAR(bMemberSet);
        BEHAVIAC_UNUSED_VAR(bLocal);

        BEHAVIAC_ASSERT(variableName && variableName[0] != '\0');

        typedef TVariable<VariableType> VariableTypeType;

        VariableTypeType* pVar = 0;

        if (varId == 0)
        {
            varId = MakeVariableId(variableName);
        }
        else
        {
            BEHAVIAC_ASSERT(varId == MakeVariableId(variableName));
        }

        Variables_t::iterator it = this->m_variables.find(varId);

        if (it == this->m_variables.end())
        {
            if (!pMember)
            {
                if (pAgent)
                {
                    pMember = pAgent->FindMember(variableName);
                }
                else
                {
                    pMember = Agent::FindMemberBase(variableName);
                }
            }

            pVar = BEHAVIAC_NEW VariableTypeType(pMember, variableName, varId);
            m_variables[varId] = pVar;
        }
        else
        {
            pVar = (VariableTypeType*)it->second;

            //BEHAVIAC_ASSERT(pVar->GetTypeId() == GetClassTypeNumberId<VariableType>() ||
            //	(pVar->GetTypeId() == GetClassTypeNumberId<void*>() && behaviac::Meta::IsRefType<VariableType>::Result));
            //TODO: IsRefType
            //BEHAVIAC_ASSERT(GetClassTypeNumberId<VariableType>() == GetClassTypeNumberId<IList>() ||
            //	GetClassTypeNumberId<VariableType>() == GetClassTypeNumberId<System::Object>() ||
            //	pVar->GetTypeId() == GetClassTypeNumberId<VariableType>());
        }

        pVar->SetValue(value, pAgent);
    }

    template<typename VariableType>
    const VariableType*  Variables::Get(const Agent* pAgent, bool bMemberGet, const CMemberBase* pMember, uint32_t varId) const
    {
        typedef TVariable<VariableType> VariableTypeType;

        if (this->m_variables.find(varId) == this->m_variables.end())
        {
            if (bMemberGet)
            {
                if (pMember != NULL)
                {
                    int typeId = ::GetClassTypeNumberId<VariableType>();
                    //BEHAVIAC_ASSERT(typeId == pMember->GetTypeId());
                    const void* val = pMember->Get(pAgent, typeId);
                    return (const VariableType*)val;
                }
            }

            //BEHAVIAC_ASSERT(false, "a compatible property is not found");
            return NULL;
        }
        else
        {
            VariableTypeType* pVar = (VariableTypeType*)this->m_variables.find(varId)->second;
            BEHAVIAC_ASSERT(pVar != NULL);

            //TODO: IsRefType
            //BEHAVIAC_ASSERT(GetClassTypeNumberId<VariableType>() == GetClassTypeNumberId<IList>() ||
            //	GetClassTypeNumberId<VariableType>() == GetClassTypeNumberId<System::Object>() ||
            //	(GetClassTypeNumberId<VariableType>() == GetClassTypeNumberId<long long>() && pVar->GetTypeId() == GetClassTypeNumberId<long>()) ||
            //	(GetClassTypeNumberId<VariableType>() == GetClassTypeNumberId<unsigned long long>() && pVar->GetTypeId() == GetClassTypeNumberId<unsigned long>()) ||
            //	pVar->GetTypeId() == GetClassTypeNumberId<VariableType>());

            //if out of scope
            if (pVar->m_instantiated > 0)
            {
                return &pVar->GetValue(pAgent);
            }
			else
			{
				BEHAVIAC_LOGWARNING("A Local '%s' has been out of scope!\n", pVar->Name().c_str());
			}
        }

        return NULL;
    }

    template<typename VariableType>
    void TVariable<VariableType>::SetFromString(Agent* pAgent, const CMemberBase* pMember, const char* valueString)
    {
        if (valueString)
        {
            VariableType value;

            if (behaviac::StringUtils::FromString(valueString, value))
            {
                if (!(Details::Equal(this->m_value, value)))
                {
                    this->m_value = value;
#if !BEHAVIAC_RELEASE
                    this->m_changed = true;
#endif

                    if (pMember)
                    {
                        int typeId = ::GetClassTypeNumberId<VariableType>();

                        if (pMember && typeId == pMember->GetTypeId())
                        {
                            pMember->Set(pAgent, &value, typeId);
                        }
                    }
                }
            }
        }
    }


    template<typename VariableType>
    void AgentState::Set(bool bMemberSet, Agent* pAgent, bool bLocal, const CMemberBase* pMember, const char* variableName, const VariableType& value, uint32_t varId)
    {
        //if (variableName == "DirtyRooms")
        //{
        //    Debug.Check(true);
        //}

        // not in planning
        if (pAgent->m_planningTop == -1 && !bLocal)
        {
            Variables::Set(bMemberSet, pAgent, bLocal, pMember, variableName, value, varId);
            return;
        }

        if (this->state_stack.size() > 0)
        {
			size_t stackIndex = 0;

            if (bLocal)
            {
                //top
                stackIndex = this->state_stack.size() - 1;
            }
            else
            {
                //bottom
                stackIndex = pAgent->m_planningTop;
            }

            AgentState* t = this->state_stack[stackIndex];

            //if there are something in the state stack, it is used for planning, so, don't really set member
            t->Set(false, pAgent, bLocal, NULL, variableName, value, varId);
        }
        else
        {
            Variables::Set(bMemberSet, pAgent, bLocal, pMember, variableName, value, varId);
        }
    }

    template<typename VariableType>
    const VariableType* AgentState::Get(const Agent* pAgent, bool bMemberGet, const CMemberBase* pMember, uint32_t varId) const
    {
        if (this->state_stack.size() > 0)
        {
			for (int i = (int)this->state_stack.size() - 1; i >= 0; --i)
            {
                AgentState* t = this->state_stack[i];
                const VariableType* result = t->Get<VariableType>(pAgent, false, pMember, varId);

                if (result != NULL)
                {
                    return result;
                }
            }
        }

        //CTagObject* result1 = Variables::Get(pAgent, bMemberGet, pMember, varId);

        const VariableType* result1 = Variables::Get<VariableType>(pAgent, bMemberGet, pMember, varId); //(pAgent, bMemberGet, pMember, varId);
        return result1;
    }

	template<typename T, bool bStruct>
	struct UserDefinedTypeAssert
	{
		static bool IsAKindOf(T p, const char* szType)
		{
			return false;
		}
	};

	template<typename T>
	struct UserDefinedTypeAssert<T, true>
	{
		static bool IsAKindOf(T p, const char* szType)
		{
			return p->IsAKindOf(CStringID(szType));
		}
	};

	template<typename T>
	static bool IsChildOf(T child, const char* baseClass)
	{
		//agent or user defined struct
		return UserDefinedTypeAssert<T, behaviac::Meta::IsPtr<T>::Result && behaviac::Meta::IsRefType<T>::Result>::IsAKindOf(child, baseClass);
	}

#if BEHAVIAC_ENABLE_ASSERTS
	template<typename T>
	static bool CheckCompatibleType(const behaviac::string& typeName) {
		behaviac::string t = GetTypeDescString<T>();

		if (typeName == t) {
			return true;
		}

		t = GetTypeDescString<T&>();

		if (typeName == t) {
			return true;
		}

		return false;
	}
#endif

    template<typename ParamType>
    void CNamedEvent::SetParam(Agent* pAgent, const ParamType& param)
    {
        BEHAVIAC_ASSERT(this->m_paramTypes.size() == 1);
		BEHAVIAC_ASSERT(CheckCompatibleType<ParamType>(this->m_paramTypes[0]) || IsChildOf(param, this->m_paramTypes[0].c_str()), "SetParam's Param is not compatible");

        AgentState* currentState = pAgent->m_variables.Push(false);

		BEHAVIAC_UNUSED_VAR(currentState);

        behaviac::string eventName = FormatString("%s%d", BEHAVIAC_LOCAL_TASK_PARAM_PRE, 0);

        pAgent->SetVariable(eventName.c_str(), param);
    }

    template<typename ParamType1, typename ParamType2>
	void CNamedEvent::SetParam(Agent* pAgent, const ParamType1& param1, const ParamType2& param2)
    {
        BEHAVIAC_ASSERT(this->m_paramTypes.size() == 2);
		BEHAVIAC_ASSERT(CheckCompatibleType<ParamType1>(this->m_paramTypes[0]) || IsChildOf(param1, this->m_paramTypes[0].c_str()), "SetParam's Param1 is not compatible");
        BEHAVIAC_ASSERT(CheckCompatibleType<ParamType2>(this->m_paramTypes[1]) || IsChildOf(param2, this->m_paramTypes[1].c_str()), "SetParam's Param2 is not compatible");

        AgentState* currentState = pAgent->m_variables.Push(false);
		BEHAVIAC_UNUSED_VAR(currentState);

        behaviac::string eventName1 = FormatString("%s%d", BEHAVIAC_LOCAL_TASK_PARAM_PRE, 0);
        pAgent->SetVariable(eventName1.c_str(), param1);
        behaviac::string eventName2 = FormatString("%s%d", BEHAVIAC_LOCAL_TASK_PARAM_PRE, 1);
        pAgent->SetVariable(eventName2.c_str(), param2);
    }

    template<typename ParamType1, typename ParamType2, typename ParamType3>
	void CNamedEvent::SetParam(Agent* pAgent, const ParamType1& param1, const ParamType2& param2, const ParamType3& param3)
    {
        BEHAVIAC_ASSERT(this->m_paramTypes.size() == 3);
		BEHAVIAC_ASSERT(CheckCompatibleType<ParamType1>(this->m_paramTypes[0]) || IsChildOf(param1, this->m_paramTypes[0].c_str()), "SetParam's Param1 is not compatible");
        BEHAVIAC_ASSERT(CheckCompatibleType<ParamType2>(this->m_paramTypes[1]) || IsChildOf(param2, this->m_paramTypes[1].c_str()), "SetParam's Param2 is not compatible");
        BEHAVIAC_ASSERT(CheckCompatibleType<ParamType3>(this->m_paramTypes[2]) || IsChildOf(param3, this->m_paramTypes[2].c_str()), "SetParam's Param3 is not compatible");

        AgentState* currentState = pAgent->m_variables.Push(false);
		BEHAVIAC_UNUSED_VAR(currentState);

        behaviac::string eventName1 = FormatString("%s%d", BEHAVIAC_LOCAL_TASK_PARAM_PRE, 0);
        pAgent->SetVariable(eventName1.c_str(), param1);
        behaviac::string eventName2 = FormatString("%s%d", BEHAVIAC_LOCAL_TASK_PARAM_PRE, 1);
        pAgent->SetVariable(eventName2.c_str(), param2);
        behaviac::string eventName3 = FormatString("%s%d", BEHAVIAC_LOCAL_TASK_PARAM_PRE, 2);
        pAgent->SetVariable(eventName3.c_str(), param3);
    }
}//namespace behaviac

