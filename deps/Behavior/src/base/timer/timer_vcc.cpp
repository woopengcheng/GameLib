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

#if BEHAVIAC_COMPILER_MSVC
#include <windows.h>

//////////////////////////////////////////////////////////////////////////
// CHighPerfTimer

static uint64_t GetTimeFrequency()
{
    LARGE_INTEGER l;

    if (!QueryPerformanceFrequency(&l))
    {
        BEHAVIAC_ASSERT(false);
    }

    return uint64_t(l.QuadPart);
}

const uint64_t CHighPerfTimer::s_kfFrequency = GetTimeFrequency();
const behaviac::Float64 CHighPerfTimer::s_kfPeriod = 1.0 / behaviac::Float64(GetTimeFrequency()); // 1/frequency

uint64_t CHighPerfTimer::GetTimeValue()
{
    LARGE_INTEGER l;
    QueryPerformanceCounter(&l);
    return uint64_t(l.QuadPart);
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

static uint32_t s_appLaunchTickCount = GetTickCount();  // s_appLaunchTickCount is the tick count since the launch of the application

//get time from multimedia timer
/////////////////////////////////////////////////////
static float GetMMTime()
{
    s_timerVar.m_curTime = ::GetTickCount();
    return ((float)(s_timerVar.m_secsPerTick * (s_timerVar.m_curTime - s_timerVar.m_timerStart)));
}

//get time from performance counter
/////////////////////////////////////////////////////
static float GetPerformanceCounterTime()
{
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&s_timerVar.m_curTime));
    float res = ((float)(s_timerVar.m_secsPerTick * (s_timerVar.m_curTime - s_timerVar.m_timerStart)));

    if (res < 0.f)
    {
        res = 0.f;
    }

    return res;
}

//init the timer
/////////////////////////////////////////////////////
bool CTimer::Init()
{
    LARGE_INTEGER TTicksPerSec;

    if (QueryPerformanceFrequency(&TTicksPerSec))
    {
        // performance counter is available, use it instead of multimedia timer
        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);
        memcpy(&s_timerVar.m_timerStart, &t, sizeof(s_timerVar.m_timerStart));
        s_timerVar.m_secsPerTick = double(1.0 / double(TTicksPerSec.QuadPart));
        m_pfnUpdate = &GetPerformanceCounterTime;

    }
    else
    {
        //Use MM timer if unable to use the High Frequency timer
        s_timerVar.m_timerStart = GetTickCount();
        //m_SecsPerTick = 1.0 / 1000.0;
        m_pfnUpdate = &GetMMTime;
    }

    Reset();
    return true;
}

//get elapsed time
/////////////////////////////////////////////////////
unsigned int CTimer::GetElapsedTime(int& hours, int& minutes, int& seconds)
{
    uint32_t tickCount = GetTickCount() - s_appLaunchTickCount;
    hours = ((tickCount / 1000) / 60) / 60;
    minutes = ((tickCount - (hours * 60 * 60 * 1000)) / 1000) / 60;
    seconds = (tickCount - (hours * 60 * 60 * 1000) - (minutes * 60 * 1000)) / 1000;
    return tickCount;
}

#endif//#if BEHAVIAC_COMPILER_MSVC
