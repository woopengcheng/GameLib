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

#ifndef BEHAVIAC_PROPERTY_H
#define BEHAVIAC_PROPERTY_H

#include "behaviac/base/dynamictype.h"
#include "behaviac/base/core/factory.h"
#include "behaviac/base/core/thread/asyncvalue.h"

#include "behaviac/base/base.h"
#include "behaviac/base/object/tagobject.h"

#include "behaviac/base/string/valuefromstring.h"
#include "behaviac/property/vector_ext.h"
#include "behaviac/base/object/typehandler.h"
#include "behaviac/base/functions.h"

class CMemberBase;

namespace behaviac
{
    //------------------------------------------------------------------------
    BEHAVIAC_API uint32_t MakeVariableId(const char* idString);
    BEHAVIAC_API const char* GetNameWithoutClassName(const char* variableName);
    enum EComputeOperator
    {
        ECO_INVALID,
        ECO_ADD,
        ECO_SUB,
        ECO_MUL,
        ECO_DIV
    };

    class Agent;
    class IVariable;
    class BEHAVIAC_API Property
    {
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(Property);

        Property(const CMemberBase* pMemberBase, bool bIsConst);
        virtual ~Property();

        void SetVariableName(const char* variableName);

        const char* GetVariableName() const
        {
            return this->m_variableName.c_str();
        }

        uint32_t GetVariableId() const
        {
            return m_variableId;
        }

        const char* GetVariableFullName() const
        {
            return this->m_varaibleFullName.c_str();
        }

        const char* GetClassNameString() const;

        const char* GetInstanceNameString() const;

        virtual int GetTypeId() const = 0;

        virtual void SetFrom(Agent* pAgentfrom, behaviac::IAsyncValue* from, Agent* pAgentTo) = 0;
        virtual void SetFrom(Agent* pAgentfrom, const Property* from, Agent* pAgentTo) = 0;
        virtual void SetFrom(Agent* pAgentfrom, const CMemberBase* from, Agent* pAgentTo) = 0;
        virtual void SetFrom(Agent* pAgentFrom, const CMethodBase* from, Agent* pAgentTo) = 0;

        virtual void SetVectorElementTo(Agent* pAgentFrom, int index, const Property* to, Agent* pAgentTo) = 0;
        virtual void* GetVectorElementFrom(Agent* pAgentFrom, int index) = 0;
        virtual void SetVectorElementTo(Agent* pAgentTo, int index, void* pValue) = 0;
        virtual void SetFrom(Agent* pAgentFrom, const CMethodBase* from, Agent* pAgentTo, int index) = 0;

        virtual void ComputeFrom(Agent* pAgentfrom, behaviac::IAsyncValue* from, Agent* pAgentTo, EComputeOperator opr) = 0;
        virtual void ComputeFrom(Agent* pAgentFrom, const Property* from, Agent* pAgentTo, EComputeOperator opr) = 0;

        virtual void SetDefaultValueString(const char* valString) = 0;
        virtual void SetDefaultValue(const Property* r) = 0;
        virtual void SetDefaultInteger(int count) = 0;
        virtual uint64_t GetDefaultInteger() const = 0;
		virtual double GetDoubleValue(Agent* pAgent) const = 0;
        virtual void Instantiate(Agent* pAgent) = 0;
        virtual void UnInstantiate(Agent* pAgent) = 0;
        virtual void UnLoad(Agent* pAgent) = 0;

        virtual IVariable* CreateVar() = 0;
        virtual const IList* CreateList(const Agent* parent, const Agent* parHolder) const = 0;

        virtual const char* GetString(const behaviac::Agent* parent, const behaviac::Agent* parHolder) const = 0;
        virtual void SetString(behaviac::Agent* parHolder, const char* value) = 0;

        Agent* GetParentAgent(const Agent* pAgent) const;
		virtual float DifferencePercentage(const Property* reference, const Property* other) const = 0;

        Property* LoadRight(const char* value, const behaviac::string& propertyName, behaviac::string& typeName);

        void SetRefName(const char* refParName);
        const behaviac::string& GetRefName() const;
        uint32_t GetRefNameId() const;

        static void RegisterBasicTypes();
        static void UnRegisterBasicTypes();

        ///create instance property,
        ///create class scope static property
        static Property* Create(const char* typeName, const char* valueStr);
		static Property*  Create(const char* typeName, const char* nameStr, const char* valueStr);
        static Property* Create(const char* typeName, const char* fullName, bool bIsStatic, const char* arrayIndexStr);
        static Property* Create(const char* typeName, const char* instanceName, const char* agentType, const char* propertyName, const char* valueStr);

		virtual Property* clone() = 0;

        static void DeleteFromCache(Property* property_);

        template<typename T>
        static Property* Creator(const char* value, const CMemberBase* pMemberBase, bool bConst);

        template<typename T>
        static Property* Creator(const CMemberBase* pMemberBase, bool bConst);

        static void Cleanup();
        const char* GetInstanceName();
        void SetInstanceName(const char* data);
        const char* Name();
        void Name(const char* _name);

        virtual Property* CreateElelmentAccessor(const char* vecotrAcessorIndex);
        virtual void SetVectorElementAsDefault(Property* pProperty);

        const CMemberBase* GetMember() const
        {
            return this->m_memberBase;
        }
    protected:
        Property(const Property& copy);
        Property(Property* parent, const char* indexStr);
        Property*				m_parent;
        Property*				m_index;

    private:
        typedef Property* PropertyCreator(const char* value, const CMemberBase* pMemberBase, bool bConst);
        typedef behaviac::map<behaviac::string, PropertyCreator*> PropertyCreators_t;
        typedef PropertyCreators_t::iterator PropertyCreatorIterator;
        static PropertyCreators_t* ms_propertyCreators;
        static PropertyCreators_t& PropertyCreators();
        static Property* create(const CMemberBase* pMember, bool bConst, const char* typeName, const char* variableName, const char* instanceName, const char* valueStr);

        typedef behaviac::map<Property*, bool> Properties_t;
        static Properties_t* ms_properties;
        static Properties_t& Properties();

        static const char* ParseInstanceNameProperty(const char* fullName, char* agentIntanceName, char* agentType);

		template<typename T, bool bRefType>
		struct CreatorSelector
		{
			static void Register(PropertyCreators_t& creators, const char* typeName)
			{
				creators[typeName] = &Creator<T>;
			}
		};

		template<typename T>
		struct CreatorSelector<T, true>
		{
			static void Register(PropertyCreators_t& creators, const char* typeName)
			{
				typedef REAL_BASETYPE(T) BaseType;

				creators[typeName] = &Creator<BaseType*>;
			}
		};

        template<typename T>
        static bool Register(const char* typeName)
        {
			PropertyCreators_t& creators = PropertyCreators();
			CreatorSelector<T, behaviac::Meta::IsRefType<T>::Result>::Register(creators, typeName);

            return true;
        }
        template<typename T>
        static void UnRegister(const char* typeName)
        {
            PropertyCreators().erase(typeName);
        }

        friend class TypeRegister;

    protected:
        behaviac::string		m_variableName;

        //used for Delete
        behaviac::string		m_varaibleFullName;
        uint32_t				m_variableId;

        behaviac::string		m_refParName;
        uint32_t				m_refParNameId;

        const CMemberBase*		m_memberBase;
        behaviac::string		m_instanceName;

        bool					m_bValidDefaultValue;
        bool					m_bIsConst;

    public:
        bool                    m_bIsStatic;
        bool                    m_bIsLocal;
    };

}


#endif//BEHAVIAC_PROPERTY_H
