#ifndef __msg_internal_msg_task_h__
#define __msg_internal_msg_task_h__ 
#include "MsgLib/inc/MsgCommon.h"
#include "ThreadPool/inc/ThreadTask.h"

namespace Msg
{ 
	class ObjectMsgCall;
	class InnerMsg;
	
	class DLL_EXPORT  InternalMsgTask : public ThreadPool::ThreadTask
	{   
	public:
		InternalMsgTask(InnerMsg * pInnerMsg , ObjectMsgCall * pMsg);
		virtual ~InternalMsgTask();

	public:
		virtual CErrno	Update( void ) override;

	protected: 
		ObjectMsgCall * m_pMsg;
		InnerMsg      * m_pInnerMsg;
	};
	 
}
#endif