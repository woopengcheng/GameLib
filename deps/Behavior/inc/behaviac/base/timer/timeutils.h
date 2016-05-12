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

#ifndef BEHAVIAC_CORE_TIMEUTILS_H
#define BEHAVIAC_CORE_TIMEUTILS_H

#include <time.h>

namespace TimeUtils
{
    inline time_t TimeZoneOffset()
    {
        time_t dummy_time = 0;
        return mktime(localtime(&dummy_time)) - mktime(gmtime(&dummy_time));
    }

    inline time_t GMTToLocal(time_t gmt_time)
    {
        return gmt_time + TimeZoneOffset();
    }

    inline time_t LocalToGMT(time_t gmt_time)
    {
        return gmt_time - TimeZoneOffset();
    }

    inline tm GMTToLocal(const tm& gmt_tm)
    {
        tm tmp_tm = gmt_tm;
        time_t tmp_time = GMTToLocal(mktime(&tmp_tm));
        return *localtime(&tmp_time);
    }

    inline tm LocalToGMT(const tm& local_tm)
    {
        tm tmp_tm = local_tm;
        time_t tmp_time = LocalToGMT(mktime(&tmp_tm));
        return *localtime(&tmp_time);
    }

    inline void GetLocalTime(tm& local_tm)
    {
        time_t tmp = time(NULL);
        local_tm = *localtime(&tmp);
    }

    inline void GetGMTTime(tm& gmt_tm)
    {
        time_t tmp = time(NULL);
        gmt_tm = *gmtime(&tmp);
    }
}

#endif //BEHAVIAC_CORE_TIMEUTILS_H
