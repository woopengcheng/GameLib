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

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"
#include "behaviac/base/core/container/string_t.h"
#include "behaviac/base/core/string/formatstring.h"
#include "behaviac/base/string/stringutils.h"

#include "behaviac/base/core/logging/log.h"
#include "behaviac/base/core/logging/consoleout.h"
#include "behaviac/base/core/system.h"

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>

#if BEHAVIAC_COMPILER_MSVC
#include <windows.h>
#endif//BEHAVIAC_COMPILER_MSVC

#include <new>

#if BEHAVIAC_COMPILER_ANDROID
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "libbehaviac"
#define  LOGI(str)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, str)
#else
#define  LOGI(str)  printf("%s", str)
#endif//#if BEHAVIAC_COMPILER_ANDROID

namespace behaviac
{
    namespace Private
    {
        // This class is solely intended to discard the calls to Lock() and Unlock()
        // while behaviac's initialization is not complete.
        class SafeLock
        {
        public:
            SafeLock()
                : m_tagIsInitialized(false)
                , m_tagInitializationSignal(false)
            {
            }

            void Lock()
            {
                // We update the value of tagIsInitialized only in Lock()
                // to ensure we won't unlock a mutex that has never been locked,
                // if the initialization is signaled between a Lock() and an Unlock().
                if (!m_tagIsInitialized)
                {
                    m_tagIsInitialized = m_tagInitializationSignal;
                }

                if (m_tagIsInitialized)
                {
                    m_mutex.Lock();
                }
            }

            void Unlock()
            {
                if (m_tagIsInitialized)
                {
                    m_mutex.Unlock();
                }
            }

            void SetInitializationSignal()
            {
                m_tagInitializationSignal = true;
            }

        private:
            Mutex m_mutex;
            bool m_tagIsInitialized;
            bool m_tagInitializationSignal;
        };
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool                            ConsoleOut::IsInit = false;
	uint32_t                        ConsoleOut::EnableMask = ELOG_VCOUTPUT | ELOG_FILE
#if BEHAVIAC_COMPILER_ANDROID
		| ELOG_CONSOLE
#endif
		;

