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

#ifndef BEHAVIAC_PROPERTY_T_H
#define BEHAVIAC_PROPERTY_T_H

#include "behaviac/property/property.h"
#include "behaviac/agent/agent.h"
#include "behaviac/base/object/member.h"

class CMemberBase;

namespace behaviac
{
    template<typename VariableType>
    inline void ComputeTo(VariableType v, Property* pPropertyTo, Agent* pAgentTo, EComputeOperator opr)
    {
        BEHAVIAC_UNUSED_VAR(v);
        BEHAVIAC_UNUSED_VAR(pPropertyTo);
        BEHAVIAC_UNUSED_VAR(pAgentTo);
        BEHAVIAC_UNUSED_VAR(opr);

        BEHAVIAC_ASSERT(0);
    };

    template<typename VariableType, bool bVector>
    class TTProperty : public Property
    {
        TTProperty& operator=(const TTProperty& copy);

    protected:
        TTProperty(const TTProperty& copy) : Property(copy), m_defaultValue(copy.m_defaultValue)
        {}
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(TTProperty);
        TTProperty(const CMemberBase* pMemberBase, bool bConst) : Property(pMemberBase, bConst)
        {
        }

        TTProperty(Property* parent, const char* indexStr) : Property(parent, indexStr)
        {
            BEHAVIAC_ASSERT(!bVector);

            if (m_bValidDefaultValue)
            {
                parent->SetVectorElementAsDefault(this);
            }
        }

        virtual ~TTProperty()
        {}

        virtual Property* clone()
        {
            Property* p = BEHAVIAC_NEW TTProperty(*this);

            return p;
        }

        virtual int GetTypeId() const
        {
            return GetClassTypeNumberId<VariableType>();
        }

        virtual void SetFrom(Agent* pAgentFrom, behaviac::IAsyncValue* from, Agent* pAgentTo)
        {
            BEHAVIAC_UNUSED_VAR(pAgentFrom);
            const VariableType& retV = ((behaviac::AsyncValue<VariableType>*)from)->get();
            this->SetValue(pAgentTo, retV);
        }

        virtual void SetFrom(Agent* pAgentFrom, const Property* from, Agent* pAgentTo);

        virtual void SetFrom(Agent* pAgentfrom, const CMemberBase* from, Agent* pAgentTo)
        {
            int typeId = this->GetTypeId();
            const void* pData = from->Get(pAgentfrom, typeId);
            const VariableType* pV = (const VariableType*)pData;

            this->SetValue(pAgentTo, *pV);
        }

        virtual void SetFrom(Agent* pAgentFrom, const CMethodBase* from, Agent* pAgentTo)
        {
            ((CMethodBase*)from)->Invoke(pAgentFrom);

			VariableType retV = ((CMethodBase*)from)->GetReturnValue<VariableType>(pAgentFrom);
            
            this->SetValue(pAgentTo, retV);
        }

        virtual void SetVectorElementTo(Agent* pAgentFrom, int index, const Property* to, Agent* pAgentTo)
        {
            BEHAVIAC_UNUSED_VAR(pAgentFrom);
            BEHAVIAC_UNUSED_VAR(index);
            BEHAVIAC_UNUSED_VAR(to);
            BEHAVIAC_UNUSED_VAR(pAgentTo);
            BEHAVIAC_ASSERT(false);
        }

        virtual void* GetVectorElementFrom(Agent* pAgentFrom, int index)
        {
            BEHAVIAC_UNUSED_VAR(pAgentFrom);
            BEHAVIAC_UNUSED_VAR(index);
            BEHAVIAC_ASSERT(false);
            return 0;
        }


        virtual void SetVectorElementTo(Agent* pAgentTo, int index, void* pValue)
        {
            BEHAVIAC_UNUSED_VAR(pAgentTo);
            BEHAVIAC_UNUSED_VAR(index);
            BEHAVIAC_UNUSED_VAR(pValue);
            BEHAVIAC_ASSERT(false);
        }

        virtual void SetFrom(Agent* pAgentFrom, const CMethodBase* from, Agent* pAgentTo, int index);
        virtual void ComputeFrom(Agent* pAgentFrom, const Property* from, Agent* pAgentTo, EComputeOperator opr);

