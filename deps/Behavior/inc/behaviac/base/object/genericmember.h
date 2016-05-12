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

#ifndef BEHAVIAC_ENGINESERVICES_GENERICMEMBER_H
#define BEHAVIAC_ENGINESERVICES_GENERICMEMBER_H

#include "behaviac/base/object/typehandler.h"
#include "behaviac/base/object/uitype.h"
#include "behaviac/base/object/member.h"
#include "behaviac/property/property.h"
#include "behaviac/property/operators.inl"
#include "behaviac/agent/agent.h"

#include "behaviac/network/network.h"

template<class ObjectType, class MemberType, class MemberHandler, uint32_t PropertyFlags>
class CGenericMemberBase : public CMemberBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMemberBase);

    CGenericMemberBase(MemberType ObjectType::* memberPtr, const char* className, const char* propertyName, UiGenericType* uiWrapper) : CMemberBase(propertyName, className), m_memberPtr(memberPtr), m_getter(0), m_setter(0), m_uiWrapper(uiWrapper)
    {
    }

    //typedef const MemberType& (ObjectType::*Getter)();
    //typedef void(ObjectType::*Setter)(const MemberType&);

    CGenericMemberBase(const MemberType & (ObjectType::*getter)() const, void (ObjectType::*setter)(const MemberType&), const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CMemberBase(propertyName, className), m_memberPtr(0), m_getter(getter), m_setter(setter), m_uiWrapper(uiWrapper)
    {
    }

    CGenericMemberBase(const CGenericMemberBase& copy) : CMemberBase(copy), m_memberPtr(copy.m_memberPtr), m_getter(copy.m_getter), m_setter(copy.m_setter), m_uiWrapper(copy.m_uiWrapper)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CGenericMemberBase(*this);

        return p;
    }

    virtual void Load(CTagObject* parent, const behaviac::ISerializableNode* node)
    {
        if (PropertyFlags & EPersistenceType_Description_Load)
        {
            MemberHandler::Load(node, ((ObjectType*)parent)->*m_memberPtr, this->m_classFullName, m_propertyID);
        }
    }

    virtual void Save(const CTagObject* parent, behaviac::ISerializableNode* node)
    {
        if (PropertyFlags & EPersistenceType_Description_Save)
        {
            MemberHandler::Save(node, ((ObjectType*)parent)->*m_memberPtr, this->m_classFullName, m_propertyID);
        }
    }

    virtual void LoadState(CTagObject* parent, const behaviac::ISerializableNode* node)
    {
        if (PropertyFlags & EPersistenceType_State_Load)
        {
            MemberHandler::LoadState(node, ((ObjectType*)parent)->*m_memberPtr, this->m_classFullName, m_propertyID);
        }
    }

    virtual void SaveState(const CTagObject* parent, behaviac::ISerializableNode* node)
    {
        if (PropertyFlags & EPersistenceType_State_Save)
        {
            MemberHandler::SaveState(node, ((ObjectType*)parent)->*m_memberPtr, this->m_classFullName, m_propertyID);
        }
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        if (PropertyFlags & EPersistenceType_UiInfo)
        {
            int readonlyFlag = this->READONLYFLAG();
            MemberHandler::GetUiInfo(types, xmlNode, ((ObjectType*)parent)->*m_memberPtr, m_bStatic, readonlyFlag, this->m_classFullName, m_propertyID, this->GetDisplayName(), this->GetDesc(), m_uiWrapper);
        }
    }

    virtual void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        MemberHandler::GetMethodsDescription(types, xmlNode, ((ObjectType*)parent)->*m_memberPtr, this->m_classFullName, m_propertyID.GetString());
    }

    virtual int READONLYFLAG() const
    {
        int flag = 0;

        if (this->m_bIsConst)
        {
            flag |= 0x1;
        }

        if (this->m_getter != 0)
        {
            flag |= 0x2;

            if (this->m_setter == 0)
            {
                flag |= 0x1;
            }
        }

        return flag;
    }

    virtual void Set(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(value);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(::GetClassTypeNumberId<MemberType>() == typeId);
        BEHAVIAC_ASSERT(false);
    }

    virtual const void* Get(const CTagObject* parent, int typeId) const
    {
        bool bOk = (typeId == ::GetClassTypeNumberId<System::Object>() ||
                    ::GetClassTypeNumberId<MemberType>() == typeId);

        BEHAVIAC_ASSERT(bOk);

        if (bOk)
        {
            if (m_memberPtr)
            {
                return &(((ObjectType*)parent)->*m_memberPtr);
            }
            else if (this->m_getter)
            {
                return &(((ObjectType*)parent)->*m_getter)();
            }
        }

        return 0;
    }

    virtual void SetVariable(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_ASSERT(::GetClassTypeNumberId<MemberType>() == typeId);

        if (::GetClassTypeNumberId<MemberType>() == typeId)
        {
            behaviac::Agent* pAgent = (behaviac::Agent*)parent;
            const MemberType& v = *(MemberType*)value;

            const char* propertyName = m_propertyID.GetString();
            //pAgent->SetVariable(propertyName, v);
            //BEHAVIAC_ASSERT(false);
            pAgent->SetVariableRegistry(true, this, propertyName, v, 0, this->m_propertyID.GetID().GetUniqueID());

            //behaviac::NetworkRole netRole = this->GetNetRole();

            //if (netRole != behaviac::NET_ROLE_DEFAULT)
            //{
            //	behaviac::Network* pNw = behaviac::Network::GetInstance();

            //	if (pNw && !pNw->IsSinglePlayer())
            //	{
            //		if (netRole == behaviac::NET_ROLE_AUTHORITY && !pNw->IsAuthority() ||
            //			netRole == behaviac::NET_ROLE_NONAUTHORITY && pNw->IsAuthority())
            //		{
            //			pNw->ReplicateVariable(pAgent, propertyName, typeId, (void*)&v, true);
            //		}
            //	}
            //}
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void ReplicateProperty(behaviac::Agent* pAgent)
    {
        const char* propertyName = m_propertyID.GetString();
        behaviac::NetworkRole netRole = this->NETROLE();

        if (netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                bool bSend = false;
                bool bReceive = false;

                if (pNw->IsAuthority())
                {
                    if (netRole == behaviac::NET_ROLE_NONAUTHORITY)
                    {
                        bSend = true;

                    }
                    else if (netRole == behaviac::NET_ROLE_AUTHORITY)
                    {
                        bReceive = true;
                    }
                }
                else
                {
                    if (netRole == behaviac::NET_ROLE_AUTHORITY)
                    {
                        bSend = true;

                    }
                    else if (netRole == behaviac::NET_ROLE_NONAUTHORITY)
                    {
                        bReceive = true;
                    }
                }

                if (bSend || bReceive)
                {
                    int typeId = GetClassTypeNumberId<MemberType>();

                    pNw->ReplicateVariable(pAgent, propertyName, typeId, (void*) & (((ObjectType*)pAgent)->*m_memberPtr), bSend);
                }
            }
        }
    }
#endif//#if BEHAVIAC_ENABLE_NETWORKD
    virtual void*       GetVariable(const CTagObject* parent, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(!"Only works with CGenericMembers");
        return NULL;
    };

    virtual int GetTypeId() const
    {
        return ::GetClassTypeNumberId<MemberType>();
    }

    virtual void SetFromProperty(CTagObject* parent, const CTagObject* parHolder, const behaviac::Property* pProperty) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_UNUSED_VAR(pProperty);
    }

    virtual bool       Equal(const CTagObject* lhs, const CTagObject* rhs) const
    {
        const MemberType& l_v = *(const MemberType*)this->Get(lhs, GetClassTypeNumberId<MemberType>());
        const MemberType& r_v = *(const MemberType*)this->Get(rhs, GetClassTypeNumberId<MemberType>());

        return behaviac::Details::Equal(l_v, r_v);
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
        typedef PARAM_BASETYPE(MemberType)				StoredMemberType;

        behaviac::Property* pProperty = behaviac::Property::Creator<StoredMemberType>(defaultValue, this, bConst);

        return pProperty;
    }

    virtual float DifferencePercentage(const behaviac::Property* l, const behaviac::Property* r)
    {
        BEHAVIAC_UNUSED_VAR(l);
        BEHAVIAC_UNUSED_VAR(r);
        return 0.0f;
    }

