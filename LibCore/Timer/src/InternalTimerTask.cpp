// #include "Timer/inc/InternalTimerTask.h"
// #include "Timer/inc/TimerNode.h"
// #include "Timer/inc/TimerTask.h"
// #include "Timer/inc/GlobalTimer.h"
// #include "LogLib/inc/Log.h"
// 
// namespace Timer
// { 
// 	InternalTimerTask::InternalTimerTask(TimerInterface * pTimerInterface , TimerNode * pNode )
// 		: ThreadNormalTask(DEFAULT_TIMER_HANDLE_THREAD_ID , "InternalTimerTask")
// 		, m_pNode(pNode)
// 		, m_pTimerInterface(pTimerInterface)
// 	{
// 
// 	}
// 
// 	InternalTimerTask::~InternalTimerTask( void )
// 	{
// 
// 	}
// 
// 	CErrno InternalTimerTask::Update()
// 	{
// 		if (m_pNode)
// 		{
// 			TimerTask * pTask = (TimerTask *)(m_pNode->GetObject());
// 			if (pTask)
// 			{
// 				UINT32 unTimers = m_pNode->GetTimes();
// 				if (m_pNode->GetCallBackFunc())
// 				{
// 					m_pNode->GetCallBackFunc()(pTask , m_pNode->GetTimerID() , unTimers);
// 				}
// 				else
// 				{
// 					pTask->OnTimer(m_pNode->GetTimerID() , unTimers);  
// 				}
// 
// 				if (unTimers == 0)
// 				{
// 					pTask->SetTimer(m_pNode->GetTimeInterval() , unTimers);
// 				}
// 				else if(unTimers == 1)
// 				{
// //					m_pNode->SetDelete(TRUE);
// //					m_pTimerInterface->RemoveTimer(m_pNode->GetTimerID());
// 				}
// 				else
// 				{
// 					m_pNode->SetTimes(--unTimers);
// 					pTask->SetTimer(m_pNode->GetTimeInterval() , unTimers , 0);
// 				}
// 
// 			}
// 			else
// 			{
// 				UINT32 unTimers = m_pNode->GetTimes();
// 				if (m_pNode->GetCallBackFunc())
// 				{
// 					m_pNode->GetCallBackFunc()(NULL , m_pNode->GetTimerID() , unTimers);
// 				}
// 				else
// 				{
// 					gErrorStream("no object and func exec this timer. timerID:" << m_pNode->GetTimerID());
// 				}
// 
// 				if (unTimers == 0)
// 				{
// 					m_pTimerInterface->SetTimer(m_pNode->GetTimeInterval() , unTimers , NULL , m_pNode->GetCallBackFunc());
// 				}
// 				else if(unTimers == 1)
// 				{
// //					m_pNode->SetDelete(TRUE);
// //					m_pTimerInterface->RemoveTimer(m_pNode->GetTimerID());
// 				}
// 				else
// 				{
// 					m_pNode->SetTimes(--unTimers); 
// 					m_pTimerInterface->SetTimer(m_pNode->GetTimeInterval() , unTimers , NULL , m_pNode->GetCallBackFunc());
// 				}
// 
// 			}
// 			
// 			SAFE_DELETE(m_pNode);      //5 这里删除也是醉了.  
// 
// 			return CErrno::Success();
// 		}
// 
// 		return CErrno::Failure();
// 	}
// 
// }