        virtual void ComputeFrom(Agent* pAgentfrom, behaviac::IAsyncValue* from, Agent* pAgentTo, EComputeOperator opr)
        {
            BEHAVIAC_UNUSED_VAR(pAgentfrom);
            const VariableType retV = ((behaviac::AsyncValue<VariableType>*)from)->get();

            ComputeTo(retV, this, pAgentTo, opr);
        }

        virtual void SetDefaultInteger(int count)
        {
            VariableType v;

            ConvertFromInteger(count, v);

            this->SetDefaultValue(v);
        }

        virtual uint64_t GetDefaultInteger() const
        {
            const VariableType& retV = this->GetDefaultValue();

            uint64_t result = ConvertToInteger(retV);
            return result;
        }

		virtual double GetDoubleValue(Agent* pAgent) const
		{
#ifdef _DEBUG
            typedef VALUE_TYPE(VariableType) TTYPE;
			BEHAVIAC_ASSERT(GetClassTypeNumberId<double>() == GetClassTypeNumberId<TTYPE>() ||
				GetClassTypeNumberId<float>() == GetClassTypeNumberId<TTYPE>());
#endif
			const VariableType& retV = this->GetValue(pAgent);

			double result = ConvertToDouble(retV);
			return result;
		}

		virtual float DifferencePercentage(const Property* reference, const Property* other) const
        {
			return const_cast<CMemberBase*>(this->m_memberBase)->DifferencePercentage(reference, other);
        }

        virtual void Instantiate(Agent* pAgent)
        {
            //if it is a member property, don't instantiate it, otherwise it will have two copies can causes inconsistency
            if (this->m_memberBase == NULL)
            {
                VariableType v = this->GetDefaultValue();

                if (this->m_bIsStatic)
                {
                    this->SetValue(pAgent, v);
                }
                else
                {
                    pAgent->Instantiate(v, this);
                }
            }
        }

        virtual void UnInstantiate(Agent* pAgent)
        {
            if (this->m_memberBase == NULL)
            {
                pAgent->UnInstantiate<VariableType>(this->m_variableName.c_str());
            }
        }

        virtual void UnLoad(Agent* pAgent)
        {
            BEHAVIAC_ASSERT(!this->m_memberBase);

            pAgent->UnLoad<VariableType>(this->m_variableName.c_str());
        }

        virtual IVariable* CreateVar()
        {
            IVariable* v = BEHAVIAC_NEW TVariable<VariableType>(0, this, this->m_defaultValue);

            return v;
        }

        virtual const IList* CreateList(const Agent* parent, const Agent* parHolder) const
        {
            BEHAVIAC_UNUSED_VAR(parent);
            BEHAVIAC_UNUSED_VAR(parHolder);
            BEHAVIAC_ASSERT(false);
            return 0;
        }

        virtual void SetDefaultValueString(const char* valString)
        {
            VariableType v;

            if (behaviac::StringUtils::FromString(valString, v))
            {
                this->SetDefaultValue(v);
            }
        }

        virtual void SetDefaultValue(const Property* r);

        void SetDefaultValue(const VariableType& v)
        {
            this->m_bValidDefaultValue = true;
            this->m_defaultValue = v;
        }

        const VariableType& GetDefaultValue() const
        {
            BEHAVIAC_ASSERT(this->m_bValidDefaultValue);

            return this->m_defaultValue;
        }


        virtual const char* GetString(const behaviac::Agent* parent, const behaviac::Agent* parHolder) const
        {
            BEHAVIAC_UNUSED_VAR(parent);
            BEHAVIAC_UNUSED_VAR(parHolder);
            BEHAVIAC_ASSERT(false);
            return 0;
        }


        virtual void SetString(behaviac::Agent* parHolder, const char* value)
        {
            BEHAVIAC_UNUSED_VAR(value);
            BEHAVIAC_UNUSED_VAR(parHolder);
            BEHAVIAC_ASSERT(false);
        }

