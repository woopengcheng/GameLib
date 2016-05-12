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
#include "behaviac/base/timer/timer.h"

#if !BEHAVIAC_COMPILER_MSVC

//////////////////////////////////////////////////////////////////////////
// CHighPerfTimer

static uint64_t GetTimeFrequency()
{
    return uint64_t(1);
}

const uint64_t CHighPerfTimer::s_kfFrequency = GetTimeFrequency();
const behaviac::Float64 CHighPerfTimer::s_kfPeriod = 1.0 / behaviac::Float64(GetTimeFrequency()); // 1/frequency

uint64_t CHighPerfTimer::GetTimeValue()
{
    BEHAVIAC_ASSERT(0);
    return uint64_t(0);
}

//////////////////////////////////////////////////////////////////////////
// Variables used by timers
//////////////////////////////////////////////////////////////////////////

struct SInternalTimerVariables
{
    SInternalTimerVariables()
        : m_curTime(0),
          m_timerStart(0),
          m_secsPerTick(0.f)
    {}
    int64_t m_curTime;
    int64_t m_timerStart;
    double m_secsPerTick;
};

static SInternalTimerVariables s_timerVar;

//init the timer
/////////////////////////////////////////////////////
bool CTimer::Init()
{
    BEHAVIAC_ASSERT(0);

    Reset();
    return false;
}

//get elapsed time
/////////////////////////////////////////////////////
unsigned int CTimer::GetElapsedTime(int& hours, int& minutes, int& seconds)
{
    BEHAVIAC_UNUSED_VAR(hours);
    BEHAVIAC_UNUSED_VAR(minutes);
    BEHAVIAC_UNUSED_VAR(seconds);

    BEHAVIAC_ASSERT(0);
    return 0;
}
#endif//#if !BEHAVIAC_COMPILER_MSVC
