#ifndef __net_i_net_reactor_epoll_h__
#define __net_i_net_reactor_epoll_h__ 

#ifdef _LINUX

#include "NetLib/inc/INetHandler.h"
#include "NetLib/inc/INetReactor.h"

namespace Net 
{   
	const INT32 kMAX_EPOOL_WAIT_TIME_OUT = 25;

	class DLL_EXPORT NetReactorEpoll :public INetReactor
	{
	public:
		explicit NetReactorEpoll(UINT32 unMaxConnectionCount = DEFAULT_MAX_CONNECTION_COUNT , NetThread * pThread = NULL);
		virtual ~NetReactorEpoll(void);

	public:
		virtual CErrno		Init( void ) override;
		virtual CErrno		Cleanup( void ) override;
		virtual CErrno		Update( void ) override;

	public:
		virtual CErrno		AddNetHandler(INetHandlerPtr  pNetHandler  , ENetHandlerFuncMask objMask = NET_FUNC_DEFAULT) override;
		virtual CErrno		DelNetHandler(INetHandlerPtr  pNetHandler  , BOOL bEraseHandler = TRUE) override;
		virtual CErrno		ModNetHandler(INetHandlerPtr  pNetHandler  , ENetHandlerFuncMask objMask) override;

	protected:
		INT32				m_nEpoll;
		UINT32				m_unMaxConnectionCount;
		struct epoll_event* m_pEvents;
	}; 

	typedef Net::NetReactorEpoll NetReactorDefault;

	class DLL_EXPORT NetReactorUDS :public NetReactorEpoll
	{
	public:
		NetReactorUDS(UINT32 unMaxConnectionCount);
		virtual ~NetReactorUDS(void);
	};
}
#endif

#endif
