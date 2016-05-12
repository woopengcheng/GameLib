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

#include "behaviac/base/core/assert_t.h"
#include "behaviac/base/core/crc.h"
#include "behaviac/property/property.h"
#include "behaviac/agent/agent.h"
#include "behaviac/base/object/method.h"
#include "behaviac/property/property_t.h"

#include "behaviac/htn/agentproperties.h"

#include "behaviac/base/core/logging/log.h"
#include "behaviac/property/typeregister.h"

behaviac::vector<IList::IListPool**>* IList::ms_pools;

behaviac::vector<IList::IListPool**>& IList::GetPools()
{
    if (ms_pools == 0)
    {
        ms_pools = BEHAVIAC_NEW behaviac::vector<IListPool**>;
    }

    return *ms_pools;
}

void IList::Cleanup()
{
    if (ms_pools)
    {
        for (behaviac::vector<IListPool**>::iterator it = ms_pools->begin(); it != ms_pools->end(); ++it)
        {
            IListPool** ppListPool = *it;
            IListPool* pListPool = *ppListPool;
            pListPool->cleanup();
            BEHAVIAC_DELETE pListPool;
            (*ppListPool) = 0;
        }

        ms_pools->clear();
        BEHAVIAC_DELETE ms_pools;
        ms_pools = 0;
    }
}

namespace behaviac
{
    uint32_t MakeVariableId(const char* idString)
    {
        return CRC32::CalcCRC(idString);
    }

    bool IsParVar(const char* variableName);

    Property::PropertyCreators_t* Property::ms_propertyCreators;
    Property::PropertyCreators_t& Property::PropertyCreators()
    {
        if (!ms_propertyCreators)
        {
            ms_propertyCreators = BEHAVIAC_NEW PropertyCreators_t;
        }

        BEHAVIAC_ASSERT(ms_propertyCreators);

        return *ms_propertyCreators;
    }

    Property::Properties_t* Property::ms_properties;

    Property::Properties_t& Property::Properties()
    {
        if (!ms_properties)
        {
            ms_properties = BEHAVIAC_NEW Properties_t;
        }

        BEHAVIAC_ASSERT(ms_properties);

        return *ms_properties;
    }

    void Property::Cleanup()
    {
        if (ms_propertyCreators)
        {
            //for (PropertyCreators_t::iterator it = ms_propertyCreators->begin(); it != ms_propertyCreators->end(); ++it)
            //{
            //}

            ms_propertyCreators->clear();
            BEHAVIAC_DELETE(ms_propertyCreators);
            ms_propertyCreators = 0;
        }

        if (ms_properties)
        {
            for (Properties_t::iterator it = ms_properties->begin(); it != ms_properties->end(); ++it)
            {
                Property* p = it->first;
                BEHAVIAC_DELETE(p);
            }

            ms_properties->clear();
            BEHAVIAC_DELETE(ms_properties);
            ms_properties = 0;
        }

        IList::Cleanup();
    }

    Property::Property(const CMemberBase* pMemberBase, bool bIsConst) :
        m_parent(0), m_index(0), m_bIsStatic(false), m_bIsLocal(false)
    {
        m_memberBase = pMemberBase;
        m_variableId = 0;
		m_refParNameId = 0;
        m_bValidDefaultValue = false;
        m_bIsConst = bIsConst;
    }

    Property::Property(const Property& copy) :
        m_parent(copy.m_parent), m_index(copy.m_index),
        m_variableName(copy.m_variableName), m_varaibleFullName(copy.m_varaibleFullName),
        m_variableId(copy.m_variableId),
        m_refParName(copy.m_refParName),
        m_refParNameId(copy.m_refParNameId),
        m_memberBase(copy.m_memberBase),
        m_instanceName(copy.m_instanceName),
        m_bValidDefaultValue(copy.m_bValidDefaultValue), m_bIsConst(copy.m_bIsConst), m_bIsStatic(copy.m_bIsStatic), m_bIsLocal(copy.m_bIsLocal)
    {
    }

    Property::Property(Property* parent, const char* indexStr)
    {
        m_variableName = parent->m_variableName + "[]";
        m_instanceName = parent->m_instanceName;
        m_variableId = MakeVariableId(m_variableName.c_str());
        //m_refParName = parent->m_refParName;
        //m_refParNameId = parent->m_refParNameId;
        m_memberBase = NULL;
        m_parent = parent;

        m_bValidDefaultValue = parent->m_bValidDefaultValue;

        m_bIsConst = false;
        m_bIsStatic = false;
        m_bIsLocal = parent->m_bIsLocal;

        behaviac::vector<behaviac::string> tokens;

        behaviac::StringUtils::SplitIntoArray(indexStr, " ", tokens);

        if (tokens.size() == 1)
        {
            this->m_index = Property::Create("int", indexStr);
        }
        else
        {
            BEHAVIAC_ASSERT(tokens[0] == "int");
            this->m_index = Property::Create(tokens[0].c_str(), tokens[1].c_str(), false, 0);
        }
    }

