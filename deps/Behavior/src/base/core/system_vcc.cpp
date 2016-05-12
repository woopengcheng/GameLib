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
#include "behaviac/base/core/system.h"

#if BEHAVIAC_COMPILER_MSVC
#include <windows.h>

namespace behaviac
{
    THREAD_ID_TYPE GetTID()
    {
        return ::GetCurrentThreadId();
    }

    Address GetModuleID()
    {
        return BEHAVIAC_PTR_TO_ADDR(::GetModuleHandle(0));
    }

    void Sleep(uint32_t timeMS)
    {
        ::Sleep(timeMS);
    }

    void YieldCPU()
    {
#if _MSC_VER >= 1500 && ((BEHAVIAC_CFG_PLATFORM == BEHAVIAC_PLATFORM_WIN32) || (BEHAVIAC_CFG_PLATFORM == BEHAVIAC_PLATFORM_XENON))
        ::SwitchToThread();
#else
        ::Sleep(1);
#endif
        //NOTE :
        //       Think about what you are telling Windows to do when you use Sleep(0) and
        //       also consider that everyone must share the same CPU, bus, etc.
        //       In general, under Windows, which is not a true RTOS (real time operating
        //       system), using Sleep(0) is not a true yield as in a RTOS and if you use it a
        //       tight loop, calling it many times, will cause high context switching.
        //       So if there aren't other threads to be running at the SAME level, its going
        //       to be switching back and forth so much,  your CPU drag will be very high.
        //       Now if you increase the priority, you make it worst.  While the scheduler
        //       will time slice the equal priority thread better now,  the high priority
        //       thread with using Sleep(0) in a tight loop is going to be interrupting now
        //       only its equal priority threads but the lower priority threads as well.  So
        //       there will be a lot of context switching.
        //       It is better to use something like Sleep(1) or so which will give it some
        //       residence time (at least 10-13 msecs depending on the OS and hardware), this
        //       allowing the Window scheduler to time slice better, but more importantly the
        //       context switching is reduced.
        //       Look at it this way:
        //       The more context switching you have, the more the other threads will be
        //       preempted making those threads less efficient as well.   So basically what
        //       you are telling windows to do is to reduce the residence time a thread will
        //       have because you are telling window to stop it, and go back to the sleep(0)
        //       thread immediately.  It is doing this so many times, that your system is
        //       basically running very inefficient.
        //       In a nut shell, the basic definition of a RTOS is that the residence time
        //       for each thread is guaranteed.   Equal round robin time slicing. Windows CE
        //       is suppose to offer RTOS functionality.  Real time programmers demand this
        //       functionality.  So in a RTOS, a Sleep(0) is a true yield and it is suppose
        //       to give every thread time to do some work before it comes back.   That's not
        //       the case with standard Windows. If you use Sleep(0) incorrectly, your system
        //       efficiency degrades.
        //       In short, don't do this:
        //               while(1) {
        //                   // ....
        //                   // op code that takes less than 1 quantum and no kernel wait
        //       objects
        //                   // .....
        //                   Sleep(0)
        //               }
        //       Just change it by giving the sleep some time, like Sleep(1) or Sleep(10),
        //       etc.
        //       Now, if all this is about getting the thread to do a lot of work very fast,
        //       well, go ahead and increase the priority, but you must think about not being
        //       a HOG and allow windows to give other threads to do some work.  Everyone on
        //       must share that CPU. Its that simple.  Consider even possibly using a kernel
        //       wait object with 0 time.   Windows will do better time slicing when using
        //       kernel wait objects.  In fact, it gives you a way to break out of a loop by
        //       signaling the wait object if indeed you need this type of logic.
        //       Hope this provides some insight.
        //       --
        //       Hector Santos
        //       WINSERVER "Wildcat! Interactive Net Server"
    }

