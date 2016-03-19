#ifndef __net_msg_process_h__
#define __net_msg_process_h__ 
#include "NetLib/inc/NetCommon.h"
#include "NetLib/inc/ISession.h"

namespace Net
{     
	class MsgProcess
	{
	public:
		MsgProcess(){}
		virtual ~MsgProcess(){} 

	public:
		virtual BOOL IsAccept(UINT32 unMsgID) { return TRUE; }

	public:
		virtual CErrno Process(ISession * pSession , UINT32 unMsgID, const char* pBuffer, UINT32 unLength) = 0 { return CErrno::Failure(); } 
	};
	  
} //Net

#endif