    Property* Property::CreateElelmentAccessor(const char* vecotrAcessorIndex)
    {
        BEHAVIAC_UNUSED_VAR(vecotrAcessorIndex);

        //Property* elementAccessor = new Property(this, vecotrAcessorIndex);
        //return elementAccessor;
        BEHAVIAC_ASSERT(0);
        return 0;
    }

    void Property::SetVectorElementAsDefault(Property* pProperty)
    {
        BEHAVIAC_UNUSED_VAR(pProperty);

        BEHAVIAC_ASSERT(false);
    }

    Property::~Property()
    {
		BEHAVIAC_ASSERT(true);
	}

    void Property::SetRefName(const char* refParName)
    {
        m_refParName = refParName;
        m_refParNameId = MakeVariableId(refParName);
    }

    const behaviac::string& Property::GetRefName() const
    {
        return this->m_refParName;
    }

    uint32_t Property::GetRefNameId() const
    {
        return this->m_refParNameId;
    }

    void Property::SetVariableName(const char* variableName)
    {
        this->m_varaibleFullName = variableName;
        const char* nameOnly = GetNameWithoutClassName(variableName);

        this->m_variableId = MakeVariableId(nameOnly);
        this->m_variableName = nameOnly;
    }

    const char* Property::GetClassNameString() const
    {
        return m_memberBase ? m_memberBase->GetClassNameString() : 0;
    }

    const char* Property::GetInstanceNameString() const
    {
        if (!m_instanceName.empty())
        {
            return m_instanceName.c_str();
        }

        return m_memberBase ? m_memberBase->GetInstanceNameString() : 0;
    }

    Agent* Property::GetParentAgent(const Agent* pAgent) const
    {
		BEHAVIAC_ASSERT(pAgent);
		Agent* pParent = Agent::GetInstance(pAgent, this->m_instanceName.c_str());
		BEHAVIAC_ASSERT(pParent);

        return pParent;
    }

	const size_t kInstanceLength = 256;
    const char*  Property::ParseInstanceNameProperty(const char* fullName, char* agentIntanceName, char* agentType)
    {
        //Self.AgentActionTest::Action2(0)
        const char* pClassBegin = strchr(fullName, '.');

        if (pClassBegin)
        {
			size_t posClass = pClassBegin - fullName;
            BEHAVIAC_ASSERT(posClass < kInstanceLength);
            string_ncpy(agentIntanceName, fullName, posClass);
            agentIntanceName[posClass] = '\0';

            const char* pAgentType = pClassBegin + 1;

            const char* pPropertyName = strrchr(pAgentType, ':');
            BEHAVIAC_ASSERT(pPropertyName);
			size_t agentTypeLength = pPropertyName - 1 - pAgentType;
            string_ncpy(agentType, pAgentType, agentTypeLength);
            agentType[agentTypeLength] = '\0';

            const char* propertyName = pPropertyName + 1;

            return propertyName;
        }

        return fullName;
    }

    Property*  Property::Create(const char* typeName, const char* valueStr)
    {
        bool bConst = true;

        Property*  p = Property::create(0, bConst, typeName, 0, 0, valueStr);

        return p;
    }

	Property*  Property::Create(const char* typeName, const char* nameStr, const char* valueStr)
	{
		bool bConst = false;

		Property*  p = Property::create(0, bConst, typeName, nameStr, 0, valueStr);

		return p;
	}

	Property* Property::clone() {
		BEHAVIAC_ASSERT(false);

		return 0;
	}

    const char* Property::GetInstanceName()
    {
        return this->m_instanceName.c_str();
    }
    void Property::SetInstanceName(const char* data)
    {
        this->m_instanceName = data;
    }
    const char* Property::Name()
    {
        return this->m_variableName.c_str();
    }
    void Property::Name(const char* _name)
    {
        this->SetVariableName(_name);
    }
    Property* Property::Create(const char* typeName, const char* fullName, bool bIsStatic, const char* arrayIndexStr)
    {
        BEHAVIAC_UNUSED_VAR(bIsStatic);
        char instanceName[kInstanceLength];
        char agentType[kInstanceLength];
        const char* variableName = ParseInstanceNameProperty(fullName, instanceName, agentType);

        Property*  pProperty = AgentProperties::GetProperty(agentType, variableName);

        if (pProperty != 0)
        {
            BEHAVIAC_ASSERT(pProperty != 0);

            if (strcmp(pProperty->GetInstanceName(), instanceName) != 0)
            {
                //Properties().insert(std::pair<Property*, bool>(pProperty, true));
                BEHAVIAC_ASSERT(strcmp(pProperty->Name(), variableName) == 0);

				Property* pNew = pProperty->clone();
				pNew->SetInstanceName(instanceName);

				pProperty = pNew;

				AgentProperties::AddPropertyInstance(agentType, pNew);
            }
            else
            {
            }


            BEHAVIAC_ASSERT(pProperty->m_bIsStatic == bIsStatic);

            if (arrayIndexStr)
            {
                Property*  vectorAccessor = pProperty->CreateElelmentAccessor(arrayIndexStr);

                return vectorAccessor;
            }
        }
        else
        {
            //BEHAVIAC_ASSERT(false, "accessing a not declared local variable");
            const char* valueStr = 0;
            pProperty = AgentProperties::AddLocal(agentType, typeName, variableName, valueStr);
        }

        return pProperty;
    }

