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

#ifndef BEHAVIAC_ENGINESERVICES_METHOD_H
#define BEHAVIAC_ENGINESERVICES_METHOD_H

#include "behaviac/base/core/staticassert.h"
#include "behaviac/base/core/thread/asyncvalue.h"
#include "behaviac/base/meta/meta.h"
#include "behaviac/base/meta/isptr.h"

#include "behaviac/base/base.h"
#include "behaviac/property/property.h"
#include "behaviac/property/vector_ext.h"
#include "behaviac/base/object/typehandler.h"
#include "behaviac/network/network.h"

class CTagObject;

namespace behaviac
{
    namespace Meta
    {
        template <typename T, bool bAgent>
        class ParamTypeConverterAgent
        {
        public:
            typedef REAL_BASETYPE(T)		BaseType;
            typedef POINTER_TYPE(T)			PointerType;
            //typedef T*						BaseType;
            //typedef T**						PointerType;
        };

        template <typename T>
        class ParamTypeConverterAgent<T, true>
        {
            typedef REAL_BASETYPE(T)		TBaseType;
        public:
            typedef TBaseType*		BaseType;
            typedef TBaseType**		PointerType;
        };

        template <typename T>
        class ParamTypeConverter<T, true>
        {
			enum { BIsAgent = IsRefType<T>::Result };
        public:
            typedef typename ParamTypeConverterAgent<T, BIsAgent>::BaseType			BaseType;
            typedef typename ParamTypeConverterAgent<T, BIsAgent>::PointerType		PointerType;
        };
    }
}

template <typename MemberType, bool bAgent>
struct SaveToXMLDispatcher
{
    static void SaveToXML(behaviac::ISerializableNode& xmlNode, const MemberType& value, const char* paramName)
    {
        GenericTypeHandler<MemberType>::SaveToXML(xmlNode, const_cast<MemberType&>(value), 0, paramName);
    }
};

template <typename MemberType>
struct SaveToXMLDispatcher<MemberType, true>
{
    static void SaveToXML(behaviac::ISerializableNode& xmlNode, const MemberType& value, const char* paramName)
    {
        GenericTypeHandler<MemberType>::SaveToXML(xmlNode, value, 0, paramName);
    }
};

template <typename T>
class ParamVariablePrimitiveBase
{
public:
    ParamVariablePrimitiveBase() : bDefaultValid(false), prop(0)
    {}

    virtual ~ParamVariablePrimitiveBase()
    {
        this->prop = 0;
    }

    bool IsDefaultValid() const
    {
        return this->bDefaultValid;
    }

    void operator=(const ParamVariablePrimitiveBase& copy)
    {
        this->bDefaultValid = copy.bDefaultValid;

        if (copy.prop)
        {
            this->prop = copy.prop;
        }
        else
        {
            this->prop = 0;
        }
    }

	virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode, const char* paramName)
	{
		BEHAVIAC_UNUSED_VAR(parent);

		behaviac::CSerializationID id(paramName);
		const char* str = xmlNode.getAttrRaw(id);

		if (str)
		{
			//str[0] = '"', then it is a string                                                                                                                                                                                            
			if (str[0] != '"' && strchr(str, ' '))
			{
				behaviac::vector<behaviac::string> tokens;
				char* arrayindex = (char*)strchr(str, '[');
				if (arrayindex == NULL)
				{
					behaviac::StringUtils::SplitIntoArray(str, " ", tokens);

					if (tokens.size() == 2)
					{
						//int AgentTest::Property1                                                                                                                                                                                         
						this->prop = behaviac::Property::Create(tokens[0].c_str(), tokens[1].c_str(), false, 0);
					}
					else
					{
						//static int AgentTest::Property6                                                                                                                                                                                  
						BEHAVIAC_ASSERT(tokens[0] == "static");
						this->prop = behaviac::Property::Create(tokens[1].c_str(), tokens[2].c_str(), true, 0);
					}
				}
				else
				{
					// int AgentTest:Property[int xxx]                                                                                                                                                                                     
					*arrayindex = '\0';
					behaviac::StringUtils::SplitIntoArray(str, " ", tokens);
					arrayindex += 1;
					*(arrayindex + strlen(arrayindex) - 1) = '\0';
					this->prop = behaviac::Property::Create(tokens[0].c_str(), tokens[1].c_str(), false, arrayindex);
				}
			}
		}
	}

    void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode, const char* paramName)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        const T& value = GetValue(parent, 0);
        //SaveToXMLDispatcher<T, behaviac::Meta::IsRefType<REAL_BASETYPE(T)>::Result>::
        //	SaveToXML(xmlNode, const_cast<T&>(value), paramName);
        GenericTypeHandler<T>::SaveToXML(xmlNode, const_cast<T&>(value), 0, paramName);
    }

    bool IsRefOut() const
    {
        bool bConst = behaviac::Meta::IsConst<T>::Result;
        bool bRef = behaviac::Meta::IsRef<T>::Result;
        bool bPtr = behaviac::Meta::IsPtr<T>::Result;

        if (bConst)
        {
            return false;
        }

        return bRef || bPtr;
    }

    void SetValue(const T& v)
    {
        BEHAVIAC_UNUSED_VAR(v);
        BEHAVIAC_ASSERT(false);
    }

    const T& GetValue() const
    {
        BEHAVIAC_ASSERT(false);
        return this->GetValue(0, 0);
    }

    const T& GetValue(const CTagObject* parent, const CTagObject* parHolder) const;

    void SetVariableRegistry(const CTagObject* parHolder, const T& value);

    const char* GetString(const CTagObject* parent, const CTagObject* parHolder) const;

protected:
    bool					bDefaultValid;
    behaviac::Property*		prop;
};

template <typename T>
class ParamVariablePrimitive : public ParamVariablePrimitiveBase<T>
{
    //if agent, to save the ptr
    typedef STORED_TYPE(T) StoredType_t;
public:
    typedef const T& GetReturnType;

    ParamVariablePrimitive()
    {}

    ~ParamVariablePrimitive()
    {
    }

    void operator=(const ParamVariablePrimitive& copy)
    {
        this->ParamVariablePrimitiveBase<T>::operator=(copy);
        this->param = copy.param;
    }

    const T& GetValue(const CTagObject* parent, const CTagObject* parHolder) const;

    void SetValue(const T& v)
    {
        this->bDefaultValid = true;
        this->param = v;
    }

    const T& GetValue() const
    {
        //return this->param;
		return GetValueSelector<T, behaviac::Meta::IsRefType<T>::Result, behaviac::Meta::IsPtr<T>::Result>::GetValue(this->param);
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode, const char* paramName)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::CSerializationID id(paramName);
        const char* str = xmlNode.getAttrRaw(id);

        if (str)
        {
            //str[0] = '"', then it is a string
            if (str[0] != '"' && strchr(str, ' '))
            {
                ParamVariablePrimitiveBase<T>::LoadFromXML(parent, xmlNode, paramName);
            }
            else
            {
                if (!behaviac::StringUtils::FromString(str, this->param))
                {
                    BEHAVIAC_LOG1(BEHAVIAC_LOG_WARNING, "Fail read param count from behaviac::string:%s\n", str);
                }
            }
        }
    }
protected:
    StoredType_t			param;
};

template <>
class ParamVariablePrimitive<char*> : public ParamVariablePrimitiveBase<char*>
{
public:
    virtual ~ParamVariablePrimitive<char*>() {}

    typedef const char* GetReturnType;

    void operator=(const ParamVariablePrimitive& copy)
    {
        this->ParamVariablePrimitiveBase<char*>::operator=(copy);
        this->param = copy.param;
    }

    const char* GetValue(const CTagObject* parent, const CTagObject* parHolder) const;
    //const char* GetValue(const CTagObject* parent, const CTagObject* parHolder) const {
    //	return this->param.c_str();
    //}

    void SetValue(const char* v)
    {
        this->bDefaultValid = true;
        this->param = v;
    }

    const char* GetValue() const
    {
        return this->param.c_str();
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode, const char* paramName)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::CSerializationID id(paramName);
        const char* str = xmlNode.getAttrRaw(id);

        if (str)
        {
            //str[0] = '"', then it is a string
            if (str[0] != '"' && str[0] != '\"' && strchr(str, ' '))
            {
                ParamVariablePrimitiveBase<char*>::LoadFromXML(parent, xmlNode, paramName);
            }
            else
            {
                if (!behaviac::StringUtils::FromString(str, this->param))
                {
                    BEHAVIAC_LOG1(BEHAVIAC_LOG_WARNING, "Fail read param count from behaviac::string:%s\n", str);
                }
            }
        }
    }

    void SetVariableRegistry(const CTagObject* parHolder, GetReturnType value);

protected:
    typedef behaviac::string StoredType_t;

    StoredType_t			param;
};

template <>
class ParamVariablePrimitive<const char*> : public ParamVariablePrimitiveBase<const char*>
{
public:
    typedef const char* GetReturnType;

    void operator=(const ParamVariablePrimitive& copy)
    {
        this->ParamVariablePrimitiveBase<const char*>::operator=(copy);
        this->param = copy.param;
    }

    const char* GetValue(const CTagObject* parent, const CTagObject* parHolder) const;
    //const char* GetValue(const CTagObject* parent, const CTagObject* parHolder) const {
    //	return this->param.c_str();
    //}

    void SetValue(const char* v)
    {
        this->bDefaultValid = true;
        this->param = v;
    }

    const char* GetValue() const
    {
        return this->param.c_str();
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode, const char* paramName)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::CSerializationID id(paramName);
        const char* str = xmlNode.getAttrRaw(id);

        if (str)
        {
            //str[0] = '"', then it is a string
            if (str[0] != '"' &&  str[0] != '\"' && strchr(str, ' '))
            {
                ParamVariablePrimitiveBase<const char*>::LoadFromXML(parent, xmlNode, paramName);
            }
            else
            {
                if (!behaviac::StringUtils::FromString(str, this->param))
                {
                    BEHAVIAC_LOG1(BEHAVIAC_LOG_WARNING, "Fail read param count from behaviac::string:%s\n", str);
                }
            }
        }
    }

    void SetVariableRegistry(const CTagObject* parHolder, GetReturnType value);

protected:
    typedef behaviac::string StoredType_t;

    StoredType_t			param;
};

template <>
class ParamVariablePrimitive<IList> : public ParamVariablePrimitiveBase<IList>
{
public:
    typedef const IList& GetReturnType;
    virtual const IList& GetValue() const;
    virtual const IList& GetValue(const CTagObject* parent, const CTagObject* parHolder) const;
    void SetVariableRegistry(const CTagObject* parHolder, const IList& value)
    {
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_UNUSED_VAR(value);
        BEHAVIAC_ASSERT(true);
    }
};

template <>
class ParamVariablePrimitive<System::Object> : public ParamVariablePrimitiveBase<System::Object>
{
public:
    typedef const System::Object& GetReturnType;
    virtual const System::Object& GetValue() const;
    virtual const System::Object& GetValue(const CTagObject* parent, const CTagObject* parHolder) const;
    void SetVariableRegistry(const CTagObject* parHolder, const System::Object& value)
    {
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_UNUSED_VAR(value);
        BEHAVIAC_ASSERT(true);
    }
};

namespace behaviac
{
    namespace StringUtils
    {
        BEHAVIAC_API bool ParseForStruct(const char* str, behaviac::string& strT, behaviac::map<behaviac::CStringID, behaviac::Property*>& props);
    }
}

template <typename T>
class ParamVariablePtrPrimitive : public ParamVariablePrimitive<T>
{
public:
    ParamVariablePtrPrimitive()
    {
        this->param = 0;
    }

    void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode, const char* paramName)
    {
        behaviac::CSerializationID id(paramName);
        const char* str = xmlNode.getAttrRaw(id);

        if (str && string_icmp(str, "null") == 0)
        {
            BEHAVIAC_ASSERT(this->param == 0);

            return;
        }

        ParamVariablePrimitive<T>::LoadFromXML(parent, xmlNode, paramName);
    }
};

template <typename T>
class ParamVariableStruct : public ParamVariablePrimitive<T>
{
public:
    ParamVariableStruct()
    {}

    ~ParamVariableStruct()
    {
        for (PropsMap_t::iterator it = this->m_props.begin();
             it != this->m_props.end(); ++it)
        {
            behaviac::Property* p = it->second;
            BEHAVIAC_DELETE(p);
        }

        this->m_props.clear();
    }

    void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode, const char* paramName)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::CSerializationID id(paramName);
        const char* str = xmlNode.getAttrRaw(id);

        if (str)
        {
            if (*str != '{')
            {
                ParamVariablePrimitive<T>::LoadFromXML(parent, xmlNode, paramName);

            }
            else
            {
                behaviac::string strT;

                //{bLive=false;color=Red;name=0;weight=float par_float;}
                //{bLive=false;color=Red;name=0;weight=static int AgentTest::Property6;}
                //skip par and record par
                if (behaviac::StringUtils::ParseForStruct(str, strT, this->m_props))
                {
                    if (!behaviac::StringUtils::FromString(strT.c_str(), this->param))
                    {
                        BEHAVIAC_LOG1(BEHAVIAC_LOG_WARNING, "Fail read param count from behaviac::string:%s\n", str);
                    }
                }
            }
        }
    }

    const T& GetValue() const
    {
        return this->param;
    }

    const T& GetValue(const CTagObject* parent, const CTagObject* parHolder) const;

    void operator=(const ParamVariableStruct& copy)
    {
        ParamVariablePrimitive<T>* pBase = (ParamVariablePrimitive<T>*)this;
        *pBase = *(ParamVariablePrimitive<T>*)&copy;

        for (PropsMap_t::iterator it = this->m_props.begin();
             it != this->m_props.end(); ++it)
        {
            const behaviac::CStringID& propId = it->first;
            behaviac::Property* p = it->second;

            this->m_props[propId] = p;
        }
    }

    void SetVariableRegistry(const CTagObject* parHolder, const T& value);

private:
    typedef behaviac::map<behaviac::CStringID, behaviac::Property*> PropsMap_t;
    PropsMap_t		m_props;
};

template <typename T, bool bIsPtr>
struct ParamVariablePtrSelector
{
    typedef ParamVariablePrimitive<T> ParamVariableType;
};

template <typename T>
struct ParamVariablePtrSelector<T, true>
{
    typedef ParamVariablePtrPrimitive<T> ParamVariableType;
};

template <>
struct ParamVariablePtrSelector<char*, true>
{
    typedef ParamVariablePrimitive<char*> ParamVariableType;
};

template <>
struct ParamVariablePtrSelector<const char*, true>
{
    typedef ParamVariablePrimitive<const char*> ParamVariableType;
};

template <typename T, bool bStruct>
struct ParamVariableSelector
{
    //typedef ParamVariablePrimitive<T> ParamVariableType;
    typedef typename ParamVariablePtrSelector<T, behaviac::Meta::IsPtr<T>::Result>::ParamVariableType ParamVariableType;
};

template <typename T>
struct ParamVariableSelector<T, true>
{
    typedef ParamVariableStruct<T> ParamVariableType;
};


namespace behaviac
{
    namespace Meta
    {
        template<>
        struct HasFromString<IList>
        {
        public:
            enum
            {
                Result = false
            };

        };
    }
}
template <typename T>
struct ParamVariable : public ParamVariableSelector<T, behaviac::Meta::HasFromString<T>::Result>::ParamVariableType
{
    //
    virtual ~ParamVariable() {};
};

class IMethodReturn
{
public:
    IMethodReturn() : m_methodPtr(0), m_methodPtr_Static(0)
    {}

    IMethodReturn(const IMethodReturn& copy)
    {
        this->m_methodPtr = copy.m_methodPtr;
        this->m_methodPtr_Static = copy.m_methodPtr_Static;
    }

    virtual ~IMethodReturn()
    {
    }

    virtual IMethodReturn* clone() = 0;

    virtual void run(void* ret, const CTagObject* parent, behaviac::IAsyncValue& param)
    {
        BEHAVIAC_UNUSED_VAR(ret);
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(param);
    }
protected:
    void (IMethodReturn::*m_methodPtr)();

    void(*m_methodPtr_Static)();
};

template <typename RTYPE, typename ObjectType, typename ParamType>
class CMethodReturn : public IMethodReturn
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CMethodReturn);
    CMethodReturn() : IMethodReturn()
    {}

    virtual ~CMethodReturn()
    {
    }

    CMethodReturn(const CMethodReturn& copy) : IMethodReturn(copy)
    {
    }

    virtual IMethodReturn* clone()
    {
        return BEHAVIAC_NEW CMethodReturn(*this);
    }

    void SetHandler(RTYPE(ObjectType::*methodPtr)(ParamType))
    {
        typedef void (IMethodReturn::*MethodPtr_t)();
        this->m_methodPtr = reinterpret_cast<MethodPtr_t>(methodPtr);
    }

    virtual void run(void* ret, const CTagObject* parent, behaviac::IAsyncValue& param)
    {
        typedef RTYPE(ObjectType::*MethodPtr_t)(ParamType);
        behaviac::AsyncValue<ParamType>& paramValue = (behaviac::AsyncValue<ParamType>&)param;
        MethodPtr_t methodPtr = (MethodPtr_t)this->m_methodPtr;
        RTYPE result = (((ObjectType*)parent)->*methodPtr)(paramValue.get());
        *(RTYPE*)ret = result;
    }
};

