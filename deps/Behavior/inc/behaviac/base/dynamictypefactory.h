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

#ifndef BEHAVIAC_ENGINESERVICES_DYNAMICTYPEFACTORY_H
#define BEHAVIAC_ENGINESERVICES_DYNAMICTYPEFACTORY_H

#include "behaviac/base/core/singleton.h"
#include "behaviac/base/core/factory.h"
#include "behaviac/base/dynamictype.h"

class BEHAVIAC_API CDynamicTypeFactory
{
public:
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CDynamicTypeFactory);

private:
    CDynamicTypeFactory();
    ~CDynamicTypeFactory();

    CFactory<CDynamicType> m_factory;

    BEHAVIAC_DELCARE_SINGLETON(CDynamicTypeFactory);

public:
    bool Register(const behaviac::CStringID& typeID, CFactory<CDynamicType>::InstantiateFunctionPointer instantiate);
    bool UnRegister(const behaviac::CStringID& typeID);

    // templated version assuming the parameters are accessible
    // by the static methods GetClassTypeId and CreateObject
    template<class T>
    bool Register()
    {
        return Register(T::GetClassTypeId(), &T::CreateObject);
    }

    template<class T>
    bool UnRegister()
    {
        return m_factory.UnRegister(T::GetClassTypeId());
    }

    // visitor function
    template<typename VISITOR>
    void Visit(VISITOR& visitor)
    {
        m_factory.Visit(visitor);
    }

    CDynamicType* Create(const behaviac::CStringID& typeID);
};

#endif // #ifndef BEHAVIAC_ENGINESERVICES_DYNAMICTYPEFACTORY_H
