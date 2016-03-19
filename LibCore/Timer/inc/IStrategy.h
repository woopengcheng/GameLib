#ifndef __timer_i_strategy_h__
#define __timer_i_strategy_h__ 
#include "Timer/inc/TimerNode.h"  


namespace Timer
{ 
	class DLL_EXPORT IStrategy
	{ 
	public:
		IStrategy(void){}
		virtual ~IStrategy(void){ }

	public:
		virtual CErrno			Init(void) = 0;
		virtual CErrno			Cleanup(void) = 0;
		virtual TimerNode	*	Update(void) = 0;

	public:
		virtual CErrno			InsertNode(UINT32 unTimerID ,TimerNode * pNode , bool bRemoveSame = true) = 0 ;
		virtual CErrno			RemoveNode(UINT32 unTimerID) = 0;
	};   
} 
#endif