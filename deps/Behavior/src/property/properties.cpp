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
#include "behaviac/property/properties.h"
#include "behaviac/base/workspace.h"
#include "behaviac/agent/agent.h"

namespace behaviac
{
    Variables::Variables()
    {
        BEHAVIAC_ASSERT(this->m_variables.size() == 0);
    }

    Variables::~Variables()
    {
        this->Clear(true);
    }

	void Variables::Clear(bool bFull)
	{
		if (bFull) {
			for (Variables_t::iterator it = this->m_variables.begin();
				it != this->m_variables.end(); ++it)
			{
				IVariable* pVar = it->second;

				BEHAVIAC_DELETE(pVar);
			}

			this->m_variables.clear();
		}
		else {
			for (Variables_t::iterator it = this->m_variables.begin();
				it != this->m_variables.end(); )
			{
				IVariable* pVar = it->second;

				Variables_t::iterator it_temp = it;
				++it;

				if (pVar->IsLocal()) {
					BEHAVIAC_DELETE(pVar);
					this->m_variables.erase(it_temp);
				}
			}
		}
	}

    void Variables::Log(const Agent* pAgent, bool bForce)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
        BEHAVIAC_UNUSED_VAR(bForce);
#if !BEHAVIAC_RELEASE

        if (Config::IsLoggingOrSocketing())
        {
            for (Variables_t::iterator it = this->m_variables.begin();
                 it != this->m_variables.end(); ++it)
            {
                IVariable* pVar = it->second;

                bool bToLog = false;

                if (bForce)
                {
                    bToLog = true;
                }
                else
                {
                    if (pVar->IsChanged())
                    {
                        bToLog = true;
                    }
                    else
                    {
                        bToLog = pVar->CheckIfChanged(pAgent);
                    }
                }

                if (bToLog)
                {
                    pVar->Log(pAgent);
                }
            }
        }

#endif//BEHAVIAC_RELEASE
    }

    void Variables::Reset()
    {
        for (Variables_t::iterator it = this->m_variables.begin();
             it != this->m_variables.end(); ++it)
        {
            IVariable* pVar = it->second;

            pVar->Reset();
        }
    }

    void IVariable::CopyTo(Agent* pAgent)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
    }

    void Variables::CopyTo(Agent* pAgent, Variables& target) const
    {
        for (Variables_t::iterator it = target.m_variables.begin();
             it != target.m_variables.end(); ++it)
        {
            IVariable* pVar = it->second;

            BEHAVIAC_DELETE(pVar);
        }

        target.m_variables.clear();

        for (Variables_t::const_iterator it = this->m_variables.begin();
             it != this->m_variables.end(); ++it)
        {
            const IVariable* pVar = it->second;
            IVariable* pNew = pVar->clone();

            target.m_variables[pNew->GetId()] = pNew;
        }

        if (pAgent)
        {
            for (Variables_t::iterator it = target.m_variables.begin();
                 it != target.m_variables.end(); ++it)
            {
                IVariable* pVar = it->second;
                pVar->CopyTo(pAgent);
            }
        }
    }

    void IVariable::Save(ISerializableNode* node) const
    {
        BEHAVIAC_UNUSED_VAR(node);
    }

    void IVariable::Load(ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(node);
    }

    void Variables::Save(ISerializableNode* node) const
    {
        CSerializationID  variablesId("vars");
        ISerializableNode* varsNode = node->newChild(variablesId);

        for (Variables_t::const_iterator it = this->m_variables.begin();
             it != this->m_variables.end(); ++it)
        {
            const IVariable* pVar = it->second;

            //skip agent members
            if (!pVar->IsMember())
            {
                pVar->Save(varsNode);
            }
        }
    }

    void Variables::Load(ISerializableNode* node)
    {
        CSerializationID  attrId("agentType");
        behaviac::string agentTypeStr;
        node->getAttr(attrId, agentTypeStr);

        CSerializationID  variablesId("vars");
        ISerializableNode* varsNode = node->findChild(variablesId);

        if (varsNode)
        {
            int varsCount = varsNode->getChildCount();

            for (int i = 0; i < varsCount; ++i)
            {
                ISerializableNode* varNode = varsNode->getChild(i);

                CSerializationID  nameId("name");
                behaviac::string nameStr;
                varNode->getAttr(nameId, nameStr);

                CSerializationID  valueId("value");
                behaviac::string valueStr;
                varNode->getAttr(valueId, valueStr);

                CSerializationID  typeId("type");
                behaviac::string typeStr;
                varNode->getAttr(typeId, typeStr);

#if !BEHAVIAC_RELEASE
                CStringID agentType(agentTypeStr.c_str());
                CStringID memberId(nameStr.c_str());

                //members are skipped
                BEHAVIAC_ASSERT(Agent::FindMemberBase(agentType, memberId) == 0);
#endif
				Property* p = Property::Create(typeStr.c_str(), nameStr.c_str(), valueStr.c_str());

                IVariable* pVar = p->CreateVar();

                uint32_t varId = MakeVariableId(nameStr.c_str());
                this->m_variables[varId] = pVar;
            }
        }

        //for (Variables_t::iterator it = this->m_variables.begin();
        //	it != this->m_variables.end(); ++it)
        //{
        //	const IVariable* pVar = it->second;
        //	pVar->Load(node);
        //}
    }

    void Variables::Unload()
    {
        for (Variables_t::iterator it = this->m_variables.begin();
             it != this->m_variables.end();)
        {
            Variables_t::iterator itCurrent = it;
            Variables_t::iterator itNext = ++it;

            IVariable* pVar = itCurrent->second;

            if (!pVar->m_pMember)
            {
                //par
                this->m_variables.erase(itCurrent->first);
            }

            it = itNext;
        }
    }

    void Variables::Cleanup()
    {}
}//namespace behaviac
