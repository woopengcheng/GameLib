extern "C"
{
#include "zmq.h" 
}
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"
#include "NetLib/inc/NetHandlerTransit.h"
#include "NetLib/inc/NetHelper.h"
#include "NetLib/inc/ByteOrder.h"
#include "NetLib/inc/ISession.h"
#include "NetLib/inc/NetReactorUDP.h"
#include "NetLib/inc/NetReactorRakNet.h"
#include "NetLib/inc/INetReactor.h"
#include "NetLib/inc/NetThread.h"
#include "Timer/inc/TimerHelp.h"

#ifdef WIN32
#include "Winsock.h"
#endif

namespace Net
{
	NetHandlerTransit::NetHandlerTransit( INetReactor * pNetReactor , ISession * pSession )
		: INetHandler(pNetReactor, pSession)
		, m_pZmqContext(NULL)
		, m_pZmqMsg(NULL)
		, m_pZmqSocket(NULL)
	{
		m_objRecvBuf.Init(DEFAULT_CIRCLE_BUFFER_SIZE);
 		m_objSendBuf.Init(DEFAULT_CIRCLE_BUFFER_SIZE);

		if (m_pSession && m_pNetReactor)
		{
			m_pSession->SetReactorType(m_pNetReactor->GetReactorType());
		}
	}

	NetHandlerTransit::~NetHandlerTransit( void )
	{
		m_objSendBuf.Cleanup();
		m_objRecvBuf.Cleanup();

		if (m_pNetReactor->GetReactorType() == REACTOR_TYPE_ZMQ)
		{
			MsgAssert(zmq_close(m_pZmqSocket), "error in zmq_close" << zmq_strerror(errno));

			MsgAssert(zmq_term(m_pZmqContext), "error in zmq_term:" << zmq_strerror(errno));

			SAFE_DELETE(m_pZmqMsg);
		}
	}

	CErrno NetHandlerTransit::OnMsgRecving( void )
	{
		char szBuf[DEFAULT_CIRCLE_BUFFER_SIZE];  

		NetSocket socket  = m_pSession->GetSocket();
		INT32  nBufSize = 0;
		UINT32 unMaxBufSize = sizeof(szBuf);

		do
		{
			switch (m_pSession->GetReactorType())
			{
				case REACTOR_TYPE_UDP:
				{
					UDPContext * pContext = (UDPContext *)m_pSession->GetContext();
					if (pContext)
					{
						INT32 nRecLen = sizeof(pContext->GetPeerAddr());
						nBufSize = NetHelper::RecvMsg(socket, szBuf, sizeof(szBuf), (sockaddr*)&(pContext->GetPeerAddr()), &nRecLen);
					}
				}break;
				case REACTOR_TYPE_UDS:
				case REACTOR_TYPE_UDSEX:
				{
					UDSContext * pContext = (UDSContext *)m_pSession->GetContext();
					if (pContext)
					{
						INT32 nRecvFD = 0;
						nBufSize = NetHelper::RecvMsg(socket, szBuf, sizeof(szBuf), nRecvFD);
						pContext->SetTransferFD(nRecvFD);
					}
				}break;
				case REACTOR_TYPE_RAKNET:
				{
					nBufSize = RecvMsgRakNet(szBuf, sizeof(szBuf));
				}break;
				case REACTOR_TYPE_ZMQ:
				{
					INT32 nResult = zmq_msg_init(m_pZmqMsg);
					if (nResult != 0)
					{
						gErrorStream("error in zmq_msg_init: %s\n" << zmq_strerror(errno));
						return CErrno::Failure();
					}

					nBufSize = NetHelper::RecvMsg(m_pZmqSocket , m_pZmqMsg , szBuf, sizeof(szBuf));
				}break;
				default:
				{
					nBufSize = NetHelper::RecvMsg(socket, szBuf, sizeof(szBuf));
				}break;
			}

			if( nBufSize <  0 && NetHelper::IsSocketEagain() && m_pNetReactor->GetReactorType() != REACTOR_TYPE_ZMQ)
				return CErrno::Success();
			if( nBufSize <= 0 )
				return CErrno::Failure();

			CErrno result = OnMsgRecving(szBuf , nBufSize);
			if( !result.IsSuccess() )
				return result;

			if (m_pNetReactor->GetReactorType() == REACTOR_TYPE_ZMQ)
			{
				INT32 nResult = zmq_msg_close(m_pZmqMsg);
				if (nResult != 0)
				{
					gErrorStream("error in zmq_msg_close: %s" << zmq_strerror(errno));
					return CErrno::Failure();
				}
			}
		}while(0);

		return CErrno::Success();
	}

