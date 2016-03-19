#ifndef __net_net_handler_common_client_h__
#define __net_net_handler_common_client_h__ 
#include "NetLib/inc/NetHandlerPing.h"
#include "NetLib/inc/MsgProcess.h"
#include "NetLib/inc/ClientSession.h"

namespace Net 
{
	class DLL_EXPORT NetHandlerClient :public NetHandlerPing
	{
	public:
		NetHandlerClient(INetReactor * pNetReactor , ISession * pSession , MsgProcess * pMsgProcess = NULL);
		virtual ~NetHandlerClient();

	public:
		virtual CErrno	Init( void ) override;
		virtual CErrno	Init(const std::string & ip,int port );
		virtual CErrno	Update( void ) override;
		virtual CErrno	OnReconnect(void);
		virtual CErrno	HandleMsg(ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength) override;

	public: 
		BOOL			Reconnect(void);
	
	protected:
		CErrno			InitZMQ();
		CErrno			InitUDS();
		CErrno			InitRakNet();
		INT32			Connect(const char* ip, int port);
		INT32			ConnectUDS(const char* ip, int port);
		INT32			ConnectUDP(const char* ip, int port);
		INT32			ConnectZMQ(const char* ip, int port);
		INT32			ConnectRakNet(const char* ip, int port);
		INT32			ConnectCommon(const char* ip, int port);

	protected: 
		MsgProcess	*	m_pMsgProcess;
	}; 

	DECLARE_BOOST_POINTERS(NetHandlerClient);
	  
}

#endif


