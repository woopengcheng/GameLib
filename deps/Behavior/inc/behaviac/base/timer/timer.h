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

#ifndef BEHAVIAC_BASE_TIME_TIMER_H
#define BEHAVIAC_BASE_TIME_TIMER_H

#include "behaviac/base/base.h"
#include "behaviac/base/core/string/formatstring.h"
#include "behaviac/base/core/singleton.h"

// simple class to get access to a high performance timer
class BEHAVIAC_API CHighPerfTimer
{
public:
    static const uint64_t s_kfFrequency;
    static const behaviac::Float64 s_kfPeriod; // 1/frequency
    static uint64_t GetTimeValue(); // platform specific
    static behaviac::Float32 GetDeltaInSeconds(uint64_t timeInf, uint64_t timeSup)
    {
        return behaviac::Float32(behaviac::Float64(timeSup - timeInf) * s_kfPeriod);
    }
};

//////////////////////////////////////////////////////////////////////////
// the 2 following classes use CHighPerfTimer services to offer nicer
// services

// This class is a simple profiler start/stop class to profile using explicit method calls
class BEHAVIAC_API CTimerProfiler
{
public:
    void Start()
    {
        m_startTime = CHighPerfTimer::GetTimeValue();
    }

    // Stop() return the time elapsed in seconds since the last Start() call
    // Calling Stop() without previously Start return undefined value.
    behaviac::Float32 Stop()
    {
        return CHighPerfTimer::GetDeltaInSeconds(m_startTime, CHighPerfTimer::GetTimeValue());
    }

    // StopMS() return the time elapsed in milliseconds since the last Start() call
    // Calling Stop() without previously Start return undefined value.
    uint32_t StopMS()
    {
        return (uint32_t)(Stop() * 1000.0f);
    }

    // The original start time
    uint64_t GetStartTime()
    {
        return m_startTime;
    }

protected:
    uint64_t m_startTime;
};

// This class is used for automatic profiling of a section of the code.
// Create an instance of this class, and upon exiting from the code section
class BEHAVIAC_API CTimerAutoProfiler
{
public:
    CTimerAutoProfiler(behaviac::Float32& timeMeasure)
        : m_timeMeasure(timeMeasure),
          m_startTime(CHighPerfTimer::GetTimeValue())
    {
    }

    ~CTimerAutoProfiler()
    {
        m_timeMeasure += CHighPerfTimer::GetDeltaInSeconds(m_startTime, CHighPerfTimer::GetTimeValue());
    }

private:
    behaviac::Float32& 	  m_timeMeasure;
    const uint64_t    m_startTime;

    CTimerAutoProfiler& operator=(const CTimerAutoProfiler&);
};

class BEHAVIAC_API CTimerAutoProfilerLog
{
public:
    CTimerAutoProfilerLog(const char* tag) : m_timerTag(tag), m_startTime(CHighPerfTimer::GetTimeValue())
    {
    }

    ~CTimerAutoProfilerLog()
    {
        BEHAVIAC_LOGINFO("%s: %.5f seconds\n", m_timerTag, CHighPerfTimer::GetDeltaInSeconds(m_startTime, CHighPerfTimer::GetTimeValue()));
    }

private:
    const char*     m_timerTag;
    const uint64_t    m_startTime;
    CTimerAutoProfilerLog& operator=(const CTimerAutoProfilerLog&);
};

//! Implements all common timing routines
class BEHAVIAC_API CTimer
{
public:
    BEHAVIAC_DELCARE_SINGLETON(CTimer)
    BEHAVIAC_DECLARE_MEMORY_OPERATORS(CTimer);

public:
    CTimer();
    ~CTimer();

    bool Init();
    void Reset();
    void Update();

    void SetMaxDeltaTime(double maxDT)
    {
        m_maxDeltaTime = maxDT;
    }
    double GetMaxDeltaTime()
    {
        return m_maxDeltaTime;
    }

    void SetGamePaused(bool paused)
    {
        m_gamePaused = paused;
    }
    bool IsGamePaused() const
    {
        return m_gamePaused;
    }

    void SetGamePausedByCheat(bool paused)
    {
        m_gamePausedByCheat = paused;
    }
    bool IsGamePausedByCheat()
    {
        return m_gamePausedByCheat;
    }

    //Retrieve the current system time ( time since game started )
    inline float	GetSysTime() const
    {
        return float(m_CurrentTime);
    }

    inline float	GetFrameSysTime() const
    {
        return float(m_FrameTime);
    }

    inline float	GetFrameSysTimeNoSlowmo() const
    {
        return float(m_FrameTimeNoSlowmo);
    }

    //This is getting the systime from the clock
    inline float	GetAsyncSysTime()
    {
        return float((*m_pfnUpdate)() - m_BaseTime);
    }

    //Retrieve the current game time( this time should be use if you want to pause correctly )
    inline float GetGameTime() const
    {
        return float(m_CurrentTimeNoPauseTime);
    }

    float	GetFrameGameTime() const;

    float				GetFrameRate();

    // Call to get elapse time since start of the application
    // Return value is the total number of ms since start
    unsigned int		GetElapsedTime(int& hours, int& minutes, int& seconds);

    // return the frame id for the timer.
    unsigned int        GetFrameID() const;

    void				LockFrameRate(uint32_t virtualFrameRate, uint32_t realFrameRate);
    void				UnlockFrameRate();
    bool				IsLockedFrameRate() const
    {
        return m_bLockedFrameRate;
    }

    void SetSlowmoFactor(float t)
    {
        m_slowmoFactor = t;
    }
    float GetSlowmoFactor()
    {
        return (float)m_slowmoFactor;
    }

    //@}
private:
    unsigned int m_ulFrameCounter;

    typedef float(*TimeUpdateFunc)();
    TimeUpdateFunc m_pfnUpdate;

    double		m_BaseTime;
    double		m_LastTime;
    //double		m_SecsPerTick;

    double		m_CurrentTime;
    double		m_CurrentTimeNoPauseTime;
    double		m_FrameTime;
    double      m_FrameTimeNoSlowmo;
    bool        m_gamePaused;
    bool        m_gamePausedByCheat;

    double      m_slowmoFactor;

    double      m_maxDeltaTime;

    // locking
    bool	m_bLockedFrameRate;
    uint32_t	m_iVirtualFrameRate;
    uint32_t	m_iRealFrameRate;
    uint64_t   m_iLockedFrameRateLastTime;
};

#endif //BEHAVIAC_BASE_TIME_TIMER_H