template <typename RTYPE, typename ObjectType>
class CMethodReturn<RTYPE, ObjectType, void> : public IMethodReturn
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CMethodReturn);
    CMethodReturn() : IMethodReturn()
    {}

    CMethodReturn(const CMethodReturn& copy) : IMethodReturn(copy)
    {
    }

    virtual ~CMethodReturn()
    {
    }

    virtual IMethodReturn* clone()
    {
        return BEHAVIAC_NEW CMethodReturn(*this);
    }

    void SetHandler(RTYPE(ObjectType::*methodPtr)())
    {
        typedef void (IMethodReturn::*MethodPtr_t)();
        this->m_methodPtr = reinterpret_cast<MethodPtr_t>(methodPtr);
    }

    virtual void run(void* ret, const CTagObject* parent, behaviac::IAsyncValue& param)
    {
        typedef RTYPE(ObjectType::*MethodPtr_t)();
        MethodPtr_t methodPtr = (MethodPtr_t)this->m_methodPtr;
        RTYPE result = (((ObjectType*)parent)->*methodPtr)();
        *(RTYPE*)ret = result;
    }
};

template <typename RTYPE, typename ParamType>
class CMethodReturnStatic : public IMethodReturn
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CMethodReturnStatic);
    CMethodReturnStatic() : IMethodReturn()
    {}

    CMethodReturnStatic(const CMethodReturnStatic& copy) : IMethodReturn(copy)
    {
    }

    virtual IMethodReturn* clone()
    {
        return BEHAVIAC_NEW CMethodReturnStatic(*this);
    }

    void SetHandler(RTYPE(*methodPtr)(ParamType))
    {
        typedef void(*MethodPtr_t)();
        this->m_methodPtr_Static = reinterpret_cast<MethodPtr_t>(methodPtr);
    }

    virtual void run(void* ret, const CTagObject* parent, behaviac::IAsyncValue& param)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        typedef RTYPE(*MethodPtr_t)(ParamType);
        behaviac::AsyncValue<ParamType>& paramValue = (behaviac::AsyncValue<ParamType>&)param;
        MethodPtr_t methodPtr = (MethodPtr_t)this->m_methodPtr_Static;
        RTYPE result = (*methodPtr)(paramValue.get());
        *(RTYPE*)ret = result;
    }
};

template <typename RTYPE>
class CMethodReturnStatic<RTYPE, void> : public IMethodReturn
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CMethodReturnStatic);
    CMethodReturnStatic() : IMethodReturn()
    {}

    CMethodReturnStatic(const CMethodReturnStatic& copy) : IMethodReturn(copy)
    {
    }

    virtual IMethodReturn* clone()
    {
        return BEHAVIAC_NEW CMethodReturnStatic(*this);
    }

    void SetHandler(RTYPE(*methodPtr)())
    {
        typedef void(*MethodPtr_t)();
        this->m_methodPtr_Static = reinterpret_cast<MethodPtr_t>(methodPtr);
    }

    virtual void run(void* ret, const CTagObject* parent, behaviac::IAsyncValue& param)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(param);

        typedef RTYPE(*MethodPtr_t)();
        MethodPtr_t methodPtr = (MethodPtr_t)this->m_methodPtr_Static;
        RTYPE result = (*methodPtr)();
        *(RTYPE*)ret = result;
    }
};

class CCheckReturn
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CCheckReturn);
public:
    CCheckReturn() : m_method(0)
    {}

    CCheckReturn(const CCheckReturn& copy)
    {
        if (copy.m_method)
        {
            this->m_method = copy.m_method->clone();

        }
        else
        {
            this->m_method = 0;
        }
    }

    CCheckReturn* clone()
    {
        return BEHAVIAC_NEW CCheckReturn(*this);
    }

    ~CCheckReturn()
    {
        BEHAVIAC_DELETE(m_method);
    }

    template <typename RTYPE, typename ObjectType, typename ParamType>
    void SetHandler(RTYPE(ObjectType::*methodPtr)(ParamType))
    {
        typedef CMethodReturn<RTYPE, ObjectType, ParamType> MethodType;
        MethodType* method = BEHAVIAC_NEW MethodType();
        method->SetHandler(methodPtr);
        this->m_method = method;
    }

    template <typename RTYPE, typename ObjectType>
    void SetHandler(RTYPE(ObjectType::*methodPtr)())
    {
        typedef CMethodReturn<RTYPE, ObjectType, void> MethodType;
        MethodType* method = BEHAVIAC_NEW MethodType();
        method->SetHandler(methodPtr);
        this->m_method = method;
    }

    template <typename RTYPE, typename ParamType>
    void SetHandler(RTYPE(*methodPtr)(ParamType))
    {
        typedef CMethodReturnStatic<RTYPE, ParamType> MethodType;
        MethodType* method = BEHAVIAC_NEW MethodType();
        method->SetHandler(methodPtr);
        this->m_method = method;
    }

    template <typename RTYPE>
    void SetHandler(RTYPE(*methodPtr)())
    {
        typedef CMethodReturnStatic<RTYPE, void> MethodType;
        MethodType* method = BEHAVIAC_NEW MethodType();
        method->SetHandler(methodPtr);
        this->m_method = method;
    }

    template <typename RTYPE>
    void run(RTYPE& ret, const CTagObject* parent, behaviac::IAsyncValue& param)
    {
        this->m_method->run(&ret, parent, param);
    }
private:
    IMethodReturn*	m_method;
};

#define CMETHODBASE_RANGE_MIN_DEFAULT (-90000000.f)
#define CMETHODBASE_RANGE_MAX_DEFAULT (90000000.f)

struct DefaultParam_t
{
    int type_;

    union
    {
        bool				b_;
        char				c_;
        unsigned char		uc_;
        short				s_;
        unsigned short		us_;
        int					i_;
        unsigned int		ui_;
        long				l_;
        unsigned long		ul_;
        long long			ll_;
        unsigned long long	ull_;
        float				f_;
        double				d_;
        void*				v_;
    };

    behaviac::string		str_;

    /*
    please specialize it for your type

    template <>
    inline EnumTest DefaultParam_t::GetValue<EnumTest>() const
    {
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<short>());
    return (EnumTest)this->i_;
    }
    */
    template <typename T>
    T GetValue() const
    {
        BEHAVIAC_ASSERT(0, "please specialize this function for your type");
        static T t;
        return t;
    }

    bool IsValid() const
    {
        return this->type_ != 0;
    }

    DefaultParam_t() : type_(0), ull_(0)
    {
    }

    DefaultParam_t(bool p) : b_(p)
    {
        type_ = GetClassTypeNumberId<bool>();
    }

    DefaultParam_t(char p) : c_(p)
    {
        type_ = GetClassTypeNumberId<char>();
    }

    DefaultParam_t(unsigned char p) : uc_(p)
    {
        type_ = GetClassTypeNumberId<unsigned char>();
    }

    DefaultParam_t(short p) : s_(p)
    {
        type_ = GetClassTypeNumberId<short>();
    }

    DefaultParam_t(unsigned short p) : us_(p)
    {
        type_ = GetClassTypeNumberId<unsigned short>();
    }

    DefaultParam_t(int p) : i_(p)
    {
        type_ = GetClassTypeNumberId<int>();
    }

    DefaultParam_t(unsigned int p) : ui_(p)
    {
        type_ = GetClassTypeNumberId<unsigned int>();
    }

    DefaultParam_t(long p) : l_(p)
    {
        type_ = GetClassTypeNumberId<long>();
    }

    DefaultParam_t(unsigned long p) : ul_(p)
    {
        type_ = GetClassTypeNumberId<unsigned long>();
    }

    DefaultParam_t(long long p) : ll_(p)
    {
        type_ = GetClassTypeNumberId<long long>();
    }

    DefaultParam_t(unsigned long long p) : ull_(p)
    {
        type_ = GetClassTypeNumberId<unsigned long long>();
    }

    DefaultParam_t(float p) : f_(p)
    {
        type_ = GetClassTypeNumberId<float>();
    }

    DefaultParam_t(double p) : d_(p)
    {
        type_ = GetClassTypeNumberId<double>();
    }

    DefaultParam_t(char* p) : str_(p)
    {
        type_ = GetClassTypeNumberId<behaviac::string>();
    }

    DefaultParam_t(const char* p) : str_(p)
    {
        type_ = GetClassTypeNumberId<behaviac::string>();
    }
};

template <>
inline bool DefaultParam_t::GetValue<bool>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<bool>());
    return this->b_;
}

template <>
inline char DefaultParam_t::GetValue<char>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<char>());
    return this->c_;
}

template <>
inline unsigned char DefaultParam_t::GetValue<unsigned char>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<unsigned char>());
    return this->uc_;
}

template <>
inline short DefaultParam_t::GetValue<short>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<short>());
    return this->s_;
}

template <>
inline unsigned short DefaultParam_t::GetValue<unsigned short>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<unsigned short>());
    return this->us_;
}

template <>
inline int DefaultParam_t::GetValue<int>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<int>());
    return this->i_;
}

template <>
inline unsigned int DefaultParam_t::GetValue<unsigned int>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<unsigned int>());
    return this->ui_;
}

template <>
inline long DefaultParam_t::GetValue<long>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<long>());
    return this->l_;
}

template <>
inline unsigned long DefaultParam_t::GetValue<unsigned long>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<unsigned long>());
    return this->ul_;
}

template <>
inline long long DefaultParam_t::GetValue<long long>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<long long>());
    return this->ll_;
}

template <>
inline unsigned long long DefaultParam_t::GetValue<unsigned long long>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<unsigned long long>());
    return this->ull_;
}

template <>
inline float DefaultParam_t::GetValue<float>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<float>());
    return this->f_;
}

template <>
inline double DefaultParam_t::GetValue<double>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<double>());
    return this->d_;
}

template <>
inline behaviac::string DefaultParam_t::GetValue<behaviac::string>() const
{
    //BEHAVIAC_ASSERT(this->type_ == GetClassTypeNumberId<behaviac::string>());
    return this->str_;
}

/////////////////////////////////////////////////////////
///////////////     METHOD INTERFACE     ////////////////
/////////////////////////////////////////////////////////
class BEHAVIAC_API CMethodBase
{
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CMethodBase);

public:
    const static short KMETHOD_NAMEDEVENT = 0x0001;

    CMethodBase(const char* propertyName, const char* classFullName) : m_classFullName(classFullName), m_propertyName(propertyName),
        m_propertyID(propertyName), m_checkReturnTask(0), m_return(0), m_bStatic(false), m_flag(0), m_netRole(behaviac::NET_ROLE_DEFAULT)
    {
    }
    CMethodBase(const CMethodBase& copy) : m_classFullName(copy.m_classFullName), m_instanceName(copy.m_instanceName),
        m_propertyName(copy.m_propertyName), m_propertyID(copy.m_propertyID), m_displayName(copy.m_displayName), m_desc(copy.m_desc),
        m_bStatic(copy.m_bStatic), m_flag(copy.m_flag), m_netRole(copy.m_netRole)
    {
        if (copy.m_checkReturnTask)
        {
            this->m_checkReturnTask = copy.m_checkReturnTask->clone();
        }
        else
        {
            this->m_checkReturnTask = 0;
        }

        if (copy.m_return)
        {
            this->m_return = copy.m_return->clone();
        }
        else
        {
            this->m_return = 0;
        }
    }

    virtual ~CMethodBase()
    {
        BEHAVIAC_DELETE(this->m_checkReturnTask);
        BEHAVIAC_DELETE(this->m_return);
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode) = 0;
    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode) = 0;
    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode) = 0;

    inline const behaviac::CSerializationID& GetID() const
    {
        return m_propertyID;
    }

    void SetFlag(short flag)
    {
        this->m_flag = flag;

        if (this->m_flag)
        {
            //getter, return type can't be void
            BEHAVIAC_ASSERT(this->m_return && !this->m_return->IsVoid(), "getter should have a valid return.");
        }
    }

    CMethodBase& DISPLAYNAME(const wchar_t* displayName)
    {
        BEHAVIAC_UNUSED_VAR(displayName);

        if (displayName)
        {
            m_displayName = displayName;
        }

        return *this;
    }

    CMethodBase& DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (paramDesc)
        {
            m_desc = paramDesc;
        }

        return *this;
    }

    /// deprecated, to use PARAM_DISPLAY_INFO
    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_ASSERT(0, "no params!");
        return *this;
    }

    /// deprecated, to use PARAM_DISPLAY_INFO
    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_ASSERT(0, "no params!");
        return *this;
    }

    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        BEHAVIAC_ASSERT(0, "no params!");
        return *this;
    }
#if BEHAVIAC_ENABLE_NETWORKD
    CMethodBase& NETROLE(behaviac::NetworkRole netRole);
    void SubsribeToNetwork(behaviac::Agent* pAgent);
    void UnSubsribeToNetwork(behaviac::Agent* pAgent);
