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

#ifndef BEHAVIAC_PROPERTY_PROPERTIES_H
#define BEHAVIAC_PROPERTY_PROPERTIES_H

#include "behaviac/base/base.h"
#include "behaviac/base/object/tagobject.h"
#include "behaviac/base/object/typehandler.h"
#include "behaviac/base/logging/logmanager.h"
#include "behaviac/base/string/valuetostring.h"
#include "behaviac/property/operators.inl"
#include "behaviac/property/property.h"

namespace behaviac
{
    class Agent;
    class Property;

    class BEHAVIAC_API IVariable
    {
    public:
        IVariable(const CMemberBase* pMember, const char* variableName, uint32_t id) :
            m_id(id), m_name(variableName), m_property(0), m_pMember(pMember), m_instantiated(1)
#if !BEHAVIAC_RELEASE
            , m_changed(true)
#endif
        {}

        IVariable(const CMemberBase* pMember, const Property* property_) :
            m_property(property_), m_pMember(pMember), m_instantiated(1)
#if !BEHAVIAC_RELEASE
            , m_changed(true)
#endif
        {
            BEHAVIAC_ASSERT(this->m_property);

            this->m_name = this->m_property->GetVariableName();
            this->m_id = this->m_property->GetVariableId();
        }

        IVariable(const IVariable& copy) :
            m_id(copy.m_id), m_name(copy.m_name), m_property(copy.m_property), m_pMember(copy.m_pMember), m_instantiated(copy.m_instantiated)
#if !BEHAVIAC_RELEASE
            , m_changed(copy.m_changed)
#endif
        {
        }

        virtual ~IVariable()
        {}

#if !BEHAVIAC_RELEASE
        bool IsChanged() const
        {
            return m_changed;
        }

        virtual bool CheckIfChanged(const Agent* pAgent) = 0;
#endif
        virtual int GetTypeId() const
        {
            return 0;
        }

        uint32_t GetId() const
        {
            return this->m_id;
        }

        const behaviac::Property*	GetProperty() const
        {
            return this->m_property;
        }

        void SetProperty(const Property* p)
        {
            if (p)
            {
                BEHAVIAC_ASSERT(this->m_name == p->GetVariableName());
                BEHAVIAC_ASSERT(this->m_id == p->GetVariableId());
            }

            this->m_property = p;
        }
        behaviac::string Name()
        {
            return m_name;
        }
        virtual IVariable* clone() const = 0;
        virtual void CopyTo(Agent* pAgent) = 0;
        virtual void Save(ISerializableNode* node) const;
        virtual void Load(ISerializableNode* node);

        virtual void SetFromString(Agent* pAgent, const CMemberBase* pMember, const char* value) = 0;
        virtual void Log(const Agent* pAgent) = 0;

        virtual void Reset() = 0;

        bool IsMember() const {
            return this->m_pMember != 0;
        }

		bool IsLocal() const {
			if (this->m_pMember == 0) {
				if (this->m_property) {
					return this->m_property->m_bIsLocal;
				}

				return true;
			}

			return false;
		}

    protected:
        uint32_t			m_id;
        behaviac::string	m_name;
        const Property*		m_property;
        const CMemberBase*	m_pMember;
        unsigned char		m_instantiated;
#if !BEHAVIAC_RELEASE
        bool				m_changed;
#endif
        friend class Variables;
    };

    template<typename VariableType>
    class TVariable : public IVariable
    {
    public:
        TVariable(const CMemberBase* pMember, const char* variableName, uint32_t varId) : IVariable(pMember, variableName, varId)
        {}

        TVariable(const CMemberBase* pMember, const Property* property_, const VariableType& value) : IVariable(pMember, property_), m_value(value)
        {}

        TVariable(const TVariable& copy) : IVariable(copy), m_value(copy.m_value)
        {}

        const VariableType& GetValue(const Agent* pAgent = 0) const
        {
            if (this->m_pMember)
            {
                int typeId = ::GetClassTypeNumberId<VariableType>();
                BEHAVIAC_UNUSED_VAR(typeId);
                BEHAVIAC_ASSERT(typeId == ::GetClassTypeNumberId<System::Object>() ||
                                typeId == this->m_pMember->GetTypeId());

                const void* pAddr = this->m_pMember->Get(pAgent, typeId);

                return *(VariableType*)pAddr;
            }

            return this->m_value;
        }

