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

#ifndef BEHAVIAC_BASE_LOGGING_CONSOLEOUT_H
#define BEHAVIAC_BASE_LOGGING_CONSOLEOUT_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"

#include "behaviac/base/core/thread/mutex.h"

#include <stdarg.h>

namespace behaviac
{
    namespace Private
    {
        class SafeLock;
    }

    enum ELogOutput
    {
        ELOG_CONSOLE = 1,
        ELOG_FILE = 2,
        ELOG_VCOUTPUT = 4
    };

    /// ConsoleOut is a class that implements a basic ouput console mechanism
    /*! ConsoleOut can be hooked to output on devices
    */
    class BEHAVIAC_API ConsoleOut
    {
    public:
        // Type of a console out callback function
        typedef void(*ConsoleOutCallBack)(uint32_t, const char*);

        /**
        to pass ELogOutput enum as mask

        you can call ConsoleOut::SetEnableMask(0) to disable the output
        you can call ConsoleOut::SetEnableMask(ELOG_CONSOLE) to enable only the console output
        */
        static void SetEnableMask(uint32_t enableMask);

        /// Send a formatted behaviac::string to the console hook chain.
        static void Print(uint32_t LogFilter, const char* format, ...);
        static void Print(const char* format, ...);
        static void VPrint(const char* format, va_list& argList);
        static void VPrint(uint32_t LogFilter, const char* format, va_list& argList);

        /// Send a single behaviac::string to the console hook chain (faster than Print)
        static void PrintLines(uint32_t LogFilter, const char* pStr);
        static void PrintLines(const char* pStr);

        /// Send a memory dump on the console hook chain
        /*! Here's a example of Dump(pMyDataAt340000, 32)
        	\code
        	00340000  4D 45 4D 4F 52 59 4C 4F   47 47 45 51 01 20 02 64  MEMORYLOGGER. .d
        	00240010  10 01 38 26 3E 41 01 00   00 34 00 00 10 00 00 00  ..8&>A...4......
        	\endcode
        	\param LogFilter    Filter to apply to the dump
        	\param pData        A pointer on the section of memory to dump (Must be != NULL)
        	\param Size         The size in byte of the memory dump
        	\return None
        	*/
        static void Dump(uint32_t LogFilter, const void* pData, size_t size);
        static void Dump(const void* pData, size_t size);

        /// Tell the consoleout module that the initialization of behaviac is now completed
        static void InitializationCompleted(void);

        /// Default console out target.
        /// To remove this default hook, call RemoveHook(ConsoleOut::DefaultConsoleOutput);
        static void DefaultConsoleOutput(uint32_t Filter, const char* pMsg);

        /// Default std console out target.
        /// To remove this default hook, call RemoveHook(ConsoleOut::DefaultStdConsoleOutput);
        static void DefaultStdConsoleOutput(uint32_t Filter, const char* pMsg);
    protected:
        // Output a single behaviac::string to the hook chain
        static void Output(uint32_t Filter, const char* pStr);

        static void OutputLine(const char* pStr);
		static void OutputDecoratedLine(uint32_t LogFilter, const char* pStr);

        // Test if init, and if not, init the object. Should be @ beginning of every public function
        // since ConsoleOut can be called before behaviac is even initialized.
        static void TestInit(void);

        static bool              IsInit;
        static uint32_t          EnableMask;

    private:
        // This is the hook for Assert.
        static int ConsoleAssertCb(const char* cond, const char* msg, const char* file, int line, int extra);
    };
}

#endif//BEHAVIAC_BASE_LOGGING_CONSOLEOUT_H