#endif//#if BEHAVIAC_ENABLE_NETWORKD

    template <typename R, typename ObjectType, typename P>
    CMethodBase& AddResultHanlder(R(ObjectType::*methodPtr)(P))
    {
        BEHAVIAC_ASSERT(methodPtr);

        {
            CCheckReturn* pMethod = BEHAVIAC_NEW CCheckReturn();
            pMethod->SetHandler(methodPtr);
            this->m_checkReturnTask = pMethod;
        }

        typedef behaviac::AsyncValue<R> ReturnType;
        this->m_return = BEHAVIAC_NEW ReturnType();
        return *this;
    }

    template <typename R, typename ObjectType>
    CMethodBase& AddResultHanlder(R(ObjectType::*methodPtr)())
    {
        BEHAVIAC_ASSERT(methodPtr);
        {
            CCheckReturn* pMethod = BEHAVIAC_NEW CCheckReturn();
            pMethod->SetHandler(methodPtr);
            this->m_checkReturnTask = pMethod;
        }

        typedef behaviac::AsyncValue<R> ReturnType;
        this->m_return = BEHAVIAC_NEW ReturnType();
        return *this;
    }

    template <typename R, typename P>
    CMethodBase& AddResultHanlder(R(*methodPtr)(P))
    {
        BEHAVIAC_ASSERT(methodPtr);

        {
            CCheckReturn* pMethod = BEHAVIAC_NEW CCheckReturn();
            pMethod->SetHandler(methodPtr);
            this->m_checkReturnTask = pMethod;
        }

        typedef behaviac::AsyncValue<R> ReturnType;
        this->m_return = BEHAVIAC_NEW ReturnType();
        return *this;
    }

    template <typename R>
    CMethodBase& AddResultHanlder(R(*methodPtr)())
    {
        BEHAVIAC_ASSERT(methodPtr);
        {
            CCheckReturn* pMethod = BEHAVIAC_NEW CCheckReturn();
            pMethod->SetHandler(methodPtr);
            this->m_checkReturnTask = pMethod;
        }

        typedef behaviac::AsyncValue<R> ReturnType;
        this->m_return = BEHAVIAC_NEW ReturnType();
        return *this;
    }

    template <typename R>
    CMethodBase& AddResultHanlder()
    {
        typedef behaviac::AsyncValue<R> ReturnType;
        this->m_return = BEHAVIAC_NEW ReturnType();
        return *this;
    }

    CMethodBase& AddResultHanlder()
    {
        //typedef behaviac::AsyncValue<R> ReturnType;
        //this->m_return = BEHAVIAC_NEW ReturnType();
        return *this;
    }

	double GetReturnDoubleValue(const CTagObject* parent)
	{
		BEHAVIAC_UNUSED_VAR(parent);
		BEHAVIAC_ASSERT(this->m_return);

		double result = this->m_return->GetDouble();

		return result;
	}

    /**
    be careful to use this function only when you know the return type
    */
    template <typename R>
    R GetReturnValue(const CTagObject* parent)
    {
        BEHAVIAC_UNUSED_VAR(parent);
		BEHAVIAC_ASSERT(this->m_return);

		R returnValue = ((behaviac::AsyncValue<R>*)this->m_return)->get();
		return returnValue;
    }

    /**
    be careful to use this function only when you know the return type
    */
    template <typename R>
    R GetReturnValue(const CTagObject* parent, int index)
    {
        BEHAVIAC_UNUSED_VAR(parent);
		BEHAVIAC_ASSERT(this->m_return);
		behaviac::vector<R>& returnVec = ((behaviac::AsyncValue<behaviac::vector<R> >*)this->m_return)->get();
		R returnValue = returnVec[index];
		return returnValue;
    }

    const char* GetReturnString(const CTagObject* parent) const
    {
        BEHAVIAC_UNUSED_VAR(parent);

        if (this->m_return)
        {
            const char* str = this->m_return->GetString();
            return str;
        }

        return 0;
    }

    bool GetReturnValue(const CTagObject* parent, behaviac::Property* ret, CTagObject* parentReturn)
    {
        if (this->m_return && ret)
        {
            ret->SetFrom((behaviac::Agent*)parent, this->m_return, (behaviac::Agent*)parentReturn);
            return true;
        }

        return false;
    }

    bool ComputeReturnValue(const CTagObject* parent, behaviac::Property* ret, CTagObject* parentReturn, behaviac::EComputeOperator opr)
    {
        if (this->m_return && ret)
        {
            ret->ComputeFrom((behaviac::Agent*)parent, this->m_return, (behaviac::Agent*)parentReturn, opr);
            return true;
        }

        return false;
    }

    int CheckReturn(const CTagObject* parent, const CTagObject* parentCheckResult, CMethodBase* checkResultMethod)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        if (checkResultMethod)
        {
            BEHAVIAC_ASSERT(this->m_return);
            int result = checkResultMethod->vRun(parentCheckResult, *this->m_return);

            return result;
        }

        return 0;
    }

    template <typename RTYPE>
    void CheckReturn(const CTagObject* parent, RTYPE& result)
    {
        if (this->m_checkReturnTask)
        {
            BEHAVIAC_ASSERT(this->m_return);
            this->m_checkReturnTask->run(result, parent, *this->m_return);

        }
        else if (this->m_return)
        {
            result = ((behaviac::AsyncValue<RTYPE>*)this->m_return)->get();

        }
        else
        {
            BEHAVIAC_ASSERT(false);
        }
    }

    void CheckReturn(const CTagObject* parent, bool& result)
    {
        if (this->m_checkReturnTask)
        {
            BEHAVIAC_ASSERT(this->m_return);
            this->m_checkReturnTask->run(result, parent, *this->m_return);

        }
        else if (this->m_return)
        {
            result = ((behaviac::AsyncValue<bool>*)this->m_return)->get();

        }
        else
        {
            BEHAVIAC_ASSERT(false);
        }
    }

    virtual int GetParams()
    {
        return -1;
    }

    // Invoking a method by calling an explicit function for each possible signature.
    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        BEHAVIAC_UNUSED_VAR(idx);
        BEHAVIAC_ASSERT(!"Not Implemented");
        return "";
    }
    virtual int vRun(const CTagObject* parent, behaviac::IAsyncValue& returnResult)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(returnResult);
        BEHAVIAC_ASSERT(!"Not Implemented");

        return 0;
    };
    virtual void vCall(const CTagObject* parent)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_ASSERT(!"Not Implemented");
    };
    virtual void vCall(const CTagObject* parent, const void* param1)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(param1);
        BEHAVIAC_ASSERT(!"Not Implemented");
    };
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(param1);
        BEHAVIAC_UNUSED_VAR(param2);

        BEHAVIAC_ASSERT(!"Not Implemented");
    };
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(param1);
        BEHAVIAC_UNUSED_VAR(param2);
        BEHAVIAC_UNUSED_VAR(param3);

        BEHAVIAC_ASSERT(!"Not Implemented");
    };
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(param1);
        BEHAVIAC_UNUSED_VAR(param2);
        BEHAVIAC_UNUSED_VAR(param3);
        BEHAVIAC_UNUSED_VAR(param4);

        BEHAVIAC_ASSERT(!"Not Implemented");
    };
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4, const void* param5)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(param1);
        BEHAVIAC_UNUSED_VAR(param2);
        BEHAVIAC_UNUSED_VAR(param3);
        BEHAVIAC_UNUSED_VAR(param4);
        BEHAVIAC_UNUSED_VAR(param5);

        BEHAVIAC_ASSERT(!"Not Implemented");
    };
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4, const void* param5, const void* param6)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(param1);
        BEHAVIAC_UNUSED_VAR(param2);
        BEHAVIAC_UNUSED_VAR(param3);
        BEHAVIAC_UNUSED_VAR(param4);
        BEHAVIAC_UNUSED_VAR(param5);
        BEHAVIAC_UNUSED_VAR(param6);

        BEHAVIAC_ASSERT(!"Not Implemented");
    };

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
        BEHAVIAC_UNUSED_VAR(defaultValue);
        BEHAVIAC_UNUSED_VAR(bConst);

        return 0;
    }

    virtual CMethodBase* clone() const = 0;
    behaviac::Agent* GetParentAgent(const behaviac::Agent* pAgent);

    void Invoke(const CTagObject* pAgent)
    {
		const behaviac::Agent* pParent = 0;
		if (pAgent) {
			pParent = this->GetParentAgent((behaviac::Agent*)pAgent);
		}

        this->run((CTagObject*)pParent, pAgent);
    }

    void Invoke(const CTagObject* pParent, const CTagObject* pAgent)
    {
        this->run(pParent, pAgent);
    }

    //public void Invoke(Agent pSelf, void* param)
    //{
    //	BEHAVIAC_ASSERT(0);
    //}

    //public void Invoke(Agent pSelf, object[] paramsValue)
    //{
    //	BEHAVIAC_ASSERT(0);
    //}

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params) = 0;
#endif//#if BEHAVIAC_ENABLE_NETWORKD

    const char* GetName() const
    {
        return this->m_propertyName.c_str();
    }

    const behaviac::wstring& GetDisplayName() const
    {
        return this->m_displayName;
    }

    const behaviac::wstring& GetDesc() const
    {
        return this->m_desc;
    }

    const char* GetClassNameString() const
    {
        return m_classFullName.c_str();
    }

    void GetReturnTypeName(behaviac::string& typeName)
    {
        if (this->m_return)
        {
            this->m_return->GetTypeName(typeName);
        }
        else
        {
            typeName = "void";
        }
    }

    const char* GetInstanceNameString() const
    {
        return m_instanceName.c_str();
    }

    void SetInstanceNameString(const char* name, ParentType pt)
    {
        BEHAVIAC_UNUSED_VAR(pt);
        this->m_instanceName = name;
    }

    virtual bool IsNamedEvent() const
    {
        return false;
    }

    bool IsStatic() const
    {
        return this->m_bStatic;
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder) = 0;

protected:

    //return true then it can only used for 'action'
    bool IsActionMethodOnly() const
    {
        //bool bActionMethodOnly = (this->m_checkReturnTask != 0);

        //return bActionMethodOnly;

        //this feature is disabled, so always returns false
        return false;
    }

protected:
    behaviac::string				m_classFullName;
    behaviac::string				m_instanceName;
    behaviac::string				m_propertyName;
    behaviac::CSerializationID		m_propertyID;

    behaviac::wstring				m_displayName;
    behaviac::wstring				m_desc;

    CCheckReturn*					m_checkReturnTask;
    behaviac::IAsyncValue*			m_return;

    //ParentType					m_parentType;

    bool							m_bStatic;

    //event
    short							m_flag;
    behaviac::NetworkRole			m_netRole;
};

template<typename T, bool bComplexType>
struct UserTypeInfo
{
    static void Export(CTagTypeDescriptor::TypesMap_t* types, bool returnType, behaviac::XmlNodeRef memberNode)
    {
        BEHAVIAC_UNUSED_VAR(returnType);
        BEHAVIAC_UNUSED_VAR(memberNode);
        BEHAVIAC_UNUSED_VAR(types);
    }
};

template<typename T>
void TExportType_(CTagTypeDescriptor::TypesMap_t* types, behaviac::XmlNodeRef memberNode)
{
    if (types != NULL)
    {
        const char* typeName = ::GetClassTypeName((T*)0);

        if (types->find(typeName) == types->end())
        {
            RegisterPropertiesGetter<T, behaviac::Meta::HasFromString<T>::Result>::_RegisterProperties();
            const CTagObjectDescriptor& objDesc = ObjectDescriptorGettter<T, behaviac::Meta::HasFromString<T>::Result>::_GetObjectDescriptor();
            objDesc.GetMembersDescription(types, 0, memberNode);

            (*types)[typeName] = &objDesc;
        }
    }
}

template<bool bComplexType>
struct UserTypeInfo<IList, bComplexType>
{
    static void Export(CTagTypeDescriptor::TypesMap_t* types, bool returnType, behaviac::XmlNodeRef memberNode)
    {
        BEHAVIAC_UNUSED_VAR(returnType);
        TExportType_<IList>(types, memberNode);
    }
};

template<bool bComplexType>
struct UserTypeInfo<System::Object, bComplexType>
{
    static void Export(CTagTypeDescriptor::TypesMap_t* types, bool returnType, behaviac::XmlNodeRef memberNode)
    {
        BEHAVIAC_UNUSED_VAR(returnType);
        TExportType_<System::Object>(types, memberNode);
    }
};

template<typename T, bool bEnum>
struct UserTypeInfoStruct
{
    static void Export(CTagTypeDescriptor::TypesMap_t* types, behaviac::XmlNodeRef memberNode)
    {
        TExportType_<T>(types, memberNode);
    }
};

template<typename T>
struct UserTypeInfoStruct<T, true>
{
    static void Export(CTagTypeDescriptor::TypesMap_t* types, behaviac::XmlNodeRef memberNode)
    {
        EnumClassUiInfoGetter<T, true>::GetUiInfo(types, memberNode);
    }
};

template<typename T>
struct UserTypeInfo<T, true>
{
    static void Export(CTagTypeDescriptor::TypesMap_t* types, bool returnType, behaviac::XmlNodeRef memberNode)
    {
        BEHAVIAC_UNUSED_VAR(returnType);
        //if (returnType)
        //{
        //	behaviac::XmlNodeRef memberNode1 = memberNode->newChild("ReturnType");
        //	const char* typeName = GetClassTypeName((T*)0);
        //	memberNode1->setAttr("Type", typeName);
        //	UserTypeInfoStruct<T, behaviac::Meta::IsEnum<T>::Result>::Export(types, memberNode1);
        //}
        //else
        {
            UserTypeInfoStruct<T, behaviac::Meta::IsEnum<T>::Result>::Export(types, memberNode);
        }
    }
};

template<typename T>
struct UserTypeInfoExport
{
    static void Export(CTagTypeDescriptor::TypesMap_t* types, bool returnType, behaviac::XmlNodeRef memberNode)
    {
		typedef REAL_BASETYPE(T)		TBaseType;

		UserTypeInfo < TBaseType, behaviac::Meta::IsEnum<TBaseType>::Result || behaviac::Meta::HasToString<TBaseType>::Result >::Export(types, returnType, memberNode);
    }
};
/////////////////////////////////////////////////////////
////////////////         METHOD          ////////////////
/////////////////////////////////////////////////////////

#define BEGIN_REGISTER_METHOD(propertyName, methodName)                                         \
    {                                                                                           \
        CMethodBase* _property = &CMethodFactory::Create(&objectType::methodName, objectType::GetClassTypeName(), propertyName

#define END_REGISTER_METHOD()                                                                   \
    );                                                                                          \
    ms_methods.push_back(_property);                                                           \
    }

#define ADD_PARAM(paramName)    ,paramName

BEHAVIAC_FORCEINLINE CMethodBase& _addMethod(behaviac::vector<class CMethodBase*>& methods, CMethodBase* _method)
{
    methods.push_back(_method);
    return *_method;
}

class MethodsContainer;

/////////////////////////////////////////////////////////
////////////////     CGenericMethod      ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ObjectType>
class CGenericMethod_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethod_);

    CGenericMethod_(R(ObjectType::*methodPtr)(void),
                    const char* className, const char* propertyName) : CMethodBase(propertyName, className), m_methodPtr(methodPtr)
    {
    }

    typedef R(ObjectType::*MethodType_t)();
    CGenericMethod_(R(ObjectType::*methodPtr)(void) const,
                    const char* className, const char* propertyName) : CMethodBase(propertyName, className), m_methodPtr(MethodType_t(methodPtr))
    {
    }

    CGenericMethod_(const CGenericMethod_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(xmlNode);
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(xmlNode);
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(xmlNode);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName.c_str());
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        typedef PARAM_BASETYPE(R)	ReturnBaseType;
        UserTypeInfoExport<ReturnBaseType>::Export(types, true, memberNode);
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent)
    {
        (((ObjectType*)parent)->*this->m_methodPtr)();
    }

    virtual int GetParams()
    {
        return 0;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        BEHAVIAC_UNUSED_VAR(idx);

        return behaviac::string();
    }

protected:
    R(ObjectType::*m_methodPtr)(void);
};

template<typename R, class ObjectType>
class CGenericMethod_R : public CGenericMethod_<R, ObjectType>
{
public:
    CGenericMethod_R(R(ObjectType::*methodPtr)(void),
                     const char* className, const char* propertyName) : CGenericMethod_<R, ObjectType>(methodPtr, className, propertyName)
    {}
    CGenericMethod_R(R(ObjectType::*methodPtr)(void) const,
                     const char* className, const char* propertyName) : CGenericMethod_<R, ObjectType>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod_R()
    {
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;

		behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod_R(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (CGenericMethod_<R, ObjectType>::m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                bHandled = pNw->SendRemoteEvent(this->m_netRole, nameTemp.c_str());
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)();

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_UNUSED_VAR(params);

        R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)();

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//#if BEHAVIAC_ENABLE_NETWORKD
    virtual int vRun(const CTagObject* parent, behaviac::IAsyncValue& returnResult)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(returnResult);
        BEHAVIAC_ASSERT(0);
        return 0;
    }
};

template<typename R, class ObjectType>
class CGenericMethod : public CGenericMethod_R<R, ObjectType>
{
public:
    CGenericMethod(R(ObjectType::*methodPtr)(void),
                   const char* className, const char* propertyName) : CGenericMethod_R<R, ObjectType>(methodPtr, className, propertyName)
    {}
    CGenericMethod(R(ObjectType::*methodPtr)(void) const,
                   const char* className, const char* propertyName) : CGenericMethod_R<R, ObjectType>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod(*this);
    }
};

template<class ObjectType>
class CGenericMethod<void, ObjectType> : public CGenericMethod_<void, ObjectType>
{
public:
    CGenericMethod(void (ObjectType::*methodPtr)(void),
                   const char* className, const char* propertyName) : CGenericMethod_<void, ObjectType>(methodPtr, className, propertyName)
    {}

    CGenericMethod(void (ObjectType::*methodPtr)(void) const,
                   const char* className, const char* propertyName) : CGenericMethod_<void, ObjectType>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                bHandled = pNw->SendRemoteEvent(this->m_netRole, nameTemp.c_str());
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            (((ObjectType*)parent)->*this->m_methodPtr)();
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
        BEHAVIAC_UNUSED_VAR(params);

