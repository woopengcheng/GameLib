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

#include "behaviac/base/dynamictypefactory.h"

BEHAVIAC_IMPLEMNT_SINGLETON(CDynamicTypeFactory);

CDynamicTypeFactory::CDynamicTypeFactory()
{
}

CDynamicTypeFactory::~CDynamicTypeFactory()
{
}

bool CDynamicTypeFactory::Register(const behaviac::CStringID& typeID, CFactory<CDynamicType>::InstantiateFunctionPointer instantiate)
{
    return m_factory.Register(typeID, instantiate);
}

bool CDynamicTypeFactory::UnRegister(const behaviac::CStringID& typeID)
{
    return m_factory.UnRegister(typeID);
}

CDynamicType* CDynamicTypeFactory::Create(const behaviac::CStringID& typeID)
{
    return m_factory.CreateObject(typeID);
}