    Property*  Property::Create(const char* typeName, const char* instanceName, const char* agentType, const char* propertyName, const char* valueStr)
    {
        BEHAVIAC_ASSERT(propertyName);
        BEHAVIAC_ASSERT(!StringUtils::EndsWith(propertyName, "]"));

        const CMemberBase* pMember = 0;
        bool bConst = false;

        if (agentType)
        {
            CStringID agentClassId(agentType);
            CStringID propertyId(propertyName);

            pMember = Agent::FindMemberBase(agentClassId, propertyId);
        }
        else
        {
            BEHAVIAC_ASSERT(true);
        }

        Property*  p = Property::create(pMember, bConst, typeName, propertyName, instanceName, valueStr);

        return p;
    }

    // static bool IsAgentPtr(const char* typeName, const char* valueStr)
    // {
    //     bool bAgentPtr = false;

    //     //it might be par or the right value of condition/assignment
    //     if (!StringUtils::IsNullOrEmpty(valueStr) && StringUtils::StrEqual(valueStr, "null")) {
    //         BEHAVIAC_ASSERT(Agent::IsAgentClassName(typeName));
    //         bAgentPtr = true;
    //     }

    //     return bAgentPtr;
    // }

    Property*  Property::create(const CMemberBase* pMember, bool bConst, const char* typeName, const char* variableName, const char* instanceName, const char* valueStr)
    {
        BEHAVIAC_ASSERT(variableName == 0 || !StringUtils::EndsWith(variableName, "]"));
        bool bValidName = variableName && variableName[0] != '\0';

        if (bValidName)
        {
            Property*  pProperty = 0;

            if (pMember != 0)
            {
                BEHAVIAC_ASSERT(variableName);
                pProperty = pMember->CreateProperty(valueStr, bConst);
                BEHAVIAC_ASSERT(pProperty != 0);
            }
            else
            {
                pProperty = Agent::CreateProperty(typeName, variableName, valueStr);
            }

            if (pProperty != 0)
            {
                if (!bConst)
                {
                    BEHAVIAC_ASSERT(variableName);
                    pProperty->SetVariableName(variableName);
                    pProperty->SetInstanceName(instanceName);
                }

                return pProperty;
            }
        }

        {
            bool bAgentPtr = false;

            //it might be par or the right value of condition/assignment
            if (valueStr && StringUtils::StrEqual(valueStr, "null"))
            {
                //BEHAVIAC_ASSERT(Agent::IsAgentClassName(typeName));
                bAgentPtr = true;
                typeName = "void*";
                valueStr = "0";
            }

            BEHAVIAC_UNUSED_VAR(bAgentPtr);

            PropertyCreator* pCreator = PropertyCreators()[typeName];
            BEHAVIAC_ASSERT(pCreator, "%s is not registerd, please add TypeRegister::Register<%s>(\"%s\")", typeName, typeName, typeName);

            if (pCreator)
            {
                Property* pProperty = (*pCreator)(valueStr, 0, bConst);

                if (bValidName && pProperty && !bConst)
                {
                    pProperty->SetVariableName(variableName);
                }

                return pProperty;
            }
        }

        BEHAVIAC_ASSERT(0);
        return 0;
    }

    void Property::DeleteFromCache(Property* property_)
    {
        Properties_t::iterator it = Properties().find(property_);

        if (it != Properties().end())
        {
            BEHAVIAC_DELETE property_;

            Properties().erase(it);
        }
    }


    void Property::RegisterBasicTypes()
    {
#undef BEHAVIAC_DECLARE_PRIMITE_TYPE
#define BEHAVIAC_DECLARE_PRIMITE_TYPE(type, typeName)    TypeRegister::Register<type>(#typeName);

        BEHAVIAC_M_PRIMITIVE_TYPES();

        TypeRegister::Register<char*>("szstring");
        TypeRegister::Register<const char*>("cszstring");
		TypeRegister::Register<behaviac::Agent*>("behaviac::Agent");
        TypeRegister::Register<behaviac::EBTStatus>("behaviac::EBTStatus");

    }

    void Property::UnRegisterBasicTypes()
    {
#undef BEHAVIAC_DECLARE_PRIMITE_TYPE
#define BEHAVIAC_DECLARE_PRIMITE_TYPE(type, typeName)	TypeRegister::UnRegister<type>(#typeName);

        BEHAVIAC_M_PRIMITIVE_TYPES();

        TypeRegister::UnRegister<char*>("szstring");
        TypeRegister::UnRegister<const char*>("cszstring");
		TypeRegister::UnRegister<behaviac::Agent*>("behaviac::Agent");
        TypeRegister::UnRegister<behaviac::EBTStatus>("behaviac::EBTStatus");

    }
}//namespace behaviac
