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
#include "behaviac/base/object/method.h"

#if BEHAVIAC_ENABLE_NETWORKD
CMethodBase& CMethodBase::NETROLE(behaviac::NetworkRole netRole)
{
    this->m_netRole = netRole;

    if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
    {
        behaviac::Network* pNw = behaviac::Network::GetInstance();

        if (pNw && !pNw->IsSinglePlayer())
        {
            behaviac::string nameTemp;

            if (this->m_className)
            {
                nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

            }
            else
            {
                nameTemp = this->m_propertyName;
            }

            pNw->RegisterRemoteEvent(this->m_netRole, nameTemp.c_str());
        }
    }

    return *this;
}

void CMethodBase::SubsribeToNetwork(behaviac::Agent* pAgent)
{
    if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
    {
        behaviac::Network* pNw = behaviac::Network::GetInstance();

        if (pNw && !pNw->IsSinglePlayer())
        {
            behaviac::string nameTemp;

            if (this->m_className)
            {
                nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

            }
            else
            {
                nameTemp = this->m_propertyName;
            }

            pNw->BindToEvent(this->m_netRole, nameTemp.c_str(), pAgent, this);
        }
    }
}

void CMethodBase::UnSubsribeToNetwork(behaviac::Agent* pAgent)
{
    if (this->m_netRole != behaviac::NET_ROLE_DEFAULT)
    {
        behaviac::Network* pNw = behaviac::Network::GetInstance();

        if (pNw && !pNw->IsSinglePlayer())
        {
            behaviac::string nameTemp;

            if (this->m_className)
            {
                nameTemp = FormatString("%s::%s", this->m_className, this->m_propertyName);

            }
            else
            {
                nameTemp = this->m_propertyName;
            }

            pNw->UnBindToEvent(this->m_netRole, nameTemp.c_str(), pAgent);
        }
    }
}
#endif//#if BEHAVIAC_ENABLE_NETWORKD
