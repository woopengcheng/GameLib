#ifdef _MSC_VER
#include <WinSock2.h>  
#include <MSWSock.h>  
#include "NetLib/inc/NetReactorIOCP.h"
#include "NetLib/inc/NetHandlerListener.h"
#include "NetLib/inc/NetHelper.h"
#include "Timer/inc/TimerHelp.h"
#include "LogLib/inc/Log.h"

namespace Net
{
	class IocpContext
	{
	public:
		IocpContext();
		~IocpContext();

		enum AsyncOper
		{
			ASYNC_OPER_RECV = 100,
			ASYNC_OPER_SEND,
			ASYNC_OPER_ACCEPT,
		};
		struct MYOVERLAPPED
		{
			OVERLAPPED overlapped;
			AsyncOper asyncoper;
		};

	public:
		void			SetSendBuf(const char* buf, INT32 len);
		INT32			CreateClientSocketForAccept();

	public:
		WSABUF			recvBuf;
		INT32			recvNum;

		WSABUF			sendBuf;
		INT32			sendNum;

		MYOVERLAPPED	recvOverlapped;
		MYOVERLAPPED	sendOverlapped;

		INT32			acceptNum;
		MYOVERLAPPED	acceptOverlapped;
		char			acceptAddr[sizeof(sockaddr_in) * 2 + 32];
		INT32			acceptClientSock;

		char		*	sendBufPtr;
		INT32			sendbufSize;
	};

	IocpContext::IocpContext()
	{
		recvNum = 0;
		sendNum = 0;
		memset(&recvBuf, 0, sizeof(WSABUF));
		memset(&sendBuf, 0, sizeof(WSABUF));
		memset(&recvOverlapped, 0, sizeof(OVERLAPPED));
		memset(&sendOverlapped, 0, sizeof(OVERLAPPED));
		memset(&acceptOverlapped, 0, sizeof(OVERLAPPED));
		recvOverlapped.asyncoper = ASYNC_OPER_RECV;
		sendOverlapped.asyncoper = ASYNC_OPER_SEND;
		acceptOverlapped.asyncoper = ASYNC_OPER_ACCEPT;
		sendBufPtr = NULL;
		sendbufSize = 0;
		acceptNum = 0;
		memset(&acceptAddr, 0, sizeof(acceptAddr));
		acceptClientSock = -1;
	}

	IocpContext::~IocpContext()
	{
		free(sendBufPtr);
	}

	void IocpContext::SetSendBuf(const char* buf, INT32 len)
	{
		if (len > sendbufSize)
		{
			INT32 nsize = 512;
			while (nsize < len)
				nsize *= 2;

			sendbufSize = nsize;
			sendBufPtr = (char*)realloc(sendBufPtr, sendbufSize);
		}
		memcpy(sendBufPtr, buf, len);
		sendBuf.buf = sendBufPtr;
		sendBuf.len = len;
	}


	INT32 IocpContext::CreateClientSocketForAccept()
	{
		return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	}


	INT32 NetHelper::WSARecv(INetHandler * pNetHandler)
	{
		if (pNetHandler && pNetHandler->GetSession())
		{
			SOCKET sock = pNetHandler->GetSession()->GetSocket();
			IocpContext* ctx = (IocpContext*)pNetHandler->GetSession()->GetContext();

			Assert_ReF1(ctx);

			if (ctx->recvNum > 0)
				return -1;

			DWORD bytes = 0;
			DWORD flags = 0;
			if (::WSARecv(sock, &ctx->recvBuf, 1, &bytes, &flags, &ctx->recvOverlapped.overlapped, NULL) < 0)
			{
				INT32 err = WSAGetLastError();
				switch (err)
				{
				case WSA_IO_PENDING:
					break;
				case WSAECONNRESET:
				case WSAEDISCON:
				{
					pNetHandler->GetNetReactor()->DelNetHandler(INetHandlerPtr(pNetHandler));
					pNetHandler->OnClose();

					return -10;
				}
				break;
				default:
				{
					gErrorStream("SocketReactorImplIocp::PostRecv Error:" << err);
					return -200;
				}
				break;
				}
			}
			ctx->recvNum++;
		}
		return 0;
	}

