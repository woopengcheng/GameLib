#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"
#include "NetLib/inc/NetHandlerListener.h" 
#include "NetLib/inc/NetHandlerServer.h"
#include "NetLib/inc/ServerSession.h"
#include "NetLib/inc/INetReactor.h"
#include "NetLib/inc/NetHelper.h"
#include "NetLib/inc/NetReactorRakNet.h"
#include "NetLib/inc/NetReactorEpollEx.h"
#include "NetLib/inc/NetThread.h"

#ifdef _LINUX
#include "Coroutine/libco/co_routine.h"
#endif

#ifdef WIN32
#include <WinSock.h>
#endif

namespace Net
{ 
	NetHandlerListener::NetHandlerListener( INetReactor * pNetReactor , ISession * pSession)
		: INetHandler(pNetReactor , pSession)
	{ 
		switch (m_pSession->GetReactorType())
		{
		case REACTOR_TYPE_UDS:
		case REACTOR_TYPE_UDSEX:
		{
			InitUDS();
		}break;
		default:
		{
			InitCommon();
		}break;
		}
	}

	void NetHandlerListener::InitUDS()
	{
		Assert(m_pSession);
		NetSocket socket = NetHelper::CreateSocket(AF_UNIX, SOCK_STREAM, 0);
		NetHelper::SetDefaultSocket(socket, m_pSession->GetSendBufSize(), m_pSession->GetRecvBufSize());

		m_pSession->SetSocket(socket);
	}

	void NetHandlerListener::InitCommon()
	{
		Assert(m_pSession);
		NetSocket socket = NetHelper::CreateSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		int nValueTrue = 1;
		NetHelper::SetIOCtrl(socket, FIOASYNC, &nValueTrue);
		m_pSession->SetSocket(socket);
	}

	CErrno NetHandlerListener::Init( const char * pAddress , INT32 nPort , BOOL bResueAddr /*= TRUE */, INT32 nListenerCount /*= DEFAULT_LISTENER_COUNT*/ )
	{ 
		Assert_ReF(!Bind(pAddress , nPort , bResueAddr));

		if (Listen(nListenerCount) == 0)
		{
			m_pSession->SetClosed(FALSE);
			m_pSession->SetNetState(NET_STATE_CONNECTED);
#ifdef _LINUX
			if (m_pNetReactor->GetReactorType() == REACTOR_TYPE_EPOLLEX)
			{
				EpollexCoTask * pTask = new EpollexCoTask;
				pTask->fd = kInvalidFD;
				pTask->pReactor = m_pNetReactor;
				m_pSession->SetContext(pTask);
				co_create(&(pTask->co), NULL, NetHandlerListener::EpollexUpdateAccept, pTask);
				co_resume(pTask->co);
			}
#endif
			return CErrno::Success();
		}
		return CErrno::Failure();
	}

	CErrno NetHandlerListener::Cleanup()
	{ 
		SAFE_DELETE(m_pSession);
		return CErrno::Success();
	}
	CErrno NetHandlerListener::OnMsgRecving(void)
	{
		if (m_pSession)
		{
			switch (m_pSession->GetReactorType())
			{
				case REACTOR_TYPE_IOCP:
				{
					return OnMsgRecvingIOCP();
				}break;
				case REACTOR_TYPE_RAKNET:
				{
					return OnMsgRecvingRakNet();
				}break;
				default:
				{
					return OnMsgRecvingCommon();
				}break;
			}
		}
		return CErrno::Failure();
	}

	CErrno NetHandlerListener::OnMsgRecvingCommon(void)
	{
		sockaddr_in addr = {0};
#ifdef __linux
		socklen_t nAddrLen = sizeof(sockaddr_in);
#else
		INT32 nAddrLen = sizeof(sockaddr_in);
#endif
		NetSocket socket = ::accept(m_pSession->GetSocket(),(sockaddr*)&addr , &nAddrLen);
		if( socket != -1)
		{ 
			NetHelper::SetSocket(socket);	  
			this->OnAccept(socket , &addr);
		}
		return CErrno::Success();
	}	
		
	INT32 NetHandlerListener::Listen( INT32 nListenerCount /*= DEFAULT_LISTENER_COUNT*/)
	{
		return ::listen(m_pSession->GetSocket() , nListenerCount);
	}