        void SetValue(Agent* pSelf, const VariableType& v)
        {
            BEHAVIAC_ASSERT(pSelf);
            BEHAVIAC_ASSERT(!m_bIsConst);

            if (this->m_parent != NULL)
            {
                this->SetVectorElement(pSelf, v);
                return;
            }

            const char* staticClassName = 0;

            if (this->m_memberBase)
            {
                if (this->m_memberBase->ISSTATIC())
                {
                    staticClassName = this->m_memberBase->GetClassNameString();
                }
            }
            else if (this->m_bIsStatic)
            {
                staticClassName = pSelf->GetObjectTypeName();
            }

            pSelf->SetVariableRegistry(this->m_bIsLocal, this->m_memberBase, this->m_variableName.c_str(), v, staticClassName, this->m_variableId);
        }

        const VariableType& GetValue(const Agent* pSelf) const
        {
            if (this->m_parent != 0)
            {
                const VariableType& var = this->GetVectorElement(pSelf);
				return var;
            }

            if (pSelf == 0 || m_bIsConst)
            {
                return this->GetDefaultValue();
            }
            else
            {
                if (this->m_memberBase)
                {
#if !BEHAVIAC_RELEASE
                    {
                        //Agent* pInstance = this->GetParentAgent(pSelf);
                        //BEHAVIAC_ASSERT(pInstance == pSelf);
                    }
#endif//BEHAVIAC_RELEASE
                    int typeId = GetClassTypeNumberId<VariableType>();
                    VariableType* pVariable = (VariableType*)this->m_memberBase->Get(pSelf, typeId);

                    return *pVariable;
                }
                else
                {
                    return pSelf->GetVariable<VariableType>(this->m_variableName.c_str());
                }
            }
        }

        const VariableType& GetVectorElement(const Agent* pSelf) const;
        void SetVectorElement(Agent* pSelf, const VariableType& v);


        const VariableType& GetValue(const Agent* parent, const Agent* pSelf) const
        {
            if (!parent || m_bIsConst)
            {
                return this->GetDefaultValue();
            }
            else
            {
                if (this->m_parent != NULL)
                {
                    const VariableType& var = this->GetVectorElement(pSelf);
					return var;
                }

                const char* staticClassName = NULL;

                if (this->m_memberBase != NULL)
                {
                    Agent* pInstance = this->GetParentAgent(pSelf);
                    pSelf = pInstance;
                }
                else if (this->m_bIsStatic)
                {
                    staticClassName = pSelf->GetClassTypeName();
                }

                const VariableType* pVariable = pSelf->GetVariableRegistry<VariableType>(staticClassName, this->m_memberBase, this->m_variableId);

                return *pVariable;
            }
        }

    private:
        VariableType	m_defaultValue;
    };

    template<typename T>
    void LogManager::LogVarValue(Agent* pAgent, const behaviac::string& name, const T& value)
    {
        behaviac::string valueStr = StringUtils::ToString(value);
        behaviac::string typeName = GetClassTypeName((T*)0);

        behaviac::string full_name = name;

        const CMemberBase* pMember = pAgent->FindMember(name.c_str());

        if (pMember != 0)
        {
            const char* classFullName = pMember->GetClassNameString();
            full_name = FormatString("%s::%s", classFullName, name.c_str());
        }

        this->Log(pAgent, typeName.c_str(), full_name.c_str(), valueStr.c_str());
    }

    template<typename VariableType>
    class TTProperty<VariableType, true> : public TTProperty<VariableType, false>
    {
        typedef typename behaviac::Meta::IsVector<VariableType>::ElementType ElementType;
        TTProperty& operator=(const TTProperty& copy);

    protected:
        TTProperty(const TTProperty& copy) : TTProperty<VariableType, false>(copy)
        {}

        TTProperty(Property* parent, const char* indexStr) : TTProperty<VariableType, false>(parent, indexStr)
        {
            BEHAVIAC_ASSERT(true);
        }
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(TTProperty);
        TTProperty(const CMemberBase* pMemberBase, bool bConst) : TTProperty<VariableType, false>(pMemberBase, bConst)
        {
        }

        virtual ~TTProperty()
        {}

        virtual Property* clone()
        {
            Property* p = BEHAVIAC_NEW TTProperty(*this);

            return p;
        }

        virtual void SetVectorElementTo(Agent* pAgentFrom, int index, const Property* to, Agent* pAgentTo);

