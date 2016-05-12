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

#ifndef BEHAVIAC_BASE_LOGGING_LOGGING_H
#define BEHAVIAC_BASE_LOGGING_LOGGING_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/assert_t.h"

#include "behaviac/base/core/logging/consoleout.h"

/// List of broadcast Log levels. All theses level can be masked in TagLog
enum ETagLogLevel
{
    BEHAVIAC_LOG_NONE = 0x00000000,        // The log will not be displayed
    BEHAVIAC_LOG_INFO = 0x00000001,        // Show on console stuff requested by clients (Show commands)
    BEHAVIAC_LOG_MSG = 0x00000002,        // Messages are displayed to inform of what is going on
    BEHAVIAC_LOG_WARNING = 0x00000004,        // Warning are special condition that the user would like to know
    BEHAVIAC_LOG_ERROR = 0x00000008,        // Non critical error that the user should know about
    BEHAVIAC_LOG_CRITICAL = 0x00000010,        // Same level as Assert. The program should be stopped.
    BEHAVIAC_LOG_USER_DEFINED = 0x00000020        // Users can define their own logging level
};

namespace behaviac
{
#define BEHAVIAC_LOG_CONSOLE            0x01000000        // Channel used by all consoles (should be off by default)
#define BEHAVIAC_LOG_PRIVATE_MASK       0xFF000000        // All channels in the private mask cannot be masked.
#define BEHAVIAC_LOG_BROADCAST_MASK     0x00FFFFFF        // All channels in the broadcast mask are intended for system logging

    /// This is the TraceManager class, used to Set/Get the Logging level
    /*! behaviac is using the TagLog module to log information, warning & error messages
    	during execution. Macro are used since they are the only reliable way of getting
    	rid of strings when compiling in retail, since they are removed at pre-compilation.
    	(Some compilers keep strings in the final binary if they are passed as parameter, even
    	to an inline function and even if they are not referenced).
    	The Logging system work in a "channel" scheme, and the user can filter which type
    	of message he wants to see. The Levels (or channels) are defined in taglog_stub.h
    	Basically, behaviac is using 2 types of generic messages: BEHAVIAC_LOG_INFO, which is normally
    	logs requested by the clients (Any functions used to show a object status) and the
    	BEHAVIAC_LOG_MSG function, which is message displayed to help understand what is going
    	on when behaviac is running (usually this level is filtered).
    	The three other level, are used for warnings & errors and should not be filtered.
    	The TraceManager can be access anytime, even before behaviac is initialized.
    	This module can be used by clients if they want to add ther own channels. The current
    	filter has 32 bits and only 5 of them are used by behaviac. A client could add their own
    	channel for logging.
    	*/
    class BEHAVIAC_API TraceManager
    {
    public:
        /// Set the logging filter.
        /*! \param NewFilter The Log filter, which is a bitfield of all channels we want
        					 to see on the Console. Ex: BEHAVIAC_LOG_INFO | BEHAVIAC_LOG_CRITICAL
        					 */
        static void SetLoggingFilter(uint32_t NewFilter);

        /// Returns the current logging Filter
        static uint32_t  GetLoggingFilter(void);

        static void SetLoggingLevel(ETagLogLevel NewLevel);

        /// This function is used by internal macros to determine if we should display on the console.
        /*! \param  Channel  Which channel you want to display on
        	\return             Returns True if the filter lets you display on this channel
        	or False if this message should be filtered.
        	*/
        static bool CanLog(uint32_t Channel);

        static bool CanLog(ETagLogLevel level);

        /// This function is used by internal macros to determine if we should display on the console.
        /// If the message it displayed once, it will never be displayed again
        /*! \param  Channel  Which channel you want to display on
        	\param  LogOnce  [I/O] A static value used to keep track if the message has been displayed
        	\return             Returns True if the filter lets you display on this channel
        	or False if this message should be filtered.
        	*/
        static bool CanLogOnce(uint32_t Channel, uint8_t& LogOnce);

    private:
        static uint32_t    m_TagLogFilter;       // The current channel allowed to display
        static uint32_t    m_TagLogLevel;        // The current level allowed to display
    };

