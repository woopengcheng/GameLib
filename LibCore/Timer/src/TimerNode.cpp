#include "Timer/inc/TimerNode.h"
#include "ThreadPool/inc/ThreadLock.h"

namespace Timer
{ 
	UINT32 TimerNode::GetTimeInterval()
	{
// 		ThreadPool::AutoSpinRWLock objLock(m_objLock , false);
		return m_unTimeInterval;
	}

	void TimerNode::SetTimes( UINT32 unTimes )
	{
//  	ThreadPool::AutoSpinRWLock objLock(m_objLock);
		m_unTimes = unTimes;
	}

	UINT32 TimerNode::GetTimes()
	{
// 		ThreadPool::AutoSpinRWLock objLock(m_objLock , false);
		return m_unTimes;
	}

	TimeCount & TimerNode::GetTimeCount()
	{  
		return m_objTimerCount;
	}

	void TimerNode::SetObject( void * pObj )
	{
//		ThreadPool::AutoSpinRWLock objLock(m_objLock);
		m_pObj = pObj;
	}

	void   * TimerNode::GetObject()
	{
		return m_pObj;
	}
	
	UINT32 TimerNode::GetTimerID()
	{
// 		ThreadPool::AutoSpinRWLock objLock(m_objLock , false);
		return m_unTimerID;
	}

	void TimerNode::SetDelete(BOOL bDelete)
	{
//		ThreadPool::AutoSpinRWLock objLock(m_objLock);
		m_bDelete = bDelete; 
	}

	BOOL TimerNode::IsDelete()
	{
// 		ThreadPool::AutoSpinRWLock objLock(m_objLock , false);
		return m_bDelete;

	}

	UINT64 TimerNode::GetEndTime(void) 
	{
//		ThreadPool::AutoSpinRWLock objLock(m_objLock, false);
		return m_llEndTime;
	}

	ThreadPool::ThreadSpinRWMutex & TimerNode::GetLock()
	{
		return m_objLock;
	}

}