        (((ObjectType*)parent)->*this->m_methodPtr)();
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethod1_     ////////////////
/////////////////////////////////////////////////////////
template<typename StoredType, typename BaseType, bool bAgent>
struct DefaulParamSetValueSelector
{
    static void SetValue(ParamVariable<StoredType>& p, const DefaultParam_t& defaultParam)
    {
        const StoredType& value = defaultParam.GetValue<BaseType>();
        p.SetValue(value);
    }
};

template<typename StoredType, typename BaseType>
struct DefaulParamSetValueSelector < StoredType, BaseType, true >
{
    static void SetValue(ParamVariable<StoredType>& p, const DefaultParam_t& defaultParam)
    {
    }
};

template<typename ParamType>
struct StoredTypeSelector
{
    typedef PARAM_BASETYPE(ParamType)								ParamBaseType;
    typedef PARAM_POINTERTYPE(ParamType)							ParamPointerType;
	typedef typename behaviac::Meta::IfThenElse<behaviac::Meta::IsRefType<ParamType>::Result, VALUE_TYPE(ParamType), ParamBaseType>::Result StoredType;
};

template<>
struct StoredTypeSelector<char*>
{
    typedef char*	ParamBaseType;
    typedef char**	ParamPointerType;
    typedef char*	StoredType;
};

template<>
struct StoredTypeSelector<char*&>
{
    typedef char*	ParamBaseType;
    typedef char**	ParamPointerType;
    typedef char*	StoredType;
};

template<>
struct StoredTypeSelector<const char*>
{
    typedef const char*		ParamBaseType;
    typedef const char**	ParamPointerType;
    typedef const char*		StoredType;
};

template<>
struct StoredTypeSelector<const char*&>
{
    typedef const char*		ParamBaseType;
    typedef const char**	ParamPointerType;
    typedef const char*		StoredType;
};

template<typename R, class ObjectType, class ParamType>
class CGenericMethod1_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethod1_);

    typedef typename StoredTypeSelector<ParamType>::ParamBaseType		ParamBaseType;
    typedef typename StoredTypeSelector<ParamType>::ParamPointerType	ParamPointerType;
    typedef typename StoredTypeSelector<ParamType>::StoredType			StoredType;

    CGenericMethod1_(R(ObjectType::*methodPtr)(ParamType), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr), m_paramRangeValid(false), m_param()
    {
		this->m_min = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    typedef R(ObjectType::*MethodType_t)(ParamType);

    CGenericMethod1_(R(ObjectType::*methodPtr)(ParamType) const, const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(MethodType_t(methodPtr)), m_paramRangeValid(false), m_param()
    {
		this->m_min = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethod1_(const CGenericMethod1_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName = copy.m_paramDisplayName;

        this->m_param = copy.m_param;
        this->m_paramRangeValid = copy.m_paramRangeValid;

		this->m_min = copy.m_min;
		this->m_max = copy.m_max;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param.LoadFromXML(parent, xmlNode, "param1");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param.SaveToXML(parent, xmlNode, "param1");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName.c_str());
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* returnTypeStr = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", returnTypeStr);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName);
            paramNode->setAttr("Desc", m_paramDesc);

            behaviac::string typeStr = ::GetTypeDescString<ParamType>();
            paramNode->setAttr("Type", typeStr.c_str());

            if (this->m_param.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param.GetValue());
            }

            if (m_paramRangeValid)
            {
                paramNode->setAttr("RangeMin", m_min);
                paramNode->setAttr("RangeMax", m_max);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType, behaviac::Meta::IsEnum<ParamBaseType>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        m_paramDisplayName = paramDisplayName;

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        m_paramDesc = paramDesc;
        return *this;
    }

    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        m_paramDisplayName = paramDisplayName;
        m_paramDesc = (paramDesc == 0 ? m_paramDisplayName : paramDesc);

        if (defaultParam.IsValid())
        {
			DefaulParamSetValueSelector<StoredType, ParamBaseType, behaviac::Meta::IsRefType<ParamType>::Result>::SetValue(this->m_param, defaultParam);
        }

        int typeId = GetClassTypeNumberId<ParamBaseType>();

        if (IsNumberClassTypeNumberId(typeId))
        {
            BEHAVIAC_ASSERT(rangeMin <= rangeMax);
            this->m_paramRangeValid = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
            this->m_min = rangeMin;
            this->m_max = rangeMax;
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 1;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr = ::GetTypeDescString<ParamType>();
                return typeStr;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1)
    {
        (((ObjectType*)parent)->*this->m_methodPtr)(*(ParamPointerType)param1);
    }

protected:
    R(ObjectType::*m_methodPtr)(ParamType);

    behaviac::wstring					m_paramDisplayName;
    behaviac::wstring					m_paramDesc;

    float								m_min, m_max;
    bool								m_paramRangeValid;
    ParamVariable<StoredType>			m_param;
};

template<typename R, class ObjectType, class ParamType>
class CGenericMethod1_R : public CGenericMethod1_<R, ObjectType, ParamType>
{
public:
    typedef typename CGenericMethod1_<R, ObjectType, ParamType>::ParamBaseType ParamBaseType;
    typedef typename CGenericMethod1_<R, ObjectType, ParamType>::StoredType StoredType;

    CGenericMethod1_R(R(ObjectType::*methodPtr)(ParamType),
                      const char* className, const char* propertyName) : CGenericMethod1_<R, ObjectType, ParamType>(methodPtr, className, propertyName)
    {
		this->m_min = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max = CMETHODBASE_RANGE_MAX_DEFAULT;
	}

    CGenericMethod1_R(R(ObjectType::*methodPtr)(ParamType) const,
                      const char* className, const char* propertyName) : CGenericMethod1_<R, ObjectType, ParamType>(methodPtr, className, propertyName)
    {
		this->m_min = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max = CMETHODBASE_RANGE_MAX_DEFAULT;
	}

    virtual ~CGenericMethod1_R()
    {
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod1_R(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                const ParamBaseType& v = this->m_param.GetValue(parent, parHolder);
                bHandled = pNw->SendRemoteEvent<ParamType>(CGenericMethod1_<R, ObjectType, ParamType>::m_netRole, nameTemp.c_str(),
                                                           (PARAM_CALLEDTYPE(ParamType))v);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType>::GetReturnType v = this->m_param.GetValue(parent, parHolder);

            R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType))v);

            CGenericMethod1_<R, ObjectType, ParamType>::m_param.SetVariableRegistry(parent, v);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 1);

        PARAM_BASETYPE(ParamType) p;
        params[0]->GetValue(p);

        R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType))p);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
    virtual int vRun(const CTagObject* parent, behaviac::IAsyncValue& returnResult)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(returnResult);
        BEHAVIAC_ASSERT(0);
        return 0;
    }
};

template<typename R, class ObjectType, class ParamType>
class CGenericMethod1 : public CGenericMethod1_R<R, ObjectType, ParamType>
{
public:
    CGenericMethod1(R(ObjectType::*methodPtr)(ParamType),
                    const char* className, const char* propertyName) : CGenericMethod1_R<R, ObjectType, ParamType>(methodPtr, className, propertyName)
    {
	}

    CGenericMethod1(R(ObjectType::*methodPtr)(ParamType) const,
                    const char* className, const char* propertyName) : CGenericMethod1_R<R, ObjectType, ParamType>(methodPtr, className, propertyName)
    {
	}

    virtual ~CGenericMethod1()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod1(*this);
    }
};

template<class ObjectType, class ParamType>
class CGenericMethod1<void, ObjectType, ParamType> : public CGenericMethod1_<void, ObjectType, ParamType>
{
public:
    typedef typename CGenericMethod1_<void, ObjectType, ParamType>::StoredType StoredType;
    typedef typename CGenericMethod1_<void, ObjectType, ParamType>::ParamBaseType ParamBaseType;

    CGenericMethod1(void (ObjectType::*methodPtr)(ParamType),
                    const char* className, const char* propertyName) :
        CGenericMethod1_<void, ObjectType, ParamType>(methodPtr, className, propertyName)
    {
	}

    CGenericMethod1(void (ObjectType::*methodPtr)(ParamType) const,
                    const char* className, const char* propertyName) :
        CGenericMethod1_<void, ObjectType, ParamType>(methodPtr, className, propertyName)
    {
	}

    virtual ~CGenericMethod1()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod1(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                const ParamBaseType& v = this->m_param.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType>(this->m_netRole, nameTemp.c_str(),
                                                           (PARAM_CALLEDTYPE(ParamType))v);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType>::GetReturnType v = this->m_param.GetValue(parent, parHolder);
            (((ObjectType*)parent)->*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType))v);

            this->m_param.SetVariableRegistry(parent, v);
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_ASSERT(params.size() == 1);

        PARAM_BASETYPE(ParamType) p;
        params[0]->GetValue(p);

        (((ObjectType*)parent)->*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType))p);

        this->m_param.SetVariableRegistry(parHolder, p);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethod2_     ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ObjectType, class ParamType1, class ParamType2>
class CGenericMethod2_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethod2_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType		ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType		ParamPointerType2;

    typedef typename StoredTypeSelector<ParamType1>::StoredType				StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType				StoredType2;

    CGenericMethod2_(R(ObjectType::*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr),
        m_paramRangeValid1(false), m_paramRangeValid2(false),
        m_param1(), m_param2()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    typedef R(ObjectType::*MethodType_t)(ParamType1, ParamType2);

    CGenericMethod2_(R(ObjectType::*methodPtr)(ParamType1, ParamType2) const, const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(MethodType_t(methodPtr)),
        m_paramRangeValid1(false), m_paramRangeValid2(false),
        m_param1(), m_param2()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethod2_(const CGenericMethod2_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName.c_str());
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DEFAULTVALUE(int paramIndex, const DefaultParam_t& defaultParam)
    {
        if (paramIndex == 0)
        {
			DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);

        }
        else if (paramIndex == 1)
        {
			DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);

        }
        else
        {
            BEHAVIAC_ASSERT(0, "not supported param");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 2;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2)
    {
        (((ObjectType*)parent)->*this->m_methodPtr)(*((ParamPointerType1)param1), *((ParamPointerType2)param2));
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod2_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2>(this->m_netRole, nameTemp.c_str(),
                                                                        (PARAM_CALLEDTYPE(ParamType1))v1,
                                                                        (PARAM_CALLEDTYPE(ParamType2))v2);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2);

            this->m_param1.SetVariableRegistry(parent, v1);
            this->m_param2.SetVariableRegistry(parent, v2);
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 2);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);

        (((ObjectType*)parent)->*this->m_methodPtr)((PARAM_CALLEDTYPE(ParamType1))p1, (PARAM_CALLEDTYPE(ParamType2))p2);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
protected:
    R(ObjectType::*m_methodPtr)(ParamType1, ParamType2);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;

    ParamVariable<StoredType1>		m_param1;
    ParamVariable<StoredType2>		m_param2;
};

template<typename R, class ObjectType, class ParamType1, class ParamType2>
class CGenericMethod2 : public CGenericMethod2_<R, ObjectType, ParamType1, ParamType2>
{
public:
    typedef typename CGenericMethod2_<R, ObjectType, ParamType1, ParamType2>::StoredType1 StoredType1;
    typedef typename CGenericMethod2_<R, ObjectType, ParamType1, ParamType2>::StoredType2 StoredType2;
    typedef typename CGenericMethod2_<R, ObjectType, ParamType1, ParamType2>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod2_<R, ObjectType, ParamType1, ParamType2>::ParamBaseType2 ParamBaseType2;

    CGenericMethod2(R(ObjectType::*methodPtr)(ParamType1, ParamType2),
                    const char* className, const char* propertyName
                   ) :
        CGenericMethod2_<R, ObjectType, ParamType1, ParamType2>(methodPtr, className, propertyName)
    {
	}

    CGenericMethod2(R(ObjectType::*methodPtr)(ParamType1, ParamType2) const,
                    const char* className, const char* propertyName
                   ) :
        CGenericMethod2_<R, ObjectType, ParamType1, ParamType2>(methodPtr, className, propertyName)
    {
	}

    virtual ~CGenericMethod2()
    {
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod2(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2>(this->m_netRole, nameTemp.c_str(),
                                                                        (PARAM_CALLEDTYPE(ParamType1))v1,
                                                                        (PARAM_CALLEDTYPE(ParamType2))v2);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

            R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2);

            this->m_param1.SetVariableRegistry(parent, v1);
            this->m_param2.SetVariableRegistry(parent, v2);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 2);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);

        R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ObjectType, class ParamType1, class ParamType2>
class CGenericMethod2<void, ObjectType, ParamType1, ParamType2> : public CGenericMethod2_<void, ObjectType, ParamType1, ParamType2>
{
public:
    typedef typename CGenericMethod2_<void, ObjectType, ParamType1, ParamType2>::StoredType1 StoredType1;
    typedef typename CGenericMethod2_<void, ObjectType, ParamType1, ParamType2>::StoredType2 StoredType2;
    typedef typename CGenericMethod2_<void, ObjectType, ParamType1, ParamType2>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod2_<void, ObjectType, ParamType1, ParamType2>::ParamBaseType2 ParamBaseType2;

    CGenericMethod2(void (ObjectType::*methodPtr)(ParamType1, ParamType2),
                    const char* className, const char* propertyName
                   ) :
        CGenericMethod2_<void, ObjectType, ParamType1, ParamType2>(methodPtr, className, propertyName)
    {
	}

    CGenericMethod2(void (ObjectType::*methodPtr)(ParamType1, ParamType2) const,
                    const char* className, const char* propertyName
                   ) :
        CGenericMethod2_<void, ObjectType, ParamType1, ParamType2>(methodPtr, className, propertyName)
    {
	}

    virtual ~CGenericMethod2()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod2(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2>(this->m_netRole, nameTemp.c_str(),
                                                                        (PARAM_CALLEDTYPE(ParamType1))v1,
                                                                        (PARAM_CALLEDTYPE(ParamType2))v2);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2);

            this->m_param1.SetVariableRegistry(parent, v1);
            this->m_param2.SetVariableRegistry(parent, v2);
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 2);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethod3_     ////////////////
/////////////////////////////////////////////////////////

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
class CGenericMethod3_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethod3_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType		ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType		ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType		ParamPointerType3;

    typedef typename StoredTypeSelector<ParamType1>::StoredType				StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType				StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType				StoredType3;

    CGenericMethod3_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false),
        m_param1(), m_param2(), m_param3()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    typedef R(ObjectType::*MethodType_t)(ParamType1, ParamType2, ParamType3);
    CGenericMethod3_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3) const, const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false),
        m_param1(), m_param2(), m_param3()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
	}

    CGenericMethod3_(const CGenericMethod3_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName.c_str());
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 3;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3)
    {
        (((ObjectType*)parent)->*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod3_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3>(this->m_netRole, nameTemp.c_str(),
                                                                                    (PARAM_CALLEDTYPE(ParamType1))v1,
                                                                                    (PARAM_CALLEDTYPE(ParamType2))v2,
                                                                                    (PARAM_CALLEDTYPE(ParamType3))v3);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 3);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(ObjectType::*m_methodPtr)(ParamType1, ParamType2, ParamType3);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;

    ParamVariable<StoredType1>		m_param1;
    ParamVariable<StoredType2>		m_param2;
    ParamVariable<StoredType3>		m_param3;
};

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
class CGenericMethod3 : public CGenericMethod3_<R, ObjectType, ParamType1, ParamType2, ParamType3>
{
public:
    typedef typename CGenericMethod3_<R, ObjectType, ParamType1, ParamType2, ParamType3>::StoredType1 StoredType1;
    typedef typename CGenericMethod3_<R, ObjectType, ParamType1, ParamType2, ParamType3>::StoredType2 StoredType2;
    typedef typename CGenericMethod3_<R, ObjectType, ParamType1, ParamType2, ParamType3>::StoredType3 StoredType3;
    typedef typename CGenericMethod3_<R, ObjectType, ParamType1, ParamType2, ParamType3>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod3_<R, ObjectType, ParamType1, ParamType2, ParamType3>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod3_<R, ObjectType, ParamType1, ParamType2, ParamType3>::ParamBaseType3 ParamBaseType3;

    CGenericMethod3(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3),
                    const char* className, const char* propertyName
                   ) :
        CGenericMethod3_<R, ObjectType, ParamType1, ParamType2, ParamType3>(methodPtr, className, propertyName)
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
	}

    CGenericMethod3(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3) const,
                    const char* className, const char* propertyName
                   ) :
        CGenericMethod3_<R, ObjectType, ParamType1, ParamType2, ParamType3>(methodPtr, className, propertyName)
    {
        this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
        this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
        this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
        this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
        this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
        this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
	}

    virtual ~CGenericMethod3()
    {
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod3(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3>(this->m_netRole, nameTemp.c_str(),
                                                                                    (PARAM_CALLEDTYPE(ParamType1))v1,
                                                                                    (PARAM_CALLEDTYPE(ParamType2))v2,
                                                                                    (PARAM_CALLEDTYPE(ParamType3))v3);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

            R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 3);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);

        R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ObjectType, class ParamType1, class ParamType2, class ParamType3>
class CGenericMethod3<void, ObjectType, ParamType1, ParamType2, ParamType3> : public CGenericMethod3_<void, ObjectType, ParamType1, ParamType2, ParamType3>
{
public:
    typedef typename CGenericMethod3_<void, ObjectType, ParamType1, ParamType2, ParamType3>::StoredType1 StoredType1;
    typedef typename CGenericMethod3_<void, ObjectType, ParamType1, ParamType2, ParamType3>::StoredType2 StoredType2;
    typedef typename CGenericMethod3_<void, ObjectType, ParamType1, ParamType2, ParamType3>::StoredType3 StoredType3;
    typedef typename CGenericMethod3_<void, ObjectType, ParamType1, ParamType2, ParamType3>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod3_<void, ObjectType, ParamType1, ParamType2, ParamType3>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod3_<void, ObjectType, ParamType1, ParamType2, ParamType3>::ParamBaseType3 ParamBaseType3;

    CGenericMethod3(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3),
                    const char* className, const char* propertyName
                   ) :
        CGenericMethod3_<void, ObjectType, ParamType1, ParamType2, ParamType3>(methodPtr, className, propertyName)
    {}

    CGenericMethod3(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3) const,
                    const char* className, const char* propertyName
                   ) :
        CGenericMethod3_<void, ObjectType, ParamType1, ParamType2, ParamType3>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod3()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod3(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3>(this->m_netRole, nameTemp.c_str(),
                                                                                    (PARAM_CALLEDTYPE(ParamType1))v1,
                                                                                    (PARAM_CALLEDTYPE(ParamType2))v2,
                                                                                    (PARAM_CALLEDTYPE(ParamType3))v3);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 3);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethod4_     ///////////////
