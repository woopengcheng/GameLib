#ifdef _MSC_VER
#include <WinSock2.h>
#include "NetLib/inc/NetReactorWES.h"
#include "Timer/inc/TimerHelp.h"
#include "LogLib/inc/Log.h"

namespace Net
{ 

	NetReactorWES::NetReactorWES(UINT32 unMaxConnectionCount, NetThread * pThread/* = NULL*/)
		: INetReactor(REACTOR_TYPE_WES, pThread)
		, m_unMaxConnectionCount(unMaxConnectionCount)
		, m_nNumConnection(0)
	{
		m_nNumConnection = 0;
		m_pEvents = new WSAEVENT[m_unMaxConnectionCount];
		m_pConnections = new INetHandlerPtr[m_unMaxConnectionCount];

		memset(m_pConnections, 0, sizeof(INetHandlerPtr) * m_unMaxConnectionCount);
		memset(m_pEvents, 0, sizeof(WSAEVENT*) * m_unMaxConnectionCount);
	}

	NetReactorWES::~NetReactorWES(void)
	{
		Cleanup();

		SAFE_DELETE_ARRAY(m_pConnections);
		SAFE_DELETE_ARRAY(m_pEvents);
	}

	CErrno NetReactorWES::Init(void)
	{

		return CErrno::Success();
	}

	CErrno NetReactorWES::Cleanup( void )
	{
		for (int i = 0; i < m_nNumConnection; ++i)
		{
			CloseHandle(m_pEvents[i]);
			m_pEvents[i] = NULL;
		}
		m_nNumConnection = 0;

		return INetReactor::Cleanup();
	}

	CErrno NetReactorWES::AddNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask /*= NET_FUNC_DEFAULT*/)
	{
		Assert_ReF(m_unMaxConnectionCount > (UINT32)m_nNumConnection && pNetHandler->GetSession());

		INT32  nFDMask = 0;
		if (objMask & NET_FUNC_READ)
			nFDMask |= FD_READ;
		if (objMask & NET_FUNC_WRITE)
			nFDMask |= FD_WRITE;
		if (objMask & NET_FUNC_ACCEPT)
			nFDMask |= FD_ACCEPT;
		if (objMask & NET_FUNC_EXCEPT)
			nFDMask |= FD_CLOSE;

		WSAEVENT hEvent = WSACreateEvent();
		if (WSAEventSelect((SOCKET)pNetHandler->GetSession()->GetSocket(), hEvent, nFDMask) != 0)
		{
			CloseHandle(hEvent);
			return CErrno::Failure();
		}

		m_pConnections[m_nNumConnection] = pNetHandler;
		m_pEvents[m_nNumConnection] = hEvent;
		++m_nNumConnection;

		return INetReactor::AddNetHandler(pNetHandler, objMask);
	}

	CErrno NetReactorWES::DelNetHandler(INetHandlerPtr  pNetHandler , BOOL bEraseHandler/* = TRUE*/)
	{
		INT32 index = GetIndexByNetHandler(pNetHandler);
		if (index == -1)
			return CErrno::Failure();


		std::swap(m_pConnections[index], m_pConnections[m_nNumConnection - 1]);
		std::swap(m_pEvents[index], m_pEvents[m_nNumConnection - 1]);
		CloseHandle(m_pEvents[m_nNumConnection - 1]);
		m_pConnections[m_nNumConnection - 1] = NULL;
		m_pEvents[m_nNumConnection - 1] = NULL;
		--m_nNumConnection;

		return INetReactor::DelNetHandler(pNetHandler, bEraseHandler);
	}

	CErrno NetReactorWES::ModNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask)
	{
		INT32 index = GetIndexByNetHandler(pNetHandler);
		if (index == -1)
			return CErrno::Failure();

		INT32  nFDMask = 0;
		if (objMask & NET_FUNC_READ)
			nFDMask |= FD_READ;
		if (objMask & NET_FUNC_WRITE)
			nFDMask |= FD_WRITE;
		if (objMask & NET_FUNC_ACCEPT)
			nFDMask |= FD_ACCEPT;
		if (objMask & NET_FUNC_EXCEPT)
			nFDMask |= FD_CLOSE;

		if (WSAEventSelect(m_pConnections[index]->GetSession()->GetSocket() , m_pEvents[index], nFDMask) != 0)
			return CErrno::Failure();

		return INetReactor::ModNetHandler(pNetHandler, objMask);
	}

	INT32 NetReactorWES::GetIndexByNetHandler(INetHandlerPtr pNetHandler)
	{
		INT32 index = -1;
		for (INT32 i = 0; i < m_nNumConnection; ++i)
		{
			if (m_pConnections[i] == pNetHandler)
			{
				index = i;
				break;
			}
		}

		return index;
	}
	
	CErrno NetReactorWES::Update(void)
	{
		const INT32 MAX_PER_UPDATE = 64;

		INT32 count = m_nNumConnection / MAX_PER_UPDATE;
		if ((m_nNumConnection % MAX_PER_UPDATE) != 0) 
			count++;

		for (INT32 i = count - 1; i >= 0; --i)
		{
			iUpdate(i * MAX_PER_UPDATE, __min((i + 1) * MAX_PER_UPDATE, m_nNumConnection));
		}

		return CErrno::Success();
	}

	CErrno NetReactorWES::iUpdate(INT32 start, INT32 end)
	{
		INT32 count = end - start;
		DWORD result = WaitForMultipleObjects(count, &m_pEvents[start], FALSE, 1);

		if (result >= WAIT_OBJECT_0 && result < (WAIT_OBJECT_0 + count))
		{
			int index = result - WAIT_OBJECT_0 + start;
			INetHandlerPtr pNetHandler = m_pConnections[index];
			WSANETWORKEVENTS events = { 0 };
			WSAEnumNetworkEvents((SOCKET)pNetHandler->GetSession()->GetSocket(), m_pEvents[index], &events);

			bool bClosed = false;
			if (events.lNetworkEvents & FD_READ)
			{
				bClosed = !pNetHandler->OnMsgRecving().IsSuccess() || bClosed;
			}
			if (events.lNetworkEvents & FD_ACCEPT)
			{
				bClosed = !pNetHandler->OnMsgRecving().IsSuccess() || bClosed;
			}
			if (events.lNetworkEvents & FD_WRITE)
			{
				bClosed = !pNetHandler->OnMsgSending().IsSuccess() || bClosed;
			}
			if (events.lNetworkEvents & FD_CLOSE)
			{
				bClosed = true;
			}

			ISession * pSession = pNetHandler->GetSession();
			if (pSession && pSession->GetObjTimeout().IsExpired() || pSession->IsClosed())
			{
				bClosed = TRUE;
			}

			if (bClosed)
			{
				gDebugStream("delete curNodeName=" << pNetHandler->GetSession()->GetCurNodeName() << ":remoteName=" << pNetHandler->GetSession()->GetRemoteName() << ":address=" << pNetHandler->GetSession()->GetAddress() << ":port=" << pNetHandler->GetSession()->GetPort());
				DelNetHandler(pNetHandler);
				pNetHandler->OnClose();
			}
		}
		else if (result == WAIT_FAILED)
		{
			gDebugStream("***********Error***********" << "Wait Failed");
		}
		else if (result >= WAIT_ABANDONED_0 && result < (WAIT_ABANDONED_0 + count))
		{
			gDebugStream("***********Error***********" << "WAIT_ABANDONED_0" << result - WAIT_ABANDONED_0);
		}
		else if (result == WAIT_TIMEOUT)
		{

		}
		return CErrno::Success();
	}
}

#endif