#include "Timer/inc/GlobalTimer.h" 
#include "Timer/inc/TimerNode.h" 
#include "Timer/inc/TimerTask.h"
// #include "Timer/inc/InternalTimerTask.h" 

namespace Timer
{  
	CErrno GlobalTimer::Init(ETimerStrategyType objTimerStrategyType /*= TIMER_STRATEGY_DEFAULT*/)
	{
		return TimerInterface::Init(objTimerStrategyType);
	}

	CErrno GlobalTimer::Cleanup(void)
	{

		return TimerInterface::Cleanup();            //5 这里必须调用
	} 

	CErrno   GlobalTimer::Update( void )
	{  
		return TimerInterface::Update();
	} 

}