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

#ifndef BEHAVIAC_ENGINESERVICES_MEMBER_H
#define BEHAVIAC_ENGINESERVICES_MEMBER_H

#include "behaviac/base/object/typehandler.h"
#include "behaviac/base/object/uitype.h"

#include "behaviac/property/property.h"

#include "behaviac/network/network.h"

// Used in ContainedMember and GroupMember
#define CreateChildNode true
#define DoNotCreateChildNode false

class CTagObject;

namespace behaviac
{
    class Property;
    class Agent;
}

template<typename T>
inline float FloatReturnSubtract(const T& lhs, const T& rhs)
{
    BEHAVIAC_UNUSED_VAR(lhs);
    BEHAVIAC_UNUSED_VAR(rhs);

    return 0.0f;
}

inline float FloatReturnSubtract(const int& lhs, const int& rhs)
{
    int result = lhs > rhs ? (lhs - rhs) : rhs - lhs;

    return (float)result;
}

inline float FloatReturnSubtract(const unsigned int& lhs, const unsigned int& rhs)
{
    unsigned int result = lhs > rhs ? (lhs - rhs) : rhs - lhs;

    return (float)result;
}

template<>
inline float FloatReturnSubtract(const short& lhs, const short& rhs)
{
    short result = lhs > rhs ? (lhs - rhs) : rhs - lhs;

    return (float)result;
}

template<>
inline float FloatReturnSubtract(const unsigned short& lhs, const unsigned short& rhs)
{
    unsigned short result = lhs > rhs ? (lhs - rhs) : rhs - lhs;

    return (float)result;
}

template<>
inline float FloatReturnSubtract(const char& lhs, const char& rhs)
{
    char result = lhs > rhs ? (lhs - rhs) : rhs - lhs;

    return (float)result;
}

template<>
inline float FloatReturnSubtract(const unsigned char& lhs, const unsigned char& rhs)
{
    unsigned char result = lhs > rhs ? (lhs - rhs) : rhs - lhs;

    return (float)result;
}

template<>
inline float FloatReturnSubtract(const long& lhs, const long& rhs)
{
    long result = lhs > rhs ? (lhs - rhs) : rhs - lhs;

    return (float)result;
}

template<>
inline float FloatReturnSubtract(const unsigned long& lhs, const unsigned long& rhs)
{
    unsigned long result = lhs > rhs ? (lhs - rhs) : rhs - lhs;

    return (float)result;
}
/////////////////////////////////////////////////////////
///////////////    MEMBER INTERFACE    ////////////////
/////////////////////////////////////////////////////////
class BEHAVIAC_API CMemberBase
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CMemberBase);