protected:
    MemberType ObjectType::*    m_memberPtr;

    const MemberType& (ObjectType::*m_getter)() const;
    void(ObjectType::*m_setter)(const MemberType&);

    UiGenericType*              m_uiWrapper;
};

template<class ObjectType, class MemberType, class MemberHandler, uint32_t PropertyFlags>
class CGenericMemberNonConst : public CGenericMemberBase<ObjectType, MemberType, MemberHandler, PropertyFlags>
{
public:
    CGenericMemberNonConst(MemberType ObjectType::* memberPtr, const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CGenericMemberBase<ObjectType, MemberType, MemberHandler, PropertyFlags>(memberPtr, className, propertyName, uiWrapper)
    {
    }

    CGenericMemberNonConst(const MemberType & (ObjectType::*getter)() const, void (ObjectType::*setter)(const MemberType&), const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CGenericMemberBase<ObjectType, MemberType, MemberHandler, PropertyFlags>(getter, setter, className, propertyName, uiWrapper)
    {
    }

    CGenericMemberNonConst(const CGenericMemberNonConst& copy) : CGenericMemberBase<ObjectType, MemberType, MemberHandler, PropertyFlags>(copy)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CGenericMemberNonConst(*this);

        return p;
    }

    virtual void SetFromProperty(CTagObject* parent, const CTagObject* parHolder, const behaviac::Property* pProperty) const
    {
        int typeId = this->GetTypeId();
        BEHAVIAC_ASSERT(pProperty->GetTypeId() == typeId);

        const behaviac::TProperty<MemberType>* pTProperty = (const behaviac::TProperty<MemberType>*)pProperty;
        const MemberType& v = pTProperty->GetValue((behaviac::Agent*)parHolder);

        this->Set(parent, &v, typeId);
    }

    virtual void Set(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_ASSERT(::GetClassTypeNumberId<MemberType>() == typeId);

        if (::GetClassTypeNumberId<MemberType>() == typeId)
        {
            if (this->m_memberPtr)
            {
                ((ObjectType*)parent)->*this->m_memberPtr = *((MemberType*)value);
            }
            else if (this->m_setter)
            {
                (((ObjectType*)parent)->*this->m_setter)(*((MemberType*)value));
            }
            else
            {
                BEHAVIAC_ASSERT(false, "weird, a property's setter is not set");
            }
        }
    }

    virtual float DifferencePercentage(const behaviac::Property* l, const behaviac::Property* r)
    {
        return CMemberBase::TDifferencePercentage<MemberType>(l, r);
    }
};

template<class ObjectType, class MemberType, class MemberHandler, uint32_t PropertyFlags>
class CGenericMemberConst : public CGenericMemberBase<ObjectType,  MemberType, MemberHandler, PropertyFlags>
{
public:
    CGenericMemberConst(MemberType ObjectType::* memberPtr, const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CGenericMemberBase<ObjectType, MemberType, MemberHandler, PropertyFlags>(memberPtr, className, propertyName, uiWrapper)
    {
        this->m_bIsConst = true;
    }

    CGenericMemberConst(const MemberType & (ObjectType::*getter)() const, void (ObjectType::*setter)(const MemberType&), const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CGenericMemberBase<ObjectType, MemberType, MemberHandler, PropertyFlags>(getter, setter, className, propertyName, uiWrapper)
    {
    }

    CGenericMemberConst(const CGenericMemberConst& copy) : CGenericMemberBase<ObjectType, MemberType, MemberHandler, PropertyFlags>(copy)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CGenericMemberConst(*this);

        return p;
    }

    virtual void Set(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_ASSERT(::GetClassTypeNumberId<MemberType>() == typeId);
        BEHAVIAC_ASSERT(false, "wierd, a const member is set value");
    }

    virtual float DifferencePercentage(const behaviac::Property* l, const behaviac::Property* r)
    {
        return CMemberBase::TDifferencePercentage<typename behaviac::Meta::RemoveConst<MemberType>::Result>(l, r);
    }
};


template<class ObjectType, class MemberType, class MemberHandler, uint32_t PropertyFlags, bool bIsConst>
struct CGenericMemberSelector
{
    typedef CGenericMemberNonConst<ObjectType, MemberType, MemberHandler, PropertyFlags> CGenericMember;
};

template<class ObjectType, class MemberType, class MemberHandler, uint32_t PropertyFlags>
struct CGenericMemberSelector<ObjectType, MemberType, MemberHandler, PropertyFlags, true>
{
    typedef CGenericMemberConst<ObjectType, MemberType, MemberHandler, PropertyFlags> CGenericMember;
};

template<class ObjectType, class MemberType, class MemberHandler, uint32_t PropertyFlags>
class CGenericMember : public CGenericMemberSelector<ObjectType, MemberType, MemberHandler, PropertyFlags, behaviac::Meta::IsConst<MemberType>::Result>::CGenericMember
{
public:
    CGenericMember(MemberType ObjectType::* memberPtr, const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CGenericMemberSelector<ObjectType, MemberType, MemberHandler, PropertyFlags, behaviac::Meta::IsConst<MemberType>::Result>::CGenericMember(memberPtr, className, propertyName, uiWrapper)
    {
    }

    CGenericMember(const MemberType & (ObjectType::*getter)() const, void (ObjectType::*setter)(const MemberType&), const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CGenericMemberSelector<ObjectType, MemberType, MemberHandler, PropertyFlags, behaviac::Meta::IsConst<MemberType>::Result>::CGenericMember(getter, setter, className, propertyName, uiWrapper)
    {
    }

    CGenericMember(const CGenericMember& copy) : CGenericMemberSelector<ObjectType, MemberType, MemberHandler, PropertyFlags, behaviac::Meta::IsConst<MemberType>::Result>::CGenericMember(copy)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CGenericMember(*this);

        return p;
    }
};


template<class MemberType, class MemberHandler, uint32_t PropertyFlags>
class CStaticMemberBase : public CMemberBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CStaticMemberBase);

