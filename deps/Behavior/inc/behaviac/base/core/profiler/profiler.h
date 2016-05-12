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

#ifndef BEHAVIAC_CORE_BASE_PROFILER_H
#define BEHAVIAC_CORE_BASE_PROFILER_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"
#include "behaviac/base/core/container/string_t.h"
#include "behaviac/base/core/container/vector_t.h"
#include "behaviac/base/core/container/map_t.h"
#include "behaviac/base/string/stringutils.h"

#include "behaviac/base/workspace.h"

namespace behaviac
{
    class Agent;
    class ProfilerBlock;

    /// Hierarchical performance profiler subsystem.
    class BEHAVIAC_API Profiler
    {
    private:
        static Profiler* ms_instance;
    public:
        static Profiler* GetInstance();
        static Profiler* CreateInstance();
        static void DestroyInstance();

    public:
        BEHAVIAC_DECLARE_MEMORY_OPERATORS(Profiler);
        /// Construct.
        Profiler();
        /// Destruct.
        virtual ~Profiler();

        /// some profiler block is only for debug and support, which can be set to output or not
        void SetOutputDebugBlock(bool bOutput);

        /// begin timing a profiling block.
        void BeginBlock(const char* name, const behaviac::Agent* agent = 0, bool bDebugBlock = false);

        /// end timing the current profiling block.
        void EndBlock(bool bSend = false);

        /// begin the profiling frame. Called by HandleBeginFrame().
        void BeginFrame();

        /// end the profiling frame. Called by HandleEndFrame().
        void EndFrame();

        /// begin a new interval.
        void BeginInterval();

        void SetHierarchy(bool bHierarchy);

        /// Return profiling data as text output.
        behaviac::string GetData(bool showUnused = false, bool showTotal = false, unsigned maxDepth = 10000) const;

        /// Return the current profiling block.
        const ProfilerBlock* GetCurrentBlock() const;
        /// Return the root profiling block.
        const ProfilerBlock* GetRootBlock() const;

    private:
        /// Return profiling data as text output for a specified profiling block.
        void GetData(const ProfilerBlock* block, behaviac::string& output, unsigned depth, unsigned maxDepth, bool showUnused, bool showTotal) const;

        struct ThreadProfilerBlock_t
        {
            behaviac::THREAD_ID_TYPE		threadId;
            ProfilerBlock*					block;
            ProfilerBlock*					root;

            const static int kMaxBlockDepth = 100;
            ProfilerBlock*					m_currentStack[kMaxBlockDepth];
            int								m_currentIndex;

            ThreadProfilerBlock_t() : threadId(0), block(0), root(0), m_currentIndex(0)
            {
            }

            void clear()
            {
                threadId = 0;
                block = 0;
                root = 0;
                m_currentIndex = 0;
            }
        };

        const static int kMaxThreads = 32;

        /// Current profiling block.
        ThreadProfilerBlock_t				current_[kMaxThreads];
        int									threads_;

        int									frameStarted_;

        bool								outputDebugBlock_;
        bool								m_bHierarchy;

        /// Frames in the current interval.
        unsigned intervalFrames_;
        /// Total frames.
        unsigned totalFrames_;
    };

    /// Helper class for automatically beginning and ending a profiling block
    class BEHAVIAC_API AutoProfileBlock
    {
    public:
        void setProfiler(Profiler* profiler)
        {
            this->profiler_ = profiler;
        }

        /// Construct. begin a profiling block with the specified name and optional call count.
        AutoProfileBlock(Profiler* profiler, const char* name, bool bDebugBlock);

        /// Construct. begin a profiling block with the specified name and optional call count.
        AutoProfileBlock(Profiler* profiler, const behaviac::string& name);

        /// Destruct. end the profiling block.
        ~AutoProfileBlock();

    private:
        /// Profiler.
        Profiler* profiler_;
    };
}

//#define BEHAVIAC_PROFILE_V2(name, bDebugBlock) behaviac::AutoProfileBlock BEHAVIAC_UNIQUE_NAME(_profile) (behaviac::Profiler::GetInstance(), name, bDebugBlock)
//#define BEHAVIAC_PROFILE_V1(name) BEHAVIAC_PROFILE_V2(name, false)
//
//#define BEHAVIAC_PROFILE(...) ARGUMENT_SELECTOR3((__VA_ARGS__, BEHAVIAC_PROFILE_V2, BEHAVIAC_PROFILE_V1))(__VA_ARGS__)
//#define ARGUMENT_SELECTOR3(__args) GET_3TH_ARGUMENT __args
//#define GET_3TH_ARGUMENT(__p1,__p2,__n, ...) __n

#define BEHAVIAC_PROFILE_DEBUGBLOCK(name, bDebugBlock) behaviac::AutoProfileBlock BEHAVIAC_UNIQUE_NAME(_profile) (behaviac::Profiler::GetInstance(), name, bDebugBlock)

#define BEHAVIAC_PROFILE(name) BEHAVIAC_PROFILE_DEBUGBLOCK(name, false)

#endif//BEHAVIAC_CORE_BASE_PROFILER_H
