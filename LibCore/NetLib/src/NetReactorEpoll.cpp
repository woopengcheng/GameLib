#ifdef _LINUX
#include "NetLib/inc/NetReactorEpoll.h"
#include "Timer/inc/TimerHelp.h"
#include "LogLib/inc/Log.h"

namespace Net
{ 

	NetReactorEpoll::NetReactorEpoll(UINT32 unMaxConnectionCount , NetThread * pThread/* = NULL*/)
		: INetReactor(REACTOR_TYPE_EPOLL, pThread)
		, m_unMaxConnectionCount(unMaxConnectionCount)
	{
		m_nEpoll = epoll_create(m_unMaxConnectionCount);  //5 生成一个epoll专用的文件描述符,其实是在内核申请一空间，用来存放你想关注的socket fd上是否发生以及发生了什么事件。
		m_pEvents = (struct epoll_event*)malloc(sizeof(struct epoll_event) * m_unMaxConnectionCount);
	}

	NetReactorEpoll::~NetReactorEpoll(void)
	{
		Cleanup();
		::close(m_nEpoll);
		m_nEpoll = -1;
		free(m_pEvents);
	}

	CErrno NetReactorEpoll::Init(void)
	{

		return CErrno::Success();
	}

	CErrno NetReactorEpoll::Cleanup( void )
	{

		return INetReactor::Cleanup();
	}

	CErrno NetReactorEpoll::AddNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask /*= NET_FUNC_DEFAULT*/)
	{
		INT32 nEvent = 0;
		if (objMask & NET_FUNC_READ)
			nEvent |= EPOLLIN;
		if (objMask & NET_FUNC_EXCEPT)
			nEvent |= EPOLLPRI;
		if (objMask & NET_FUNC_WRITE)
			nEvent |= EPOLLOUT;
		if (objMask & NET_FUNC_ACCEPT)
			nEvent |= EPOLLIN;
		if (objMask & NET_FUNC_EPOLLET)
			nEvent |= EPOLLET;

		epoll_event ev;
		ev.events = nEvent;
		ev.data.ptr = pNetHandler.get();

		//5 该函数用于控制某个epoll文件描述符上的事件，可以注册事件，修改事件，删除事件。 
		if (epoll_ctl(m_nEpoll, EPOLL_CTL_ADD, pNetHandler->GetSession()->GetSocket(), &ev) == -1)
			return CErrno::Failure();

		return INetReactor::AddNetHandler(pNetHandler, objMask);
	}

	CErrno NetReactorEpoll::DelNetHandler(INetHandlerPtr  pNetHandler , BOOL bEraseHandler/* = TRUE*/)
	{
		epoll_event ev;
		ev.events = 0;
		ev.data.ptr = pNetHandler.get();

		//5 该函数用于控制某个epoll文件描述符上的事件，可以注册事件，修改事件，删除事件。 
		if (epoll_ctl(m_nEpoll, EPOLL_CTL_DEL, pNetHandler->GetSession()->GetSocket(), &ev) == -1)
			return CErrno::Failure();

		return INetReactor::DelNetHandler(pNetHandler, bEraseHandler);
	}

	CErrno NetReactorEpoll::ModNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask)
	{
		INT32 nEvent = 0;
		if (objMask & NET_FUNC_READ)
			nEvent |= EPOLLIN;
		if (objMask & NET_FUNC_EXCEPT)
			nEvent |= EPOLLPRI;
		if (objMask & NET_FUNC_WRITE)
			nEvent |= EPOLLOUT;
		if (objMask & NET_FUNC_ACCEPT)
			nEvent |= EPOLLIN;
		if (objMask & NET_FUNC_EPOLLET)
			nEvent |= EPOLLET;

		epoll_event ev;
		ev.events = nEvent;
		ev.data.ptr = pNetHandler.get();

		//5 该函数用于控制某个epoll文件描述符上的事件，可以注册事件，修改事件，删除事件。 
		if (epoll_ctl(m_nEpoll, EPOLL_CTL_MOD, pNetHandler->GetSession()->GetSocket(), &ev) == -1)
			return CErrno::Failure();

		return INetReactor::ModNetHandler(pNetHandler, objMask);
	}

	CErrno NetReactorEpoll::Update( void )
	{
		int nfds, ret;
		memset(m_pEvents, 0, sizeof(struct epoll_event) * m_unMaxConnectionCount);
		
		//5 等待EPOLL事件的发生，相当于监听，至于相关的端口，需要在初始化EPOLL的时候绑定。 
		nfds = epoll_wait(m_nEpoll, m_pEvents, m_unMaxConnectionCount, kMAX_EPOOL_WAIT_TIME_OUT);

		if (nfds <= 0)
			return CErrno::Success();

		for (int i = 0; i < nfds; ++i)
		{
			bool closed = false;
			INetHandler * pHandler = (INetHandler *)m_pEvents[i].data.ptr;
			if (pHandler)
			{
				if (m_pEvents[i].events & EPOLLIN)
				{
					closed = !pHandler->OnMsgRecving().IsSuccess() || closed;
				}
				if (m_pEvents[i].events & EPOLLOUT)
				{
					closed = !pHandler->OnMsgSending().IsSuccess() || closed;
				}

				ISession	* pSession = pHandler->GetSession();
				if (pSession && pSession->GetObjTimeout().IsExpired() || pSession->IsClosed() || m_pEvents[i].events & EPOLLPRI)
				{
					closed = true;
				}

				if (closed)
				{
					gDebugStream("delete curNodeName=" << pHandler->GetSession()->GetCurNodeName() << ":remoteName=" << pHandler->GetSession()->GetRemoteName()<<":address=" << pHandler->GetSession()->GetAddress() << ":port=" << pHandler->GetSession()->GetPort());
					DelNetHandler(pHandler);
				}
			}
		}
		
		return CErrno::Success();
	}

	//////////////////////////////////////////////////////////////////////////UDS///

	NetReactorUDS::NetReactorUDS(UINT32 unMaxConnectionCount/* = DEFAULT_MAX_CONNECTION_COUNT*/, NetThread * pThread/* = NULL*/)
		: NetReactorEpoll(unMaxConnectionCount, pThread)
	{
		SetReactorType(REACTOR_TYPE_UDS);
	}

	NetReactorUDS::~NetReactorUDS(void)
	{
	}

}

#endif