/////////////////////////////////////////////////////////

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
class CGenericMethod4_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethod4_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamBaseType			ParamBaseType4;

    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType			ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType			ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType			ParamPointerType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamPointerType			ParamPointerType4;

    typedef typename StoredTypeSelector<ParamType1>::StoredType		StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType		StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType		StoredType3;
    typedef typename StoredTypeSelector<ParamType4>::StoredType		StoredType4;

    CGenericMethod4_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4),
                     const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false),
        m_param1(), m_param2(), m_param3(), m_param4()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    typedef R(ObjectType::*MethodType_t)(ParamType1, ParamType2, ParamType3, ParamType4);

    CGenericMethod4_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4) const,
                     const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(MethodType_t(methodPtr)),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false),
        m_param1(), m_param2(), m_param3(), m_param4()
    {
        this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
        this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
        this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
        this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
        this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
        this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
        this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
        this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethod4_(const CGenericMethod4_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;
        this->m_paramDisplayName4 = copy.m_paramDisplayName4;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_param4 = copy.m_param4;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
        this->m_paramRangeValid4 = copy.m_paramRangeValid4;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
        this->m_param4.LoadFromXML(parent, xmlNode, "param4");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
        this->m_param4.SaveToXML(parent, xmlNode, "param4");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName.c_str());
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName4);
            paramNode->setAttr("Desc", m_paramDesc4);

            behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
            paramNode->setAttr("Type", typeStr4.c_str());

            if (this->m_param4.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param4.GetValue());
            }

            if (m_paramRangeValid4)
            {
                paramNode->setAttr("RangeMin", m_min4);
                paramNode->setAttr("RangeMax", m_max4);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType4, behaviac::Meta::IsEnum<ParamBaseType4>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else if (m_paramDesc4.empty())
        {
            m_paramDesc4 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;
            m_paramDesc4 = (paramDesc == 0 ? m_paramDisplayName4 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType4>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid4 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min4 = rangeMin;
                this->m_max4 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 4;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            case 3:
            {
                behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
                return typeStr4;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4)
    {
        (((ObjectType*)parent)->*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3, *(ParamPointerType4)param4);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod4_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4>(this->m_netRole, nameTemp.c_str(),
                                                                                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                                                                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                                                                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                                                                                (PARAM_CALLEDTYPE(ParamType4))v4);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 4);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(ObjectType::*m_methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;
    behaviac::wstring				m_paramDisplayName4;
    behaviac::wstring				m_paramDesc4;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;
    bool							m_paramRangeValid4;
    float							m_min4, m_max4;

    ParamVariable<StoredType1>		m_param1;
    ParamVariable<StoredType2>		m_param2;
    ParamVariable<StoredType3>		m_param3;
    ParamVariable<StoredType4>		m_param4;
};

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
class CGenericMethod4 : public CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>
{
public:
    typedef typename CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType1 StoredType1;
    typedef typename CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType2 StoredType2;
    typedef typename CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType3 StoredType3;
    typedef typename CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType4 StoredType4;
    typedef typename CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType4 ParamBaseType4;

    CGenericMethod4(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4),
                    const char* className, const char* propertyName) :
        CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>(methodPtr, className, propertyName)
    {}

    CGenericMethod4(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4) const,
                    const char* className, const char* propertyName) :
        CGenericMethod4_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod4()
    {
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod4(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4>(this->m_netRole, nameTemp.c_str(),
                                                                                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                                                                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                                                                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                                                                                (PARAM_CALLEDTYPE(ParamType4))v4);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

            R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                (PARAM_CALLEDTYPE(ParamType4))v4);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 4);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);

        R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3,
                            (PARAM_CALLEDTYPE(ParamType4))p4);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
class CGenericMethod4<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> : public CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>
{
public:
    typedef typename CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType1 StoredType1;
    typedef typename CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType2 StoredType2;
    typedef typename CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType3 StoredType3;
    typedef typename CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::StoredType4 StoredType4;
    typedef typename CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>::ParamBaseType4 ParamBaseType4;

    CGenericMethod4(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4),
                    const char* className, const char* propertyName) :
        CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>(methodPtr, className, propertyName)
    {}

    CGenericMethod4(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4) const,
                    const char* className, const char* propertyName) :
        CGenericMethod4_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod4()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod4(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4>(this->m_netRole, nameTemp.c_str(),
                                                                                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                                                                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                                                                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                                                                                (PARAM_CALLEDTYPE(ParamType4))v4);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
        }
    }
#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 4);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethod5_     ///////////////
/////////////////////////////////////////////////////////

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
class CGenericMethod5_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethod5_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamBaseType			ParamBaseType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamBaseType			ParamBaseType5;

    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType			ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType			ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType			ParamPointerType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamPointerType			ParamPointerType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamPointerType			ParamPointerType5;

    typedef typename StoredTypeSelector<ParamType1>::StoredType		StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType		StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType		StoredType3;
    typedef typename StoredTypeSelector<ParamType4>::StoredType		StoredType4;
    typedef typename StoredTypeSelector<ParamType5>::StoredType		StoredType5;

    CGenericMethod5_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5),
                     const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false),
        m_param1(), m_param2(), m_param3(), m_param4(), m_param5()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    typedef R(ObjectType::*MethodType_t)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5);

    CGenericMethod5_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5) const,
                     const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(MethodType_t(methodPtr)),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false),
        m_param1(), m_param2(), m_param3(), m_param4(), m_param5()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethod5_(const CGenericMethod5_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;
        this->m_paramDisplayName4 = copy.m_paramDisplayName4;
        this->m_paramDisplayName5 = copy.m_paramDisplayName5;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_param4 = copy.m_param4;
        this->m_param5 = copy.m_param5;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
        this->m_paramRangeValid4 = copy.m_paramRangeValid4;
        this->m_paramRangeValid5 = copy.m_paramRangeValid5;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
        this->m_param4.LoadFromXML(parent, xmlNode, "param4");
        this->m_param5.LoadFromXML(parent, xmlNode, "param5");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
        this->m_param4.SaveToXML(parent, xmlNode, "param4");
        this->m_param5.SaveToXML(parent, xmlNode, "param5");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName.c_str());
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName4);
            paramNode->setAttr("Desc", m_paramDesc4);

            behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
            paramNode->setAttr("Type", typeStr4.c_str());

            if (this->m_param4.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param4.GetValue());
            }

            if (m_paramRangeValid4)
            {
                paramNode->setAttr("RangeMin", m_min4);
                paramNode->setAttr("RangeMax", m_max4);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType4, behaviac::Meta::IsEnum<ParamBaseType4>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName5);
            paramNode->setAttr("Desc", m_paramDesc5);

            behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
            paramNode->setAttr("Type", typeStr5.c_str());

            if (this->m_param5.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param5.GetValue());
            }

            if (m_paramRangeValid5)
            {
                paramNode->setAttr("RangeMin", m_min5);
                paramNode->setAttr("RangeMax", m_max5);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType5, behaviac::Meta::IsEnum<ParamBaseType5>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;

        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else if (m_paramDesc4.empty())
        {
            m_paramDesc4 = paramDesc;

        }
        else if (m_paramDesc5.empty())
        {
            m_paramDesc5 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;
            m_paramDesc4 = (paramDesc == 0 ? m_paramDisplayName4 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType4>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid4 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min4 = rangeMin;
                this->m_max4 = rangeMax;
            }
        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;
            m_paramDesc5 = (paramDesc == 0 ? m_paramDisplayName5 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType5, ParamBaseType5, behaviac::Meta::IsRefType<ParamType5>::Result>::SetValue(this->m_param5, defaultParam);
            }

            int typeId = GetClassTypeNumberId<ParamBaseType5>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid5 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min5 = rangeMin;
                this->m_max5 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 5;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            case 3:
            {
                behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
                return typeStr4;
            }

            case 4:
            {
                behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
                return typeStr5;
            }

            default:
                return "";
        }
    }


    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4, const void* param5)
    {
        (((ObjectType*)parent)->*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3, *(ParamPointerType4)param4, *(ParamPointerType5)param5);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod5_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 5);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(ObjectType::*m_methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;
    behaviac::wstring				m_paramDisplayName4;
    behaviac::wstring				m_paramDesc4;
    behaviac::wstring				m_paramDisplayName5;
    behaviac::wstring				m_paramDesc5;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;
    bool							m_paramRangeValid4;
    float							m_min4, m_max4;
    bool							m_paramRangeValid5;
    float							m_min5, m_max5;

    ParamVariable<StoredType1>		m_param1;
    ParamVariable<StoredType2>		m_param2;
    ParamVariable<StoredType3>		m_param3;
    ParamVariable<StoredType4>		m_param4;
    ParamVariable<StoredType5>		m_param5;
};

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
class CGenericMethod5 : public CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>
{
public:
    typedef typename CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType1 StoredType1;
    typedef typename CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType2 StoredType2;
    typedef typename CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType3 StoredType3;
    typedef typename CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType4 StoredType4;
    typedef typename CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType5 StoredType5;
    typedef typename CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType5 ParamBaseType5;

    CGenericMethod5(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5),
                    const char* className, const char* propertyName) :
        CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(methodPtr, className, propertyName)
    {}

    CGenericMethod5(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5) const,
                    const char* className, const char* propertyName) :
        CGenericMethod5_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod5()
    {
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod5(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

            R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                (PARAM_CALLEDTYPE(ParamType4))v4,
                                (PARAM_CALLEDTYPE(ParamType5))v5);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 5);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);

        R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3,
                            (PARAM_CALLEDTYPE(ParamType4))p4,
                            (PARAM_CALLEDTYPE(ParamType5))p5);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
class CGenericMethod5<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> : public CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>
{
public:
    typedef typename CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType1 StoredType1;
    typedef typename CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType2 StoredType2;
    typedef typename CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType3 StoredType3;
    typedef typename CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType4 StoredType4;
    typedef typename CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::StoredType5 StoredType5;
    typedef typename CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>::ParamBaseType5 ParamBaseType5;

    CGenericMethod5(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5),
                    const char* className, const char* propertyName) :
        CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(methodPtr, className, propertyName)
    {}

    CGenericMethod5(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5) const,
                    const char* className, const char* propertyName) :
        CGenericMethod5_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod5()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod5(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 5);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethod6_     ///////////////
