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

#include "behaviac/base/base.h"
#include "behaviac/base/socket/socketconnect.h"
#include "behaviac/base/core/socket/socketconnect_base.h"
#include "behaviac/base/file/filesystem.h"

#include "behaviac/base/core/logging/log.h"
#include "behaviac/base/core/container/spscqueue.h"
#include "behaviac/base/core/container/hash_exmemory.h"
#include "behaviac/base/core/thread/mutex.h"

#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"
#include "behaviac/base/logging/logmanager.h"
#include "behaviac/agent/context.h"
#if BEHAVIAC_COMPILER_MSVC
#include <windows.h>
#endif//BEHAVIAC_COMPILER_MSVC

namespace behaviac
{
    uint32_t Packet::CalcPacketSize() const
    {
		size_t packetSize(0);

        if (command == CommandId::CMDID_TEXT)
        {
            //packetSize = sizeof(Text);
            packetSize = strlen((char*)this->data);
            BEHAVIAC_ASSERT(packetSize <= kMaxTextLength);
        }
        else
        {
            BEHAVIAC_ASSERT(false, "Unknown command");
        }

        packetSize += sizeof(command);
        return (uint32_t)packetSize;
    }
}

namespace behaviac
{
    namespace Socket
    {
        void SendWorkspaceSettings();
    }

#pragma pack(push, 1)
    struct InitialSettingsPacket
    {
        InitialSettingsPacket()
            : messageSize(0),
              command(behaviac::CommandId::CMDID_INITIAL_SETTINGS),
              platform(Platform::WINDOWS)
        {
#if BEHAVIAC_COMPILER_MSVC
            HANDLE processHandle = GetCurrentProcess();
            this->processId = GetProcessId(processHandle);
#else
            this->processId = 0;
#endif
        }

        size_t PrepareToSend()
        {
            messageSize = sizeof(InitialSettingsPacket) - 1;
            return messageSize + 1;
        }

        uint8_t	messageSize;
        uint8_t	command;
        uint8_t	platform;
        uint32_t processId;
    };
#pragma pack(pop)

    class ConnectorImpl : public ConnectorInterface
    {
    public:
        ConnectorImpl();
        virtual ~ConnectorImpl();

        virtual void OnConnection();

        bool IsWorkspaceSent() const
        {
            return m_workspaceSent;
        }

        void SetWorkspaceSent(bool bSent)
        {
            m_workspaceSent = bSent;
        }
    private:
        volatile bool				m_workspaceSent;

        void SendInitialSettings();
        void SendInitialProperties();
        virtual void Clear()
        {
            ConnectorInterface::Clear();

            m_workspaceSent = false;
        }
    };

    ConnectorImpl					s_tracer;

    ConnectorImpl::ConnectorImpl() : m_workspaceSent(false)
    {
        //don't handle message automatically
        m_bHandleMessage = false;
    }

    ConnectorImpl::~ConnectorImpl()
    {
    }

    void ConnectorImpl::SendInitialSettings()
    {
        InitialSettingsPacket initialPacket;
        const size_t bytesToSend = initialPacket.PrepareToSend();
        size_t bytesWritten(0);

        if (!behaviac::Socket::Write(m_writeSocket, &initialPacket, bytesToSend, bytesWritten) ||
            bytesWritten != bytesToSend)
        {
            Log("behaviac: Couldn't send initial settings.\n");
        }

        gs_packetsStats.init++;
    }

    void ConnectorImpl::OnConnection()
    {
        Log("behaviac: sending initial settings.\n");

        this->SendInitialSettings();

        Socket::SendWorkspaceSettings();

        this->SendInitialProperties();

        {
            ScopedInt_t scopedInt(&gs_threadFlag);
            Log("behaviac: sending packets before connecting.\n");

            this->SendExistingPackets();
        }

        behaviac::Socket::SendText("[connected]precached message done");

        //when '[connected]' is handled in the designer, it will send back all the breakpoints if any and '[breakcpp]' and '[start]'
        //here we block until all those messages have been received, otherwise, if we don't block here to wait for all those messages
        //the breakpoints checking might be wrong.
        bool bLoop = true;

        while (bLoop && !m_isDisconnected && this->m_writeSocket)
        {
            //sending packets if any
            if (m_packetsCount > 0)
            {
                SendAllPackets();
            }

            const char* kStartMsg = "[start]";
            bool bFound = this->ReceivePackets(kStartMsg);

            if (bFound)
            {
                bLoop = false;

            }
            else
            {
                behaviac::Thread::Sleep(1);
            }
        }

        Log("behaviac: OnConnection done.\n");
        //this->m_bHandleMessage = false;
    }