        virtual void* GetVectorElementFrom(Agent* pAgentFrom, int index)
        {
            const VariableType& retV_vec = this->GetValue(pAgentFrom);
			const ElementType& retV = retV_vec[index];

#if 1//BEHAVIAC_COMPILER_APPLE || BEHAVIAC_COMPILER_MSVC2015 || BEHAVIAC_COMPILER_ANDROID
			//this is not thread safe
			ASSERT_MAIN_THREAD();
			//xcode, it reports compiling error, so to use a static here, not perfect
			static ElementType retV2 = retV;
			retV2 = retV;

			return (void*)&retV2;
#else
            return (void*)&retV;
#endif//BEHAVIAC_COMPILER_APPLE
        }

        virtual void SetVectorElementTo(Agent* pAgentTo, int index, void* pValue)
        {
            VariableType& vecT = (VariableType&)this->GetValue(pAgentTo);
            const ElementType& v = *(const ElementType*)pValue;

            vecT[index] = v;
        }


        virtual Property* CreateElelmentAccessor(const char* vecotrAcessorIndex)
        {
            Property* elementAccessor = BEHAVIAC_NEW TTProperty<ElementType, false>(this, vecotrAcessorIndex);

            return elementAccessor;
        }

        virtual void SetVectorElementAsDefault(Property* pProperty);

        virtual const IList* CreateList(const Agent* parent, const Agent* parHolder) const
        {
            const VariableType& vector_ = this->GetValue(parent, parHolder);

            IList* pList = TList<VariableType>::CreateList(&vector_);

            return pList;
        }
    };

    template<typename VariableType>
    class TProperty : public TTProperty<VariableType, behaviac::Meta::IsVector<VariableType>::Result>
    {
    protected:
        TProperty(const TProperty& copy) : TTProperty<VariableType, behaviac::Meta::IsVector<VariableType>::Result>(copy)
        {}
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(TProperty);
        TProperty(const CMemberBase* pMemberBase, bool bConst) : TTProperty<VariableType, behaviac::Meta::IsVector<VariableType>::Result>(pMemberBase, bConst)
        {
        }

        virtual Property* clone()
        {
            Property* p = BEHAVIAC_NEW TProperty(*this);

            return p;
        }
    };

    template<>
    class TProperty<char*> : public TTProperty<char*, false>
    {
    protected:
        TProperty(const TProperty& copy) : TTProperty<char*, false>(copy)
        {}
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(TProperty);
        TProperty(const CMemberBase* pMemberBase, bool bConst) : TTProperty<char*, false>(pMemberBase, bConst)
        {
        }

        virtual Property* clone()
        {
            Property* p = BEHAVIAC_NEW TProperty(*this);

            return p;
        }


        virtual const char* GetString(const behaviac::Agent* parent, const behaviac::Agent* parHolder) const
        {
            const char* v = this->GetValue(parent, parHolder);
            return v;
        }

        virtual void SetString(behaviac::Agent* parHolder, const char* value)
        {
            this->SetValue(parHolder, (char * const)value);
        }
    };

    template<>
    class TProperty<const char*> : public TTProperty<const char*, false>
    {
    protected:
        TProperty(const TProperty& copy) : TTProperty<const char*, false>(copy)
        {}
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(TProperty);
        TProperty(const CMemberBase* pMemberBase, bool bConst) : TTProperty<const char*, false>(pMemberBase, bConst)
        {
        }

        virtual Property* clone()
        {
            Property* p = BEHAVIAC_NEW TProperty(*this);

            return p;
        }

        virtual const char* GetString(const behaviac::Agent* parent, const behaviac::Agent* parHolder) const
        {
            const char* v = this->GetValue(parent, parHolder);
            return v;
        }

        virtual void SetString(behaviac::Agent* parHolder, const char* value)
        {
            this->SetValue(parHolder, value);
        }
    };

    template<>
    class TProperty<behaviac::string> : public TTProperty<behaviac::string, false>
    {
    protected:
        TProperty(const TProperty& copy) : TTProperty<behaviac::string, false>(copy)
        {}
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(TProperty);
        TProperty(const CMemberBase* pMemberBase, bool bConst) : TTProperty<behaviac::string, false>(pMemberBase, bConst)
        {
        }