public:
    CMemberBase(const char* propertyName, const char* classFullName)
        : m_parentType(PT_INVALID), m_bStatic(false), m_bIsConst(false), m_netRole(behaviac::NET_ROLE_DEFAULT),
          m_classFullName(classFullName), m_instaceName(classFullName), m_propertyID(propertyName), m_range(1.0f)
    {
        BEHAVIAC_ASSERT(m_classFullName);
    }

    CMemberBase(const CMemberBase& copy) :
        m_parentType(copy.m_parentType), m_bStatic(copy.m_bStatic), m_bIsConst(copy.m_bIsConst), m_netRole(copy.m_netRole),
        m_classFullName(copy.m_classFullName), m_instaceName(copy.m_instaceName), m_propertyID(copy.m_propertyID),
        m_displayName(copy.m_displayName), m_desc(copy.m_desc), m_range(copy.m_range)
    {}

    virtual ~CMemberBase()
    {
    }

    virtual void        Load(CTagObject* parent, const behaviac::ISerializableNode* node) = 0;
    virtual void        Save(const CTagObject* parent, behaviac::ISerializableNode* node) = 0;
    virtual void        LoadState(CTagObject* parent, const behaviac::ISerializableNode* node) = 0;
    virtual void        SaveState(const CTagObject* parent, behaviac::ISerializableNode* node) = 0;
    virtual void        GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode) = 0;
    virtual void        GetMethodsDescription(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode) = 0;
    virtual CMemberBase* clone() const = 0;
    virtual behaviac::CSerializationID GetID() const
    {
        return m_propertyID;
    }

    virtual void        Set(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(value);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(!"Only works with CGenericMembers");
    };
    virtual const void*       Get(const CTagObject* parent, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(!"Only works with CGenericMembers");
        return NULL;
    }

    virtual void        SetVariable(const CTagObject* parent, const void* value, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(value);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(!"Only works with CGenericMembers");
    }

    virtual void*       GetVariable(const CTagObject* parent, int typeId) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(typeId);

        BEHAVIAC_ASSERT(!"Only works with CGenericMembers");
        return NULL;
    }

    virtual int GetTypeId() const
    {
        BEHAVIAC_ASSERT(!"Only works with CGenericMembers");

        return 0;
    }

    virtual void SetFromProperty(CTagObject* parent, const CTagObject* parHolder, const behaviac::Property* pProperty) const
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_UNUSED_VAR(pProperty);
        BEHAVIAC_ASSERT(!"implemented by subclass!");
    }

    virtual bool       Equal(const CTagObject* lhs, const CTagObject* rhs) const
    {
        BEHAVIAC_UNUSED_VAR(lhs);
        BEHAVIAC_UNUSED_VAR(rhs);

        BEHAVIAC_ASSERT(!"Only works with CGenericMembers");

        return false;
    }

    ParentType GetParentType() const
    {
        return this->m_parentType;
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
        BEHAVIAC_UNUSED_VAR(defaultValue);
        BEHAVIAC_UNUSED_VAR(bConst);

        return 0;
    }

    template <typename T>
    float TDifferencePercentage(const behaviac::Property* l, const behaviac::Property* r);

    virtual float DifferencePercentage(const behaviac::Property* l, const behaviac::Property* r)
    {
        BEHAVIAC_UNUSED_VAR(l);
        BEHAVIAC_UNUSED_VAR(r);

        return 0.0f;
    }

    const char* GetClassNameString() const
    {
        return m_classFullName;
    }

    const char* GetInstanceNameString() const
    {
        return m_instaceName.c_str();
    }

    void SetInstanceNameString(const char* name)
    {
        m_instaceName = name;
    }

    const char* GetName() const
    {
        return this->m_propertyID.GetString();
    }

    const behaviac::wstring& GetDisplayName() const
    {
        return this->m_displayName;
    }

    const behaviac::wstring& GetDesc() const
    {
        return this->m_desc;
    }

    bool ISSTATIC() const
    {
        return this->m_bStatic;
    }

    CMemberBase& RANGE(float range)
    {
        this->m_range = range;

        return *this;
    }

    /// deparated, to use DISPLAY_INFO
    CMemberBase& DISPLAYNAME(const wchar_t* displayName)
    {
        BEHAVIAC_UNUSED_VAR(displayName);

        if (displayName)
        {
            m_displayName = displayName;
        }

        return *this;
    }

    /// deparated, to use DISPLAY_INFO
    CMemberBase& DESC(const wchar_t* desc)
    {
        BEHAVIAC_UNUSED_VAR(desc);

        if (desc)
        {
            m_desc = desc;
        }

        return *this;
    }

    CMemberBase& DISPLAY_INFO(const wchar_t* displayName, const wchar_t* desc)
    {
        BEHAVIAC_UNUSED_VAR(displayName);
        BEHAVIAC_UNUSED_VAR(desc);

        if (displayName)
        {
            m_displayName = displayName;
        }

        if (desc)
        {
            m_desc = desc;

        }
        else
        {
            m_desc = m_displayName;
        }

        return *this;
    }

    CMemberBase& SETREADONLY()
    {
        this->m_bIsConst = true;

        return *this;
    }

    virtual int READONLYFLAG() const
    {
        return this->m_bIsConst ? 0x1 : 0;
    }

#if BEHAVIAC_ENABLE_NETWORKD
    CMemberBase& NETROLE(behaviac::NetworkRole netRole)
    {
        m_netRole = netRole;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, m_propertyID.GetString());

                }
                else
                {
                    nameTemp = m_propertyID.GetString();
                }

                pNw->RegisterReplicatedProperty(this->m_netRole, nameTemp.c_str());
            }
        }

        return *this;
    }

    behaviac::NetworkRole NETROLE() const
    {
        return m_netRole;
    }

    virtual void ReplicateProperty(behaviac::Agent* pAgent)
    {
        BEHAVIAC_UNUSED_VAR(pAgent);
    }
#endif//#if BEHAVIAC_ENABLE_NETWORKD
protected:
    ParentType					m_parentType;
    bool						m_bStatic;
    bool						m_bIsConst;
    behaviac::NetworkRole		m_netRole;
    const char*					m_classFullName;
    behaviac::string			m_instaceName;
    behaviac::CSerializationID            m_propertyID;

    behaviac::wstring			m_displayName;
    behaviac::wstring			m_desc;

    float						m_range;
};

enum EPersistenceType
{
    EPersistenceType_Description_Load = 1 << 0,
    EPersistenceType_Description_Save = 1 << 1,
    EPersistenceType_Description = EPersistenceType_Description_Load | EPersistenceType_Description_Save,
    EPersistenceType_State_Load = 1 << 2,
    EPersistenceType_State_Save = 1 << 3,
    EPersistenceType_State = EPersistenceType_State_Load | EPersistenceType_State_Save,
    EPersistenceType_UiInfo = 1 << 4,
    EPersistenceType_Description_UiInfo = EPersistenceType_Description | EPersistenceType_UiInfo,

    EPersistenceType_All = EPersistenceType_Description | EPersistenceType_State,
    EPersistenceType_All_UiInfo = EPersistenceType_Description | EPersistenceType_State | EPersistenceType_UiInfo,
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_MEMBER_H
