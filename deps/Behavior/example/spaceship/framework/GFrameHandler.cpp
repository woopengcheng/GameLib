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

#include "GFrameHandler.h"
#include <SDL.h>

GFrameHandler::GFrameHandler()
{
	reset();
}

void GFrameHandler::reset()
{
    frame_count = 0,
    last_fps_time = SDL_GetTicks(),
    previous_time = last_fps_time,
    elapsed_time = 0,
    fps = 0,
    tpf = 0;
    tpf_limit = 20;
}

void GFrameHandler::set_fps_limit(int fps_)
{
    if (fps_ < 1)
    {
        return;
    }

    float sleep_interval = 1000.0f / (float)fps_;

    tpf_limit = (int)(sleep_interval + 0.495f);
}

void GFrameHandler::set_tpf_limit(int millisecond)
{
    tpf_limit = millisecond;
}

void GFrameHandler::limit_frame(void)
{
    tpf = SDL_GetTicks() - previous_time;

    if (tpf < tpf_limit)
    {
        SDL_Delay(tpf_limit - tpf - 1);
    }

    while (tpf < tpf_limit)
    {
        tpf = SDL_GetTicks() - previous_time;
    }
}

void GFrameHandler::limit_frame_fast(void)
{
    tpf = SDL_GetTicks() - previous_time;

    if (tpf < tpf_limit)
    {
        SDL_Delay(tpf_limit - tpf - 1);
    }
}

void GFrameHandler::calculate(void)
{
    unsigned int curr_time = SDL_GetTicks();

    frame_count++;

    if (curr_time - last_fps_time >= 1000)
    {
        fps = (int)(frame_count * (2.0f - (float)(curr_time - last_fps_time) * 0.001f)
                    + 0.495f);
        frame_count = 0;
        last_fps_time = curr_time;
    }

    elapsed_time = curr_time - previous_time;
    previous_time = curr_time;
}

int GFrameHandler::get_fps(void) const
{
    return fps;
}

int GFrameHandler::get_elapsed_time(void) const
{
    return elapsed_time;
}
