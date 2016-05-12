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

#include "behaviac/base/core/logging/log.h"

namespace behaviac
{
    uint32_t TraceManager::m_TagLogFilter = BEHAVIAC_LOG_PRIVATE_MASK;

    uint32_t TraceManager::m_TagLogLevel = BEHAVIAC_LOG_USER_DEFINED;

    void TraceManager::SetLoggingFilter(uint32_t NewFilter)
    {
        // Bits in BEHAVIAC_LOG_PRIVATE_MASK cannot be filtered
        m_TagLogFilter = NewFilter | BEHAVIAC_LOG_PRIVATE_MASK;
    }

    /// Returns the current logging Filter
    uint32_t  TraceManager::GetLoggingFilter(void)
    {
        return m_TagLogFilter;
    }

    /// This function is used by internal macros to determine if we should display on the console.
    /*! \param  Channel  Which channel you want to display on
    \return             Returns True if the filter lets you display on this channel
    or False if this message should be filtered.
    */
    bool TraceManager::CanLog(uint32_t Channel)
    {
        return (Channel & m_TagLogFilter) != 0;
    }

    /// This function is used by internal macros to determine if we should display on the console.
    /// If the message it displayed once, it will never be displayed again
    /*! \param  Channel  Which channel you want to display on
    \param  LogOnce  [I/O] A static value used to keep track if the message has been displayed
    \return             Returns True if the filter lets you display on this channel
    or False if this message should be filtered.
    */
    bool TraceManager::CanLogOnce(uint32_t Channel, uint8_t& LogOnce)
    {
        if ((Channel & m_TagLogFilter) && (LogOnce == 0))
        {
            LogOnce = 1;
            return true;
        }

        return false;
    }

    void TraceManager::SetLoggingLevel(ETagLogLevel NewLevel)
    {
        m_TagLogLevel = NewLevel;
    }

    bool TraceManager::CanLog(ETagLogLevel level)
    {
        return (uint32_t)level <= m_TagLogLevel;
    }
}