    static Private::SafeLock*		gs_lock;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::TestInit(void)
    {
        if (!IsInit)
        {
            IsInit = true;
            static char s_buffer[sizeof(Private::SafeLock)];
            gs_lock = new(s_buffer)Private::SafeLock();
            gs_lock->SetInitializationSignal();
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::PrintLines(uint32_t Filter, const char* pStr)
    {
        BEHAVIAC_UNUSED_VAR(Filter);

        TestInit();

        const int kMaxStringLength = 2048;
        int count = 0;
        char line[kMaxStringLength];

        while (*pStr != '\0' && *pStr != '\n')
        {
            line[count++] = *pStr++;
        }

        if (*pStr == '\n')
        {
            line[count++] = *pStr++;
        }

        line[count++] = '\0';

        BEHAVIAC_ASSERT(count < kMaxStringLength);
        count = 0;

        //the first line
		OutputDecoratedLine(Filter, line);

        while (*pStr != '\0')
        {
            while (*pStr != '\0' && *pStr != '\n')
            {
                line[count++] = *pStr++;
            }

            if (*pStr == '\n')
            {
                line[count++] = *pStr++;
            }

            line[count++] = '\0';

            BEHAVIAC_ASSERT(count < kMaxStringLength);
            count = 0;

            OutputLine(line);
        }
    }

    void ConsoleOut::PrintLines(const char* pStr)
    {
        PrintLines(BEHAVIAC_LOG_INFO, pStr);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::Print(uint32_t Filter, const char* Format, ...)
    {
        va_list ArgList;
        va_start(ArgList, Format);
        VPrint(Filter, Format, ArgList);
        va_end(ArgList);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::Print(const char* Format, ...)
    {
        va_list ArgList;
        va_start(ArgList, Format);
        VPrint(Format, ArgList);
        va_end(ArgList);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::VPrint(const char* Format, va_list& argList)
    {
        VPrint(BEHAVIAC_LOG_INFO, Format, argList);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::VPrint(uint32_t Filter, const char* Format, va_list& argList)
    {
        TestInit();
        char Buffer[4096];
        {
            //weirdly, it is found, sometime it crashed in vsnprintf
            //so, to protect it for the multithreading
            gs_lock->Lock();
            string_vnprintf(Buffer, sizeof(Buffer), Format, argList);
            gs_lock->Unlock();
        }
        Output(Filter, Buffer);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::Dump(uint32_t Filter, const void* pUserData, size_t UserSize)
    {
        TestInit();

        if ((pUserData) && (UserSize > 0))
        {
            char        Str[80];
            size_t		i;
            uint8_t*    pData;
            size_t		Size;
            size_t		StartingOfs;
            size_t		EndingOfs;
            pData = BEHAVIAC_ALIGN_PTR(uint8_t*, pUserData, 16);
            StartingOfs = (size_t)BEHAVIAC_DIFF_PTR(pUserData, pData);
            EndingOfs = StartingOfs + UserSize;
            Size = BEHAVIAC_ROUND(EndingOfs, 16);

            for (i = 0; i < (Size / 16); i++)
            {
                memset(Str, ' ', sizeof(Str));
                string_snprintf(Str, sizeof(Str), "%p", pData + i * 16);
                Str[8] = ':';

                for (size_t j = 0; j < 16; j++)
                {
                    size_t Ofs = i * 16 + j;
                    size_t Index = (j * 3) + ((j > 7) ? 11 : 10);

                    if ((Ofs >= StartingOfs) && (Ofs < EndingOfs))
                    {
                        string_snprintf(Str + Index, sizeof(Str) - Index, "%02X", pData[Ofs]);
                        Str[Index + 2] = ' ';
                        //Str[60+j] = Str::IsPrintable(pData[Ofs]) ? pData[Ofs] : '.';
                        Str[60 + j] = pData[Ofs];
                    }
                }

                Str[76] = '\n';
                Str[77] = 0;
                Output(Filter, Str);
            } // for(i = 0; i < (Size / 16) ; i++)
        } // if((pData) && (Size > 0))
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::Output(uint32_t LogFilter, const char* pStr)
    {
        BEHAVIAC_UNUSED_VAR(LogFilter);
        BEHAVIAC_UNUSED_VAR(pStr);

        //you can call ConsoleOut::SetEnableMask(0) to disable the output
        if (EnableMask != 0)
        {
			OutputDecoratedLine(LogFilter, pStr);
        }
    }

    void ConsoleOut::OutputLine(const char* temp)
    {
        gs_lock->Lock();

#if BEHAVIAC_COMPILER_MSVC
#if _MSC_VER >= 1500
        static BOOL s_debugger = IsDebuggerPresent();
#else
        static BOOL s_debugger = false;
#endif//#if _MSC_VER >= 1500

        if (s_debugger && (EnableMask & ELOG_VCOUTPUT))
        {
            OutputDebugString(temp);
        }

#endif//

        //to console window
        if (EnableMask & ELOG_CONSOLE)
        {
            LOGI(temp);
        }

        if (EnableMask & ELOG_FILE)
        {
            static FILE* s_file = 0;

            if (!s_file)
            {
                s_file = fopen("_behaviac_$_$_.log", "wt");

                if (s_file)
                {
                    behaviac::THREAD_ID_TYPE threadId = behaviac::GetTID();

                    time_t tTime = time(NULL);
                    tm* ptmCurrent = localtime(&tTime);

                    behaviac::string buffer = FormatString("[behaviac][%05d][thread %04d]CREATED ON %d-%.2d-%.2d\n\n", 0, threadId, ptmCurrent->tm_year + 1900, ptmCurrent->tm_mon + 1, ptmCurrent->tm_mday);

                    fwrite(buffer.c_str(), 1, buffer.size(), s_file);
                }
            }

            if (s_file)
            {
                fwrite(temp, 1, strlen(temp), s_file);
                fflush(s_file);
            }
        }

        gs_lock->Unlock();
    }

	static const char* GetLogFilterString(uint32_t LogFilter)
	{
		const char* szStr = "NONE";

		if (LogFilter == BEHAVIAC_LOG_INFO)
		{ 
			szStr = "INFO";
		}
		else if (LogFilter == BEHAVIAC_LOG_MSG)
		{
			szStr = "MSG";
		}
		else if (LogFilter == BEHAVIAC_LOG_WARNING)
		{
			szStr = "WARN";
		}
		else if (LogFilter == BEHAVIAC_LOG_ERROR)
		{
			szStr = "ERROR";
		}
		else if (LogFilter == BEHAVIAC_LOG_CRITICAL)
		{
			szStr = "FATAL";
		}
		else
		{
			BEHAVIAC_ASSERT(true);
		}

		return szStr;
	}

	void ConsoleOut::OutputDecoratedLine(uint32_t LogFilter, const char* pStr)
    {
        behaviac::THREAD_ID_TYPE threadId = behaviac::GetTID();
        time_t tTime = time(NULL);
        tm* ptmCurrent = localtime(&tTime);

		const char* LogFilterStr = GetLogFilterString(LogFilter);

        char szTime[64];
        string_snprintf(szTime, sizeof(szTime) - 1,
                        "%.2d:%.2d:%.2d",
                        ptmCurrent->tm_hour, ptmCurrent->tm_min, ptmCurrent->tm_sec);
        static int s_index = 0;
        int index = s_index++;
        const int kMaxStringLength = 2048;
        char temp[kMaxStringLength];
#if BEHAVIAC_COMPILER_MSVC
        string_snprintf(temp, kMaxStringLength, "[behaviac][%05d][thread %04d][%s][%s]%s", index, (int)threadId, szTime, LogFilterStr, pStr);
#else
		string_snprintf(temp, kMaxStringLength, "[behaviac][%05d][thread %p][%s][%s]%s", index, threadId, szTime, LogFilterStr, pStr);
#endif
        temp[kMaxStringLength - 1] = '\0';

        OutputLine(temp);
    }

    void ConsoleOut::SetEnableMask(uint32_t enableMask)
    {
        EnableMask = enableMask;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::InitializationCompleted(void)
    {
        TestInit();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::DefaultConsoleOutput(uint32_t Filter, const char* pMsg)
    {
        BEHAVIAC_UNUSED_VAR(Filter);
        BEHAVIAC_UNUSED_VAR(pMsg);

        // Filter private channels; Keep only message that are broadcasted
        if (Filter & BEHAVIAC_LOG_BROADCAST_MASK)
        {
            //NativeDebugOutput(pMsg);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ConsoleOut::DefaultStdConsoleOutput(uint32_t Filter, const char* pMsg)
    {
        BEHAVIAC_UNUSED_VAR(Filter);
        BEHAVIAC_UNUSED_VAR(pMsg);

        // Filter private channels; Keep only message that are broadcasted
        if (Filter & BEHAVIAC_LOG_BROADCAST_MASK)
        {
            //StdConsoleOutput(pMsg);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int ConsoleOut::ConsoleAssertCb(const char* cond, const char* msg, const char* file, int line, int /* extra */)
    {
        Print(BEHAVIAC_LOG_CRITICAL, "\n\n%s(%u) : ASSERT FAILURE: %s(%s)\n", file, line, cond, msg);
        //BEHAVIAC_HW_BREAK();
        BEHAVIAC_ASSERT(false);
        return 0;
    }
}
