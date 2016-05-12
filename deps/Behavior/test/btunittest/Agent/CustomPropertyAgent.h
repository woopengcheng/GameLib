#ifndef BTUNITEST_CUSTOMPROPERTYAGENT_H_
#define BTUNITEST_CUSTOMPROPERTYAGENT_H_
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

namespace UnityEngine
{
    struct Vector3
    {
        float x;
        float y;
        float z;

        Vector3() : x(0), y(0), z(0)
        {
        }

        DECLARE_BEHAVIAC_STRUCT(UnityEngine::Vector3);
    };
}

class CustomPropertyAgent : public behaviac::Agent
{
public:
    CustomPropertyAgent();
    virtual ~CustomPropertyAgent();

    DECLARE_BEHAVIAC_AGENT(CustomPropertyAgent, behaviac::Agent);

    void resetProperties()
    {
        IntProperty = 0;
        FloatPropertyReadonly = 0.0f;

        BoolMemberReadonly = false;

        StringMemberReadonly = "read only sting";
    }

    void init()
    {
        resetProperties();
    }

    void finl()
    {
    }

    int IntProperty;

    float FloatPropertyReadonly;

    bool BoolMemberReadonly;

    static int IntMemberConst;

    behaviac::string StringMemberReadonly ;

    UnityEngine::Vector3 Location;

    void FnWithOutParam(int& param)
    {
        param = 1;
    }
};

#endif//BTUNITEST_CUSTOMPROPERTYAGENT_H_
