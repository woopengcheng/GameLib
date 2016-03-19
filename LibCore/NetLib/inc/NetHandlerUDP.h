#ifndef __net_net_handler_common_client_h__
#define __net_net_handler_common_client_h__ 
#include "NetLib/inc/NetHandlerTransit.h"
#include "NetLib/inc/MsgProcess.h"
#include "NetLib/inc/ClientSession.h"

namespace Net 
{  
	class DLL_EXPORT NetHandlerClient :public NetHandlerTransit
	{
	public:
		NetHandlerClient(INetReactor * pNetReactor , ISession * pSession , MsgProcess * pMsgProcess = NULL);
		virtual ~NetHandlerClient();

	public:
		virtual CErrno  Init( void ); 
		virtual CErrno  Init( const char* ip,int port );
		virtual CErrno  Cleanup( void );
		virtual CErrno  OnClose( void );
		virtual CErrno  Update( void );
		virtual CErrno  OnReconnect( void );

	public:
		virtual CErrno HandleMsg(ISession * pSession , UINT32 unMsgID, const char* pBuffer, UINT32 unLength);

	public: 
		BOOL  Reconnect(void)
		{ 
			if (m_pSession && ((Net::ClientSession * )m_pSession)->IsReconnect() && 
				m_pSession->GetNetState() == NET_STATE_LOSTED && m_pSession->IsClosed())
			{
				OnReconnect();
			}
			return TRUE;
		}
	
	protected:
		INT32  Connect( const char* ip,int port );

	protected: 
		MsgProcess * m_pMsgProcess;
	}; 

	DECLARE_BOOST_POINTERS(NetHandlerClient);
	  
}

#endif


