// #ifndef __timer_internal_timer_task_h__
// #define __timer_internal_timer_task_h__
// #include "ThreadPool/inc/ThreadTask.h"
// 
// namespace Timer
// {
// 	class TimerNode;
// 	class TimerInterface;
// 
// 	class DLL_EXPORT InternalTimerTask : public ThreadPool::ThreadNormalTask
// 	{
// 	public:
// 		InternalTimerTask(TimerInterface * pTimerInterface , TimerNode * pNode);
// 		virtual ~InternalTimerTask( void );
// 
// 	public:
// 		virtual CErrno		Update() override;
// 
// 	private:
// 		TimerNode		*	m_pNode;
// 		TimerInterface	*	m_pTimerInterface;
// 	};
// }
// #endif