	CErrno NetHandlerTransit::OnMsgRecving(const char * pBuf , UINT32 unSize)
	{
		return RecvToCircleBuffer( pBuf , unSize);
	}

	CErrno NetHandlerTransit::RecvToCircleBuffer(const char * pBuf, UINT32 unSize)
	{
		if(m_objRecvBuf.GetSpace() > unSize)
		{
			if (m_pSession)
			{
				m_pSession->OnRecvMsg();
			} 

			m_objRecvBuf.PushBuffer(pBuf , unSize);
			ParaseRecvMsg();
		}
		else
		{
			MsgAssert_Re(0 , CErrno::Failure() , "buffer full.");  
			this->m_objRecvBuf.SkipBytes(m_objRecvBuf.GetDataLength());
			return CErrno::Failure();
		}
		return CErrno::Success();
	}

	CErrno NetHandlerTransit::ParaseRecvMsg()
	{
		while(m_objRecvBuf.GetDataLength() > 0)
		{
			UINT32 unMsgLength = 0; 

			INT32 nRecvBuf = (size_t)m_objRecvBuf.TryGetBuffer((char*)&unMsgLength , sizeof(UINT32));

			if(nRecvBuf < sizeof(UINT32))
			{
				gErrorStream( "parase msg header failed.");
				return CErrno::Failure(); 
			}

//			Convert<UINT32>::ToHostOrder(unMsgLength);
			 
			if(unMsgLength > MAX_MESSAGE_LENGTH || unMsgLength <= 0)
			{
				gErrorStream( "error package len ,discard connection");
				return CErrno::Failure();   
			}

			if(m_objRecvBuf.GetDataLength() < unMsgLength)
				return CErrno::Success();

			char szBuf[MAX_MESSAGE_LENGTH];
			m_objRecvBuf.GetBuffer(szBuf , unMsgLength);
			 
			HandleMsg(szBuf, unMsgLength);
		}

		return CErrno::Success();
	}

	CErrno NetHandlerTransit::OnMsgSending( void )
	{
		if (m_pSession)
		{
			m_pSession->SetCanWrite(TRUE);
		}
		FlushSendBuffer();
		return CErrno::Success();
	}

	INT32 NetHandlerTransit::FlushSendBuffer( void )
	{ 
		INT32  nSendBytes = 0;

		if (m_pSession)
		{
			m_pSession->OnSendMsg();
		}

		while(true)
		{
			if(!m_objSendBuf.IsVaild() || m_objSendBuf.GetDataLength() <= 0 || !m_pSession->IsCanWrite())
				break;

			char   szBuf[DEFAULT_CIRCLE_BUFFER_SIZE];
			INT32  nLength = m_objSendBuf.TryGetBuffer(szBuf,sizeof(szBuf));

			INT32 nSendLength = Send(szBuf, nLength);

// #ifndef WIN32
// 			INT32 nSendLength = ::send(m_pSession->GetSocket(),szBuf,nLength,MSG_DONTWAIT);
// #else
// 			INT32 nSendLength = ::send(m_pSession->GetSocket(),szBuf,nLength,0);
// #endif 
			if( nSendLength > 0 )
			{ 
				nSendBytes += nSendLength;
				m_objSendBuf.SkipBytes(nSendLength);
			}
			else if(nSendLength < 0 && NetHelper::IsSocketEagain())
			{
				if (m_pSession)
				{
					m_pSession->SetCanWrite(FALSE);
				}
				break;
			}
			else
			{ 
				gErrorStream("send error , close it.");
				m_pSession->SetClosed(TRUE); 
				break;
			}
		}

		return nSendBytes;  
	} 