    CStaticMemberBase(MemberType* memberPtr, const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CMemberBase(propertyName, className), m_memberPtr(memberPtr), m_getter(0), m_setter(0), m_uiWrapper(uiWrapper)
    {
        m_bStatic = true;
    }

    CStaticMemberBase(const MemberType & (*getter)(), void (*setter)(const MemberType&), const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CMemberBase(propertyName, className), m_memberPtr(0), m_getter(getter), m_setter(setter), m_uiWrapper(uiWrapper)
    {
        m_bStatic = true;
    }

    CStaticMemberBase(const CStaticMemberBase& copy) :
        CMemberBase(copy), m_memberPtr(copy.m_memberPtr), m_getter(copy.m_getter), m_setter(copy.m_setter), m_uiWrapper(copy.m_uiWrapper)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CStaticMemberBase(*this);

        return p;
    }

    virtual void Load(CTagObject* parent, const behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        if (PropertyFlags & EPersistenceType_Description_Load)
        {
            MemberHandler::Load(node, *m_memberPtr, this->m_classFullName, m_propertyID);
        }
    }

    virtual void Save(const CTagObject* parent, behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        if (PropertyFlags & EPersistenceType_Description_Save)
        {
            MemberHandler::Save(node, *m_memberPtr, this->m_classFullName, m_propertyID);
        }
    }

    virtual void LoadState(CTagObject* parent, const behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(node);

        if (PropertyFlags & EPersistenceType_State_Load)
        {
            MemberHandler::LoadState(node, *m_memberPtr, this->m_classFullName, m_propertyID);
        }
    }

    virtual void SaveState(const CTagObject* parent, behaviac::ISerializableNode* node)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(node);

        if (PropertyFlags & EPersistenceType_State_Save)
        {
            MemberHandler::SaveState(node, *m_memberPtr, this->m_classFullName, m_propertyID);
        }
    }