	static LPFN_ACCEPTEX gpfnAcceptEx = NULL;
	int NetHelper::WSAAccept(INetHandler * pNetHandler)
	{
		SOCKET sock = pNetHandler->GetSession()->GetSocket();
		IocpContext* ctx = (IocpContext*)pNetHandler->GetSession()->GetContext();
		Assert_ReF1(ctx);

		if (gpfnAcceptEx == NULL)
		{
			GUID guidAcceptEx = WSAID_ACCEPTEX;
			DWORD bytes = 0;
			int result = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidAcceptEx, sizeof(guidAcceptEx), &gpfnAcceptEx, sizeof(gpfnAcceptEx), &bytes, NULL, NULL);
			Assert_ReF1(result == 0 && gpfnAcceptEx);
		}

		DWORD bytes = 0;
		ctx->acceptClientSock = ctx->CreateClientSocketForAccept();
		if (gpfnAcceptEx(sock, ctx->acceptClientSock, &ctx->acceptAddr[0], 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
			&bytes, &ctx->acceptOverlapped.overlapped) < 0)
		{
			int err = WSAGetLastError();
			switch (err)
			{
			case WSA_IO_PENDING:
				break;
			case WSAECONNRESET:
			case WSAEDISCON:
			{
				pNetHandler->GetNetReactor()->DelNetHandler(INetHandlerPtr(pNetHandler));
				pNetHandler->OnClose();
				return -10;
			}
			break;
			default:
			{
				gErrorStream("SocketReactorImplIocp::PostAccept Error:" << err);
				return -200;
			}
			break;
			}
		}