	INT32 NetHandlerListener::Bind( const char * pAddress , INT32 nPort , BOOL bResueAddr)
	{
		Assert_ReF1(pAddress);
		 
		int nValueTrue = 1;
		if (bResueAddr)
		{
			setsockopt(m_pSession->GetSocket(), SOL_SOCKET , SO_REUSEADDR , (char *)&nValueTrue , sizeof(nValueTrue)); 
		}

		if (m_pSession->GetReactorType() == REACTOR_TYPE_UDS || REACTOR_TYPE_UDSEX == m_pSession->GetReactorType())
		{
#ifdef _LINUX
			std::string strFile = pAddress;
			strFile += ".";
			strFile += itoa(port);

			if (判断文件是否存在)
			{
				cunzai.shanchu
			}

			sockaddr_un addr = { 0 };
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_UNIX;
			strncpy(addr->sun_path, strFile.c_str(), sizeof(addr->sun_path) - 1);
			addr->sun_path[sizeof(addr->sun_path) - 1] = '\0';
			unlink(addr->sun_path);
			
			return ::bind(m_pSession->GetSocket(), (sockaddr*)&addr, sizeof(sockaddr_un));
#endif
		}
		else
		{
			sockaddr_in addr = { 0 };
			addr.sin_family = AF_INET;
			addr.sin_port = htons(nPort);
			addr.sin_addr.S_un.S_addr = INADDR_ANY;
			if (pAddress != 0 && strlen(pAddress) > 0)
			{
#ifdef WIN32
				inet_pton(AF_INET, pAddress, (PVOID*)(&addr.sin_addr.s_addr));
#else
				address.sin_addr.s_addr = inet_addr(ip);
#endif
			}

			return ::bind(m_pSession->GetSocket(), (sockaddr*)&addr, sizeof(sockaddr_in));
		}

		return -1;
	}
	
	void NetHandlerListener::OnAccept(NetSocket socket, sockaddr_in * addr)
	{
		if (m_pNetReactor)
		{
			char szAddress[MAX_NAME_LENGTH];
			UINT16 usPort = 0;
			Net::NetHelper::GetAddressAndPortByAddrIn(addr , szAddress , usPort);
			std::string strName = "";
			if (m_pNetReactor && m_pNetReactor->GetNetThread())
			{
				strName = m_pNetReactor->GetNetThread()->GetNetNodeName();
			}
			ServerSession * pServerSession = new ServerSession(szAddress , usPort , strName , "" , -1, NET_STATE_CONNECTED , socket);
			NetHandlerServerPtr pServer( new NetHandlerServer(m_pNetReactor , pServerSession) ); 

			if (m_pNetReactor->GetReactorType() == REACTOR_TYPE_EPOLLEX)
			{
#ifdef _LINUX
				EpollexCoTask * pCoTask = m_pNetReactor->GetEpollExTasks().top();
				if (pCoTask)
				{
					pCoTask->fd = fd;
					pCoTask->pNetHandler = pServer;
					pCoTask->pReactor = m_pNetReactor;
					co_resume(pCoTask->co);
				}
				m_pNetReactor->AddNetHandler(pServer , (ENetHandlerFuncMask)(NET_FUNC_READ | NET_FUNC_EXCEPT | NET_FUNC_TIMEOUT | NET_FUNC_POLLHUP));
#endif
			}
			else
			{
				m_pNetReactor->AddNetHandler(pServer);
			}
			
			gDebugStream("accept client. address:" << szAddress << ":port=" << usPort);
		} 
	}


	void	 NetHandlerListener::EpollexUpdateAccept(void * pArg)
	{
#ifdef _LINUX
// 		co_enable_hook_sys();

		EpollexCoTask * pTask = (EpollexCoTask*)pArg;
		MsgAssert(pTask && pTask->pReactor, "error NetHandlerListener::EpollexUpdateAccept.");
		NetReactorEpollEx * pReactor = dynamic_cast<NetReactorEpollEx*>(pTask->pReactor);
		MsgAssert(pReactor, "error NetHandlerListener::EpollexUpdateAccept.error pReactor");
		if (pTask)
		{
			while (true)
			{
				if (pTask->fd == kInvalidFD)
				{
					//5 等待创建成功.
					struct pollfd pf = { 0 };
					pf.fd = -1;
					poll(&pf, 1, 1000);
					continue;
				}

				std::stack<EpollexCoTask*> & epollTasks = pReactor->GetEpollExTasks();
				if (epollTasks.empty())
				{
					pReactor->CreateEpollExTasks();

					//5 等待创建,沉睡一秒
					struct pollfd pf = { 0 };
					pf.fd = -1;
					poll(&pf, 1, 1000);

					continue;
				}
				
				struct sockaddr  * addr;
				INT32 nLength = 0;
				if (pReactor->GetReactorType() == REACTOR_TYPE_UDS || REACTOR_TYPE_UDSEX == pReactor->GetReactorType())
				{
					struct sockaddr_un addr_in;
					memset(&addr_in , 0 , sizeof(addr_in));
					addr = &addr_in;
					nLength = sizeof(addr_in);
				}
				else
				{
					struct sockaddr_in addr_un;
					memset(&addr_un, 0, sizeof(addr_un));
					addr = &addr_un;
					nLength = sizeof(addr_un);
				}

				SOCKET fd = co_accept(pTask->fd, addr, nLength);
				if (fd < 0)
				{
					struct pollfd pf = {0};
					pf.fd = pTask->fd;
					pf.events = pTask->events;
					co_poll(co_get_epoll_ct(), &pf, 1, 1000);
					continue;
				}
				if (pReactor->GetEpollExTasks().empty())
				{
					close(fd);
					gErrorStream("epollex cotasks is full. accept wrong.");
					continue;
				}

				NetHelper::SetSocket(fd);
				NetHandlerListener * pHandler = dynamic_cast<NetHandlerListener*>(pTask->pNetHandler);
				if (pHandler)
				{
					pHandler->OnAccept(fd, addr);
				}
			}
		}
#endif
		return ;
	}

}