    bool IsValidThread(uint32_t threadID)
    {
        uint32_t err = 0;
        ::SetLastError(0);
        HANDLE handle = ::OpenThread(THREAD_ALL_ACCESS, FALSE, threadID);

        if (handle == 0 || handle == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        // WaitForSingleObject will return -1 for invalid handle
        uint32_t res1 = ::WaitForSingleObject(handle, 0);
        ::CloseHandle(handle);
        // Purge the error
        err = ::GetLastError();
        return (res1 != 0 && res1 != 0xFFFFFFFF);
    }

#if BEHAVIAC_CFG_SETEXENAME_BUF_SIZE > 0
    static Char g_ExeName[BEHAVIAC_CFG_SETEXENAME_BUF_SIZE];
#endif

    bool IsBadReadPointer(void* ptr, uint32_t memSize)
    {
        // Check buffer validity using VirtualQuery. This is somewhat safer than trying to rewrite the same value everywhere
        // Anyway, this should only be used in debug code to avoid accessing invalid members(such as in MemDebug implementation)
        bool isBad = true;
        MEMORY_BASIC_INFORMATION info;
        size_t result = VirtualQuery(ptr, &info, sizeof(info));

        if (result == sizeof(info))
        {
            // Check that the associated pages are committed and with right protection level. Also, check
            // that the whole buffer pointed to by the pointer is within the same protection region.
            if (info.State == MEM_COMMIT &&
                (info.AllocationProtect & (PAGE_READWRITE | PAGE_READONLY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) &&
                BEHAVIAC_PTR_TO_ADDR(ptr) + memSize < BEHAVIAC_PTR_TO_ADDR(info.BaseAddress) + info.RegionSize)
            {
                isBad = false;
            }
        }

        return isBad;
    }

    bool IsBadWritePointer(void* ptr, uint32_t memSize)
    {
        // Check buffer validity using VirtualQuery. This is a somewhat safer than trying to rewrite the same value everywhere
        // Anyway, this should only be used in debug code to avoid accessing invalid members(such as in MemDebug implementation)
        bool isBad = true;
        MEMORY_BASIC_INFORMATION info;
        size_t result = VirtualQuery(ptr, &info, sizeof(info));

        if (result == sizeof(info))
        {
            // Check that the associated pages are committed and with right protection level. Also, check
            // that the whole buffer pointed to by the pointer is within the same protection region.
            if (info.State == MEM_COMMIT &&
                (info.AllocationProtect & (PAGE_READWRITE | PAGE_EXECUTE_WRITECOPY | PAGE_EXECUTE_READWRITE)) &&
                BEHAVIAC_PTR_TO_ADDR(ptr) + memSize < BEHAVIAC_PTR_TO_ADDR(info.BaseAddress) + info.RegionSize)
            {
                isBad = false;
            }
        }

        return isBad;
    }

    const Char* GetExeName(Char* exeNameBuffer, uint32_t exeNameBufferSize)
    {
#if BEHAVIAC_CFG_SETEXENAME_BUF_SIZE > 0

        if (g_ExeName[0] != 0)
            memcpy(exeNameBuffer, g_ExeName,
                   (BEHAVIAC_CFG_SETEXENAME_BUF_SIZE > exeNameBufferSize ? exeNameBufferSize : BEHAVIAC_CFG_SETEXENAME_BUF_SIZE));

        else
#endif
        {
            char moduleName[MAX_PATH];
            GetModuleFileNameA(NULL, moduleName, sizeof(moduleName));
            // Remove path, return only exe name.
            const Char* execName = moduleName;
            Char* p = moduleName;

            while (*p != 0)
            {
                if (*p == '\\')
                {
                    execName = p + 1;
                }

                p++;
            }

            p = exeNameBuffer;

            while (*execName && p < exeNameBuffer + exeNameBufferSize)
            {
                *p = *execName;
                p++;
                execName++;
            }

            *p = 0;
        }

        return exeNameBuffer;
    }

    void SetExeName(const Char* exeNameBuffer)
    {
        // This assert will be raised if you try to use SetExeName with the default value
        // of 0 configured in BEHAVIAC_CFG_SETEXENAME_BUF_SIZE. To be able to use SetExeName,
        // you need to configure BEHAVIAC_CFG_SETEXENAME_BUF_SIZE to a value big enough to contain
        // the longest exe name behaviac::string you want to use.
        uint32_t bufferSize = BEHAVIAC_CFG_SETEXENAME_BUF_SIZE;
        BEHAVIAC_ASSERT(bufferSize > 0);
        BEHAVIAC_UNUSED_VAR(bufferSize);
#if BEHAVIAC_CFG_SETEXENAME_BUF_SIZE == 0
        BEHAVIAC_UNUSED_VAR(exeNameBuffer);
#else
        uint16_t length = 0;

        while ((exeNameBuffer[length++] != 0) && (length < BEHAVIAC_CFG_SETEXENAME_BUF_SIZE))
            ;

        memcpy(g_ExeName, exeNameBuffer, length);

        if (length == BEHAVIAC_CFG_SETEXENAME_BUF_SIZE)
        {
            g_ExeName[length - 1] = 0;
        }

#endif
    }

    void* MemCmpPattern(const void* buffer, uint8_t value, size_t size)
    {
        uint8_t* ptr = (uint8_t*)buffer;
        const uint32_t U32Value = value << 24 | value << 16 | value << 8 | value;
        const uint64_t U64Value = BEHAVIAC_MAKE64(U32Value, U32Value);
        const uint8_t* U8End = ptr + size;
        const uint8_t* U64End = U8End - sizeof(uint64_t);

        while (ptr < U8End && !BEHAVIAC_PTR_ALIGNED(ptr, sizeof(uint64_t)))
        {
            if (*ptr != value)
            {
                return ptr;
            }

            ++ptr;
        }

        for (; ptr < U64End; ptr += sizeof(uint64_t))
        {
            if (*((uint64_t*)ptr) == U64Value)
            {
                continue;

            }
            else
            {
                // uint64_t value is different, return first different address.
                while (*ptr == value)
                {
                    ++ptr;
                }

                return ptr;
            }
        }

        while (ptr < U8End)
        {
            if (*ptr != value)
            {
                return ptr;
            }

            ++ptr;
        }

        return 0;
    }

    void* MemCmpPatternReverse(const void* buffer, uint8_t value, size_t size)
    {
        uint8_t* ptr = (uint8_t*)buffer + size;
        const uint32_t U32Value = value << 24 | value << 16 | value << 8 | value;
        const uint64_t U64Value = BEHAVIAC_MAKE64(U32Value, U32Value);
        const uint8_t* U8End = (uint8_t*)buffer;
        const uint8_t* U64End = U8End + sizeof(uint64_t);

        while (ptr > U8End && !BEHAVIAC_PTR_ALIGNED(ptr, sizeof(uint64_t)))
        {
            --ptr;

            if (*ptr != value)
            {
                return ptr;
            }
        }

        while (ptr > U64End)
        {
            ptr -= sizeof(uint64_t);

            if (*((uint64_t*)ptr) == U64Value)
            {
                continue;

            }
            else
            {
                ptr += sizeof(uint64_t);

                // uint64_t value is different, return first different address.
                do
                {
                    --ptr;
                }
                while (*ptr == value);

                return ptr;
            }
        }

        while (ptr > U8End)
        {
            --ptr;

            if (*ptr != value)
            {
                return ptr;
            }
        }

        return 0;
    }

    void* memcpy_ps(void* dest, const void* src, size_t num)
    {
        return ::memcpy(dest, src, num);
    }
}

#endif//#if BEHAVIAC_COMPILER_MSVC
