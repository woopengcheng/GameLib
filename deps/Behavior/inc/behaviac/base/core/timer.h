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

#ifndef BEHAVIAC_BASE_TIMER_H
#define BEHAVIAC_BASE_TIMER_H

#include "behaviac/base/core/config.h"

namespace behaviac
{
    // Forward declaration
    struct TimerInformation;
    BEHAVIAC_API const TimerInformation& GetTimerInformation();

    struct TimerInformation
    {
        int64_t cpuFrequency;
        int64_t timerFrequency;
        float tickInSecond;
        float tickInMillisecond;
        float tickInMicrosecond;
        float tickInNanosecond;
        float tickInCycle;
    };

    struct TimeSpan
    {
        TimeSpan() {}

        BEHAVIAC_FORCEINLINE explicit TimeSpan(int64_t tick) : tick(tick) {}

        BEHAVIAC_FORCEINLINE uint64_t GetTick() const
        {
            return tick;
        }

        BEHAVIAC_FORCEINLINE float GetSecond() const
        {
            return tick * GetTimerInformation().tickInSecond;
        }

        BEHAVIAC_FORCEINLINE float GetMillisecond() const
        {
            return tick * GetTimerInformation().tickInMillisecond;
        }

        BEHAVIAC_FORCEINLINE float GetMicrosecond() const
        {
            return tick * GetTimerInformation().tickInMicrosecond;
        }

        BEHAVIAC_FORCEINLINE float GetNanosecond() const
        {
            return tick * GetTimerInformation().tickInNanosecond;
        }

        BEHAVIAC_FORCEINLINE float GetCycle() const
        {
            return tick * GetTimerInformation().tickInCycle;
        }

        int64_t tick;
    };

    struct Tick
    {
        BEHAVIAC_FORCEINLINE explicit Tick(int64_t t) : tick(t) {}

        friend BEHAVIAC_FORCEINLINE TimeSpan operator-(Tick end, Tick start)
        {
            return TimeSpan(end.tick - start.tick);
        }

        int64_t tick;
    };

    BEHAVIAC_API bool TimerStart();
    BEHAVIAC_API bool TimerStop();
    BEHAVIAC_API const TimerInformation& GetTimerInformation();
    BEHAVIAC_API Tick GetTick();
    BEHAVIAC_API int64_t ReadTSC();
} // namespace behaviac

#endif//BEHAVIAC_BASE_TIMER_H
