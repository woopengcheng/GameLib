#ifndef BTUNITEST_PROPERTYREADONLYAGENT_H_
#define BTUNITEST_PROPERTYREADONLYAGENT_H_
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

#include "behaviac/base/base.h"
#include "behaviac/agent/agent.h"
#include "behaviac/agent/registermacros.h"

class PropertyReadonlyAgent : public behaviac::Agent
{
public:
    PropertyReadonlyAgent();
    virtual ~PropertyReadonlyAgent();

    DECLARE_BEHAVIAC_AGENT(PropertyReadonlyAgent, behaviac::Agent);

    void resetProperties()
    {
        _int_member = 0;
        _int_property_getteronly = 1;

        MemberReadonlyAs = 3;

        _s_float_member = 0.0f;
    }

    void init()
    {
        //base.Init();
        resetProperties();
    }

    void finl()
    {
    }

    //=====================================================
    int _int_member;

    const int& PropertyGetterSetter_get() const
    {
        return _int_member;
    }
    void PropertyGetterSetter_set(const int& value)
    {
        _int_member = value;
    }

    //=====================================================
    static	float _s_float_member;

    static const float& StaticPropertyGetterSetter_get()
    {
        return _s_float_member;
    }
    static void StaticPropertyGetterSetter_set(const float& value)
    {
        _s_float_member = value;
    }

    //=====================================================
    int _int_property_getteronly;

    const int& PropertyGetterOnly_get() const
    {
        return _int_property_getteronly;
    }

    //=====================================================
    const int MemberReadonly;

    //=====================================================
    int MemberReadonlyAs;

    //=====================================================

    void FnWithOutParam(int& param)
    {
        param = 4;
    }

    void PassInProperty(int param)
    {
        MemberReadonlyAs = param;
    }
};

#endif//BTUNITEST_PROPERTYREADONLYAGENT_H_
