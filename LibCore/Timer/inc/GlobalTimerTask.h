#ifndef __timer_global_timer_task_h
#define __timer_global_timer_task_h
#include "Timer/inc/TimerTask.h" 


namespace Timer
{ 
	/**
	 * @class : TimerTask
	 * @author: woo
	 * @date  : 2014年8月2日
	 * @file  : TimerTask.h
	 * @brief : 这个类主要是需要使用到计时器的类继承,然后重载OnTimer函数.等待计时器触发.
	 */
	class DLL_EXPORT GlobalTimerTask : public TimerTask
	{
	public:
		typedef std::vector<int>			CollectTimerIDT;

	public:
		GlobalTimerTask( void );
		virtual ~GlobalTimerTask( void );

	public:		 
		virtual void		OnTimer(UINT32 unTimerID , UINT32 unTimers = 0) override;
		virtual INT32		SetTimer( UINT32 unInterval , UINT32 unTimes = 0, UINT32 unStartTimer = 0 , UINT32 unTimerID = 0) override;
		virtual INT32		SetTimerEx(UINT32 unStartExecuteTime , UINT32 unTimes = 0 , UINT32 unTimerID = 0);
		virtual void		ClearTimer();
		virtual	void		OnRemoveTimer(INT32 nID){;}
		
	public:
		void				RemoveTimer(INT32 nID);

	private:
		CollectTimerIDT		m_vecTimer;
	};
}

#endif