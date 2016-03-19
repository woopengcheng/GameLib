#ifndef __net_net_handler_transit_h__
#define __net_net_handler_transit_h__ 
#include "NetLib/inc/INetHandler.h"
#include "CUtil/inc/CircleBuffer.h"

struct zmq_msg_t;

namespace Net 
{
	typedef void * zmqSocketPtr;
	typedef void * zmqContextPtr;

	class INetReactor; 

	class DLL_EXPORT NetHandlerTransit :public INetHandler
	{
	public:
		NetHandlerTransit( INetReactor * pNetReactor , ISession * pSession); 
		virtual ~NetHandlerTransit( void );

	public:
		virtual CErrno		Init( void ) override;
		virtual CErrno		Cleanup( void ) override;
		virtual CErrno		Update( void ) override;
		virtual CErrno		OnClose( void ) override;

	public:                   
		virtual CErrno		OnMsgRecving(void) override;
		virtual CErrno		OnMsgRecving(const char * pBuf, UINT32 unSize);
		virtual CErrno		OnMsgSending(void) override;
		virtual INT32		SendMsg(const char * pBuf, UINT32 unSize) override;
		virtual CErrno		HandleMsg(const char* pBuffer, UINT32 unLength);
		virtual CErrno		HandleMsg(ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength);

	protected:
		INT32				Send(const char * pBuf, UINT32 unSize);
		INT32				SendTo(const char * pBuf, UINT32 unSize);
		INT32				SendCommon(const char * pBuf, UINT32 unSize);
		INT32				SendIOCP(const char * pBuf, UINT32 unSize);
		INT32				SendUDS(const char * pBuf, UINT32 unSize);//5 用于传递套接字
		INT32				SendZMQ(const char * pBuf, UINT32 unSize);
		INT32				RecvMsgRakNet(char * pBuf, UINT32 unSize);
		INT32				SendRakNet(const char * pBuf, UINT32 unSize);
		CErrno				ParaseRecvMsg();     
		INT32				FlushSendBuffer(void);
		CErrno				RecvToCircleBuffer(const char * pBuf , UINT32 unSize);   //5 将消息扔给CircleBuffer 

	protected:
		zmqSocketPtr		m_pZmqSocket;
		zmqContextPtr		m_pZmqContext;
		zmq_msg_t		*	m_pZmqMsg;
		CUtil::CircleBuffer	m_objSendBuf;
		CUtil::CircleBuffer	m_objRecvBuf;
	}; 

	DECLARE_BOOST_POINTERS(NetHandlerTransit);
	 
}

#endif