    virtual int READONLYFLAG() const
    {
        int flag = 0;

        if (this->m_bIsConst)
        {
            flag |= 0x1;
        }

        if (this->m_getter != 0)
        {
            flag |= 0x2;

            if (this->m_setter == 0)
            {
                flag |= 0x1;
            }
        }

        return flag;
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        if (PropertyFlags & EPersistenceType_UiInfo)
        {
            int readonlyFlag = this->READONLYFLAG();
            MemberHandler::GetUiInfo(types, xmlNode, *m_memberPtr, m_bStatic, readonlyFlag, this->m_classFullName, m_propertyID, this->GetDisplayName(), this->GetDesc(), m_uiWrapper);
        }
    }

    virtual void GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        MemberHandler::GetMethodsDescription(types, xmlNode, *m_memberPtr, this->m_classFullName, m_propertyID.GetString());
    }

    virtual void Set(const CTagObject* parent, const void* value, int typeId) const
    {
		BEHAVIAC_UNUSED_VAR(parent);
		BEHAVIAC_UNUSED_VAR(value);
		BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(false);
    }

    virtual const void* Get(const CTagObject* parent, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);

        BEHAVIAC_ASSERT(::GetClassTypeNumberId<MemberType>() == typeId);

        if (::GetClassTypeNumberId<MemberType>() == typeId)
        {
            if (m_memberPtr)
            {
                return (const void*) & (*m_memberPtr);
            }
            else if (this->m_getter)
            {
                return &(*m_getter)();
            }
        }

