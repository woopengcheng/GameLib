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

#include "behaviac/base/timer/timer.h"
#include "behaviac/base/core/system.h"

BEHAVIAC_IMPLEMNT_SINGLETON(CTimer)

/////////////////////////////////////////////////////
CTimer::CTimer()
{
    m_bLockedFrameRate = false;
    m_iVirtualFrameRate = 30;
    m_iRealFrameRate = 30;
    m_pfnUpdate = NULL;
    m_BaseTime = 0;
    m_LastTime = 0;
    m_FrameTime = 1 / 30.0;
    m_FrameTimeNoSlowmo = m_FrameTime;
    m_CurrentTime = 0;
    m_CurrentTimeNoPauseTime = 0;
    m_maxDeltaTime = 1.f / 10.f; // by default we don't go below 10 fps
    m_gamePaused = false;
    m_gamePausedByCheat = false;
    m_ulFrameCounter = 0;
    m_slowmoFactor = 1.0f;
    m_iLockedFrameRateLastTime = 1;
}

CTimer::~CTimer()
{
}

//get frame rate
/////////////////////////////////////////////////////
float CTimer::GetFrameRate()
{
    float diff = GetFrameSysTime();

    if (diff > 0.f)
    {
        return (1.0f / diff);
    }

    return 0.f;
}

//update the timer
/////////////////////////////////////////////////////
void CTimer::Update()
{
    m_ulFrameCounter++;
    {
        double fLowLevelTime;
        double fCurrentTime;

        for (;;)   // we wait 1ms if we are calling update too fast
        {
            fLowLevelTime = double((*m_pfnUpdate)()); // be double as the rest of the code seems to prefer double p
            fCurrentTime = fLowLevelTime - m_BaseTime;

            if (!behaviac::IsEqualWithEpsilon(fCurrentTime, m_LastTime))
            {
                break;
            }

            //BEHAVIAC_ASSERT(0); // 2 update without low level timer diff
            behaviac::Sleep(1);
        }

        double deltaTime = behaviac::Min(fCurrentTime - m_LastTime, m_maxDeltaTime);
        BEHAVIAC_ASSERT(fCurrentTime > m_LastTime);
        m_LastTime = fCurrentTime;
        m_FrameTimeNoSlowmo = deltaTime;
        m_FrameTime = m_slowmoFactor * deltaTime;
        BEHAVIAC_ASSERT(m_FrameTime > 0.0f);
        m_CurrentTime += m_FrameTime;
    }

    if (!m_gamePaused && !m_gamePausedByCheat)
    {
        m_CurrentTimeNoPauseTime += m_FrameTime;
    }
}

float	CTimer::GetFrameGameTime() const
{
    float frameTime = float(m_FrameTime);

    if (m_gamePaused || m_gamePausedByCheat)
    {
        frameTime = 0.f;
    }

    return frameTime;
}

unsigned int CTimer::GetFrameID() const
{
    return m_ulFrameCounter;
}

//reset the timer
/////////////////////////////////////////////////////
void CTimer::Reset()
{
    m_BaseTime = (*m_pfnUpdate)();
    m_LastTime = m_CurrentTime = m_FrameTime = 0;
    m_FrameTimeNoSlowmo = m_FrameTime;
    m_ulFrameCounter = 0;
}

void CTimer::LockFrameRate(uint32_t virtualFrameRate, uint32_t realFrameRate)
{
    m_bLockedFrameRate = true;
    m_iVirtualFrameRate = virtualFrameRate;
    m_iRealFrameRate = realFrameRate;
}

void CTimer::UnlockFrameRate()
{
    m_bLockedFrameRate = false;
    m_LastTime = double((*m_pfnUpdate)()) - m_BaseTime;     // Reset the last time to the current time for the unlocked mode!
}