        virtual Property* clone()
        {
            Property* p = BEHAVIAC_NEW TProperty(*this);

            return p;
        }

        virtual const char* GetString(const behaviac::Agent* parent, const behaviac::Agent* parHolder) const
        {
            const behaviac::string& v = this->GetValue(parent, parHolder);
            const char* pV = v.c_str();
            return pV;
        }

        virtual void SetString(behaviac::Agent* parHolder, const char* value)
        {
            behaviac::string v(value);

            this->SetValue(parHolder, v);
        }

        using TTProperty<behaviac::string, false>::SetFrom;
        virtual void SetFrom(Agent* pAgentFrom, const CMethodBase* from, Agent* pAgentTo)
        {
            ((CMethodBase*)from)->Invoke(pAgentFrom);

            //from->GetReturnValue((const CTagObject*)pAgentFrom, (behaviac::Property*)this, (CTagObject*)pAgentTo)
            const char* strFrom = from->GetReturnString(pAgentFrom);
            behaviac::string retV(strFrom);

            this->SetValue(pAgentTo, retV);
        }
    };

    template<>
    class TProperty<std::string> : public TTProperty<std::string, false>
    {
    protected:
        TProperty(const TProperty& copy) : TTProperty<std::string, false>(copy)
        {}
    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(TProperty);
        TProperty(const CMemberBase* pMemberBase, bool bConst) : TTProperty<std::string, false>(pMemberBase, bConst)
        {
        }

        virtual Property* clone()
        {
            Property* p = BEHAVIAC_NEW TProperty(*this);

            return p;
        }

        virtual const char* GetString(const behaviac::Agent* parent, const behaviac::Agent* parHolder) const
        {
            const std::string& v = this->GetValue(parent, parHolder);
            const char* pV = v.c_str();
            return pV;
        }

        virtual void SetString(behaviac::Agent* parHolder, const char* value)
        {
            std::string v(value);

            this->SetValue(parHolder, v);
        }
    };

    template<typename VariableType, bool bVector>
    void TTProperty<VariableType, bVector>::SetFrom(Agent* pAgentFrom, const Property* from, Agent* pAgentTo)
    {
        TProperty<VariableType>* fromT = (TProperty<VariableType>*)from;

        const VariableType& retV = fromT->GetValue(pAgentFrom);

        this->SetValue(pAgentTo, retV);
    }

    template<typename VariableType, bool bVector>
    void TTProperty<VariableType, bVector>::SetFrom(Agent* pAgentFrom, const CMethodBase* from, Agent* pAgentTo, int index)
    {
        ((CMethodBase*)from)->Invoke(pAgentFrom);

		VariableType retV = ((CMethodBase*)from)->GetReturnValue<VariableType>(pAgentFrom, index);
       
        TProperty<VariableType>* thisT = (TProperty<VariableType>*)this;
        thisT->SetValue(pAgentTo, retV);
    }

    template<typename VariableType, bool bVector>
    void TTProperty<VariableType, bVector>::ComputeFrom(Agent* pAgentFrom, const Property* from, Agent* pAgentTo, EComputeOperator opr)
    {
        TProperty<VariableType>* fromT = (TProperty<VariableType>*)from;

        const VariableType& retV = fromT->GetValue(pAgentFrom);

        ComputeTo(retV, this, pAgentTo, opr);
    }

    template<typename VariableType, bool bVector>
    void TTProperty<VariableType, bVector>::SetVectorElement(Agent* pSelf, const VariableType& v)
    {
        Agent* parentParent = this->m_parent->GetParentAgent(pSelf);
        Agent* indexParent = this->m_index->GetParentAgent(pSelf);

        TProperty<int>* pTIndex = (TProperty<int>*)this->m_index;
        int index = (int)pTIndex->GetValue(indexParent);

        this->m_parent->SetVectorElementTo(parentParent, index, (void*)&v);
#if !BEHAVIAC_RELEASE

        if (pSelf != NULL && pSelf->m_planningTop >= 0)
        {
            behaviac::string varName = FormatString("%s[%d]", this->m_parent->Name(), index);

            LogManager::GetInstance()->LogVarValue(pSelf, varName, v);
        }

#endif
    }