        void SetValue(const VariableType& value, Agent* pAgent)
        {
            bool bProperty = false;

            if (this->m_pMember)
            {
                int typeId = ::GetClassTypeNumberId<VariableType>();
                BEHAVIAC_UNUSED_VAR(typeId);
                BEHAVIAC_ASSERT(typeId == this->m_pMember->GetTypeId());

                this->m_pMember->Set(pAgent, &value, typeId);
                //devlelopment version needs to update m_value even for property, as it needs to be used in the logging
#if !BEHAVIAC_RELEASE
#else
                bProperty = true;
#endif
            }

            if (!bProperty && !(Details::Equal(this->m_value, value)))
            {
                this->m_value = value;
#if !BEHAVIAC_RELEASE
                this->m_changed = true;
#endif

            }
            else
            {
                //don't clear it here, it will be cleared after being logged
                //this->m_changed = false;
            }
        }

        virtual int GetTypeId() const
        {
            return GetClassTypeNumberId<VariableType>();
        }

#if !BEHAVIAC_RELEASE
        virtual bool CheckIfChanged(const Agent* pAgent)
        {
            if (this->m_pMember)
            {
                const VariableType& v = this->GetValue(pAgent);

                if (!Details::Equal(this->m_value, v))
                {
                    this->m_value = v;
                    this->m_changed = true;

                    return true;
                }
            }

            return false;
        }
#endif

        virtual void Log(const Agent* pAgent);

        virtual void Reset()
        {
#if !BEHAVIAC_RELEASE
            this->m_changed = false;
#endif
        }

        virtual IVariable* clone() const
        {
            IVariable* pVar = BEHAVIAC_NEW TVariable(*this);
            return pVar;
        }

        virtual void CopyTo(Agent* pAgent)
        {
            if (this->m_pMember)
            {
                int typeId = ::GetClassTypeNumberId<VariableType>();
                this->m_pMember->SetVariable(pAgent, &m_value, typeId);

            }
            else
            {
                BEHAVIAC_ASSERT(true);
            }
        }

        virtual void Save(ISerializableNode* node) const
        {
            IVariable::Save(node);

            CSerializationID  variableId("var");
            ISerializableNode* varNode = node->newChild(variableId);

            CSerializationID nameId("name");
            varNode->setAttr(nameId, this->m_name);

            CSerializationID valueId("value");
            varNode->setAttr(valueId, this->m_value);

            CSerializationID typeId("type");

            //int type_id = GetClassTypeNumberId<VariableType>();
            //varNode->setAttr(typeId, type_id);
            const char* typeStr = GetClassTypeName((VariableType*)0);
            behaviac::string typeStrStr = typeStr;

            if (StringUtils::StartsWith(typeStr, "unsigned long "))
            {
                StringUtils::ReplaceTag(typeStrStr, "unsigned long ", "u");

            }
            else if (StringUtils::StartsWith(typeStr, "unsigned "))
            {
                StringUtils::ReplaceTag(typeStrStr, "unsigned ", "u");

            }
            else if (StringUtils::StartsWith(typeStr, "signed "))
            {
                StringUtils::ReplaceTag(typeStrStr, "signed ", "");
            }

            varNode->setAttr(typeId, typeStrStr.c_str());
        }

        virtual void Load(ISerializableNode* node)
        {
            IVariable::Load(node);

            //CSerializationID  nameId("name");
            //behaviac::string nameStr;
            //node->getAttr(nameId, nameStr);

            //CSerializationID  valueId("value");
            //behaviac::string valueStr;
            //node->getAttr(valueId, valueStr);

            //CSerializationID  typeId("type");
            //behaviac::string typeStr;
            //node->getAttr(typeId, typeStr);

            //Property* p = Property::Create(typeStr.c_str(), nameStr.c_str());
            //BEHAVIAC_ASSERT(p);
            //pAgent->SetVariableFromString(nameStr.c_str(), valueStr.c_str());
        }

        virtual void SetFromString(Agent* pAgent, const CMemberBase* pMember, const char* valueString);

    private:
        VariableType m_value;
    };

    BEHAVIAC_API uint32_t MakeVariableId(const char* idString);

    class BEHAVIAC_API Variables
    {
    public:
        Variables();
        virtual ~Variables();

