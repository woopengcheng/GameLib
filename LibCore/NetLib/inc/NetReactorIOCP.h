#ifndef __net_i_net_reactor_iocp_h__
#define __net_i_net_reactor_iocp_h__ 

#ifdef _MSC_VER
#include "NetLib/inc/INetHandler.h"
#include "NetLib/inc/INetReactor.h"

namespace Net 
{
	class DLL_EXPORT NetReactorIOCP :public INetReactor
	{
	public:
		typedef std_unordered_map<UINT32, INetHandlerPtr> MapNetHandlersT;

	public:
		explicit NetReactorIOCP(NetThread * pThread = NULL);
		virtual ~NetReactorIOCP(void);

	public:
		virtual CErrno		Init( void ) override;
		virtual CErrno		Cleanup( void ) override;
		virtual CErrno		Update( void ) override;

	public:
		virtual CErrno		AddNetHandler(INetHandlerPtr  pNetHandler  , ENetHandlerFuncMask objMask = NET_FUNC_DEFAULT) override;
		virtual CErrno		DelNetHandler(INetHandlerPtr  pNetHandler  , BOOL bEraseHandler = TRUE) override;
		virtual CErrno		ModNetHandler(INetHandlerPtr  pNetHandler  , ENetHandlerFuncMask objMask) override;

	protected:
		HANDLE				m_hIocp;
		MapNetHandlersT		m_mapNetHandlers;
	};

//	typedef Net::NetReactorIOCP NetReactorDefault;

}
#endif

#endif
