#ifndef __net_net_handler_listener_h__
#define __net_net_handler_listener_h__  
#include "NetLib/inc/INetHandler.h"

struct sockaddr_in;

namespace RakNet
{
	struct SystemAddress;
}

namespace Net 
{  
	class INetReactor;

	class DLL_EXPORT NetHandlerListener : public INetHandler
	{
	public:
		NetHandlerListener( INetReactor * pNetReactor , ISession * pSession);
		virtual ~NetHandlerListener( void ){}  

	public:
		virtual CErrno	Init(const char * pAddress , INT32 nPort , BOOL bResueAddr = TRUE , INT32  nListenerCount = DEFAULT_LISTENER_COUNT);
		virtual CErrno	Cleanup() override;
		virtual void	OnAccept(NetSocket socket, sockaddr_in * addr);
		virtual void	OnAccept(RakNet::SystemAddress * pAddress);
		virtual CErrno	OnMsgRecving(void) override;

	protected:
		void			InitUDS();
		void			InitCommon();
		CErrno			OnMsgRecvingIOCP(void);
		CErrno			OnMsgRecvingCommon(void);
		INT32			Listen(INT32  nListenerCount = DEFAULT_LISTENER_COUNT);
		INT32			Bind(const char * pAddress , INT32 nPort , BOOL bResueAddr = TRUE);
		CErrno			OnMsgRecvingRakNet();
	};
	DECLARE_BOOST_POINTERS( NetHandlerListener ); 
}

#endif
