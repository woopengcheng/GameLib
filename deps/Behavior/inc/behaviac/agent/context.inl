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

namespace behaviac
{
    /**
    if staticClassName is no null, it is for static variable
    */
    template<typename VariableType>
    BEHAVIAC_FORCEINLINE void Context::SetStaticVariable(const CMemberBase* pMember, const char* variableName, const VariableType& value, const char* staticClassName, uint32_t varableId)
    {
        BEHAVIAC_ASSERT(variableName && variableName[0] != '\0');
        BEHAVIAC_ASSERT(staticClassName);

        AgentTypeStaticVariables_t::iterator it = m_static_variables.find(staticClassName);

        if (it == m_static_variables.end())
        {
            m_static_variables[staticClassName] = Variables();
        }

        Variables& variables = m_static_variables[staticClassName];
        variables.Set(true, 0, false, pMember, variableName, value, varableId);
    }
}//namespace behaviac
