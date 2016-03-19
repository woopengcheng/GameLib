#ifndef __timer_global_timer_h
#define __timer_global_timer_h
#include "Timer/inc/TimerNode.h"
#include "Timer/inc/TimerInterface.h"

namespace Timer
{   
	/**
	 * @class : GlobalTimer
	 * @author: woo
	 * @date  : 2014年8月2日
	 * @file  : GlobalTimer.h
	 * @brief : 这个类继承自ThreadSustainTask.也就是有一个线程单独跑这个计时器.然后将计时器的任务分配给其他的线程.
	 */ 
	class DLL_EXPORT  GlobalTimer : public TimerInterface
	{
	public:
		GlobalTimer(void)
		{}
		virtual ~GlobalTimer(void){}
		 
	public:
		static GlobalTimer	&	GetInstance( void ){ static GlobalTimer m_sInstance; return m_sInstance;}

	public:
		virtual CErrno			Init(ETimerStrategyType objTimerStrategyType = TIMER_STRATEGY_DEFAULT) override;
		virtual CErrno			Cleanup(void) override;

	public:  
		virtual CErrno			Update(void) override;
	};  
} 
#endif