/////////////////////////////////////////////////////////

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
class CGenericMethod6_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethod6_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamBaseType			ParamBaseType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamBaseType			ParamBaseType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamBaseType			ParamBaseType6;

    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType			ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType			ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType			ParamPointerType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamPointerType			ParamPointerType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamPointerType			ParamPointerType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamPointerType			ParamPointerType6;

    typedef typename StoredTypeSelector<ParamType1>::StoredType		StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType		StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType		StoredType3;
    typedef typename StoredTypeSelector<ParamType4>::StoredType		StoredType4;
    typedef typename StoredTypeSelector<ParamType5>::StoredType		StoredType5;
    typedef typename StoredTypeSelector<ParamType6>::StoredType		StoredType6;

    CGenericMethod6_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6),
                     const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false), m_paramRangeValid6(false),
        m_param1(), m_param2(), m_param3(), m_param4(), m_param5(), m_param6()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min6 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max6 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    typedef R(ObjectType::*MethodType_t)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6);

    CGenericMethod6_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6) const,
                     const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(MethodType_t(methodPtr)),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false), m_paramRangeValid6(false),
        m_param1(), m_param2(), m_param3(), m_param4(), m_param5(), m_param6()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min6 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max6 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethod6_(const CGenericMethod6_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;
        this->m_paramDisplayName4 = copy.m_paramDisplayName4;
        this->m_paramDisplayName5 = copy.m_paramDisplayName5;
        this->m_paramDisplayName6 = copy.m_paramDisplayName6;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_param4 = copy.m_param4;
        this->m_param5 = copy.m_param5;
        this->m_param6 = copy.m_param6;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
        this->m_paramRangeValid4 = copy.m_paramRangeValid4;
        this->m_paramRangeValid5 = copy.m_paramRangeValid5;
        this->m_paramRangeValid6 = copy.m_paramRangeValid6;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
        this->m_param4.LoadFromXML(parent, xmlNode, "param4");
        this->m_param5.LoadFromXML(parent, xmlNode, "param5");
        this->m_param6.LoadFromXML(parent, xmlNode, "param6");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
        this->m_param4.SaveToXML(parent, xmlNode, "param4");
        this->m_param5.SaveToXML(parent, xmlNode, "param5");
        this->m_param6.SaveToXML(parent, xmlNode, "param6");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName.c_str());
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName4);
            paramNode->setAttr("Desc", m_paramDesc4);

            behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
            paramNode->setAttr("Type", typeStr4.c_str());

            if (this->m_param4.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param4.GetValue());
            }

            if (m_paramRangeValid4)
            {
                paramNode->setAttr("RangeMin", m_min4);
                paramNode->setAttr("RangeMax", m_max4);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType4, behaviac::Meta::IsEnum<ParamBaseType4>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName5);
            paramNode->setAttr("Desc", m_paramDesc5);

            behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
            paramNode->setAttr("Type", typeStr5.c_str());

            if (this->m_param5.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param5.GetValue());
            }

            if (m_paramRangeValid5)
            {
                paramNode->setAttr("RangeMin", m_min5);
                paramNode->setAttr("RangeMax", m_max5);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType5, behaviac::Meta::IsEnum<ParamBaseType5>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName6);
            paramNode->setAttr("Desc", m_paramDesc6);

            behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
            paramNode->setAttr("Type", typeStr6.c_str());

            if (this->m_param6.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param6.GetValue());
            }

            if (m_paramRangeValid6)
            {
                paramNode->setAttr("RangeMin", m_min6);
                paramNode->setAttr("RangeMax", m_max6);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType6, behaviac::Meta::IsEnum<ParamBaseType6>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;

        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;

        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else if (m_paramDesc4.empty())
        {
            m_paramDesc4 = paramDesc;

        }
        else if (m_paramDesc5.empty())
        {
            m_paramDesc5 = paramDesc;

        }
        else if (m_paramDesc6.empty())
        {
            m_paramDesc6 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;
            m_paramDesc4 = (paramDesc == 0 ? m_paramDisplayName4 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType4>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid4 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min4 = rangeMin;
                this->m_max4 = rangeMax;
            }
        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;
            m_paramDesc5 = (paramDesc == 0 ? m_paramDisplayName5 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType5, ParamBaseType5, behaviac::Meta::IsRefType<ParamType5>::Result>::SetValue(this->m_param5, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType5>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid5 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min5 = rangeMin;
                this->m_max5 = rangeMax;
            }
        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;
            m_paramDesc6 = (paramDesc == 0 ? m_paramDisplayName6 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType6, ParamBaseType6, behaviac::Meta::IsRefType<ParamType6>::Result>::SetValue(this->m_param6, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType6>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid6 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min6 = rangeMin;
                this->m_max6 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 6;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            case 3:
            {
                behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
                return typeStr4;
            }

            case 4:
            {
                behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
                return typeStr5;
            }

            case 5:
            {
                behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
                return typeStr6;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4, const void* param5, const void* param6)
    {
        (((ObjectType*)parent)->*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3, *(ParamPointerType4)param4, *(ParamPointerType5)param5, *(ParamPointerType6)param6);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod6_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 6);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(ObjectType::*m_methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;
    behaviac::wstring				m_paramDisplayName4;
    behaviac::wstring				m_paramDesc4;
    behaviac::wstring				m_paramDisplayName5;
    behaviac::wstring				m_paramDesc5;
    behaviac::wstring				m_paramDisplayName6;
    behaviac::wstring				m_paramDesc6;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;
    bool							m_paramRangeValid4;
    float							m_min4, m_max4;
    bool							m_paramRangeValid5;
    float							m_min5, m_max5;
    bool							m_paramRangeValid6;
    float							m_min6, m_max6;

    ParamVariable<StoredType1>		m_param1;
    ParamVariable<StoredType2>		m_param2;
    ParamVariable<StoredType3>		m_param3;
    ParamVariable<StoredType4>		m_param4;
    ParamVariable<StoredType5>		m_param5;
    ParamVariable<StoredType6>		m_param6;
};

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
class CGenericMethod6 : public CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>
{
public:
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType1 StoredType1;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType2 StoredType2;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType3 StoredType3;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType4 StoredType4;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType5 StoredType5;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType6 StoredType6;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType6 ParamBaseType6;

    CGenericMethod6(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6),
                    const char* className, const char* propertyName) :
        CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(methodPtr, className, propertyName)
    {}

    CGenericMethod6(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6) const,
                    const char* className, const char* propertyName) :
        CGenericMethod6_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod6()
    {
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod6(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

            R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                (PARAM_CALLEDTYPE(ParamType4))v4,
                                (PARAM_CALLEDTYPE(ParamType5))v5,
                                (PARAM_CALLEDTYPE(ParamType6))v6);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 6);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);

        R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3,
                            (PARAM_CALLEDTYPE(ParamType4))p4,
                            (PARAM_CALLEDTYPE(ParamType5))p5,
                            (PARAM_CALLEDTYPE(ParamType6))p6);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
class CGenericMethod6<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> : public CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>
{
public:
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType1 StoredType1;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType2 StoredType2;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType3 StoredType3;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType4 StoredType4;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType5 StoredType5;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::StoredType6 StoredType6;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>::ParamBaseType6 ParamBaseType6;

    CGenericMethod6(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6),
                    const char* className, const char* propertyName) :
        CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(methodPtr, className, propertyName)
    {}

    CGenericMethod6(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6) const,
                    const char* className, const char* propertyName) :
        CGenericMethod6_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod6()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod6(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 6);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethod7_     ///////////////
/////////////////////////////////////////////////////////

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
class CGenericMethod7_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethod7_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamBaseType			ParamBaseType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamBaseType			ParamBaseType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamBaseType			ParamBaseType6;
    typedef typename StoredTypeSelector<ParamType7>::ParamBaseType			ParamBaseType7;

    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType			ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType			ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType			ParamPointerType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamPointerType			ParamPointerType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamPointerType			ParamPointerType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamPointerType			ParamPointerType6;
    typedef typename StoredTypeSelector<ParamType7>::ParamPointerType			ParamPointerType7;

    typedef typename StoredTypeSelector<ParamType1>::StoredType		StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType		StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType		StoredType3;
    typedef typename StoredTypeSelector<ParamType4>::StoredType		StoredType4;
    typedef typename StoredTypeSelector<ParamType5>::StoredType		StoredType5;
    typedef typename StoredTypeSelector<ParamType6>::StoredType		StoredType6;
    typedef typename StoredTypeSelector<ParamType7>::StoredType		StoredType7;

    CGenericMethod7_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7),
                     const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false), m_paramRangeValid6(false), m_paramRangeValid7(false),
        m_param1(), m_param2(), m_param3(), m_param4(), m_param5(), m_param6(), m_param7()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min6 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max6 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min7 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max7 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    typedef R(ObjectType::*MethodType_t)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7);

    CGenericMethod7_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7) const,
                     const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(MethodType_t(methodPtr)),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false), m_paramRangeValid6(false), m_paramRangeValid7(false),
        m_param1(), m_param2(), m_param3(), m_param4(), m_param5(), m_param6(), m_param7()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min6 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max6 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min7 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max7 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethod7_(const CGenericMethod7_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;
        this->m_paramDisplayName4 = copy.m_paramDisplayName4;
        this->m_paramDisplayName5 = copy.m_paramDisplayName5;
        this->m_paramDisplayName6 = copy.m_paramDisplayName6;
        this->m_paramDisplayName7 = copy.m_paramDisplayName7;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_param4 = copy.m_param4;
        this->m_param5 = copy.m_param5;
        this->m_param6 = copy.m_param6;
        this->m_param7 = copy.m_param7;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
        this->m_paramRangeValid4 = copy.m_paramRangeValid4;
        this->m_paramRangeValid5 = copy.m_paramRangeValid5;
        this->m_paramRangeValid6 = copy.m_paramRangeValid6;
        this->m_paramRangeValid7 = copy.m_paramRangeValid7;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
        this->m_param4.LoadFromXML(parent, xmlNode, "param4");
        this->m_param5.LoadFromXML(parent, xmlNode, "param5");
        this->m_param6.LoadFromXML(parent, xmlNode, "param6");
        this->m_param7.LoadFromXML(parent, xmlNode, "param7");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
        this->m_param4.SaveToXML(parent, xmlNode, "param4");
        this->m_param5.SaveToXML(parent, xmlNode, "param5");
        this->m_param6.SaveToXML(parent, xmlNode, "param6");
        this->m_param7.SaveToXML(parent, xmlNode, "param7");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName.c_str());
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName4);
            paramNode->setAttr("Desc", m_paramDesc4);

            behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
            paramNode->setAttr("Type", typeStr4.c_str());

            if (this->m_param4.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param4.GetValue());
            }

            if (m_paramRangeValid4)
            {
                paramNode->setAttr("RangeMin", m_min4);
                paramNode->setAttr("RangeMax", m_max4);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType4, behaviac::Meta::IsEnum<ParamBaseType4>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName5);
            paramNode->setAttr("Desc", m_paramDesc5);

            behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
            paramNode->setAttr("Type", typeStr5.c_str());

            if (this->m_param5.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param5.GetValue());
            }

            if (m_paramRangeValid5)
            {
                paramNode->setAttr("RangeMin", m_min5);
                paramNode->setAttr("RangeMax", m_max5);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType5, behaviac::Meta::IsEnum<ParamBaseType5>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName6);
            paramNode->setAttr("Desc", m_paramDesc6);

            behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
            paramNode->setAttr("Type", typeStr6.c_str());

            if (this->m_param6.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param6.GetValue());
            }

            if (m_paramRangeValid6)
            {
                paramNode->setAttr("RangeMin", m_min6);
                paramNode->setAttr("RangeMax", m_max6);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType6, behaviac::Meta::IsEnum<ParamBaseType6>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName7);
            paramNode->setAttr("Desc", m_paramDesc7);

            behaviac::string typeStr7 = ::GetTypeDescString<ParamType7>();
            paramNode->setAttr("Type", typeStr7.c_str());

            if (this->m_param7.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param7.GetValue());
            }

            if (m_paramRangeValid7)
            {
                paramNode->setAttr("RangeMin", m_min7);
                paramNode->setAttr("RangeMax", m_max7);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType7, behaviac::Meta::IsEnum<ParamBaseType7>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;

        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;

        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;

        }
        else if (m_paramDisplayName7.empty())
        {
            m_paramDisplayName7 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else if (m_paramDesc4.empty())
        {
            m_paramDesc4 = paramDesc;

        }
        else if (m_paramDesc5.empty())
        {
            m_paramDesc5 = paramDesc;

        }
        else if (m_paramDesc6.empty())
        {
            m_paramDesc6 = paramDesc;

        }
        else if (m_paramDesc7.empty())
        {
            m_paramDesc7 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;
            m_paramDesc4 = (paramDesc == 0 ? m_paramDisplayName4 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType4>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid4 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min4 = rangeMin;
                this->m_max4 = rangeMax;
            }
        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;
            m_paramDesc5 = (paramDesc == 0 ? m_paramDisplayName5 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType5, ParamBaseType5, behaviac::Meta::IsRefType<ParamType5>::Result>::SetValue(this->m_param5, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType5>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid5 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min5 = rangeMin;
                this->m_max5 = rangeMax;
            }
        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;
            m_paramDesc6 = (paramDesc == 0 ? m_paramDisplayName6 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType6, ParamBaseType6, behaviac::Meta::IsRefType<ParamType6>::Result>::SetValue(this->m_param6, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType6>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid6 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min6 = rangeMin;
                this->m_max6 = rangeMax;
            }
        }
        else if (m_paramDisplayName7.empty())
        {
            m_paramDisplayName7 = paramDisplayName;
            m_paramDesc7 = (paramDesc == 0 ? m_paramDisplayName7 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType7, ParamBaseType7, behaviac::Meta::IsRefType<ParamType7>::Result>::SetValue(this->m_param7, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType7>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid7 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min7 = rangeMin;
                this->m_max7 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 7;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            case 3:
            {
                behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
                return typeStr4;
            }

            case 4:
            {
                behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
                return typeStr5;
            }

            case 5:
            {
                behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
                return typeStr6;
            }

            case 6:
            {
                behaviac::string typeStr7 = ::GetTypeDescString<ParamType7>();
                return typeStr7;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4, const void* param5, const void* param6, const void* param7)
    {
        (((ObjectType*)parent)->*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3, *(ParamPointerType4)param4, *(ParamPointerType5)param5, *(ParamPointerType6)param6, *(ParamPointerType7)param7);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod7_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6,
                (PARAM_CALLEDTYPE(ParamType7))v7);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 7);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6,
            (PARAM_CALLEDTYPE(ParamType7))p7);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(ObjectType::*m_methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;
    behaviac::wstring				m_paramDisplayName4;
    behaviac::wstring				m_paramDesc4;
    behaviac::wstring				m_paramDisplayName5;
    behaviac::wstring				m_paramDesc5;
    behaviac::wstring				m_paramDisplayName6;
    behaviac::wstring				m_paramDesc6;
    behaviac::wstring				m_paramDisplayName7;
    behaviac::wstring				m_paramDesc7;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;
    bool							m_paramRangeValid4;
    float							m_min4, m_max4;
    bool							m_paramRangeValid5;
    float							m_min5, m_max5;
    bool							m_paramRangeValid6;
    float							m_min6, m_max6;
    bool							m_paramRangeValid7;
    float							m_min7, m_max7;

    ParamVariable<StoredType1>		m_param1;
    ParamVariable<StoredType2>		m_param2;
    ParamVariable<StoredType3>		m_param3;
    ParamVariable<StoredType4>		m_param4;
    ParamVariable<StoredType5>		m_param5;
    ParamVariable<StoredType6>		m_param6;
    ParamVariable<StoredType7>		m_param7;
};

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
class CGenericMethod7 : public CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>
{
public:
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType1 StoredType1;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType2 StoredType2;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType3 StoredType3;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType4 StoredType4;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType5 StoredType5;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType6 StoredType6;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType7 StoredType7;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType6 ParamBaseType6;
    typedef typename CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType7 ParamBaseType7;

    CGenericMethod7(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7),
                    const char* className, const char* propertyName) :
        CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(methodPtr, className, propertyName)
    {}

    CGenericMethod7(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7) const,
                    const char* className, const char* propertyName) :
        CGenericMethod7_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod7()
    {
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod7(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

            R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                (PARAM_CALLEDTYPE(ParamType4))v4,
                                (PARAM_CALLEDTYPE(ParamType5))v5,
                                (PARAM_CALLEDTYPE(ParamType6))v6,
                                (PARAM_CALLEDTYPE(ParamType7))v7);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 7);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);

        R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3,
                            (PARAM_CALLEDTYPE(ParamType4))p4,
                            (PARAM_CALLEDTYPE(ParamType5))p5,
                            (PARAM_CALLEDTYPE(ParamType6))p6,
                            (PARAM_CALLEDTYPE(ParamType7))p7);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
class CGenericMethod7<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> : public CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>
{
public:
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType1 StoredType1;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType2 StoredType2;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType3 StoredType3;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType4 StoredType4;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType5 StoredType5;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType6 StoredType6;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::StoredType7 StoredType7;

    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType6 ParamBaseType6;
    typedef typename CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>::ParamBaseType7 ParamBaseType7;

    CGenericMethod7(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7),
                    const char* className, const char* propertyName) :
        CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(methodPtr, className, propertyName)
    {}

    CGenericMethod7(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7) const,
                    const char* className, const char* propertyName) :
        CGenericMethod7_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod7()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod7(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6,
                (PARAM_CALLEDTYPE(ParamType7))v7);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 7);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6,
            (PARAM_CALLEDTYPE(ParamType7))p7);
    }
#endif//#if BEHAVIAC_ENABLE_NETWORKD
};