	INT32 NetHandlerTransit::SendMsg(const char * pBuf, UINT32 unSize)
	{ 
		if (!m_pSession || m_pSession->GetNetState() != NET_STATE_CONNECTED)
		{
			return -1;
		}

		if(m_objSendBuf.IsVaild() && m_objSendBuf.GetDataLength() >= 0)
		{
			if(m_objSendBuf.GetSpace() > unSize)
			{
				m_objSendBuf.PushBuffer(pBuf , unSize);
				FlushSendBuffer();
			}
			else
			{
				FlushSendBuffer();

				if(m_objSendBuf.GetSpace() > unSize)
				{
					m_objSendBuf.PushBuffer(pBuf , unSize);
				}
				else if(m_objSendBuf.GetDataLength() == 0)
				{
					INT32 nSendBytes = Send(pBuf  , unSize);
					if( nSendBytes <= 0)
					{
						gErrorStream("sendbuffer.length=0,direct send failed");
						return -1;
					}
					else if(nSendBytes != unSize)
					{
						gErrorStream("sendbuffer.length=0,direct send failed");
						return -1; 
					}

					return (nSendBytes > 0) ? nSendBytes : -1;
				}
				else
				{
					gErrorStream("sendbuff full");
					return -1;  
				}
			}
			return unSize;
		}

		return Send(pBuf , unSize);
	}

	INT32 NetHandlerTransit::SendUDS(const char * pBuf, UINT32 unSize)
	{
		UDSContext * pContext = (UDSContext *)(m_pSession->GetContext());
		if (pContext == NULL)
		{
			return -1;
		}
		INT32 send_fd = send_fd = pContext->GetTransferFD();
#ifdef _LINUX
		int  ret;
		struct  msghdr msg;
		struct  cmsghdr *p_cmsg;
		struct  iovec vec;
		char  cmsgbuf[CMSG_SPACE(sizeof(send_fd))];
		int  *p_fds;
		char  sendchar = 0;
		msg.msg_control = cmsgbuf;
		msg.msg_controllen = sizeof(cmsgbuf);
		p_cmsg = CMSG_FIRSTHDR(&msg);
		p_cmsg->cmsg_level = SOL_SOCKET;
		p_cmsg->cmsg_type = SCM_RIGHTS;
		p_cmsg->cmsg_len = CMSG_LEN(sizeof(send_fd));
		p_fds = (int  *)CMSG_DATA(p_cmsg);
		*p_fds = send_fd;  // 通过传递辅助数据的方式传递文件描述符 

		msg.msg_name = NULL;
		msg.msg_namelen = 0;
		msg.msg_iov = &vec;
		msg.msg_iovlen = 1;  //主要目的不是传递数据，故只传1个字符 
		msg.msg_flags = 0;

		vec.iov_base = &sendchar;
		vec.iov_len = sizeof(sendchar);
		ret = sendmsg(sock_fd, &msg, 0);
		if (ret != 1)
			ERR_EXIT("sendmsg");
#endif
		return -1;
	}

	INT32 NetHandlerTransit::SendRakNet(const char * pBuf, UINT32 unSize)
	{
		RakNet::BitStream bs;
		bs.Write((RakNet::MessageID)ID_DEFAULT_RAKNET_USER_PACKET);
		bs.Write(unSize);
		bs.Write(pBuf);
		RakNet::RakPeerInterface * pRakPeerInstance = ((NetReactorRakNet *)(this->GetNetReactor()))->GetRakPeerInterface();
		if (pRakPeerInstance)
		{
			UINT32 unResult = pRakPeerInstance->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pRakPeerInstance->GetSystemAddressFromIndex(0), false);
			if (unResult == 0)
				return -1;
			
			return unResult;
		}

