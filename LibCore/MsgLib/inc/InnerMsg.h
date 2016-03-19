#ifndef __msg_inner_msg_h__
#define __msg_inner_msg_h__ 
#include "MsgLib/inc/ObjectMsgCall.h"
#include "MsgLib/inc/MsgQueue.h"   
#include "MsgLib/inc/MsgDispatcher.h"

namespace ThreadPool
{
	class ThreadPoolInterface;
}

namespace Msg
{ 
	
	class DLL_EXPORT InnerMsg : public MsgDispatcher
	{
	public:
		InnerMsg(ThreadPool::ThreadPoolInterface * pThreadPoolInterface = NULL)
			: m_pThreadPoolInterface(pThreadPoolInterface)
		{

		}
		virtual ~InnerMsg(){}
		
	public:
		virtual	CErrno			Init( void );
		virtual	CErrno			Cleanup( void );
		virtual	CErrno			Update( void ); 
		virtual	void			OnRegisterMsgs(void){}

	public:
		void					RegisterMsg(void);
		CErrno					SendMsg(ObjectMsgCall * pMsg, UINT32 unTimeout = 0);
		void					SetThreadPool(ThreadPool::ThreadPoolInterface * pThreadPoolInterface) { m_pThreadPoolInterface = pThreadPoolInterface; }
		ThreadPool::ThreadPoolInterface * GetThreadPool(void){ return m_pThreadPoolInterface; }

	protected: 
		CErrno					RunMsg( ObjectMsgCall * pMsg );

	protected:
		MsgQueue						  m_objMsgQueue;
		ThreadPool::ThreadPoolInterface * m_pThreadPoolInterface;
	}; 
	 
}
#endif