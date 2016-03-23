#ifndef __net_i_net_reactor_epoll_h__
#define __net_i_net_reactor_epoll_h__ 

#ifdef _LINUX

#include "NetLib/inc/INetHandler.h"
#include "NetLib/inc/INetReactor.h"
#include "Coroutine/libco/co_routine.h"
#include "Coroutine/libco/co_routine_inner.h"

namespace Net
{
	const INT32 kInvalidFD = -1;
	const INT32 kPerCreateEpollExCoTaskCount = 1000;  //5 当协程使用完毕后.每次增加创建1000个.

	struct EpollexCoTask 
	{
		stCoRoutine_t	*	co;
		SOCKET				fd;
		INetHandlerPtr		pNetHandler;
		INT32				events;
		INetReactor		*	pReactor;
		EpollexCoTask()
			: co(NULL)
			, fd(kInvalidFD)
			, pNetHandler(NULL)
			, events(0)
			, pReactor(NULL)
		{
		}
	};

	class DLL_EXPORT NetReactorEpollEx :public INetReactor
	{
	public:
		explicit NetReactorEpollEx(UINT32 unMaxConnectionCount = DEFAULT_MAX_CONNECTION_COUNT, NetThread * pThread = NULL);
		virtual ~NetReactorEpollEx(void);

	public:
		virtual CErrno				Init(void) override;
		virtual CErrno				Cleanup(void) override;
		virtual CErrno				Update(void) override;

	public:
		virtual CErrno				AddNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask = NET_FUNC_DEFAULT) override;
		virtual CErrno				DelNetHandler(INetHandlerPtr  pNetHandler, BOOL bEraseHandler = TRUE) override;
		virtual CErrno				ModNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask) override;

	public:
		static void					CorotineUpdate(void * arg);
		std::stack<EpollexCoTask*>&	GetEpollExTasks();
		BOOL						CreateEpollExTasks(INT32 nCount = kPerCreateEpollExCoTaskCount);

	protected:
		std::stack<EpollexCoTask*>	m_pEpollexTasks;
	};

	typedef Net::NetReactorEpollEx NetReactorDefault;

	class DLL_EXPORT NetReactorUDSEx :public NetReactorEpollEx
	{
	public:
		NetReactorUDSEx(UINT32 unMaxConnectionCount = DEFAULT_MAX_CONNECTION_COUNT, NetThread * pThread = NULL);
		virtual ~NetReactorUDSEx(void);
	};
}
#endif

#endif