		return -1;
	}

	INT32 NetHandlerTransit::SendZMQ(const char * pBuf, UINT32 unSize)
	{
		int nResult = zmq_msg_init_data(m_pZmqMsg, (void *)pBuf, unSize, NULL, NULL);
		if (nResult != 0)
		{
			gErrorStream("error in zmq_msg_init_size: %s\n" << zmq_strerror(errno));
			return -1;
		}

		int nCount = zmq_sendmsg(m_pZmqSocket, m_pZmqMsg, 0);
		if (nCount < 0)
		{
			gErrorStream("error in zmq_sendmsg: %s\n" << zmq_strerror(errno));
			return -1;
		}

		nResult = zmq_msg_close(m_pZmqMsg);
		if (nResult != 0) {
			printf("error in zmq_msg_close: %s\n", zmq_strerror(errno));
			return -1;
		}

		return nCount;
	}

	INT32 NetHandlerTransit::Send(const char * pBuf, UINT32 unSize)
	{ 
		if (m_pSession)
		{
			switch (m_pSession->GetReactorType())
			{
				case REACTOR_TYPE_IOCP:
				{
					return SendIOCP(pBuf, unSize);
				}break;
				case REACTOR_TYPE_ZMQ:
				{
					return SendZMQ(pBuf, unSize);
				}break;
				case REACTOR_TYPE_UDP:
				{
					return SendTo(pBuf , unSize);
				}break;
				case REACTOR_TYPE_UDS:
				case REACTOR_TYPE_UDSEX:
				{
					return SendUDS(pBuf, unSize);
				}break;
				case REACTOR_TYPE_RAKNET:
				{
					return SendRakNet(pBuf, unSize);
				}break;
				default:
				{
					return SendCommon(pBuf, unSize);
				}break;
			}
		}
		return -1;
	}

	INT32 NetHandlerTransit::SendIOCP(const char * pBuf, UINT32 unSize)
	{
		return NetHelper::WSASend(this, pBuf , unSize);
	}

	INT32 NetHandlerTransit::SendCommon(const char * pBuf, UINT32 unSize)
	{
		if (m_pSession && !m_pSession->IsCanWrite())
		{
			return -1;
		}

		UINT32 unTotalSendBytes = 0;
		while ((unSize > unTotalSendBytes))
		{
#ifdef WIN32
			int nSendBytes = ::send(m_pSession->GetSocket(), &pBuf[unTotalSendBytes], unSize - unTotalSendBytes, 0);
#else
			int nSendBytes = ::send(m_pSession->GetSocket(), &pBuf[unTotalSendBytes], unSize - unTotalSendBytes, MSG_DONTWAIT);
#endif
			if (nSendBytes > 0)
			{
				unTotalSendBytes += nSendBytes;
			}
			else if (nSendBytes < 0 && NetHelper::IsSocketEagain())
			{
				m_pSession->SetCanWrite(FALSE);
				break;
			}
			else
			{
				gErrorStream("send error , close it.addr:" << m_pSession->GetAddress() << "=port:" << m_pSession->GetPort());
				m_pSession->SetClosed(TRUE);
				break;
			}
			Timer::sleep(1);
		}

		return unTotalSendBytes;
	}

	INT32 NetHandlerTransit::SendTo(const char * pBuf, UINT32 unSize)
	{
		if (m_pSession && !m_pSession->IsCanWrite())
		{
			return -1;
		}
		UDPContext * pContext = (UDPContext *)m_pSession->GetContext();
		if (!pContext)
		{
			return -1;
		}

		UINT32 unTotalSendBytes = 0;
		while ((unSize > unTotalSendBytes))
		{
#ifdef WIN32
			int nSendBytes = ::sendto(m_pSession->GetSocket(), &pBuf[unTotalSendBytes], unSize - unTotalSendBytes, 0 , (sockaddr*)&(pContext->GetPeerAddr()) , sizeof(pContext->GetPeerAddr()));
#else
			int nSendBytes = ::send(m_pSession->GetSocket(), &pBuf[unTotalSendBytes], unSize - unTotalSendBytes, MSG_DONTWAIT, (sockaddr*)&(pContext->GetPeerAddr()), sizeof(pContext->GetPeerAddr()));
#endif
			if (nSendBytes > 0)
			{
				unTotalSendBytes += nSendBytes;
			}
			else if (nSendBytes < 0 && NetHelper::IsSocketEagain())
			{
				m_pSession->SetCanWrite(FALSE);
				break;
			}
			else
			{
				gErrorStream("send error , close it.addr:" << m_pSession->GetAddress() << "=port:" << m_pSession->GetPort());
				m_pSession->SetClosed(TRUE);
				break;
			}
			Timer::sleep(1);
		}

		return unTotalSendBytes;
	}

	CErrno NetHandlerTransit::HandleMsg(const char* pBuffer, UINT32 unLength)
	{
		MsgHeader * pHeader = (MsgHeader*)pBuffer;

		return HandleMsg(m_pSession, pHeader->unMsgID, pBuffer + sizeof(MsgHeader), pHeader->unMsgLength - sizeof(MsgHeader));
	}

	CErrno NetHandlerTransit::HandleMsg(ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength)
	{
		return CErrno::Success();
	}

	CErrno NetHandlerTransit::OnClose(void)
	{ 
		if (m_pSession)
		{
			m_pSession->OnClose();
		} 

		m_pSession->SetClosed(TRUE);
		m_pSession->SetNetState(NET_STATE_LOSTED);
		return CErrno::Success();
	}
	  
	CErrno NetHandlerTransit::Update( void )
	{   
		return INetHandler::Update();
	}

	CErrno NetHandlerTransit::Init( void )
	{
		return INetHandler::Init();
	}

	CErrno NetHandlerTransit::Cleanup( void )
	{
		return INetHandler::Cleanup();
	}

}