		ctx->acceptNum++;
		return 0;
	}

	INT32 NetHelper::WSASend(INetHandler * pNetHandler, const char* buf, INT32 len)
	{
		SOCKET sock = pNetHandler->GetSession()->GetSocket();
		IocpContext* ctx = (IocpContext*)pNetHandler->GetSession()->GetContext();
		Assert_ReF1(ctx);

		if (ctx->sendNum > 0)
			return -1;

		ctx->SetSendBuf(buf, len);

		DWORD bytes = 0;
		DWORD flags = 0;
		if (::WSASend(sock, &ctx->sendBuf, 1, &bytes, flags, &ctx->sendOverlapped.overlapped, NULL) < 0)
		{
			int err = WSAGetLastError();
			switch (err)
			{
			case WSA_IO_PENDING:
				break;
			case WSAECONNRESET:
			case WSAEDISCON:
			{
				pNetHandler->GetNetReactor()->DelNetHandler(INetHandlerPtr(pNetHandler));
				pNetHandler->OnClose();
				return -10;
			}
			break;
			default:
			{
				gErrorStream("SocketReactorImplIocp::PostSend Error:" << err);
				return -200;
			}
			break;
			}
		}
// 		ctx->sendNum++;

		return bytes;
	}

	CErrno NetHandlerListener::OnMsgRecvingIOCP(void)
	{
		if (m_pSession)
		{
			IocpContext* ctx = (IocpContext*)m_pSession->GetContext();
			Assert_ReF(ctx);

			INT32 sock = ctx->acceptClientSock;
			ctx->acceptClientSock = -1;

			sockaddr_in addr = { 0 };
			INT32 addrlen = sizeof(addr);
			::getpeername(sock, (sockaddr*)&addr, &addrlen);

			OnAccept(sock, &addr);

			return CErrno::Success();
		}

		return CErrno::Failure();
	}

	NetReactorIOCP::NetReactorIOCP(NetThread * pThread/* = NULL*/)
		: INetReactor(REACTOR_TYPE_IOCP, pThread)
	{
		m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
		MsgAssert(m_hIocp != NULL && m_hIocp != INVALID_HANDLE_VALUE , "error iocp create.");
	}

	NetReactorIOCP::~NetReactorIOCP(void)
	{
		if (m_hIocp != NULL && m_hIocp != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hIocp);
			m_hIocp = NULL;
		}
	}

	CErrno NetReactorIOCP::Init(void)
	{

		return CErrno::Success();
	}

	CErrno NetReactorIOCP::Cleanup( void )
	{

		return INetReactor::Cleanup();
	}

	CErrno NetReactorIOCP::AddNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask /*= NET_FUNC_DEFAULT*/)
	{
		if (pNetHandler)
		{
			HANDLE sock = (HANDLE)pNetHandler->GetSession()->GetSocket();
			HANDLE hand = CreateIoCompletionPort(sock, m_hIocp, (ULONG_PTR)(pNetHandler.get()), 1);
			if (hand == NULL || hand == INVALID_HANDLE_VALUE)
				return CErrno::Failure();

			IocpContext * ctx = new IocpContext();
			pNetHandler->GetSession()->SetContext(ctx);

			INT32  nFDMask = 0;
			if (objMask & NET_FUNC_READ)
			{
				NetHelper::WSARecv(pNetHandler.get());
			}
			if (objMask & NET_FUNC_ACCEPT)
			{
				INT32 nValue = 1;
				MsgAssert_ReF(setsockopt(pNetHandler->GetSession()->GetSocket(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&nValue, sizeof(nValue)) , "create iocp accept error.");

				NetHelper::WSAAccept(pNetHandler.get());
			}

			m_mapNetHandlers.insert(std::make_pair(pNetHandler->GetSession()->GetSessionID(), pNetHandler));

			return INetReactor::AddNetHandler(pNetHandler, objMask);
		}

		return CErrno::Failure();
	}

	CErrno NetReactorIOCP::DelNetHandler(INetHandlerPtr  pNetHandler , BOOL bEraseHandler/* = TRUE*/)
	{
		m_mapNetHandlers.erase(pNetHandler->GetSession()->GetSessionID());

		IocpContext * ctx = (IocpContext *)(pNetHandler->GetSession()->GetContext());
		if (ctx != NULL)
		{
			SAFE_DELETE(ctx);
			pNetHandler->GetSession()->SetContext(NULL);
		}

		return INetReactor::DelNetHandler(pNetHandler, bEraseHandler);
	}

	CErrno NetReactorIOCP::ModNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask)
	{

		return INetReactor::ModNetHandler(pNetHandler, objMask);
	}

	CErrno NetReactorIOCP::Update( void )
	{
		DWORD bytes = 0;
		ULONG_PTR iokey = 0;
		OVERLAPPED* poverlapped = NULL;

		while (true)
		{
			BOOL result = GetQueuedCompletionStatus(m_hIocp, &bytes, &iokey, &poverlapped, 1);
			if (poverlapped)
			{
				bool bClosed = !result;
				INetHandler * pNetHandler = (INetHandler*)iokey;

				IocpContext::MYOVERLAPPED* overlapped = (IocpContext::MYOVERLAPPED*)poverlapped;
				IocpContext* ctx = (IocpContext*)(pNetHandler->GetSession()->GetContext());

				switch (overlapped->asyncoper)
				{
				case IocpContext::ASYNC_OPER_RECV:
				{
					--ctx->recvNum;
					if (!bClosed)
					{
						bClosed = !pNetHandler->OnMsgRecving().IsSuccess() || bClosed;
						if (!bClosed)
							NetHelper::WSARecv(pNetHandler);
					}
				}
				break;
				case IocpContext::ASYNC_OPER_SEND:
				{
// 					--ctx->sendNum;
					if (!bClosed)
					{
						bClosed = !pNetHandler->OnMsgSending().IsSuccess() || bClosed;
					}
				}
				break;
				case IocpContext::ASYNC_OPER_ACCEPT:
				{
					--ctx->acceptNum;
					if (!bClosed)
					{
						bClosed = !pNetHandler->OnMsgRecving().IsSuccess() || bClosed;
						if (!bClosed)
							NetHelper::WSAAccept(pNetHandler);
					}
				}
				break;
				}

				if (bClosed)
				{
					gDebugStream("delete curNodeName=" << pNetHandler->GetSession()->GetCurNodeName() << ":remoteName=" << pNetHandler->GetSession()->GetRemoteName() << ":address=" << pNetHandler->GetSession()->GetAddress() << ":port=" << pNetHandler->GetSession()->GetPort());
					DelNetHandler(INetHandlerPtr(pNetHandler));  //5 这么用智能指针,相当于是新建立的一个.刚好.这里没有对这个指针的删除.所以不会有问题.但是
				}
			}
			else
			{
				break;
			}
		}
		return CErrno::Success();
	}

}

#endif