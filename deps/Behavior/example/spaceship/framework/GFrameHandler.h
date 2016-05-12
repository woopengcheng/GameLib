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

#ifndef GFRAMEHANDLER_H
#define GFRAMEHANDLER_H

class GFrameHandler
{
private:
    int frame_count;
    int last_fps_time;
    int previous_time;
    int elapsed_time;
    int fps;
    int tpf_limit;
    int tpf;

public:
    GFrameHandler();

    /*
    Reset all stored variables. If GFrameHandler is instantiated much earlier
    before main loop, call this function right before main loop.
    */
    void reset(void);

    /*
    Set maximum FPS( frames per second) limit.
    */
    void set_fps_limit(int fps);

    /*
    Set the maximum TPF( time per frame ) limit. This is basically same with
    set_fps_limit(int) but with better better control. FPS  = 1/TPF(in second).
    */
    void set_tpf_limit(int millisecond);

    /*
    Call this function in each cycle to limit the frame time. The default frame
    time limit is 20 millisecond, use set_fps_limit(int) or set_tpf_limit(int)
    for different frame rate locking. *Highly recommended to have this function
    call right before calculate().
    */
    void limit_frame(void);

    /*
    This has the same functionality with limit_fps() but could release slightly
    more CPU time when it is possible, the trade off is lower accuracy. *Highly
    recommended to have this function call right before calculate().
    */
    void limit_frame_fast(void);

    /*
    Calculate current FPS and frame time. It is recommended to call this member
    function at the end at the cycle.
    */
    void calculate(void);

    /*
    Get current  FPS.
    */
    int get_fps(void) const;

    /*
    Get elapsed time since previous frame ( current frame time ) in millisecond.
    */
    int get_elapsed_time(void) const;
};

#endif
