#ifdef _LINUX
#include "NetLib/inc/NetReactorEpollEx.h"
#include "Timer/inc/TimerHelp.h"
#include "LogLib/inc/Log.h"
#include "Coroutine/libco/co_routine.h"

namespace Net
{

	NetReactorEpollEx::NetReactorEpollEx(UINT32 unMaxConnectionCount, NetThread * pThread/* = NULL*/)
		: INetReactor(REACTOR_TYPE_EPOLLEX, pThread)
	{
		co_enable_hook_sys();   //5 使用腾讯的协程库,需要hook住底层的网络接口.
		CreateEpollExTasks(unMaxConnectionCount);
	}

	NetReactorEpollEx::~NetReactorEpollEx(void)
	{
		Cleanup();
	}

	CErrno NetReactorEpollEx::Init(void)
	{

		return CErrno::Success();
	}

	CErrno NetReactorEpollEx::Cleanup(void)
	{

		return INetReactor::Cleanup();
	}

	CErrno NetReactorEpollEx::AddNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask /*= NET_FUNC_DEFAULT*/)
	{
		INT32 nEvent = 0;
		if (objMask & NET_FUNC_READ)
			nEvent |= POLLIN;
		if (objMask & NET_FUNC_EXCEPT)
			nEvent |= POLLERR;
		if (objMask & NET_FUNC_WRITE)
			nEvent |= POLLOUT;
		if (objMask & NET_FUNC_ACCEPT)
			nEvent |= POLLIN;
		if (objMask & NET_FUNC_EPOLLET)
			nEvent |= EPOLLET;
		if (objMask & NET_FUNC_POLLHUP)
			nEvent |= POLLHUP;

		MsgAssert_Re(pNetHandler->GetSession() && pNetHandler->GetSession()->GetContext(), CErrno::Failure(), "NetReactorEpollEx::AddNetHandler error , no context.");
		EpollexCoTask * pTask = pNetHandler->GetSession()->GetContext();
		pTask->events = nEvent;

		return INetReactor::AddNetHandler(pNetHandler, objMask);
	}

	CErrno NetReactorEpollEx::DelNetHandler(INetHandlerPtr  pNetHandler, BOOL bEraseHandler/* = TRUE*/)
	{
		return INetReactor::DelNetHandler(pNetHandler, bEraseHandler);
	}

	CErrno NetReactorEpollEx::ModNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask)
	{
		INT32 nEvent = 0;
		if (objMask & NET_FUNC_READ)
			nEvent |= POLLIN;
		if (objMask & NET_FUNC_EXCEPT)
			nEvent |= POLLERR;
		if (objMask & NET_FUNC_WRITE)
			nEvent |= POLLOUT;
		if (objMask & NET_FUNC_ACCEPT)
			nEvent |= POLLIN;
		if (objMask & NET_FUNC_EPOLLET)
			nEvent |= EPOLLET;
		if (objMask & NET_FUNC_POLLHUP)
			nEvent |= POLLHUP;

		MsgAssert_Re(pNetHandler->GetSession() && pNetHandler->GetSession()->GetContext(), CErrno::Failure(), "NetReactorEpollEx::AddNetHandler error , no context.");
		EpollexCoTask * pTask = pNetHandler->GetSession()->GetContext();
		pTask->events = nEvent;

		return INetReactor::ModNetHandler(pNetHandler, objMask);
	}

	void  NetReactorEpollEx::CorotineUpdate(void * arg)
	{
// 		co_enable_hook_sys();

		EpollexCoTask * pTask = (EpollexCoTask *)arg;
		NetReactorEpollEx * pNetReactor = dynamic_cast<NetReactorEpollEx*>(pTask->pReactor);
		MsgAssert_Re0(pTask && pNetReactor, "corotine update error. no this task.");

		while (true)
		{
			if (kInvalidFD == pTask->fd)
			{
				if (pNetReactor)
				{
					pNetReactor->GetEpollExTasks().push(pTask);
				}
				co_yield_ct();
				continue;
			}

			SOCKET fd = pTask->fd;
			pTask->fd = kInvalidFD;

			while (true)
			{
				struct pollfd pf = { 0 };
				pf.fd = fd;
				pf.events = pTask->events;
				co_poll(co_get_epoll_ct(), &pf, 1, kMAX_EPOOL_WAIT_TIME_OUT);

				if (pNetReactor)
				{
					bool closed = false;
					INetHandlerPtr pHandler = pTask->pNetHandler;
					if (pHandler)
					{
						closed = !pHandler->OnMsgRecving().IsSuccess() || closed;

						ISession	* pSession = pHandler->GetSession();
						if (pSession && pSession->GetObjTimeout().IsExpired() || pSession->IsClosed())
						{
							closed = true;
						}
					}
					if (closed)
					{
						gDebugStream("delete curNodeName=" << pHandler->GetSession()->GetCurNodeName() << ":remoteName=" << pHandler->GetSession()->GetRemoteName() << ":address=" << pHandler->GetSession()->GetAddress() << ":port=" << pHandler->GetSession()->GetPort());
						pTask->pNetHandler.reset();
						pNetReactor->DelNetHandler(pHandler);
						break;
					}
				}
			}
		}
	}

	std::stack<EpollexCoTask*>& NetReactorEpollEx::GetEpollExTasks()
	{
		return m_pEpollexTasks;
	}

	BOOL NetReactorEpollEx::CreateEpollExTasks(INT32 nCount/* = kPerCreateEpollExCoTaskCount*/)
	{
		for (UINT32 i = 0; I < nCount; ++i) //5 这里预分配这IME多个协程.不够的时候在加.
		{
			EpollexCoTask * pTask = new EpollexCoTask;
			pTask->fd = kInvalidFD;
			pTask->co = NULL;

			co_create(&(pTask->co), NULL , CorotineUpdate , pTask);
			co_resume(pTask->co);
		}
		gOtherStream("CreateEpollExTasks count=" << nCount);

		return TRUE;
	}

	CErrno NetReactorEpollEx::Update(void)
	{
		return CErrno::Success();
	}

	//////////////////////////////////////////////////////////////////////////UDS///

	NetReactorUDSEx::NetReactorUDSEx(UINT32 unMaxConnectionCount/* = DEFAULT_MAX_CONNECTION_COUNT*/, NetThread * pThread /*= NULL*/)
		: NetReactorEpollEx(unMaxConnectionCount , pThread)
	{
		SetReactorType(REACTOR_TYPE_UDSEX);
	}

	NetReactorUDSEx::~NetReactorUDSEx(void)
	{
	}

}

#endif