        return 0;
    }

    virtual void  SetVariable(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(::GetClassTypeNumberId<MemberType>() == typeId);

        if (::GetClassTypeNumberId<MemberType>() == typeId)
        {
            behaviac::Agent* pAgent = (behaviac::Agent*)parent;
            const MemberType& v = *(MemberType*)value;

            const char* propertyName = m_propertyID.GetString();
            //pAgent->SetVariable(propertyName, v);
            pAgent->SetVariableRegistry(false, this, propertyName, v, this->m_classFullName, this->m_propertyID.GetID().GetUniqueID());

            //behaviac::NetworkRole netRole = this->GetNetRole();

            //if (netRole != behaviac::NET_ROLE_DEFAULT)
            //{
            //	behaviac::Network* pNw = behaviac::Network::GetInstance();

            //	if (pNw && !pNw->IsSinglePlayer())
            //	{
            //		if (netRole == behaviac::NET_ROLE_AUTHORITY && !pNw->IsAuthority() ||
            //			netRole == behaviac::NET_ROLE_NONAUTHORITY && pNw->IsAuthority())
            //		{
            //			pNw->ReplicateVariable(this->m_classFullName, propertyName, typeId, (void*)&v, true);
            //		}
            //	}
            //}
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void ReplicateProperty(behaviac::Agent* pAgent)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);

        const char* propertyName = m_propertyID.GetString();
        behaviac::NetworkRole netRole = this->NETROLE();

        if (netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                bool bSend = false;
                bool bReceive = false;

                if (pNw->IsAuthority())
                {
                    if (netRole == behaviac::NET_ROLE_NONAUTHORITY)
                    {
                        bSend = true;

                    }
                    else if (netRole == behaviac::NET_ROLE_AUTHORITY)
                    {
                        bReceive = true;
                    }
                }
                else
                {
                    if (netRole == behaviac::NET_ROLE_AUTHORITY)
                    {
                        bSend = true;

                    }
                    else if (netRole == behaviac::NET_ROLE_NONAUTHORITY)
                    {
                        bReceive = true;
                    }
                }

                if (bSend || bReceive)
                {
                    int typeId = GetClassTypeNumberId<MemberType>();

                    pNw->ReplicateVariable(this->m_classFullName, propertyName, typeId, (void*) & (*m_memberPtr), bSend);
                }
            }
        }
    }
