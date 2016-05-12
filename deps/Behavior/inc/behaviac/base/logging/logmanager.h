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

#ifndef BEHAVIAC_BASE_LOGGING_H
#define BEHAVIAC_BASE_LOGGING_H

#include "behaviac/base/base.h"

#include "behaviac/base/core/container/map_t.h"
#include "behaviac/base/core/logging/log.h"
#include "behaviac/base/core/singleton.h"
#include "behaviac/base/socket/socketconnect.h"

#include <map>

namespace behaviac
{
    class Agent;
    class BehaviorTask;

    /**
    indicating the action result for enter/exit
    */
    enum EActionResult
    {
        EAR_none = 0,
        EAR_success = 1,
        EAR_failure = 2,
        EAR_all = EAR_success | EAR_failure
    };

    enum LogMode
    {
        ELM_tick,
        ELM_breaked,
        ELM_continue,
        ELM_jump,
        ELM_return,

        ELM_log
    };

    class BEHAVIAC_API LogManager
    {
    public:
        /**
        by default, the log file is _behaviac_$_.log in the current path.

        you can call this function to specify where to log
        */
        void SetLogFilePath(const char* logFilePath);


        //action
        void Log(const behaviac::Agent* pAgent, const char* btMsg, behaviac::EActionResult actionResult, behaviac::LogMode mode = behaviac::ELM_tick);

        //property
        void Log(const behaviac::Agent* pAgent, const char* typeName, const char* varName, const char* value);

        //profiler
        void Log(const behaviac::Agent* pAgent, const char* btMsg, long time);

        //mode
        void Log(behaviac::LogMode mode, const char* filterString, const char* format, ...);

        void Log(const char* format, ...);

		void LogWorkspace(bool bSend, const char* format, ...);

        template<typename T>
        void LogVarValue(Agent* pAgent, const behaviac::string& name, const T& value);

        void Warning(const char* format, ...);

        void Error(const char* format, ...);

        void Flush(const behaviac::Agent* pAgent);
    protected:
        LogManager();
    public:
        virtual ~LogManager();

        static LogManager* GetInstance();
		static void Cleanup();
    private:
        FILE* GetFile(const behaviac::Agent* pAgent);

		//you can override this to customize the output
        virtual void Output(const behaviac::Agent* pAgent, const char* msg);

        typedef behaviac::map<int, FILE*> Logs_t;
        Logs_t				m_logs;
        const char*			m_logFilePath;
        bool				m_bFlush;
        static LogManager*	ms_instance;
    };
}//namespace behaviac

#endif//BEHAVIAC_BASE_LOGGING_H