/////////////////////////////////////////////////////////
////////////////     CGenericMethod8_     ///////////////
/////////////////////////////////////////////////////////

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
class CGenericMethod8_ : public CMethodBase
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CGenericMethod8_);

    typedef typename StoredTypeSelector<ParamType1>::ParamBaseType			ParamBaseType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamBaseType			ParamBaseType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamBaseType			ParamBaseType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamBaseType			ParamBaseType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamBaseType			ParamBaseType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamBaseType			ParamBaseType6;
    typedef typename StoredTypeSelector<ParamType7>::ParamBaseType			ParamBaseType7;
    typedef typename StoredTypeSelector<ParamType8>::ParamBaseType			ParamBaseType8;

    typedef typename StoredTypeSelector<ParamType1>::ParamPointerType			ParamPointerType1;
    typedef typename StoredTypeSelector<ParamType2>::ParamPointerType			ParamPointerType2;
    typedef typename StoredTypeSelector<ParamType3>::ParamPointerType			ParamPointerType3;
    typedef typename StoredTypeSelector<ParamType4>::ParamPointerType			ParamPointerType4;
    typedef typename StoredTypeSelector<ParamType5>::ParamPointerType			ParamPointerType5;
    typedef typename StoredTypeSelector<ParamType6>::ParamPointerType			ParamPointerType6;
    typedef typename StoredTypeSelector<ParamType7>::ParamPointerType			ParamPointerType7;
    typedef typename StoredTypeSelector<ParamType8>::ParamPointerType			ParamPointerType8;

    typedef typename StoredTypeSelector<ParamType1>::StoredType		StoredType1;
    typedef typename StoredTypeSelector<ParamType2>::StoredType		StoredType2;
    typedef typename StoredTypeSelector<ParamType3>::StoredType		StoredType3;
    typedef typename StoredTypeSelector<ParamType4>::StoredType		StoredType4;
    typedef typename StoredTypeSelector<ParamType5>::StoredType		StoredType5;
    typedef typename StoredTypeSelector<ParamType6>::StoredType		StoredType6;
    typedef typename StoredTypeSelector<ParamType7>::StoredType		StoredType7;
    typedef typename StoredTypeSelector<ParamType8>::StoredType		StoredType8;

    CGenericMethod8_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8),
                     const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(methodPtr),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false), m_paramRangeValid6(false), m_paramRangeValid7(false), m_paramRangeValid8(false),
        m_param1(), m_param2(), m_param3(), m_param4(), m_param5(), m_param6(), m_param7(), m_param8()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min6 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max6 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min7 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max7 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    typedef R(ObjectType::*MethodType_t)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8);

    CGenericMethod8_(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8) const,
                     const char* className, const char* propertyName) :
        CMethodBase(propertyName, className), m_methodPtr(MethodType_t(methodPtr)),
        m_paramRangeValid1(false), m_paramRangeValid2(false), m_paramRangeValid3(false), m_paramRangeValid4(false), m_paramRangeValid5(false), m_paramRangeValid6(false), m_paramRangeValid7(false), m_paramRangeValid8(false),
        m_param1(), m_param2(), m_param3(), m_param4(), m_param5(), m_param6(), m_param7(), m_param8()
    {
		this->m_min1 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max1 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min2 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max2 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min3 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max3 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min4 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max4 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min5 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max5 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min6 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max6 = CMETHODBASE_RANGE_MAX_DEFAULT;
		this->m_min7 = CMETHODBASE_RANGE_MIN_DEFAULT;
		this->m_max7 = CMETHODBASE_RANGE_MAX_DEFAULT;
    }

    CGenericMethod8_(const CGenericMethod8_& copy) : CMethodBase(copy)
    {
        this->m_methodPtr = copy.m_methodPtr;

        this->m_paramDisplayName1 = copy.m_paramDisplayName1;
        this->m_paramDisplayName2 = copy.m_paramDisplayName2;
        this->m_paramDisplayName3 = copy.m_paramDisplayName3;
        this->m_paramDisplayName4 = copy.m_paramDisplayName4;
        this->m_paramDisplayName5 = copy.m_paramDisplayName5;
        this->m_paramDisplayName6 = copy.m_paramDisplayName6;
        this->m_paramDisplayName7 = copy.m_paramDisplayName7;
        this->m_paramDisplayName8 = copy.m_paramDisplayName8;

        this->m_param1 = copy.m_param1;
        this->m_param2 = copy.m_param2;
        this->m_param3 = copy.m_param3;
        this->m_param4 = copy.m_param4;
        this->m_param5 = copy.m_param5;
        this->m_param6 = copy.m_param6;
        this->m_param7 = copy.m_param7;
        this->m_param8 = copy.m_param8;
        this->m_paramRangeValid1 = copy.m_paramRangeValid1;
        this->m_paramRangeValid2 = copy.m_paramRangeValid2;
        this->m_paramRangeValid3 = copy.m_paramRangeValid3;
        this->m_paramRangeValid4 = copy.m_paramRangeValid4;
        this->m_paramRangeValid5 = copy.m_paramRangeValid5;
        this->m_paramRangeValid6 = copy.m_paramRangeValid6;
        this->m_paramRangeValid7 = copy.m_paramRangeValid7;
        this->m_paramRangeValid8 = copy.m_paramRangeValid8;
    }

    virtual void LoadFromXML(CTagObject* parent, const behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.LoadFromXML(parent, xmlNode, "param1");
        this->m_param2.LoadFromXML(parent, xmlNode, "param2");
        this->m_param3.LoadFromXML(parent, xmlNode, "param3");
        this->m_param4.LoadFromXML(parent, xmlNode, "param4");
        this->m_param5.LoadFromXML(parent, xmlNode, "param5");
        this->m_param6.LoadFromXML(parent, xmlNode, "param6");
        this->m_param7.LoadFromXML(parent, xmlNode, "param7");
        this->m_param8.LoadFromXML(parent, xmlNode, "param8");
    }

    virtual void SaveToXML(const CTagObject* parent, behaviac::ISerializableNode& xmlNode)
    {
        this->m_param1.SaveToXML(parent, xmlNode, "param1");
        this->m_param2.SaveToXML(parent, xmlNode, "param2");
        this->m_param3.SaveToXML(parent, xmlNode, "param3");
        this->m_param4.SaveToXML(parent, xmlNode, "param4");
        this->m_param5.SaveToXML(parent, xmlNode, "param5");
        this->m_param6.SaveToXML(parent, xmlNode, "param6");
        this->m_param7.SaveToXML(parent, xmlNode, "param7");
        this->m_param8.SaveToXML(parent, xmlNode, "param8");
    }

    virtual void GetUiInfo(CTagTypeDescriptor::TypesMap_t* types, const CTagObject* parent, const behaviac::XmlNodeRef& xmlNode)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        behaviac::XmlNodeRef memberNode = xmlNode;

        if (types == NULL)
        {
            memberNode = xmlNode->newChild("Method");
            memberNode->setAttr("Name", this->m_propertyName.c_str());
            memberNode->setAttr("DisplayName", this->GetDisplayName());
            memberNode->setAttr("Desc", this->GetDesc());

            if (this->m_flag & CMethodBase::KMETHOD_NAMEDEVENT)
            {
                memberNode->setAttr("Flag", "namedevent");
            }

            if (this->m_classFullName.c_str())
            {
                memberNode->setAttr("Class", this->m_classFullName.c_str());
            }

            if (m_bStatic)
            {
                memberNode->setAttr("Static", "true");
            }

            if (this->IsActionMethodOnly())
            {
                memberNode->setAttr("IsActionMethodOnly", "true");
            }

            //const char* pReturn = GetClassTypeName((R*)0);
            //memberNode->setAttr("ReturnType", pReturn);
            behaviac::string returnTypeStr = ::GetTypeDescString<R>();
            memberNode->setAttr("ReturnType", returnTypeStr.c_str());
        }

        UserTypeInfoExport<R>::Export(types, true, memberNode);

        behaviac::XmlNodeRef paramNode = memberNode;

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName1);
            paramNode->setAttr("Desc", m_paramDesc1);

            behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
            paramNode->setAttr("Type", typeStr1.c_str());

            if (this->m_param1.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param1.GetValue());
            }

            if (m_paramRangeValid1)
            {
                paramNode->setAttr("RangeMin", m_min1);
                paramNode->setAttr("RangeMax", m_max1);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType1, behaviac::Meta::IsEnum<ParamBaseType1>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName2);
            paramNode->setAttr("Desc", m_paramDesc2);

            behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
            paramNode->setAttr("Type", typeStr2.c_str());

            if (this->m_param2.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param2.GetValue());
            }

            if (m_paramRangeValid2)
            {
                paramNode->setAttr("RangeMin", m_min2);
                paramNode->setAttr("RangeMax", m_max2);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType2, behaviac::Meta::IsEnum<ParamBaseType2>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName3);
            paramNode->setAttr("Desc", m_paramDesc3);

            behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
            paramNode->setAttr("Type", typeStr3.c_str());

            if (this->m_param3.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param3.GetValue());
            }

            if (m_paramRangeValid3)
            {
                paramNode->setAttr("RangeMin", m_min3);
                paramNode->setAttr("RangeMax", m_max3);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType3, behaviac::Meta::IsEnum<ParamBaseType3>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName4);
            paramNode->setAttr("Desc", m_paramDesc4);

            behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
            paramNode->setAttr("Type", typeStr4.c_str());

            if (this->m_param4.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param4.GetValue());
            }

            if (m_paramRangeValid4)
            {
                paramNode->setAttr("RangeMin", m_min4);
                paramNode->setAttr("RangeMax", m_max4);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType4, behaviac::Meta::IsEnum<ParamBaseType4>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName5);
            paramNode->setAttr("Desc", m_paramDesc5);

            behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
            paramNode->setAttr("Type", typeStr5.c_str());

            if (this->m_param5.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param5.GetValue());
            }

            if (m_paramRangeValid5)
            {
                paramNode->setAttr("RangeMin", m_min5);
                paramNode->setAttr("RangeMax", m_max5);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType5, behaviac::Meta::IsEnum<ParamBaseType5>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName6);
            paramNode->setAttr("Desc", m_paramDesc6);

            behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
            paramNode->setAttr("Type", typeStr6.c_str());

            if (this->m_param6.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param6.GetValue());
            }

            if (m_paramRangeValid6)
            {
                paramNode->setAttr("RangeMin", m_min6);
                paramNode->setAttr("RangeMax", m_max6);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType6, behaviac::Meta::IsEnum<ParamBaseType6>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName7);
            paramNode->setAttr("Desc", m_paramDesc7);

            behaviac::string typeStr7 = ::GetTypeDescString<ParamType7>();
            paramNode->setAttr("Type", typeStr7.c_str());

            if (this->m_param7.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param7.GetValue());
            }

            if (m_paramRangeValid7)
            {
                paramNode->setAttr("RangeMin", m_min7);
                paramNode->setAttr("RangeMax", m_max7);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType7, behaviac::Meta::IsEnum<ParamBaseType7>::Result>::GetUiInfo(types, paramNode);

        if (types == NULL)
        {
            paramNode = memberNode->newChild("Param");
            paramNode->setAttr("DisplayName", m_paramDisplayName8);
            paramNode->setAttr("Desc", m_paramDesc8);

            behaviac::string typeStr8 = ::GetTypeDescString<ParamType8>();
            paramNode->setAttr("Type", typeStr8.c_str());

            if (this->m_param8.IsDefaultValid())
            {
                paramNode->setAttr("Default", this->m_param8.GetValue());
            }

            if (m_paramRangeValid8)
            {
                paramNode->setAttr("RangeMin", m_min8);
                paramNode->setAttr("RangeMax", m_max8);
            }
        }

        EnumClassUiInfoGetter<ParamBaseType8, behaviac::Meta::IsEnum<ParamBaseType8>::Result>::GetUiInfo(types, paramNode);
    }

    virtual CMethodBase& PARAM_DISPLAYNAME(const wchar_t* paramDisplayName)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;

        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;

        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;

        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;

        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;

        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;

        }
        else if (m_paramDisplayName7.empty())
        {
            m_paramDisplayName7 = paramDisplayName;

        }
        else if (m_paramDisplayName8.empty())
        {
            m_paramDisplayName8 = paramDisplayName;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DESC(const wchar_t* paramDesc)
    {
        BEHAVIAC_UNUSED_VAR(paramDesc);

        if (m_paramDesc1.empty())
        {
            m_paramDesc1 = paramDesc;

        }
        else if (m_paramDesc2.empty())
        {
            m_paramDesc2 = paramDesc;

        }
        else if (m_paramDesc3.empty())
        {
            m_paramDesc3 = paramDesc;

        }
        else if (m_paramDesc4.empty())
        {
            m_paramDesc4 = paramDesc;

        }
        else if (m_paramDesc5.empty())
        {
            m_paramDesc5 = paramDesc;

        }
        else if (m_paramDesc6.empty())
        {
            m_paramDesc6 = paramDesc;

        }
        else if (m_paramDesc7.empty())
        {
            m_paramDesc7 = paramDesc;

        }
        else if (m_paramDesc8.empty())
        {
            m_paramDesc8 = paramDesc;

        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual CMethodBase& PARAM_DISPLAY_INFO(const wchar_t* paramDisplayName, const wchar_t* paramDesc = 0, const DefaultParam_t& defaultParam = DefaultParam_t(), float rangeMin = CMETHODBASE_RANGE_MIN_DEFAULT, float rangeMax = CMETHODBASE_RANGE_MAX_DEFAULT)
    {
        BEHAVIAC_UNUSED_VAR(paramDisplayName);
        BEHAVIAC_UNUSED_VAR(paramDesc);
        BEHAVIAC_UNUSED_VAR(defaultParam);
        BEHAVIAC_UNUSED_VAR(rangeMin);
        BEHAVIAC_UNUSED_VAR(rangeMax);

        if (m_paramDisplayName1.empty())
        {
            m_paramDisplayName1 = paramDisplayName;
            m_paramDesc1 = (paramDesc == 0 ? m_paramDisplayName1 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType1, ParamBaseType1, behaviac::Meta::IsRefType<ParamType1>::Result>::SetValue(this->m_param1, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType1>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid1 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min1 = rangeMin;
                this->m_max1 = rangeMax;
            }
        }
        else if (m_paramDisplayName2.empty())
        {
            m_paramDisplayName2 = paramDisplayName;
            m_paramDesc2 = (paramDesc == 0 ? m_paramDisplayName2 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType2, ParamBaseType2, behaviac::Meta::IsRefType<ParamType2>::Result>::SetValue(this->m_param2, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType2>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid2 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min2 = rangeMin;
                this->m_max2 = rangeMax;
            }
        }
        else if (m_paramDisplayName3.empty())
        {
            m_paramDisplayName3 = paramDisplayName;
            m_paramDesc3 = (paramDesc == 0 ? m_paramDisplayName3 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType3, ParamBaseType3, behaviac::Meta::IsRefType<ParamType3>::Result>::SetValue(this->m_param3, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType3>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid3 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min3 = rangeMin;
                this->m_max3 = rangeMax;
            }
        }
        else if (m_paramDisplayName4.empty())
        {
            m_paramDisplayName4 = paramDisplayName;
            m_paramDesc4 = (paramDesc == 0 ? m_paramDisplayName4 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType4, ParamBaseType4, behaviac::Meta::IsRefType<ParamType4>::Result>::SetValue(this->m_param4, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType4>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid4 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min4 = rangeMin;
                this->m_max4 = rangeMax;
            }
        }
        else if (m_paramDisplayName5.empty())
        {
            m_paramDisplayName5 = paramDisplayName;
            m_paramDesc5 = (paramDesc == 0 ? m_paramDisplayName5 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType5, ParamBaseType5, behaviac::Meta::IsRefType<ParamType5>::Result>::SetValue(this->m_param5, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType5>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid5 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min5 = rangeMin;
                this->m_max5 = rangeMax;
            }
        }
        else if (m_paramDisplayName6.empty())
        {
            m_paramDisplayName6 = paramDisplayName;
            m_paramDesc6 = (paramDesc == 0 ? m_paramDisplayName6 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType6, ParamBaseType6, behaviac::Meta::IsRefType<ParamType6>::Result>::SetValue(this->m_param6, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType6>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid6 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min6 = rangeMin;
                this->m_max6 = rangeMax;
            }
        }
        else if (m_paramDisplayName7.empty())
        {
            m_paramDisplayName7 = paramDisplayName;
            m_paramDesc7 = (paramDesc == 0 ? m_paramDisplayName7 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType7, ParamBaseType7, behaviac::Meta::IsRefType<ParamType7>::Result>::SetValue(this->m_param7, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType7>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid7 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min7 = rangeMin;
                this->m_max7 = rangeMax;
            }
        }
        else if (m_paramDisplayName8.empty())
        {
            m_paramDisplayName8 = paramDisplayName;
            m_paramDesc8 = (paramDesc == 0 ? m_paramDisplayName8 : paramDesc);

            if (defaultParam.IsValid())
            {
				DefaulParamSetValueSelector<StoredType8, ParamBaseType8, behaviac::Meta::IsRefType<ParamType8>::Result>::SetValue(this->m_param8, defaultParam);;
            }

            int typeId = GetClassTypeNumberId<ParamBaseType8>();

            if (IsNumberClassTypeNumberId(typeId))
            {
                BEHAVIAC_ASSERT(rangeMin <= rangeMax);
                this->m_paramRangeValid8 = (!behaviac::IsEqualWithEpsilon(rangeMin, CMETHODBASE_RANGE_MIN_DEFAULT) && !behaviac::IsEqualWithEpsilon(rangeMax, CMETHODBASE_RANGE_MAX_DEFAULT));
                this->m_min8 = rangeMin;
                this->m_max8 = rangeMax;
            }
        }
        else
        {
            BEHAVIAC_ASSERT(0, "too many params!");
        }

        return *this;
    }

    virtual int GetParams()
    {
        return 8;
    }

    virtual behaviac::string GetParamTypeName(uint32_t idx)
    {
        switch (idx)
        {
            case 0:
            {
                behaviac::string typeStr1 = ::GetTypeDescString<ParamType1>();
                return typeStr1;
            }

            case 1:
            {
                behaviac::string typeStr2 = ::GetTypeDescString<ParamType2>();
                return typeStr2;
            }

            case 2:
            {
                behaviac::string typeStr3 = ::GetTypeDescString<ParamType3>();
                return typeStr3;
            }

            case 3:
            {
                behaviac::string typeStr4 = ::GetTypeDescString<ParamType4>();
                return typeStr4;
            }

            case 4:
            {
                behaviac::string typeStr5 = ::GetTypeDescString<ParamType5>();
                return typeStr5;
            }

            case 5:
            {
                behaviac::string typeStr6 = ::GetTypeDescString<ParamType6>();
                return typeStr6;
            }

            case 6:
            {
                behaviac::string typeStr7 = ::GetTypeDescString<ParamType7>();
                return typeStr7;
            }

            case 7:
            {
                behaviac::string typeStr8 = ::GetTypeDescString<ParamType8>();
                return typeStr8;
            }

            default:
                return "";
        }
    }

    using CMethodBase::vCall;
    virtual void vCall(const CTagObject* parent, const void* param1, const void* param2, const void* param3, const void* param4, const void* param5, const void* param6, const void* param7, const void* param8)
    {
        (((ObjectType*)parent)->*this->m_methodPtr)(*(ParamPointerType1)param1, *(ParamPointerType2)param2, *(ParamPointerType3)param3, *(ParamPointerType4)param4, *(ParamPointerType5)param5, *(ParamPointerType6)param6, *(ParamPointerType7)param7, *(ParamPointerType8)param8);
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod8_(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
                typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7,
                           (PARAM_CALLEDTYPE(ParamType8))v8);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
            typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6,
                (PARAM_CALLEDTYPE(ParamType7))v7,
                (PARAM_CALLEDTYPE(ParamType8))v8);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
            this->m_param8.SetVariableRegistry(parHolder, v8);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 8);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);
        PARAM_BASETYPE(ParamType8) p8;
        params[7]->GetValue(p8);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6,
            (PARAM_CALLEDTYPE(ParamType7))p7,
            (PARAM_CALLEDTYPE(ParamType8))p8);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD

protected:
    R(ObjectType::*m_methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8);

    behaviac::wstring				m_paramDisplayName1;
    behaviac::wstring				m_paramDesc1;
    behaviac::wstring				m_paramDisplayName2;
    behaviac::wstring				m_paramDesc2;
    behaviac::wstring				m_paramDisplayName3;
    behaviac::wstring				m_paramDesc3;
    behaviac::wstring				m_paramDisplayName4;
    behaviac::wstring				m_paramDesc4;
    behaviac::wstring				m_paramDisplayName5;
    behaviac::wstring				m_paramDesc5;
    behaviac::wstring				m_paramDisplayName6;
    behaviac::wstring				m_paramDesc6;
    behaviac::wstring				m_paramDisplayName7;
    behaviac::wstring				m_paramDesc7;
    behaviac::wstring				m_paramDisplayName8;
    behaviac::wstring				m_paramDesc8;

    bool							m_paramRangeValid1;
    float							m_min1, m_max1;
    bool							m_paramRangeValid2;
    float							m_min2, m_max2;
    bool							m_paramRangeValid3;
    float							m_min3, m_max3;
    bool							m_paramRangeValid4;
    float							m_min4, m_max4;
    bool							m_paramRangeValid5;
    float							m_min5, m_max5;
    bool							m_paramRangeValid6;
    float							m_min6, m_max6;
    bool							m_paramRangeValid7;
    float							m_min7, m_max7;
    bool							m_paramRangeValid8;
    float							m_min8, m_max8;

    ParamVariable<StoredType1>		m_param1;
    ParamVariable<StoredType2>		m_param2;
    ParamVariable<StoredType3>		m_param3;
    ParamVariable<StoredType4>		m_param4;
    ParamVariable<StoredType5>		m_param5;
    ParamVariable<StoredType6>		m_param6;
    ParamVariable<StoredType7>		m_param7;
    ParamVariable<StoredType8>		m_param8;
};

template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
class CGenericMethod8 : public CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>
{
public:
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType1 StoredType1;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType2 StoredType2;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType3 StoredType3;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType4 StoredType4;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType5 StoredType5;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType6 StoredType6;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType7 StoredType7;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType8 StoredType8;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType6 ParamBaseType6;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType7 ParamBaseType7;
    typedef typename CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType8 ParamBaseType8;

    CGenericMethod8(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8),
                    const char* className, const char* propertyName) :
        CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(methodPtr, className, propertyName)
    {}

    CGenericMethod8(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8) const,
                    const char* className, const char* propertyName) :
        CGenericMethod8_<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod8()
    {
    }

    virtual behaviac::Property* CreateProperty(const char* defaultValue, bool bConst) const
    {
		typedef VALUE_TYPE(R) TTYPE;
        behaviac::Property* pProperty = behaviac::Property::Creator<TTYPE>(defaultValue, 0, bConst);

        return pProperty;
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod8(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
                typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7,
                           (PARAM_CALLEDTYPE(ParamType8))v8);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
            typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

            R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                                (PARAM_CALLEDTYPE(ParamType1))v1,
                                (PARAM_CALLEDTYPE(ParamType2))v2,
                                (PARAM_CALLEDTYPE(ParamType3))v3,
                                (PARAM_CALLEDTYPE(ParamType4))v4,
                                (PARAM_CALLEDTYPE(ParamType5))v5,
                                (PARAM_CALLEDTYPE(ParamType6))v6,
                                (PARAM_CALLEDTYPE(ParamType7))v7,
                                (PARAM_CALLEDTYPE(ParamType8))v8);

            if (this->m_return)
            {
                *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
            }

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
            this->m_param8.SetVariableRegistry(parHolder, v8);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 8);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);
        PARAM_BASETYPE(ParamType8) p8;
        params[7]->GetValue(p8);

        R returnValue = (((ObjectType*)parent)->*this->m_methodPtr)(
                            (PARAM_CALLEDTYPE(ParamType1))p1,
                            (PARAM_CALLEDTYPE(ParamType2))p2,
                            (PARAM_CALLEDTYPE(ParamType3))p3,
                            (PARAM_CALLEDTYPE(ParamType4))p4,
                            (PARAM_CALLEDTYPE(ParamType5))p5,
                            (PARAM_CALLEDTYPE(ParamType6))p6,
                            (PARAM_CALLEDTYPE(ParamType7))p7,
                            (PARAM_CALLEDTYPE(ParamType8))p8);

        if (this->m_return)
        {
            *(behaviac::AsyncValue<R>*)this->m_return = returnValue;
        }
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

template<class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
class CGenericMethod8<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> : public CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>
{
public:
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType1 StoredType1;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType2 StoredType2;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType3 StoredType3;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType4 StoredType4;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType5 StoredType5;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType6 StoredType6;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType7 StoredType7;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::StoredType8 StoredType8;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType1 ParamBaseType1;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType2 ParamBaseType2;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType3 ParamBaseType3;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType4 ParamBaseType4;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType5 ParamBaseType5;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType6 ParamBaseType6;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType7 ParamBaseType7;
    typedef typename CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>::ParamBaseType8 ParamBaseType8;

    CGenericMethod8(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8),
                    const char* className, const char* propertyName) :
        CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(methodPtr, className, propertyName)
    {}

    CGenericMethod8(void (ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8) const,
                    const char* className, const char* propertyName) :
        CGenericMethod8_<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(methodPtr, className, propertyName)
    {}

    virtual ~CGenericMethod8()
    {
    }

    virtual CMethodBase* clone() const
    {
        return BEHAVIAC_NEW CGenericMethod8(*this);
    }

    virtual void run(const CTagObject* parent, const CTagObject* parHolder)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);
#if BEHAVIAC_ENABLE_NETWORKD
        bool bHandled = false;

        if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
        {
            behaviac::Network* pNw = behaviac::Network::GetInstance();

            if (pNw && !pNw->IsSinglePlayer())
            {
                behaviac::string nameTemp;

                if (this->m_className)
                {
                    nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName.c_str());

                }
                else
                {
                    nameTemp = this->m_propertyName.c_str();
                }

                typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
                typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
                typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
                typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
                typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
                typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
                typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
                typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

                bHandled = pNw->SendRemoteEvent<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8>(this->m_netRole, nameTemp.c_str(),
                           (PARAM_CALLEDTYPE(ParamType1))v1,
                           (PARAM_CALLEDTYPE(ParamType2))v2,
                           (PARAM_CALLEDTYPE(ParamType3))v3,
                           (PARAM_CALLEDTYPE(ParamType4))v4,
                           (PARAM_CALLEDTYPE(ParamType5))v5,
                           (PARAM_CALLEDTYPE(ParamType6))v6,
                           (PARAM_CALLEDTYPE(ParamType7))v7,
                           (PARAM_CALLEDTYPE(ParamType8))v8);
            }
        }

        if (!bHandled)
#endif//#if BEHAVIAC_ENABLE_NETWORKD
        {
            typename ParamVariable<StoredType1>::GetReturnType v1 = this->m_param1.GetValue(parent, parHolder);
            typename ParamVariable<StoredType2>::GetReturnType v2 = this->m_param2.GetValue(parent, parHolder);
            typename ParamVariable<StoredType3>::GetReturnType v3 = this->m_param3.GetValue(parent, parHolder);
            typename ParamVariable<StoredType4>::GetReturnType v4 = this->m_param4.GetValue(parent, parHolder);
            typename ParamVariable<StoredType5>::GetReturnType v5 = this->m_param5.GetValue(parent, parHolder);
            typename ParamVariable<StoredType6>::GetReturnType v6 = this->m_param6.GetValue(parent, parHolder);
            typename ParamVariable<StoredType7>::GetReturnType v7 = this->m_param7.GetValue(parent, parHolder);
            typename ParamVariable<StoredType8>::GetReturnType v8 = this->m_param8.GetValue(parent, parHolder);

            (((ObjectType*)parent)->*this->m_methodPtr)(
                (PARAM_CALLEDTYPE(ParamType1))v1,
                (PARAM_CALLEDTYPE(ParamType2))v2,
                (PARAM_CALLEDTYPE(ParamType3))v3,
                (PARAM_CALLEDTYPE(ParamType4))v4,
                (PARAM_CALLEDTYPE(ParamType5))v5,
                (PARAM_CALLEDTYPE(ParamType6))v6,
                (PARAM_CALLEDTYPE(ParamType7))v7,
                (PARAM_CALLEDTYPE(ParamType8))v8);

            this->m_param1.SetVariableRegistry(parHolder, v1);
            this->m_param2.SetVariableRegistry(parHolder, v2);
            this->m_param3.SetVariableRegistry(parHolder, v3);
            this->m_param4.SetVariableRegistry(parHolder, v4);
            this->m_param5.SetVariableRegistry(parHolder, v5);
            this->m_param6.SetVariableRegistry(parHolder, v6);
            this->m_param7.SetVariableRegistry(parHolder, v7);
            this->m_param8.SetVariableRegistry(parHolder, v8);
        }
    }

