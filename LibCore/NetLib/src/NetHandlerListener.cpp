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
#include "NetLib/inc/NetThread.h"

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
		Assert_ReF(!Bind(pAddress , nPort));

		if (Listen(nListenerCount) == 0)
		{
			m_pSession->SetClosed(FALSE);
			m_pSession->SetNetState(NET_STATE_CONNECTED);
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

		if (m_pSession->GetReactorType() == REACTOR_TYPE_UDS)
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
				addr.sin_addr.S_un.S_addr = ::inet_addr(pAddress);
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
			m_pNetReactor->AddNetHandler(pServer); 
			
			gDebugStream("accept client. address:" << szAddress << ":port=" << usPort);
		} 
	}


}