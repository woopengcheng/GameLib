#include "MsgLib/inc/MsgQueue.h"
#include "MsgLib/inc/ObjectMsgCall.h"
#include "MsgLib/inc/MsgTimerNode.h"
#include "MsgLib/inc/InternalMsgTask.h"
#include "MsgLib/inc/InnerMsg.h"

namespace Msg
{ 
	CErrno MsgQueue::Init(Timer::ETimerStrategyType objType/* = Timer::TIMER_STRATEGY_DEFAULT*/)
	{ 
		return TimerInterface::Init(objType);
	}


	CErrno MsgQueue::Cleanup( void )
	{
		return TimerInterface::Cleanup();
	}


	CErrno MsgQueue::OnUpdate(Timer::TimerNode * pNode)
	{  
		if (pNode)
		{
			MsgTimerNode * pMsgNode = dynamic_cast<MsgTimerNode*>(pNode);

			if (pMsgNode)
			{
				return AddMsg(pMsgNode->GetMsgCall());
			}
		}

		return CErrno::Failure();
	}  

	CErrno MsgQueue::AddMsg( ObjectMsgCall * pMsg , UINT32 unTimeout/* = 0*/)
	{
		Assert_ReF(pMsg);
		if (unTimeout == 0)
		{
			m_queueMsgs.push(pMsg);
		}
		else
		{
			SetTimer(pMsg , unTimeout); 
		}

		return CErrno::Success(); 
	}


	ObjectMsgCall * MsgQueue::FetchMsg()
	{ 
		ObjectMsgCall * pMsg = NULL;
		if(!m_queueMsgs.try_pop(pMsg))
		{
			return NULL;
		}
		return pMsg;
	}

	INT32 MsgQueue::SetTimer( ObjectMsgCall * pMsg , UINT32 unTimeInterval, UINT32 unTimes /*= 0*/, UINT32 unStartTime /*= 0*/, void * pObj /*= NULL */, TimerCallBackFunc pFunc /*= NULL*/, UINT32 unTimerID/* = 0*/)
	{
		MsgTimerNode * pNode = new MsgTimerNode(pMsg, unTimerID, unTimeInterval, unStartTime, unTimes, pObj, pFunc);
		return Timer::TimerInterface::SetTimer(unTimeInterval, unTimes, unStartTime, pObj, pFunc, unTimerID, pNode);
	}


}