#ifndef __net_net_handler_common_server_h__
#define __net_net_handler_common_server_h__ 
#include "NetLib/inc/NetHandlerPing.h"

struct zmq_msg_t;

namespace Net 
{
	typedef void * zmqSocketPtr;
	typedef void * zmqContextPtr;

	class DLL_EXPORT NetHandlerServer :public NetHandlerPing
	{
	public:
		NetHandlerServer(INetReactor * pNetReactor, ISession * pSession);
		virtual ~NetHandlerServer();

	public:
		virtual CErrno  OnClose(void) override;
		virtual	CErrno	Update(void) override;
		virtual CErrno	HandleMsg(ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength) override;

	protected:
		CErrno			InitZMQ();
		CErrno			InitUDP();
		CErrno			InitUDS();
		CErrno			InitRakNet();
		CErrno			Init(const std::string & ip, int port);

	protected:
		zmqSocketPtr	m_pZmqSocket;
		zmqContextPtr	m_pZmqContext;
		zmq_msg_t	*	m_pZmqMsg;
	}; 

	DECLARE_BOOST_POINTERS(NetHandlerServer);
	 
}

#endif 