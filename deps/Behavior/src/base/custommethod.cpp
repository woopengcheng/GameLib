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
#include "behaviac/agent/agent.h"
#include "behaviac/base/custommethod.h"

namespace behaviac
{
    CCustomMethod::~CCustomMethod()
    {
        //if (this->m_bCopy)
        //{
        //    for (behaviac::vector<behaviac::Property*>::iterator it = this->m_params.begin(); it != this->m_params.end(); ++it)
        //    {
        //        behaviac::Property* p = *it;

        //        BEHAVIAC_DELETE p;
        //    }
        //}

        this->m_params.clear();
    }

    Property* CCustomMethod::LoadFromXML(CTagObject* parent, const ISerializableNode& xmlNode, const char* typeName, const char* paramName)
    {
        BEHAVIAC_UNUSED_VAR(parent);

        CSerializationID id(paramName);
        const char* str = xmlNode.getAttrRaw(id);

        Property* pProperty = 0;

        if (str)
        {
            //str[0] = '"', then it is a string
            if (str[0] != '"' && strchr(str, ' '))
            {
                behaviac::vector<behaviac::string> tokens;
                behaviac::StringUtils::SplitIntoArray(str, " ", tokens);

                if (tokens.size() == 2)
                {
                    //int AgentTest::Property1
                    pProperty = behaviac::Property::Create(tokens[0].c_str(), tokens[1].c_str(), false, 0);

                }
                else
                {
                    //static int AgentTest::Property6
                    BEHAVIAC_ASSERT(tokens[0] == "static");
                    pProperty = behaviac::Property::Create(tokens[1].c_str(), tokens[2].c_str(), true, 0);
                }
            }
            else
            {
                //BEHAVIAC_UNUSED_VAR(parent);
                //GenericTypeHandler<T>::LoadFromXML(xmlNode, param, 0, paramName);
                pProperty = Property::Create(typeName, str);
            }
        }

        return pProperty;
    }

    void CCustomMethod::LoadFromXML(CTagObject* parent, const ISerializableNode& xmlNode)
    {
        for (uint32_t i = 0; i < this->m_paramTypes.size(); ++i)
        {
            const behaviac::string& paramTypeName = this->m_paramTypes[i];

            const char* paramName = FormatString("param%d", i + 1);
            Property* p = this->LoadFromXML(parent, xmlNode, paramTypeName.c_str(), paramName);

            this->m_params.push_back(p);
        }
    }

    void CCustomMethod::AddParams(const behaviac::vector<behaviac::string>& params)
    {
        this->m_paramTypes = params;
    }

    void CCustomMethod::AddParamType(const char* typeName)
    {
        this->m_paramTypes.push_back(typeName);
    }

    const char* CCustomMethod::GetClassNameString()
    {
        return m_className;
    }

    void CNamedEvent::SetFired(Agent* pAgent, bool bFired)
    {
        this->m_bFired = bFired;

        if (bFired)
        {
            pAgent->btonevent(this->GetName());
        }
    }
}
