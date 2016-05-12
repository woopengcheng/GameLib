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

#ifndef _BEHAVIAC_BASE_OBJECT_TAGOBJECT_MACROS_H_
#define _BEHAVIAC_BASE_OBJECT_TAGOBJECT_MACROS_H_

#include "behaviac/base/object/member.h"

template<class ValueType>
ValueType CTagObject::GetMember(const behaviac::CStringID& nameId)
{
    const CTagObjectDescriptor& descriptor = GetDescriptor();
    CTagObjectDescriptor::MembersMap_t::const_iterator it = descriptor.ms_members.membersMap.find(nameId);

    if (it != descriptor.ms_members.membersMap.end())
    {
        const CMemberBase* member = it->second;

        BEHAVIAC_ASSERT(member->GetID() == nameId);

        int typeId = GetClassTypeNumberId<ValueType>();
        return *((ValueType*)member->Get(this, typeId));
    }

    BEHAVIAC_ASSERT(0, "CTagObject::GetMember, cannot find the parameter %s", nameId.LogStr().c_str());
    return ValueType();
}

template<class ValueType>
void CTagObject::SetMember(const behaviac::CStringID& nameId, const ValueType& value)
{
    const CTagObjectDescriptor& descriptor = GetDescriptor();

    CTagObjectDescriptor::MembersMap_t::const_iterator it = descriptor.ms_members.membersMap.find(nameId);

    if (it != descriptor.ms_members.membersMap.end())
    {
        CMemberBase* member = it->second;

        BEHAVIAC_ASSERT(member->GetID() == nameId);
        int typeId = GetClassTypeNumberId<ValueType>();
        member->Set(this, (void*)&value, typeId);
        return;
    }

    BEHAVIAC_ASSERT(0, "CTagObject::SetMember, cannot find the parameter %s", nameId.LogStr().c_str());
}

#endif // #ifndef _BEHAVIAC_BASE_OBJECT_TAGOBJECT_MACROS_H_
