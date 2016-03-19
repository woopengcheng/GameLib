extern "C"
{
#include "zmq.h" 
}
#include "NetLib/inc/NetHandlerServer.h"
#include "NetLib/inc/ISession.h"
#include "NetLib/inc/NetHelper.h" 
#include "NetLib/inc/INetReactor.h" 
#include "NetLib/inc/NetReactorUDP.h" 
#include "CUtil/inc/CUtil.h"

namespace Net
{

	NetHandlerServer::NetHandlerServer(INetReactor * pNetReactor, ISession * pSession)
		: NetHandlerPing(pNetReactor, pSession)
		, m_pZmqContext(NULL)
		, m_pZmqMsg(NULL)
		, m_pZmqSocket(NULL)
	{
		Assert(m_pSession);
		switch (m_pSession->GetReactorType())
		{
			case REACTOR_TYPE_ZMQ:
			{
				InitZMQ();
			}break;
			case REACTOR_TYPE_UDP:
			{
				InitUDP();
			}break;
			case REACTOR_TYPE_UDS:
			{
				InitUDS();
			}break;
			case REACTOR_TYPE_RAKNET:
			{
				InitRakNet();
			}break;
			default:
				break;
		}
	}

	CErrno NetHandlerServer::InitUDP()
	{
		struct sockaddr_in address;

		memset(&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(m_pSession->GetPort());

		Net::UDPContext * pContext = new Net::UDPContext;
		pContext->SetPeerAddr(address);

		m_pSession->SetContext(pContext);

		NetSocket socket = m_pSession->GetSocket();
		if (socket == -1)
		{
			socket = NetHelper::CreateSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			m_pSession->SetSocket(socket);

			NetHelper::SetDefaultSocket(socket , m_pSession->GetSendBufSize() , m_pSession->GetRecvBufSize());

			bind(socket, (struct sockaddr *)&address, sizeof(address));
		}

		return CErrno::Success();
	}

	CErrno NetHandlerServer::InitUDS()
	{
		Net::UDSContext * pContext = new Net::UDSContext;
		m_pSession->SetContext(pContext);

		return CErrno::Success();
	}

	NetHandlerServer::~NetHandlerServer()
	{
	}

	CErrno NetHandlerServer::OnClose( void )
	{ 
		return NetHandlerPing::OnClose();
	}

	CErrno NetHandlerServer::Update(void)
	{
		return NetMsgQueue::Update();		//5 这里是不需要Ping
	}

	CErrno NetHandlerServer::Init(const std::string & ip, int port)
	{
		m_pSession->SetAddress(ip);
		m_pSession->SetSocktPort(port);

		std::string str = "tcp://";
		str += ip;
		str += ":";
		str += CUtil::itoa(port);

		MsgAssert_ReF(!zmq_bind(m_pZmqSocket, str.c_str()), zmq_strerror(errno));

		return NetHandlerPing::Init();
	}
	
	CErrno NetHandlerServer::InitZMQ()
	{
		void * pContext = zmq_init(1);
		MsgAssert_ReF(pContext, zmq_strerror(errno) << "error in zmq_init");

		m_pZmqContext = pContext;

		void * pSocket = zmq_socket(pContext, ZMQ_PULL);
		MsgAssert_ReF(pSocket, zmq_strerror(errno) << "error in zmq_socket");

		m_pZmqSocket = pSocket;

		m_pZmqMsg = new zmq_msg_t;

		return Init(m_pSession->GetAddress(), m_pSession->GetPort());
	}

	CErrno  NetHandlerServer::HandleMsg(ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength)
	{
		printf("%s", pBuffer);

		if (pSession->GetReactorType() != REACTOR_TYPE_UDP && pSession->GetReactorType() != REACTOR_TYPE_ZMQ && !m_pNetReactor->IsMutilThread())
		{
			char pBuf[1024];

			const char * kSendMsg = "server sendmsg.\n";
			UINT32 unLen = (UINT32)(strlen(kSendMsg) + 1);

			((Net::MsgHeader*)pBuf)->unMsgID = 0;
			((Net::MsgHeader*)pBuf)->unMsgLength = unLen + sizeof(Net::MsgHeader);
			memcpy(pBuf + sizeof(Net::MsgHeader), kSendMsg , unLen);
			SendMsg(pBuf, unLen + sizeof(Net::MsgHeader));
		}

		return CErrno::Success();
	}

}