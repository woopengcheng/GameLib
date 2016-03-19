#ifndef __timer_timer_node_h__
#define __timer_timer_node_h__
#include "Timer/inc/TimeCount.h" 
#include "Timer/inc/TimerHelp.h"

namespace Timer
{
#define SECOND_MILL_TIME 1000

	class DLL_EXPORT  TimerNode
	{
	public:
		TimerNode(UINT32 unTimerID, UINT32 unTimeInterval, UINT32 unStartTime = 0, UINT32 unTimes = 0, void * pObj = NULL, TimerCallBackFunc pFunc = NULL)
			: m_bDelete(FALSE)
			, m_unTimerID(unTimerID)
			, m_pObj(pObj)
			, m_unTimeInterval(unTimeInterval)
			, m_unTimes(unTimes)
			, m_pCallBackFunc(pFunc)
			, m_llEndTime((TIMING_WHEEL_PRECISE_FUNC + unStartTime + unTimeInterval))
			, m_pNext(NULL)
			, m_pPrev(NULL)
			, m_unStartAddTime(0)
		{
		}
		virtual   ~TimerNode() {}

	public:
		TimerNode		*	GetNext() { return m_pNext; }
		TimerNode		*	GetPrev() { return m_pPrev; }
		UINT32				GetTimeInterval();
		void				SetTimes(UINT32 unTimes);
		UINT32				GetTimes();
		TimeCount		&	GetTimeCount();
		void			*	GetObject();
		void				SetObject(void * pObj);
		UINT32				GetTimerID();
		void				SetDelete(BOOL bDelete);
		int					IsDelete();
		TimerCallBackFunc	GetCallBackFunc() { return m_pCallBackFunc; }
		UINT64				GetEndTime(void);
		ThreadPool::ThreadSpinRWMutex & GetLock();

	public:
		TimerNode		*	m_pNext;
		TimerNode		*	m_pPrev;

	protected:
		BOOL				m_bDelete;
		UINT32				m_unTimerID;
		void			*	m_pObj;
		UINT32				m_unTimes;
		UINT32				m_unTimeInterval;
		TimeCount			m_objTimerCount;
		UINT32				m_unStartAddTime;  //5 记录开始加入时间轮的时间.
		INT64	            m_llEndTime;

		TimerCallBackFunc	m_pCallBackFunc;

	protected:
		ThreadPool::ThreadSpinRWMutex   m_objLock;
	};

}

#endif