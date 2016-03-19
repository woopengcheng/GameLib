#include "NetLib/inc/RakNetUDPManager.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "NetLib/inc/MsgWrapper.h"
#include "MessageIdentifiers.h"

namespace Net
{
	RaknetUDPManager::RaknetUDPManager()		
	{
		m_pRakPeerInstance = RakNet::RakPeerInterface::GetInstance();
	}

	RaknetUDPManager::~RaknetUDPManager()
	{
		RakNet::RakPeerInterface::DestroyInstance(m_pRakPeerInstance);
	}

	CErrno RaknetUDPManager::Init( void )
	{
//5		m_pRakPeerInstance->InitializeSecurity(0,0,0,0);
//5		RakNet::PacketLogger packetLogger;
//5     m_pRakPeerInstance->AttachPlugin(&packetLogger);
		m_pRakPeerInstance->AllowConnectionResponseIPMigration(false);
		
		RakNet::SocketDescriptor socketDescriptor(0 , 0);
		socketDescriptor.socketFamily = AF_INET;
		RakNet::StartupResult  objResult = m_pRakPeerInstance->Startup(DEFAULT_LISTENER_COUNT , &socketDescriptor , 1);
		
		if (objResult != RakNet::RAKNET_STARTED)
		{
			return CErrno::Failure();
		}
		m_pRakPeerInstance->SetOccasionalPing(true);
		
		return CErrno::Success();
	}

	CErrno RaknetUDPManager::Cleanup( void )
	{
		m_pRakPeerInstance->Shutdown(300);

		m_bConnected = FALSE;

		return CErrno::Success();
	}

	CErrno RaknetUDPManager::Connect( char* pIPAddress, UINT16 usPort )
	{ 
		if (pIPAddress == NULL)
		{
			return CErrno::Failure();
		}

		char * pPassword = NULL;      //5 这个其实就是一开始发送了一个验证密码.

#if LIBCAT_SECURITY == 1
		char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
		FILE *fp = fopen("publicKey.dat","rb");
		fread(public_key,sizeof(public_key),1,fp);
		fclose(fp);
#endif

#if LIBCAT_SECURITY == 1
		RakNet::PublicKey pk;
		pk.remoteServerPublicKey=public_key;
		pk.publicKeyMode=RakNet::PKM_USE_KNOWN_PUBLIC_KEY;
		RakNet::ConnectionAttemptResult car = m_pRakPeerInstance->Connect(pIPAddress , usPort , pPassword, 0 , &pk);	
#else
		RakNet::ConnectionAttemptResult car = m_pRakPeerInstance->Connect(pIPAddress , usPort , pPassword, 0 );
#endif 
        if(car != RakNet::CONNECTION_ATTEMPT_STARTED)
		{
			return CErrno::Failure();
		}
		 
		return CErrno::Success();
	}

	CErrno RaknetUDPManager::CloseConnection( void )
	{
		return CloseConnection(0);
	}

	CErrno RaknetUDPManager::CloseConnection( UINT32 unIndex )
	{
		m_pRakPeerInstance->CloseConnection(m_pRakPeerInstance->GetSystemAddressFromIndex(unIndex) , true);

		m_bConnected = FALSE;

		return CErrno::Success();
	}

	CErrno RaknetUDPManager::SendMsg( MsgWrapper* pMsg )
	{
		UINT32 unPacketSize = pMsg->GetMsgSize() + sizeof(pMsg->GetMsgID());
		RakNet::BitStream bs;
		bs.Write((RakNet::MessageID)ID_DEFAULT_RAKNET_USER_PACKET);
		bs.Write(unPacketSize);
		bs.Write(pMsg->GetMsgID());
		bs.Write(pMsg->GetMsg());
			
		UINT32 unResult = m_pRakPeerInstance->Send(&bs, HIGH_PRIORITY , RELIABLE_ORDERED, 0, m_pRakPeerInstance->GetSystemAddressFromIndex(0), false);
		if (unResult == 0)
			return CErrno::Failure();
		else
			return CErrno::Success();
	}

	MsgWrapper * RaknetUDPManager::RecvMsg( void )
	{
		RakNet::Packet * pPacket = NULL;
		unsigned char ucPacketIdentifier;
		for (pPacket = m_pRakPeerInstance->Receive(); pPacket; m_pRakPeerInstance->DeallocatePacket(pPacket), pPacket = m_pRakPeerInstance->Receive())
		{
			// We got a packet, get the identifier with our handy function
			ucPacketIdentifier= GetPacketIdentifier(pPacket);

			// Check if this is a network message packet
			switch (ucPacketIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
//				printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", pPacket->guid);
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n"); 
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_CONNECTION_LOST\n");
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

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", pPacket->systemAddress.ToString(true), pPacket->guid.ToString());
				printf("My external address is %s\n", m_pRakPeerInstance->GetExternalID(pPacket->systemAddress).ToString(true));
				break;
			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				printf("Ping from %s\n", pPacket->systemAddress.ToString(true));
				break;
			case ID_DEFAULT_RAKNET_USER_PACKET:
				{
					MsgWrapper * pMsgWrapper = RecvMsgInStream(pPacket);
					m_pRakPeerInstance->DeallocatePacket(pPacket);
					return  pMsgWrapper;
				}
			default:
				// It's a client, so just show the message
				printf("%s\n", pPacket->data);
				break;
			}
		}

		return NULL;
	}

	void RaknetUDPManager::Run( void )
	{

	}

	unsigned char RaknetUDPManager::GetPacketIdentifier( RakNet::Packet * pPacket )
	{
		if (!pPacket)
			return 255;

		if ((unsigned char)pPacket->data[0] == ID_TIMESTAMP)
		{
			RakAssert(pPacket->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
			return (unsigned char) pPacket->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
		}
		else
			return (unsigned char) pPacket->data[0];
	} 

	MsgWrapper * RaknetUDPManager::RecvMsgInStream( RakNet::Packet * pPacket )
	{
		RakNet::MessageID objMsgID;
		MsgHeader header;

		RakNet::BitStream bs(pPacket->data , pPacket->bitSize , false);
		bs.Read(objMsgID);
		bs.Read(header.unMsgLength);
		bs.Read(header.unMsgID);

		UINT32 unMsgSize = header.unMsgLength - sizeof(header.unMsgID);
		RakAssert(unMsgSize > MAX_MESSAGE_LENGTH); 
		RakAssert(header.unMsgID > 0); 
		 
		char * pMsg = new char[unMsgSize]; //5 这里在内部做的申请内存.需要在外部来释放.
		bs.Read(pMsg); 

		return new MsgWrapper(header.unMsgID , pMsg , unMsgSize);
	}

}