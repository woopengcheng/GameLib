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

#ifndef BEHAVIAC_CORE_FACTORY_H
#define BEHAVIAC_CORE_FACTORY_H

#include "behaviac/base/core/config.h"

#include "behaviac/base/core/thread/mutex.h"
#include "behaviac/base/core/string/stringid.h"
#include "behaviac/base/core/string/formatstring.h"
#include "behaviac/base/core/logging/log.h"

#include <vector>
#include <algorithm>

struct SFactoryBucket
{
	SFactoryBucket(const behaviac::CStringID& typeID, void* typeConstructor) : m_typeID(typeID), m_typeConstructor(typeConstructor) {}

	bool operator==(const SFactoryBucket& rhs)const
	{
		return m_typeID == rhs.m_typeID;
	}

	behaviac::CStringID		m_typeID;
	void*           m_typeConstructor;
};

struct FactoryContainer
{
	std::vector<SFactoryBucket>						m_vector;
	typedef std::vector<SFactoryBucket>::iterator	iterator;

	behaviac::Mutex										m_mutex;

	iterator begin()
	{
		return m_vector.begin();
	}

	iterator end()
	{
		return m_vector.end();
	}

	void push_back(SFactoryBucket it)
	{
		m_vector.push_back(it);
	}

	void erase(iterator it)
	{
		m_vector.erase(it);
	}

	void Lock()
	{
		m_mutex.Lock();
	}

	void Unlock()
	{
		m_mutex.Unlock();
	}
};

typedef FactoryContainer::iterator CreatorIt;

BEHAVIAC_API bool FactoryNewRegisterSub(FactoryContainer* creators, const behaviac::CStringID& typeID, void* typeConstructor);
BEHAVIAC_API bool FactoryNewUnregisterSub(FactoryContainer* creators, const behaviac::CStringID& typeID);

template< typename T >
class CFactory
{
	//Internal struct used in the container
	class IConstructType
	{
	public:
		virtual ~IConstructType() {}
		virtual T* Create() = 0;
	};

	template< typename FINAL_TYPE >
	class CConstructType : public CFactory< T >::IConstructType
	{
	public:
		virtual T* Create()
		{
			return BEHAVIAC_NEW FINAL_TYPE;
		}
	};

public:
	virtual ~CFactory()
	{
		CreatorIt it(m_creators.begin());
		CreatorIt itEnd(m_creators.end());

		while (it != itEnd)
		{
			SFactoryBucket& item = *it++;
			BEHAVIAC_FREE(item.m_typeConstructor);
		}

		m_creators.m_vector.clear();
	}

	//Same but with a given allocator
	virtual T* CreateObject(const behaviac::CStringID& typeID);

	typedef T* (*InstantiateFunctionPointer)(const behaviac::CStringID& typeID);

	bool Register(const behaviac::CStringID& typeID, InstantiateFunctionPointer instantiate, bool overwrite = false);

	//Unregister a type in the factory
	bool UnRegister(const behaviac::CStringID& typeID);

	//Register a new type in the factory
	template< typename FINAL_TYPE >
	bool Register()
	{
		typedef CConstructType<FINAL_TYPE> FinalType;
		void* p = BEHAVIAC_MALLOC(sizeof(FinalType));
		IConstructType* typeConstructor = new(p)FinalType;
		return FactoryNewRegisterSub(&m_creators, FINAL_TYPE::GetClassTypeId(), typeConstructor);
	}

	//Unregister a type in the factory
	template< typename FINAL_TYPE >
	bool UnRegister()
	{
		return FactoryNewUnregisterSub(&m_creators, FINAL_TYPE::GetClassTypeId());
	}

	bool IsRegistered(const behaviac::CStringID& typeID);

	// visitor function
	template<typename VISITOR>
	void Visit(VISITOR& visitor)
	{
		m_creators.Lock();
		CreatorIt it(m_creators.begin());
		CreatorIt itEnd(m_creators.end());

		while (it != itEnd)
		{
			visitor.Visit((*it).m_typeID);
			++it;
		}

		m_creators.Unlock();
	}

private:
	FactoryContainer m_creators;	//Container of registered type.
};

template< typename T >
BEHAVIAC_FORCEINLINE bool CFactory<T>::IsRegistered(const behaviac::CStringID& typeID)
{
	m_creators.Lock();
	//T* newObject = NULL;
	SFactoryBucket bucket(typeID, NULL);
	CreatorIt itEnd(m_creators.end());
	CreatorIt itFound(std::find(m_creators.begin(), itEnd, bucket));

	//We have a registered object, create it
	if (itFound != itEnd)
	{
		// we are done with the list, no need to keep critical section during alloc.
		m_creators.Unlock();
		return true;

	}
	else
	{
		m_creators.Unlock();
		return false;
	}
}

template< typename T >
T* CFactory<T>::CreateObject(const behaviac::CStringID& typeID)
{
	m_creators.Lock();
	//T* newObject = NULL;
	SFactoryBucket bucket(typeID, NULL);
	CreatorIt itEnd(m_creators.end());
	CreatorIt itFound(std::find(m_creators.begin(), itEnd, bucket));

	//We have a registered object, create it
	if (itFound != itEnd)
	{
		IConstructType* contructType = (IConstructType*)(*itFound).m_typeConstructor;
		// we are done with the list, no need to keep critical section during alloc.
		m_creators.Unlock();
		//Call the function that creates the abstract object
		return contructType->Create();

	}
	else
	{
#if STRINGID_USESTRINGCONTENT
		BEHAVIAC_LOGWARNING("Trying to create an unregister type 0x%08X -- %s", typeID.GetUniqueID(), typeID.c_str());
#else
		BEHAVIAC_LOGWARNING("Trying to create an unregister type 0x%08X", typeID.GetUniqueID());
#endif
		m_creators.Unlock();
		return NULL;
	}
}

template< typename T >
bool CFactory<T>::Register(const behaviac::CStringID& typeID, InstantiateFunctionPointer instantiate, bool overwrite)
{
	m_creators.Lock();
	//you should give a valid pointer to an instantiation function
	BEHAVIAC_ASSERT(instantiate);
	SFactoryBucket bucket(typeID, (void*)instantiate);
	CreatorIt itEnd(m_creators.end());
	CreatorIt itFound(std::find(m_creators.begin(), itEnd, bucket));
	bool wasThere = (itFound != itEnd);

	//Add it only once
	if (!wasThere)
	{
		m_creators.push_back(bucket);

	}
	else if (overwrite)
	{
		*itFound = bucket;
		m_creators.Unlock();
		return true;

	}
	else
	{
#if STRINGID_USESTRINGCONTENT
		BEHAVIAC_ASSERT(0, "Trying to register an already registered type %d -- %s", typeID.GetUniqueID(), typeID.c_str());
#else
		BEHAVIAC_ASSERT(0, "Trying to register an already registered type %d", typeID.GetUniqueID());
#endif // #if STRINGID_USESTRINGCONTENT
	}

	m_creators.Unlock();
	return !wasThere;
}

template< typename T >
bool CFactory<T>::UnRegister(const behaviac::CStringID& typeID)
{
	m_creators.Lock();
	SFactoryBucket bucket(typeID, NULL);
	CreatorIt itEnd(m_creators.end());
	CreatorIt itFound(std::find(m_creators.begin(), itEnd, bucket));

	if (itFound != itEnd)
	{
		m_creators.erase(itFound);
		m_creators.Unlock();
		return true;
	}

	BEHAVIAC_ASSERT("Cannot find the specified factory entry\n");
	m_creators.Unlock();
	return false;
}

#endif //BEHAVIAC_CORE_FACTORY_H