    template<typename VariableType, bool bVector>
    const VariableType& TTProperty<VariableType, bVector>::GetVectorElement(const Agent* pSelf) const
    {
        BEHAVIAC_ASSERT(this->m_index != NULL);
        Agent* parentParent = this->m_parent->GetParentAgent(pSelf);

        Agent* indexParent = this->m_index->GetParentAgent(pSelf);
        TProperty<int>* pTIndex = (TProperty<int>*)this->m_index;
        int index = (int)pTIndex->GetValue(indexParent);

        const VariableType& pVariableType = *(const VariableType*)this->m_parent->GetVectorElementFrom(parentParent, index);

        return pVariableType;
    }


    template<typename VariableType, bool bVector>
    void TTProperty<VariableType, bVector>::SetDefaultValue(const Property* r)
    {
        TProperty<VariableType>* rT = (TProperty<VariableType>*)r;
        const VariableType& v = rT->GetDefaultValue();
        this->SetDefaultValue(v);
    }

    template<typename VariableType>
    void TTProperty<VariableType, true>::SetVectorElementTo(Agent* pAgentFrom, int index, const Property* to, Agent* pAgentTo)
    {
        const VariableType& retV_vec = this->GetValue(pAgentFrom);
        const ElementType& retV = retV_vec[index];

        BEHAVIAC_ASSERT(to->GetTypeId() == GetClassTypeNumberId<ElementType>());

        TProperty<ElementType>* toT = (TProperty<ElementType>*)to;

        toT->SetValue(pAgentTo, retV);
    }

    template<typename VariableType>
    void TTProperty<VariableType, true>::SetVectorElementAsDefault(Property* pProperty)
    {
        const VariableType& retV_vec = this->GetDefaultValue();
        const ElementType& retV = retV_vec[0];

        BEHAVIAC_ASSERT(pProperty->GetTypeId() == GetClassTypeNumberId<ElementType>());

        TProperty<ElementType>* pTProperty = (TProperty<ElementType>*)pProperty;

        pTProperty->SetDefaultValue(retV);
    }

    template<typename T>
    inline Property* Property::Creator(const char* value, const CMemberBase* pMemberBase, bool bConst)
    {
        TProperty<T>* p = (TProperty<T>*)Creator<T>(pMemberBase, bConst);

        //a const must have default value
        BEHAVIAC_ASSERT(!bConst || value);

        if (value)
        {
            T v;

            if (behaviac::StringUtils::FromString(value, v))
            {
                p->SetDefaultValue(v);
            }
            else
            {
                //a const must have a valid default value
                BEHAVIAC_ASSERT(!bConst);
            }
        }

        return p;
    }

    template<typename T>
    inline Property* Property::Creator(const CMemberBase* pMemberBase, bool bConst)
    {
        typedef TProperty<T> PropertyType;
        PropertyType* p = BEHAVIAC_NEW PropertyType(pMemberBase, bConst);

        return p;
    }

#define M_COMPUTE_TO(VariableType)\
    template<>\
    inline void ComputeTo<VariableType>(VariableType v, Property* pPropertyTo, Agent* pAgentTo, EComputeOperator opr) {\
        TProperty<VariableType>* pPropertyToT = (TProperty<VariableType>*)pPropertyTo;\
        const VariableType& thisV = pPropertyToT->GetValue(pAgentTo);\
        \
        VariableType resultV = thisV;\
        \
        switch (opr) {\
            case ECO_ADD:\
                resultV += v;\
                break;\
                \
            case ECO_SUB:\
                resultV -= v;\
                break;\
                \
            case ECO_MUL:\
                resultV *= v;\
                break;\
                \
            case ECO_DIV:\
                resultV /= v;\
                break;\
                \
            default:\
                BEHAVIAC_ASSERT(0);\
                break;\
        }\
        \
        pPropertyToT->SetValue(pAgentTo, resultV);\
    }

    M_COMPUTE_TO(long);
    M_COMPUTE_TO(int);
    M_COMPUTE_TO(short);
    M_COMPUTE_TO(char);