    void ConnectorImpl::SendInitialProperties()
    {
        Workspace::GetInstance()->LogCurrentStates();
    }
}

namespace behaviac
{
    namespace Socket
    {
        bool SetupConnection(bool bBlocking, unsigned short port)
        {
            if (Config::IsSocketing())
            {
                if (!s_tracer.IsInited())
                {
                    const int		kMaxThreads = 128;

                    if (!s_tracer.Init(kMaxThreads, port, bBlocking))
                    {
                        return false;
                    }
                }

                BEHAVIAC_LOG(BEHAVIAC_LOG_INFO, "behaviac: SetupConnection successful\n");

                return true;
            }

            return false;
        }

		bool IsConnected()
		{
			if (Config::IsSocketing())
			{
				return s_tracer.IsConnected();
			}

			return false;
		}

        void ShutdownConnection()
        {
            if (Config::IsSocketing())
            {
                s_tracer.Close();

                BEHAVIAC_LOG(BEHAVIAC_LOG_INFO, "behaviac: ShutdownConnection\n");
            }
        }

        void SendText(const char* text)
        {
            if (Config::IsSocketing())
            {
                s_tracer.SendText(text);
            }
        }

        void SendWorkspace(const char* text)
        {
            if (Config::IsSocketing())
            {
                s_tracer.SendText(text, CommandId::CMDID_TEXT);
            }
        }

        bool ReadText(behaviac::string& text)
        {
            if (Config::IsSocketing())
            {
                return s_tracer.ReadText(text);
            }

            return false;
        }

        void Flush()
        {
            if (Config::IsSocketing())
            {
                while (s_tracer.GetPacketsCount())
                {
                    behaviac::Thread::Sleep(1);
                }
            }
        }

        void SendWorkspaceSettings()
        {
            if (Config::IsSocketing())
            {
				if (!s_tracer.IsWorkspaceSent() && s_tracer.IsConnected())
				{
#if BEHAVIAC_COMPILER_MSVC
					const char* platform = "Windows";
#elif BEHAVIAC_COMPILER_APPLE
					const char* platform = "iOS";
#elif BEHAVIAC_COMPILER_ANDROID
					const char* platform = "Android";
#elif BEHAVIAC_COMPILER_GCC_LINUX
					const char* platform = "Linux";
#elif BEHAVIAC_COMPILER_GCC_CYGWIN
					const char* platform = "Cygwin";
#else
					const char* platform = "Unknown Platform";
#endif

					behaviac::string msg = FormatString("[platform] %s\n", platform);
					LogManager::GetInstance()->LogWorkspace(true, msg.c_str());

					Workspace::EFileFormat format = Workspace::GetInstance()->GetFileFormat();
					const char* formatString = (format == Workspace::EFF_xml ? "xml" : "bson");

					msg = FormatString("[workspace] %s \"%s\"\n", formatString, "");
					LogManager::GetInstance()->LogWorkspace(true, msg.c_str());

					s_tracer.SetWorkspaceSent(true);
				}
            }
        }

        size_t GetMemoryOverhead()
        {
            if (Config::IsSocketing())
            {
                return s_tracer.GetMemoryOverhead();
            }

            return 0;
        }

        size_t GetNumTrackedThreads()
        {
            if (Config::IsSocketing())
            {
                return s_tracer.GetNumTrackedThreads();
            }

            return false;
        }

        void UpdatePacketsStats()
        {
            if (Config::IsSocketing())
            {
                //size_t overhead = (behaviac::GetMemoryOverhead());
                //BEHAVIAC_SETTRACEDVAR("Stats::Vars", gs_packetsStats.vars);
            }
        }
    }
} // behaviac
