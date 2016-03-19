#ifndef __thread_pool_thread_state_h__
#define __thread_pool_thread_state_h__
#include "ThreadPool/inc/ThreadCommon.h" 

namespace ThreadPool
{  
	enum ETHREAD_STATE
	{
		THREAD_STATE_UNKONWN = 0 ,
		THREAD_STATE_RUNNING ,
		THREAD_STATE_CLOSEING , 
		THREAD_STATE_CLOSED ,
	};

	class ThreadState
	{
	public:
		ThreadState(){}
		~ThreadState(){}

	public:
		void           SetThreadState(ETHREAD_STATE eThreadState){ m_eThreadState = eThreadState; }
		ETHREAD_STATE  GetThreadState(){ return m_eThreadState; }

	private:
		ETHREAD_STATE  m_eThreadState;
	};
};



#endif