    /// Log a behaviac::string on the selected Level (or channel) with variable parameters
#define BEHAVIAC_LOGV(Level, fmt, ...) \
    {																						\
        if(behaviac::TraceManager::CanLog(Level))                                           \
            ::behaviac::ConsoleOut::Print(Level, fmt, ##__VA_ARGS__);                         \
    }

#define BEHAVIAC_LOGINFO(fmt, ...)		BEHAVIAC_LOGV(BEHAVIAC_LOG_INFO, fmt, ##__VA_ARGS__)
#define BEHAVIAC_LOGMSG(fmt, ...)		BEHAVIAC_LOGV(BEHAVIAC_LOG_MSG, fmt, ##__VA_ARGS__)
#define BEHAVIAC_LOGWARNING(fmt, ...)	BEHAVIAC_LOGV(BEHAVIAC_LOG_WARNING, fmt, ##__VA_ARGS__)
#define BEHAVIAC_LOGERROR(fmt, ...)		BEHAVIAC_LOGV(BEHAVIAC_LOG_ERROR, fmt, ##__VA_ARGS__)
#define BEHAVIAC_LOGCRITICAL(fmt, ...)	BEHAVIAC_LOGV(BEHAVIAC_LOG_CRITICAL, fmt, ##__VA_ARGS__)

    // #define BEHAVIAC_LOGINFO(msg)       BEHAVIAC_LOG(BEHAVIAC_LOG_INFO, msg)
    // #define BEHAVIAC_LOGMSG(msg)        BEHAVIAC_LOG(BEHAVIAC_LOG_MSG, msg)
    // #define BEHAVIAC_LOGWARNING(msg)    BEHAVIAC_LOG(BEHAVIAC_LOG_WARNING, msg)
    // #define BEHAVIAC_LOGERROR(msg)      BEHAVIAC_LOG(BEHAVIAC_LOG_ERROR, msg)
    // #define BEHAVIAC_LOGCRITICAL(msg)   BEHAVIAC_LOG(BEHAVIAC_LOG_CRITICAL, msg)

    /// Log a single behaviac::string on the selected Level (or channel)
#define BEHAVIAC_LOG(Level, StrMsg)																\
    {																							\
        if(behaviac::TraceManager::CanLog(Level))													\
            ::behaviac::ConsoleOut::Print(Level, StrMsg);										\
    }

    /// Log a behaviac::string formatted with 1 parameter on the selected Level (or channel)
#define BEHAVIAC_LOG1(Level, StrMsg, P1)														\
    {																							\
        if(behaviac::TraceManager::CanLog(Level))													\
            ::behaviac::ConsoleOut::Print(Level, StrMsg, P1);                                   \
    }

    /// Log a behaviac::string formatted with 2 parameters on the selected Level (or channel)
#define BEHAVIAC_LOG2(Level, StrMsg, P1, P2)													\
    {																							\
        if(behaviac::TraceManager::CanLog(Level))													\
            ::behaviac::ConsoleOut::Print(Level, StrMsg, P1, P2);                               \
    }

    /// Log a behaviac::string formatted with 3 parameters on the selected Level (or channel)
#define BEHAVIAC_LOG3(Level, StrMsg, P1, P2, P3)												\
    {																							\
        if(behaviac::TraceManager::CanLog(Level))													\
            ::behaviac::ConsoleOut::Print(Level, StrMsg, P1, P2, P3);                            \
    }

    /// Dump a memory location in a nicely formated way on the console
#define BEHAVIAC_DUMP(Level, Ptr, Size)															\
    {																							\
        if(behaviac::TraceManager::CanLog(Level))													\
            ::behaviac::ConsoleOut::Dump(Level, Ptr, Size);                                     \
    }

    /// Log a single behaviac::string on the selected Level (or channel).
    /// The behaviac::string will be displayed only once in the entire program lifetime
#define BEHAVIAC_LOG_ONCE(Level, StrMsg)														\
    {																							\
        static uint8_t __TagLogOnce = 0;														\
        if(behaviac::TraceManager::CanLogOnce(Level, __TagLogOnce))								\
            ::behaviac::ConsoleOut::Print(Level, StrMsg);										\
    }

    /// Log a behaviac::string formatted with 1 parameter on the selected Level (or channel).
    /// The behaviac::string will be displayed only once in the entire program lifetime
#define BEHAVIAC_LOG_ONCE1(Level, StrMsg, P1)													\
    {																							\
        static uint8_t __TagLogOnce = 0;														\
        if(behaviac::TraceManager::CanLogOnce(Level, __TagLogOnce))								\
            ::behaviac::ConsoleOut::Print(Level, StrMsg, P1);                                    \
    }

    /// Log a behaviac::string formatted with 2 parameters on the selected Level (or channel).
    /// The behaviac::string will be displayed only once in the entire program lifetime
#define BEHAVIAC_LOG_ONCE2(Level, StrMsg, P1, P2)												\
    {																							\
        static uint8_t __TagLogOnce = 0;														\
        if(behaviac::TraceManager::CanLogOnce(Level, __TagLogOnce))								\
            ::behaviac::ConsoleOut::Print(Level, StrMsg, P1, P2);                                \
    }

    /// Log a behaviac::string formatted with 3 parameters on the selected Level (or channel).
    /// The behaviac::string will be displayed only once in the entire program lifetime
#define BEHAVIAC_LOG_ONCE3(Level, StrMsg, P1, P2, P3)											\
    {																							\
        static uint8_t __TagLogOnce = 0;														\
        if(behaviac::TraceManager::CanLogOnce(Level, __TagLogOnce))								\
            ::behaviac::ConsoleOut::Print(Level, StrMsg, P1, P2, P3);                           \
    }
}

#endif  // #ifndef BEHAVIAC_BASE_LOGGING_LOGGING_H
