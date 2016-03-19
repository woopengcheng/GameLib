#include "RakPeerInterface.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"
#include "NetLib/inc/NetReactorRakNet.h"
#include "NetLib/inc/NetHelper.h"
#include "NetLib/inc/NetHandlerClient.h"
#include "NetLib/inc/NetHandlerServer.h"
#include "NetLib/inc/NetHandlerListener.h"
#include "NetLib/inc/ServerSession.h"
#include "Timer/inc/TimerHelp.h"
#include "NetLib/inc/NetThread.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace Net
{
	class RakNetContext
	{
	public:
		RakNetContext()
			: m_pPacket(NULL)
		{
		}
		~RakNetContext()
		{
		}

	public:
		RakNet::Packet		*	GetPacket() const { return m_pPacket; }
		void					SetPacket(RakNet::Packet * val) { m_pPacket = val; }
		RakNet::SystemAddress	GetId() const { return m_pID; }
		void					SetId(RakNet::SystemAddress val) { m_pID = val; }

	private:
		RakNet::SystemAddress	m_pID;
		RakNet::Packet		*	m_pPacket;
	};

	CErrno NetHandlerListener::OnMsgRecvingRakNet()
	{
		RakNetContext * pContext = (RakNetContext *)(GetSession()->GetContext());
		RakNet::Packet * pPacket = pContext->GetPacket();
		if (pPacket)
		{
			unsigned char ucPacketIdentifier;
			ucPacketIdentifier = NetHelper::GetPacketIdentifier(pPacket);
			if (ucPacketIdentifier == ID_NEW_INCOMING_CONNECTION)
			{
				RakNet::SystemAddress ClientID = pPacket->systemAddress;

				OnAccept(&ClientID);
				pContext->SetPacket(NULL);
			}
			return CErrno::Success();
		}

		return CErrno::Failure();
	}

	INT32 NetHandlerClient::ConnectRakNet(const char* ip, int usPort)
	{
		if (ip == NULL)
		{
			return -1;
		}

		char * pPassword = NULL;      //5 这个其实就是一开始发送了一个验证密码.
		RakNet::RakPeerInterface * pRakPeerInstance = ((NetReactorRakNet *)(m_pNetReactor))->GetRakPeerInterface();
#if LIBCAT_SECURITY == 1
		char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
		FILE *fp = fopen("publicKey.dat", "rb");
		fread(public_key, sizeof(public_key), 1, fp);
		fclose(fp);
#endif

#if LIBCAT_SECURITY == 1
		RakNet::PublicKey pk;
		pk.remoteServerPublicKey = public_key;
		pk.publicKeyMode = RakNet::PKM_USE_KNOWN_PUBLIC_KEY;
		RakNet::ConnectionAttemptResult car = pRakPeerInstance->Connect(ip, usPort, pPassword, 0, &pk);
#else
		RakNet::ConnectionAttemptResult car = pRakPeerInstance->Connect(ip, usPort , pPassword, 0);
#endif 
		if (car != RakNet::CONNECTION_ATTEMPT_STARTED)
		{
			gErrorStream("Connect failure ip:" << m_pSession->GetRemoteName() << "socket:" << socket);
			return -1;
		}

		RakNetContext * pContext = new RakNetContext;

		RakNet::SystemAddress address;
		DataStructures::List< RakNet::RakNetSocket2* > sockets;
		pRakPeerInstance->GetSockets(sockets);
		for (unsigned int i = 0; i < sockets.Size(); i++)
		{
			address = sockets[i]->GetBoundAddress();
		}
		pContext->SetId(address);

		m_pSession->SetContext(pContext);
		return  0;
	}

	void NetHandlerListener::OnAccept(RakNet::SystemAddress * pAddress)
	{
		if (m_pNetReactor)
		{
			sockaddr_in * addr = &(pAddress->address.addr4);
			pAddress->address;
			char szAddress[MAX_NAME_LENGTH];
			UINT16 usPort = 0;
			Net::NetHelper::GetAddressAndPortByAddrIn(addr, szAddress, usPort);
			std::string strName = "";
			if (m_pNetReactor && m_pNetReactor->GetNetThread())
			{
				strName = m_pNetReactor->GetNetThread()->GetNetNodeName();
			}
			NetSocket socket = 1;  //5 raknet这里用不到socket

			ServerSession * pServerSession = new ServerSession(szAddress, usPort, strName, "", -1, NET_STATE_CONNECTED, socket);
			NetHandlerServerPtr pServer(new NetHandlerServer(m_pNetReactor, pServerSession));

			RakNetContext * pContext = (RakNetContext *)(GetSession()->GetContext());
			if (!pContext)
			{
				pContext = new RakNetContext;
				pContext->SetId(*pAddress);
			}
			m_pNetReactor->AddNetHandler(pServer);
		}
	}

	CErrno NetHandlerClient::InitRakNet()
	{
		RakNetContext * pContext = new RakNetContext;
		m_pSession->SetContext(pContext);

		RakNet::SocketDescriptor socketDescriptor(0 , 0);
		socketDescriptor.socketFamily = AF_INET;
		RakNet::RakPeerInterface * pInterface = ((NetReactorRakNet*)m_pNetReactor)->GetRakPeerInterface();
		if (pInterface)
		{
			pInterface->AllowConnectionResponseIPMigration(false);
			pInterface->Startup(DEFAULT_MAX_CONNECTION_COUNT, &socketDescriptor, 1);
			pInterface->SetOccasionalPing(true);
		}

		return CErrno::Success();
	}

	CErrno NetHandlerServer::InitRakNet()
	{
		RakNetContext * pContext = new RakNetContext;
		m_pSession->SetContext(pContext);

		return CErrno::Success();
	}
	
	INT32 NetHandlerTransit::RecvMsgRakNet(char * pBuf, UINT32 unSize)
	{
		INT32 nBufSize = 0;
		RakNetContext * pContext = (RakNetContext *)m_pSession->GetContext();
		if (pContext)
		{
			INT32 nRecvFD = 0;
			nBufSize = NetHelper::RecvMsg(pContext->GetPacket(), pBuf, unSize);
		}

		return nBufSize;
	}

	//////////////////////////////////////////////////////////////////////////
	NetReactorRakNet::NetReactorRakNet(UINT32 unMaxConnectionCount , NetThread * pThread/* = NULL*/)
		: INetReactor(REACTOR_TYPE_RAKNET , pThread)
		, m_pRakPeerInstance(NULL)
		, m_unMaxConnectionCount(unMaxConnectionCount)
	{
		m_pRakPeerInstance = RakNet::RakPeerInterface::GetInstance();
	}

	NetReactorRakNet::~NetReactorRakNet( void )
	{
		m_pRakPeerInstance->Shutdown(300);
		if (m_pRakPeerInstance)
		{
			RakNet::RakPeerInterface::DestroyInstance(m_pRakPeerInstance);
		}
	}

	CErrno NetReactorRakNet::Init( void )
	{
		//m_pRakPeerInstance->InitializeSecurity(0,0,0,0);
		//RakNet::PacketLogger packetLogger;
		//m_pRakPeerInstance->AttachPlugin(&packetLogger);

		return CErrno::Success();
	}

	CErrno NetReactorRakNet::InitAccept(ISession * pSession)
	{
		if (pSession)
		{
			RakNet::SocketDescriptor socketDescriptor(0, 0);
			socketDescriptor.port = pSession->GetPort();
			socketDescriptor.socketFamily = AF_INET;
			RakNet::StartupResult  objResult = m_pRakPeerInstance->Startup(m_unMaxConnectionCount, &socketDescriptor, 1);

			if (objResult != RakNet::RAKNET_STARTED)
			{
				return CErrno::Failure();
			}
			m_pRakPeerInstance->AllowConnectionResponseIPMigration(false);
			m_pRakPeerInstance->SetMaximumIncomingConnections(m_unMaxConnectionCount);
			m_pRakPeerInstance->SetOccasionalPing(true);
// 			m_pRakPeerInstance->SetIncomingPassword(pPassword , 0);
//			m_pRakPeerInstance->SetUnreliableTimeout(1000);

			RakNet::SystemAddress address;
			DataStructures::List< RakNet::RakNetSocket2* > sockets;
			m_pRakPeerInstance->GetSockets(sockets);
			for (unsigned int i = 0; i < sockets.Size(); i++)
			{
				address = sockets[i]->GetBoundAddress();
			}

			RakNetContext * pContext = (RakNetContext *)(pSession->GetContext());
			pContext->SetId(address);

			return CErrno::Success();
		}
		return CErrno::Failure();
	}

	Net::INetHandlerPtr NetReactorRakNet::GetNetHandler(const RakNet::SystemAddress & address)
	{
		MapNetHandlersT::iterator iter = m_mapNetHandlers.find(RakNet::SystemAddress::ToInteger(address));
		if (iter != m_mapNetHandlers.end())
		{
			return iter->second;
		}

		return INetHandlerPtr(NULL);
	}

	Net::INetHandlerPtr NetReactorRakNet::GetListenerNetHandler()
	{
		RakNet::SystemAddress address;
		DataStructures::List< RakNet::RakNetSocket2* > sockets;
		m_pRakPeerInstance->GetSockets(sockets);
		for (unsigned int i = 0; i < sockets.Size(); i++)
		{
			address = sockets[i]->GetBoundAddress();
		}
		
		return GetNetHandler(address);
	}

	CErrno NetReactorRakNet::CloseConnection(const RakNet::SystemAddress & address)
	{
		m_pRakPeerInstance->CloseConnection(address, true, 0);

		return CErrno::Success();
	}

	CErrno NetReactorRakNet::Cleanup(void)
	{
		m_pRakPeerInstance->Shutdown(300);

		return INetReactor::Cleanup();
	}

	CErrno NetReactorRakNet::Update( void )
	{
		RakNet::Packet * pPacket = NULL;
		for (pPacket = m_pRakPeerInstance->Receive(); pPacket; m_pRakPeerInstance->DeallocatePacket(pPacket), pPacket = m_pRakPeerInstance->Receive())
		{
			// We got a packet, get the identifier with our handy function
			unsigned char ucPacketIdentifier;
			ucPacketIdentifier = NetHelper::GetPacketIdentifier(pPacket);

			bool bClosed = false;

			INetHandlerPtr pNetHandler;
			if (ucPacketIdentifier == ID_NEW_INCOMING_CONNECTION)
			{
				pNetHandler = GetListenerNetHandler();
				RakNetContext * pContext = (RakNetContext *)(pNetHandler->GetSession()->GetContext());
				pContext->SetPacket(pPacket);
				bClosed = !pNetHandler->OnMsgRecving().IsSuccess() || bClosed;
				pContext->SetPacket(NULL);
			}
			else
			{
				pNetHandler = GetNetHandler(pPacket->systemAddress);
			}
			if (!pNetHandler || pNetHandler->GetSession())
			{
				continue;
			}
			ISession * pSession = pNetHandler->GetSession();

			// Check if this is a network message packet
			switch (ucPacketIdentifier)
			{
				case ID_DISCONNECTION_NOTIFICATION:
					// Connection lost normally
					printf("ID_DISCONNECTION_NOTIFICATION from %s\n", pPacket->systemAddress.ToString(true));;
					break;
				case ID_INCOMPATIBLE_PROTOCOL_VERSION:
					printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
					break;

				case ID_CONNECTED_PING:
				case ID_UNCONNECTED_PING:
					printf("Ping from %s\n", pPacket->systemAddress.ToString(true));
					break;
				case ID_CONNECTION_LOST:
					// Couldn't deliver a reliable packet - i.e. the other system was abnormally
					// terminated
					printf("ID_CONNECTION_LOST from %s\n", pPacket->systemAddress.ToString(true));;

					bClosed = TRUE;
					break;
				case ID_ALREADY_CONNECTED:
					// Connection lost normally
//					printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", pPacket->guid);
					pNetHandler->GetSession()->SetNetState(NET_STATE_CONNECTED);
					break;
				case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
					printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
					break;
				case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
					printf("ID_REMOTE_CONNECTION_LOST\n");
					bClosed = TRUE;
					break;
				case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
					printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
					break;
				case ID_CONNECTION_BANNED: // Banned from this server
					printf("We are banned from this server.\n");
					break;
				case ID_CONNECTION_ATTEMPT_FAILED:
					printf("Connection attempt failed\n");
					break;
				case ID_NO_FREE_INCOMING_CONNECTIONS:
					// Sorry, the server is full.  I don't do anything here but
					// A real app should tell the user
					printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
					break;

				case ID_INVALID_PASSWORD:
					printf("ID_INVALID_PASSWORD\n");
					break;
				case ID_CONNECTION_REQUEST_ACCEPTED:
					// This tells the client they have connected
					printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", pPacket->systemAddress.ToString(true), pPacket->guid.ToString());
					printf("My external address is %s\n", m_pRakPeerInstance->GetExternalID(pPacket->systemAddress).ToString(true));
					break;
				case ID_DEFAULT_RAKNET_USER_PACKET:
				{
					RakNetContext * pContext = (RakNetContext *)(pNetHandler->GetSession()->GetContext());
					MsgAssert_ReF(pContext , "NULL RakNet Context.");
					pContext->SetPacket(pPacket);

					bClosed = !pNetHandler->OnMsgRecving().IsSuccess() || bClosed;

					pContext->SetPacket(NULL);
					m_pRakPeerInstance->DeallocatePacket(pPacket);
				}
				default:
				{	// It's a client, so just show the message
					printf("%s\n", pPacket->data);
					break;
				}

				if (pSession && pSession->GetObjTimeout().IsExpired() || pSession->IsClosed())
				{
					bClosed = TRUE;
				}

				if (bClosed)
				{
					gDebugStream("delete curNodeName=" << pNetHandler->GetSession()->GetCurNodeName() << ":remoteName=" << pNetHandler->GetSession()->GetRemoteName() << ":address=" << pNetHandler->GetSession()->GetAddress() << ":port=" << pNetHandler->GetSession()->GetPort());
					DelNetHandler(pNetHandler, FALSE);
				}
			}

		}

		return CErrno::Success();
	}

	CErrno NetReactorRakNet::AddNetHandler( INetHandlerPtr  pNetHandler  , ENetHandlerFuncMask objMask /*= NET_FUNC_DEFAULT*/ )
	{
		Assert_ReF(pNetHandler->GetSession());
	
		if (objMask & NET_FUNC_ACCEPT)
		{
			if (!InitAccept(pNetHandler->GetSession()).IsSuccess())
			{
				return CErrno::Failure();
			}
		}

		RakNetContext * pContext = (RakNetContext *)(pNetHandler->GetSession()->GetContext());
		if (pContext)
		{
			RakNet::SystemAddress address = (pContext->GetId());
			m_mapNetHandlers.insert(std::make_pair(RakNet::SystemAddress::ToInteger(address), pNetHandler));

			return INetReactor::AddNetHandler(pNetHandler, objMask);
		}

		return CErrno::Failure();
	}

	CErrno NetReactorRakNet::DelNetHandler( INetHandlerPtr  pNetHandler , BOOL bEraseHandler/* = TRUE */)
	{
		RakNetContext * pContext = (RakNetContext *)(pNetHandler->GetSession()->GetContext());
		RakNet::SystemAddress address = (pContext->GetId());

		MapNetHandlersT::iterator iter = m_mapNetHandlers.find(RakNet::SystemAddress::ToInteger(address));
		if (iter != m_mapNetHandlers.end())
		{
			m_mapNetHandlers.erase(iter);
		}

		CloseConnection(address);
		pNetHandler->OnClose();

		return INetReactor::DelNetHandler(pNetHandler, bEraseHandler);
	}

	CErrno NetReactorRakNet::ModNetHandler( INetHandlerPtr  pNetHandler  , ENetHandlerFuncMask objMask )
	{

		return INetReactor::ModNetHandler(pNetHandler, objMask);
	}


}