#endif//#if BEHAVIAC_ENABLE_NETWORKD
    virtual void*       GetVariable(const CTagObject* parent, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(!"Only works with CGenericMembers");
        return NULL;
    }

    virtual int GetTypeId() const
    {
        return ::GetClassTypeNumberId<MemberType>();
    }

    virtual void SetFromProperty(CTagObject* parent, const CTagObject* parHolder, const behaviac::Property* pProperty) const
    {
    }

    virtual bool       Equal(const CTagObject* lhs, const CTagObject* rhs) const
    {
        const MemberType& l_v = *(const MemberType*)this->Get(lhs, GetClassTypeNumberId<MemberType>());
        const MemberType& r_v = *(const MemberType*)this->Get(rhs, GetClassTypeNumberId<MemberType>());

        return behaviac::Details::Equal(l_v, r_v);
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
        typedef PARAM_BASETYPE(MemberType)				StoredMemberType;

        behaviac::Property* pProperty = behaviac::Property::Creator<StoredMemberType>(defaultValue, this, bConst);

        return pProperty;
    }

    virtual float DifferencePercentage(const behaviac::Property* l, const behaviac::Property* r)
    {
        return 0.0f;
    }

protected:
    MemberType*				m_memberPtr;
    const MemberType& (*m_getter)();
    void (*m_setter)(const MemberType&);

    UiGenericType*          m_uiWrapper;
};