    M_COMPUTE_TO(unsigned long);
    M_COMPUTE_TO(unsigned int);
    M_COMPUTE_TO(unsigned short);
    M_COMPUTE_TO(unsigned char);

    M_COMPUTE_TO(float);
    M_COMPUTE_TO(double);
}

template <typename T>
const T& ParamVariablePrimitiveBase<T>::GetValue(const CTagObject* parent, const CTagObject* parHolder) const
{
    BEHAVIAC_ASSERT(this->prop && behaviac::Agent::DynamicCast(parent));

    behaviac::TProperty<T>* pT = (behaviac::TProperty<T>*)this->prop;

    return pT->GetValue((const behaviac::Agent*)parent, (const behaviac::Agent*)parHolder);
}

template <typename T>
void ParamVariablePrimitiveBase<T>::SetVariableRegistry(const CTagObject* parHolder, const T& value)
{
    if (this->prop && this->IsRefOut() && behaviac::Agent::DynamicCast(parHolder))
    {
        behaviac::TProperty<T>* pT = (behaviac::TProperty<T>*)this->prop;

        pT->SetValue((behaviac::Agent*)parHolder, value);
    }
}

template <typename T>
const char* ParamVariablePrimitiveBase<T>::GetString(const CTagObject* parent, const CTagObject* parHolder) const
{
    const char* sz = this->prop->GetString((const behaviac::Agent*)parent, (const behaviac::Agent*)parHolder);
    return sz;
}

template <typename T>
const T& ParamVariablePrimitive<T>::GetValue(const CTagObject* parent, const CTagObject* parHolder) const
{
    if (this->prop && behaviac::Agent::DynamicCast(parent))
    {
        return ParamVariablePrimitiveBase<T>::GetValue(parent, parHolder);
    }

    return this->param;
}

BEHAVIAC_FORCEINLINE const char* ParamVariablePrimitive<char*>::GetValue(const CTagObject* parent, const CTagObject* parHolder) const
{
    if (this->prop && behaviac::Agent::DynamicCast(parent))
    {
        return ParamVariablePrimitiveBase<char*>::GetString(parent, parHolder);
    }

    return this->param.c_str();
}

BEHAVIAC_FORCEINLINE const char* ParamVariablePrimitive<const char*>::GetValue(const CTagObject* parent, const CTagObject* parHolder) const
{
    if (this->prop && behaviac::Agent::DynamicCast(parent))
    {
        return ParamVariablePrimitiveBase<const char*>::GetString(parent, parHolder);
    }

    return this->param.c_str();
}

BEHAVIAC_FORCEINLINE void ParamVariablePrimitive<char*>::SetVariableRegistry(const CTagObject* parHolder, ParamVariablePrimitive<char*>::GetReturnType value)
{
    if (this->prop && this->IsRefOut() && behaviac::Agent::DynamicCast(parHolder))
    {
        this->prop->SetString((behaviac::Agent*)parHolder, (char * const&)value);
    }
}

BEHAVIAC_FORCEINLINE void ParamVariablePrimitive<const char*>::SetVariableRegistry(const CTagObject* parHolder, ParamVariablePrimitive<const char*>::GetReturnType value)
{
    if (this->prop && this->IsRefOut() && behaviac::Agent::DynamicCast(parHolder))
    {
        this->prop->SetString((behaviac::Agent*)parHolder, value);
    }
}


BEHAVIAC_FORCEINLINE const IList& ParamVariablePrimitive<IList>::GetValue() const
{
    BEHAVIAC_ASSERT(false);
    return this->GetValue(0, 0);
}

BEHAVIAC_FORCEINLINE const IList& ParamVariablePrimitive<IList>::GetValue(const CTagObject* parent, const CTagObject* parHolder) const
{
    BEHAVIAC_ASSERT(this->prop && behaviac::Agent::DynamicCast(parent));

    const IList* pList = this->prop->CreateList((const behaviac::Agent*)parent, (const behaviac::Agent*)parHolder);
    return *pList;
}

BEHAVIAC_FORCEINLINE const System::Object& ParamVariablePrimitive<System::Object>::GetValue() const
{
    BEHAVIAC_ASSERT(false);
    return this->GetValue(0, 0);
}

