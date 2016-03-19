#ifndef __net_i_net_reactor_raknet_h__
#define __net_i_net_reactor_raknet_h__ 
#include "NetLib/inc/INetHandler.h"
#include "NetLib/inc/INetReactor.h"

namespace RakNet
{
	struct Packet;
	struct SystemAddress;
	class  RakPeerInterface;
}

namespace Net 
{
	class RakNetContext;

	class DLL_EXPORT NetReactorRakNet :public INetReactor
	{
	public:
		typedef std_unordered_map<unsigned long , INetHandlerPtr> MapNetHandlersT;

	public:
		explicit NetReactorRakNet(UINT32 unMaxConnectionCount = DEFAULT_MAX_CONNECTION_COUNT , NetThread * pThread = NULL);
		virtual ~NetReactorRakNet(void);

	public:
		virtual CErrno   Init(void) override;
		virtual CErrno   Cleanup(void) override;
		virtual CErrno   Update(void) override;

	public:
		virtual CErrno   AddNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask = NET_FUNC_DEFAULT) override;
		virtual CErrno   DelNetHandler(INetHandlerPtr  pNetHandler, BOOL bEraseHandler = TRUE) override;
		virtual CErrno   ModNetHandler(INetHandlerPtr  pNetHandler, ENetHandlerFuncMask objMask) override;

	public:
		RakNet::RakPeerInterface * GetRakPeerInterface() { return m_pRakPeerInstance; }
		CErrno			InitAccept(ISession * pSession);
		INetHandlerPtr	GetNetHandler(const RakNet::SystemAddress & address);
		INetHandlerPtr	GetListenerNetHandler();
		CErrno			CloseConnection(const RakNet::SystemAddress & address);

	private:
		UINT32						m_unMaxConnectionCount;
		MapNetHandlersT				m_mapNetHandlers;
		RakNet::RakPeerInterface *	m_pRakPeerInstance;
	};


}

#endif