template<class MemberType, class MemberHandler, uint32_t PropertyFlags>
class CStaticMemberNonConst : public CStaticMemberBase<MemberType, MemberHandler, PropertyFlags>
{
public:
    CStaticMemberNonConst(MemberType* memberPtr, const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CStaticMemberBase<MemberType, MemberHandler, PropertyFlags>(memberPtr, className, propertyName, uiWrapper)
    {
    }

    CStaticMemberNonConst(const MemberType & (*getter)(), void (*setter)(const MemberType&), const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CStaticMemberBase<MemberType, MemberHandler, PropertyFlags>(getter, setter, className, propertyName, uiWrapper)
    {
    }

    CStaticMemberNonConst(const CStaticMemberNonConst& copy) : CStaticMemberBase<MemberType, MemberHandler, PropertyFlags>(copy)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CStaticMemberNonConst(*this);

        return p;
    }

    virtual void Set(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(typeId);
        BEHAVIAC_ASSERT(::GetClassTypeNumberId<MemberType>() == typeId);

        if (::GetClassTypeNumberId<MemberType>() == typeId)
        {
            if (this->m_memberPtr)
            {
                *this->m_memberPtr = *((MemberType*)value);
            }
            else if (this->m_setter)
            {
                (*this->m_setter)(*((MemberType*)value));
            }
        }
    }

    virtual void SetFromProperty(CTagObject* parent, const CTagObject* parHolder, const behaviac::Property* pProperty) const
    {
        int typeId = this->GetTypeId();
        BEHAVIAC_ASSERT(pProperty->GetTypeId() == typeId);

        const behaviac::TProperty<MemberType>* pTProperty = (const behaviac::TProperty<MemberType>*)pProperty;
        const MemberType& v = pTProperty->GetValue((behaviac::Agent*)parHolder);

        this->Set(parent, &v, typeId);
    }

    virtual float DifferencePercentage(const behaviac::Property* l, const behaviac::Property* r)
    {
        return CMemberBase::TDifferencePercentage<MemberType>(l, r);
    }
};

template<class MemberType, class MemberHandler, uint32_t PropertyFlags>
class CStaticMemberConst : public CStaticMemberBase<MemberType, MemberHandler, PropertyFlags>
{
public:
    CStaticMemberConst(MemberType* memberPtr, const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CStaticMemberBase<MemberType, MemberHandler, PropertyFlags>(memberPtr, className, propertyName, uiWrapper)
    {
    }

    CStaticMemberConst(const MemberType & (*getter)(), void(*setter)(const MemberType&), const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CStaticMemberBase<MemberType, MemberHandler, PropertyFlags>(getter, setter, className, propertyName, uiWrapper)
    {
    }

    CStaticMemberConst(const CStaticMemberConst& copy) : CStaticMemberBase<MemberType, MemberHandler, PropertyFlags>(copy)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CStaticMemberConst(*this);

        return p;
    }

    virtual void Set(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_ASSERT(::GetClassTypeNumberId<MemberType>() == typeId);
        BEHAVIAC_ASSERT(false, "wierd, a const member is set value");
    }

    virtual float DifferencePercentage(const behaviac::Property* l, const behaviac::Property* r)
    {
        return CMemberBase::TDifferencePercentage<typename behaviac::Meta::RemoveConst<MemberType>::Result>(l, r);
    }
};


template<class MemberType, class MemberHandler, uint32_t PropertyFlags, bool bIsConst>
struct CStaticMemberSelector
{
    typedef CStaticMemberNonConst<MemberType, MemberHandler, PropertyFlags> CStaticMember;
};

template<class MemberType, class MemberHandler, uint32_t PropertyFlags>
struct CStaticMemberSelector<MemberType, MemberHandler, PropertyFlags, true>
{
    typedef CStaticMemberConst<MemberType, MemberHandler, PropertyFlags> CStaticMember;
};

template<class MemberType, class MemberHandler, uint32_t PropertyFlags>
class CStaticMember : public CStaticMemberSelector<MemberType, MemberHandler, PropertyFlags, behaviac::Meta::IsConst<MemberType>::Result>::CStaticMember
{
public:
    CStaticMember(MemberType* memberPtr, const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CStaticMemberSelector<MemberType, MemberHandler, PropertyFlags, behaviac::Meta::IsConst<MemberType>::Result>::CStaticMember(memberPtr, className, propertyName, uiWrapper)
    {
    }

    CStaticMember(const MemberType & (*getter)(), void (*setter)(const MemberType&), const char* className, const char* propertyName, UiGenericType* uiWrapper) :
        CStaticMemberSelector<MemberType, MemberHandler, PropertyFlags, behaviac::Meta::IsConst<MemberType>::Result>::CStaticMember(getter, setter, className, propertyName, uiWrapper)
    {
    }

    CStaticMember(const CStaticMember& copy) : CStaticMemberSelector<MemberType, MemberHandler, PropertyFlags, behaviac::Meta::IsConst<MemberType>::Result>::CStaticMember(copy)
    {}

    virtual CMemberBase* clone() const
    {
        CMemberBase* p = BEHAVIAC_NEW CStaticMember(*this);

        return p;
    }
};


BEHAVIAC_FORCEINLINE CMemberBase& _addMember(CTagObject::MembersContainer& members, CMemberBase* _member)
{
    members.membersVector.push_back(_member);
    members.membersMap[_member->GetID().GetID()] = _member;
    return *_member;
}

namespace CMemberFactory
{
    template<template <class T> class MemberHandler, uint32_t PropertyFlags, class ObjectType, class MemberType>
    static CMemberBase* Create(const char* className, const char* propertyName, MemberType ObjectType::* memberPtr, UiGenericType* uiWrapper)
    {
        BEHAVIAC_UNUSED_VAR(className);
        typedef CGenericMember<ObjectType, MemberType, MemberHandler<MemberType>, PropertyFlags> MemberTypeType;
        return BEHAVIAC_NEW MemberTypeType(memberPtr, className, propertyName, uiWrapper);
    }