BEHAVIAC_FORCEINLINE const System::Object& ParamVariablePrimitive<System::Object>::GetValue(const CTagObject* parent, const CTagObject* parHolder) const
{
    BEHAVIAC_ASSERT(this->prop && behaviac::Agent::DynamicCast(parent));

    // behaviac::TProperty<System::Object>* pT = (behaviac::TProperty<System::Object>*)this->prop;

    // return pT->GetValue((const behaviac::Agent*)parent, (const behaviac::Agent*)parHolder);
    behaviac::TProperty<int>* pT = (behaviac::TProperty<int>*)this->prop;

    const int& v = pT->GetValue((const behaviac::Agent*)parent, (const behaviac::Agent*)parHolder);

    const System::Object& r = *(const System::Object*)&v;

    return r;
}

namespace behaviac
{
    template <typename T>
    const CMemberBase* GetMemberFromName(const behaviac::CStringID& propertyId)
    {
        const CTagObjectDescriptor& obejctDesc = T::GetObjectDescriptor();

        return obejctDesc.GetMember(propertyId);
    }
}

template <typename T>
const T& ParamVariableStruct<T>::GetValue(const CTagObject* parent, const CTagObject* parHolder) const
{
    if (this->prop && behaviac::Agent::DynamicCast(parent))
    {
        behaviac::TProperty<T>* pT = (behaviac::TProperty<T>*)this->prop;

        return pT->GetValue((const behaviac::Agent*)parent, (const behaviac::Agent*)parHolder);
    }

    if (this->m_props.size() > 0)
    {
        for (PropsMap_t::const_iterator it = this->m_props.begin();
             it != this->m_props.end(); ++it)
        {
            const behaviac::CStringID& propId = it->first;
            const behaviac::Property* pProperty = it->second;

            const CMemberBase* pM = behaviac::GetMemberFromName<T>(propId);
            pM->SetFromProperty((CTagObject*)&this->param, parHolder, pProperty);
        }
    }

    return this->param;
}

template <typename T>
void ParamVariableStruct<T>::SetVariableRegistry(const CTagObject* parHolder, const T& value)
{
    if (this->IsRefOut())
    {
        if (this->m_props.size() > 0)
        {
            for (PropsMap_t::iterator it = this->m_props.begin();
                 it != this->m_props.end(); ++it)
            {
                const behaviac::CStringID& propId = it->first;
                behaviac::Property* pProperty = it->second;

                const CMemberBase* pM = behaviac::GetMemberFromName<T>(propId);
                pProperty->SetFrom((behaviac::Agent*)&this->param, pM, (behaviac::Agent*)parHolder);
            }
        }

        if (this->prop && behaviac::Agent::DynamicCast(parHolder))
        {
            behaviac::TProperty<T>* pT = (behaviac::TProperty<T>*)this->prop;

            pT->SetValue((behaviac::Agent*)parHolder, value);
        }
    }
}


template <typename T>
float CMemberBase::TDifferencePercentage(const behaviac::Property* l, const behaviac::Property* r)
{
    const behaviac::TProperty<T>* pV = (const behaviac::TProperty<T>*)l;
    const T& v = pV->GetDefaultValue();

    const behaviac::TProperty<T>* pOtherV = (const behaviac::TProperty<T>*)r;
    const T& ov = pOtherV->GetDefaultValue();

    float result = FloatReturnSubtract(v, ov);

    return result / this->m_range;
}

namespace behaviac
{
    template<typename VariableType>
    BEHAVIAC_FORCEINLINE void Agent::SetVariable(const char* variableName, const VariableType& value, uint32_t variableId)
    {
        if (variableId == 0)
        {
            variableId = MakeVariableId(variableName);
        }

        Property* pProperty = AgentProperties::GetProperty(this->GetObjectTypeName(), variableId);

        if (pProperty != 0)
        {
            TProperty<VariableType>* pTProperty = (TProperty<VariableType>*)pProperty;
            pTProperty->SetValue(this, value);
        }
        else
        {
            bool bLocal = true;
            this->SetVariableRegistry(bLocal, 0, variableName, value, 0, variableId);
        }
    }
}

#endif//BEHAVIAC_PROPERTY_T_H