#if BEHAVIAC_ENABLE_NETWORKD
    virtual void run(const CTagObject* parent, const CTagObject* parHolder, const behaviac::Variants_t& params)
    {
        BEHAVIAC_UNUSED_VAR(parent);
        BEHAVIAC_UNUSED_VAR(parHolder);

        BEHAVIAC_ASSERT(params.size() == 7);

        PARAM_BASETYPE(ParamType1) p1;
        params[0]->GetValue(p1);
        PARAM_BASETYPE(ParamType2) p2;
        params[1]->GetValue(p2);
        PARAM_BASETYPE(ParamType3) p3;
        params[2]->GetValue(p3);
        PARAM_BASETYPE(ParamType4) p4;
        params[3]->GetValue(p4);
        PARAM_BASETYPE(ParamType5) p5;
        params[4]->GetValue(p5);
        PARAM_BASETYPE(ParamType6) p6;
        params[5]->GetValue(p6);
        PARAM_BASETYPE(ParamType7) p7;
        params[6]->GetValue(p7);
        PARAM_BASETYPE(ParamType8) p8;
        params[7]->GetValue(p8);

        (((ObjectType*)parent)->*this->m_methodPtr)(
            (PARAM_CALLEDTYPE(ParamType1))p1,
            (PARAM_CALLEDTYPE(ParamType2))p2,
            (PARAM_CALLEDTYPE(ParamType3))p3,
            (PARAM_CALLEDTYPE(ParamType4))p4,
            (PARAM_CALLEDTYPE(ParamType5))p5,
            (PARAM_CALLEDTYPE(ParamType6))p6,
            (PARAM_CALLEDTYPE(ParamType7))p7,
            (PARAM_CALLEDTYPE(ParamType8))p8);
    }
#endif//BEHAVIAC_ENABLE_NETWORKD
};

#include "behaviac/base/object/staticmethod.h"

struct CMethodFactory
{
    template<typename R, class ObjectType>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(void), const char* className, const char* propertyName)
    {
        typedef CGenericMethod<R, ObjectType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename R, class ObjectType>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(void) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod<R, ObjectType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(void), const char* className, const char* propertyName)
    {
        typedef CGenericMethod<R, ObjectType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(void), const char* className, const char* propertyName)
    {
        typedef CGenericMethod<R, ObjectType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(void) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod<R, ObjectType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(void) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod<R, ObjectType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ObjectType>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(void), void (ObjectType::*methodPtr)(void), const char* className, const char* propertyName)
    {
        typedef CGenericMethod<void, ObjectType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType>
    static CMethodBase& Create(CR(*checkResultPtr)(void), void (ObjectType::*methodPtr)(void), const char* className, const char* propertyName)
    {
        typedef CGenericMethod<void, ObjectType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ObjectType>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(void), void (ObjectType::*methodPtr)(void) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod<void, ObjectType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType>
    static CMethodBase& Create(CR(*checkResultPtr)(void), void (ObjectType::*methodPtr)(void) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod<void, ObjectType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType), const char* className, const char* propertyName)
    {
        typedef CGenericMethod1<R, ObjectType, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod1<R, ObjectType, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType), const char* className, const char* propertyName)
    {
        typedef CGenericMethod1<R, ObjectType, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType), const char* className, const char* propertyName)
    {
        typedef CGenericMethod1<R, ObjectType, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod1<R, ObjectType, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod1<R, ObjectType, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ObjectType, class ParamType>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(void), void (ObjectType::*methodPtr)(ParamType), const char* className, const char* propertyName)
    {
        typedef CGenericMethod1<void, ObjectType, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType>
    static CMethodBase& Create(CR(*checkResultPtr)(void), void (ObjectType::*methodPtr)(ParamType), const char* className, const char* propertyName)
    {
        typedef CGenericMethod1<void, ObjectType, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ObjectType, class ParamType>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(void), void (ObjectType::*methodPtr)(ParamType) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod1<void, ObjectType, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ObjectType, class ParamType>
    static CMethodBase& Create(CR(*checkResultPtr)(void), void (ObjectType::*methodPtr)(ParamType) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod1<void, ObjectType, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName)
    {
        typedef CGenericMethod2<R, ObjectType, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod2<R, ObjectType, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName)
    {
        typedef CGenericMethod2<R, ObjectType, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName)
    {
        typedef CGenericMethod2<R, ObjectType, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod2<R, ObjectType, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod2<R, ObjectType, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ObjectType, class ParamType1, class ParamType2>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void (ObjectType::*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName)
    {
        typedef CGenericMethod2<void, ObjectType, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2>
    static CMethodBase& Create(CR(*checkResultPtr)(), void (ObjectType::*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName)
    {
        typedef CGenericMethod2<void, ObjectType, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void (ObjectType::*methodPtr)(ParamType1, ParamType2) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod2<void, ObjectType, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2>
    static CMethodBase& Create(CR(*checkResultPtr)(), void (ObjectType::*methodPtr)(ParamType1, ParamType2) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod2<void, ObjectType, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName)
    {
        typedef CGenericMethod3<R, ObjectType, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod3<R, ObjectType, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName)
    {
        typedef CGenericMethod3<R, ObjectType, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName)
    {
        typedef CGenericMethod3<R, ObjectType, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod3<R, ObjectType, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod3<R, ObjectType, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName)
    {
        typedef CGenericMethod3<void, ObjectType, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName)
    {
        typedef CGenericMethod3<void, ObjectType, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod3<void, ObjectType, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod3<void, ObjectType, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName)
    {
        typedef CGenericMethod4<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod4<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName)
    {
        typedef CGenericMethod4<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName)
    {
        typedef CGenericMethod4<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod4<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod4<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName)
    {
        typedef CGenericMethod4<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName)
    {
        typedef CGenericMethod4<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod4<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod4<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName)
    {
        typedef CGenericMethod5<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod5<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName)
    {
        typedef CGenericMethod5<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName)
    {
        typedef CGenericMethod5<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod5<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod5<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName)
    {
        typedef CGenericMethod5<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName)
    {
        typedef CGenericMethod5<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod5<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod5<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName)
    {
        typedef CGenericMethod6<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod6<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName)
    {
        typedef CGenericMethod6<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName)
    {
        typedef CGenericMethod6<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod6<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod6<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName)
    {
        typedef CGenericMethod6<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName)
    {
        typedef CGenericMethod6<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod6<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod6<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName)
    {
        typedef CGenericMethod7<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod7<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName)
    {
        typedef CGenericMethod7<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName)
    {
        typedef CGenericMethod7<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod7<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod7<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName)
    {
        typedef CGenericMethod7<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName)
    {
        typedef CGenericMethod7<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod7<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod7<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName)
    {
        typedef CGenericMethod8<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod8<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName)
    {
        typedef CGenericMethod8<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName)
    {
        typedef CGenericMethod8<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod8<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, typename R, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod8<R, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName)
    {
        typedef CGenericMethod8<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName)
    {
        typedef CGenericMethod8<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(CR(ObjectType::*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod8<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    template<typename CR, class ObjectType, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(ObjectType::*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8) const, const char* className, const char* propertyName)
    {
        typedef CGenericMethod8<void, ObjectType, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename R>
    static CMethodBase& Create(R(*methodPtr)(void), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic<R> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(*methodPtr)(void), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic<R> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR>
    static CMethodBase& Create(CR(*checkResultPtr)(void), void(*methodPtr)(void), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic<void> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ParamType>
    static CMethodBase& Create(R(*methodPtr)(ParamType), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic1<R, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ParamType>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(*methodPtr)(ParamType), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic1<R, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ParamType>
    static CMethodBase& Create(CR(*checkResultPtr)(void), void(*methodPtr)(ParamType), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic1<void, ParamType> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ParamType1, class ParamType2>
    static CMethodBase& Create(R(*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic2<R, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ParamType1, class ParamType2>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic2<R, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ParamType1, class ParamType2>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(*methodPtr)(ParamType1, ParamType2), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic2<void, ParamType1, ParamType2> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(R(*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic3<R, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic3<R, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ParamType1, class ParamType2, class ParamType3>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(*methodPtr)(ParamType1, ParamType2, ParamType3), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic3<void, ParamType1, ParamType2, ParamType3> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic4<R, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic4<R, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ParamType1, class ParamType2, class ParamType3, class ParamType4>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic4<void, ParamType1, ParamType2, ParamType3, ParamType4> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic5<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic5<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic5<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic6<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic6<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic6<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic7<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic7<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic7<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic8<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder<R>();
        return *pMethod;
    }

    template<typename CR, typename R, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(CR(*checkResultPtr)(R), R(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic8<R, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }

    template<typename CR, class ParamType1, class ParamType2, class ParamType3, class ParamType4, class ParamType5, class ParamType6, class ParamType7, class ParamType8>
    static CMethodBase& Create(CR(*checkResultPtr)(), void(*methodPtr)(ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8), const char* className, const char* propertyName)
    {
        typedef CGenericMethodStatic8<void, ParamType1, ParamType2, ParamType3, ParamType4, ParamType5, ParamType6, ParamType7, ParamType8> MethodType;
        CMethodBase* pMethod = BEHAVIAC_NEW MethodType(methodPtr, className, propertyName);
        pMethod->AddResultHanlder(checkResultPtr);
        return *pMethod;
    }
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_METHOD_H