    template<template <class T> class MemberHandler, uint32_t PropertyFlags, class MemberType>
    static CMemberBase* Create(const char* className, const char* propertyName, MemberType* memberPtr, UiGenericType* uiWrapper)
    {
        BEHAVIAC_UNUSED_VAR(className);
        typedef CStaticMember<MemberType, MemberHandler<MemberType>, PropertyFlags> MemberTypeType;
        return BEHAVIAC_NEW MemberTypeType(memberPtr, className, propertyName, uiWrapper);
    }

    template<template <class T> class MemberHandler, uint32_t PropertyFlags, class MemberType>
    static CMemberBase* Create(const char* className, const char* propertyName, const MemberType & (*getter)(), UiGenericType* uiWrapper)
    {
        BEHAVIAC_UNUSED_VAR(className);
        typedef CStaticMember<MemberType, MemberHandler<MemberType>, PropertyFlags> MemberTypeType;
        return BEHAVIAC_NEW MemberTypeType(getter, 0, className, propertyName, uiWrapper);
    }

    template<template <class T> class MemberHandler, uint32_t PropertyFlags, class MemberType>
    static CMemberBase* Create(const char* className, const char* propertyName, const MemberType & (*getter)(), void (*setter)(const MemberType&), UiGenericType* uiWrapper)
    {
        BEHAVIAC_UNUSED_VAR(className);
        typedef CStaticMember<MemberType, MemberHandler<MemberType>, PropertyFlags> MemberTypeType;
        return BEHAVIAC_NEW MemberTypeType(getter, setter, className, propertyName, uiWrapper);
    }

    template<class MemberHandler, uint32_t PropertyFlags, class ObjectType, class MemberType>
    static CMemberBase* Create(const char* className, const char* propertyName, MemberType ObjectType::* memberPtr, UiGenericType* uiWrapper)
    {
        BEHAVIAC_UNUSED_VAR(className);
        typedef CGenericMember<ObjectType, MemberType, MemberHandler, PropertyFlags> MemberTypeType;
        return BEHAVIAC_NEW MemberTypeType(memberPtr, className, propertyName, uiWrapper);
    }

    template<template <class T> class MemberHandler, uint32_t PropertyFlags, class ObjectType, class MemberType>
    static CMemberBase* Create(const char* className, const char* propertyName, const MemberType & (ObjectType::*getter)() const, UiGenericType* uiWrapper)
    {
        BEHAVIAC_UNUSED_VAR(className);
        typedef CGenericMember<ObjectType, MemberType, MemberHandler<MemberType>, PropertyFlags> MemberTypeType;
        return BEHAVIAC_NEW MemberTypeType(getter, 0, className, propertyName, uiWrapper);
    }

    template<template <class T> class MemberHandler, uint32_t PropertyFlags, class ObjectType, class MemberType>
    static CMemberBase* Create(const char* className, const char* propertyName, const MemberType & (ObjectType::*getter)() const, void (ObjectType::*setter)(const MemberType&), UiGenericType* uiWrapper)
    {
        BEHAVIAC_UNUSED_VAR(className);
        typedef CGenericMember<ObjectType, MemberType, MemberHandler<MemberType>, PropertyFlags> MemberTypeType;
        return BEHAVIAC_NEW MemberTypeType(getter, setter, className, propertyName, uiWrapper);
    }
};

#include "behaviac/base/object/uitype.h"
#include "behaviac/base/object/groupmember.h"
#include "behaviac/base/object/serializationevent.h"
#include "behaviac/base/object/containermember.h"
#include "behaviac/base/object/containermemberex.h"

// Macros for register_member description
#define UIDescription_Add(description) UiBasicType(Ui_None, description)
///////////////////////////////////////

#endif // #ifndef BEHAVIAC_ENGINESERVICES_GENERICMEMBER_H