		virtual void Clear(bool bFull);

        bool IsExisting(uint32_t varId) const
        {
            Variables_t::const_iterator it = this->m_variables.find(varId);

            if (it != this->m_variables.end())
            {
                return true;
            }

            return false;
        }

        template<typename VariableType>
        void Instantiate(const Property* property_, const VariableType& value)
        {
            BEHAVIAC_ASSERT(property_);

            typedef TVariable<VariableType> VariableTypeType;

            uint32_t varId = property_->GetVariableId();
            Variables_t::iterator it = this->m_variables.find(varId);

            if (it == this->m_variables.end())
            {
                VariableTypeType* pVar = BEHAVIAC_NEW VariableTypeType(0, property_, value);
                m_variables[varId] = pVar;

            }
            else
            {
                VariableTypeType* pVar = (VariableTypeType*)it->second;
                BEHAVIAC_ASSERT(pVar->GetTypeId() == GetClassTypeNumberId<VariableType>(), "the same par is used for different types");
                BEHAVIAC_ASSERT(pVar->m_instantiated < 255, "dead loop?!");

                //don't update it, so the par set by outer scope can override the one in the internal
                if (pVar->m_instantiated == 0)
                {
                    pVar->SetProperty(property_);
                }
                else
                {
                    //pVar->GetProperty() might be 0 if pAgent->SetVariable("par_agent", pA);
                    //BEHAVIAC_ASSERT(pVar->GetProperty());
                    BEHAVIAC_ASSERT(pVar->GetTypeId() == property_->GetTypeId(), "the same name par doesn't have the same type");
                }

                //use the original value, don't update it
                pVar->m_instantiated++;
            }
        }

        template<typename VariableType>
        void UnInstantiate(const char* variableName)
        {
            BEHAVIAC_ASSERT(variableName && variableName[0] != '\0');

            typedef TVariable<VariableType> VariableTypeType;

            uint32_t varId = MakeVariableId(variableName);
            Variables_t::iterator it = this->m_variables.find(varId);

            BEHAVIAC_ASSERT(it != this->m_variables.end());

            if (it != this->m_variables.end())
            {
                VariableTypeType* pVar = (VariableTypeType*)it->second;
                BEHAVIAC_ASSERT(pVar->GetTypeId() == GetClassTypeNumberId<VariableType>(), "the same par is Instantiateed as a different type");
                BEHAVIAC_ASSERT(pVar->m_instantiated >= 1);

                //don't erase it as it might be accessed after the bt's ticking
                //this->m_variables.erase(varId);
                pVar->m_instantiated--;

                if (pVar->m_instantiated == 0)
                {
                    pVar->SetProperty(0);
                }
            }
        }

        template<typename VariableType>
        void UnLoad(const char* variableName)
        {
            BEHAVIAC_ASSERT(variableName && variableName[0] != '\0');

            typedef TVariable<VariableType> VariableTypeType;

            uint32_t varId = MakeVariableId(variableName);
            Variables_t::iterator it = this->m_variables.find(varId);

            if (it != this->m_variables.end())
            {
                VariableTypeType* pVar = (VariableTypeType*)it->second;
                BEHAVIAC_DELETE pVar;

                this->m_variables.erase(it);
            }
        }

        void SetFromString(Agent* pAgent, const char* variableName, const char* value);

        template<typename VariableType>
        void Set(bool bMemberSet, Agent* pAgent, bool bLocal, const CMemberBase* pMember, const char* variableName, const VariableType& value, uint32_t varId = 0);

        template<typename VariableType>
        const VariableType* Get(const Agent* pAgent, bool bMemberGet, const CMemberBase* pMember, uint32_t varId) const;

        void Log(const Agent* pAgent, bool bForce);
        void Reset();

        void Unload();

        static void Cleanup();

        void CopyTo(Agent* pAgent, Variables& target) const;

        void Save(ISerializableNode* node) const;
        void Load(ISerializableNode* node);
    protected:
        typedef behaviac::map<uint32_t, IVariable*> Variables_t;
        Variables_t m_variables;
    public:
        behaviac::map<uint32_t, IVariable*>& Vars()
        {
            return this->m_variables;
        };
    };
}//namespace behaviac

#endif//BEHAVIAC_PROPERTY_PROPERTIES_H
