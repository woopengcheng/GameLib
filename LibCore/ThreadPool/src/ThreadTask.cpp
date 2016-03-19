#include "ThreadPool/inc/ThreadTask.h"
#include "Timer/inc/TimerHelp.h"
#include "ThreadPool/inc/ThreadContext.h"
#include "ThreadPool/inc/ThreadState.h"
#include "CUtil/inc/CUtil.h"

namespace ThreadPool
{ 
	ThreadTask::ThreadTask( UINT32 unPriority , char * pTaskName /*= "UnknownName"*/ , BOOL bInStack /*=TRUE*/ )
		: m_unPriority(unPriority) 
		, m_bInStack(bInStack)
	{
		SetTaskName(pTaskName);   
	}

	void ThreadTask::SetThreadPriority( UINT32 unPriority )
	{
		 m_unPriority = unPriority; 
	}

	UINT32 ThreadTask::GetThreadPriority( void )
	{
		return m_unPriority; 
	} 

	void ThreadTask::OnStartThread( void )
	{

	}

	void ThreadTask::Run( void )
	{
		Update();
	}
	
	void ThreadTask::OnEndThread( void )
	{

	}

	char * ThreadTask::GetTaskName( void )
	{
		return m_aTaskName;
	}

	void ThreadTask::SetTaskName( char * pName )
	{
		CUtil::strncpy(m_aTaskName , MAX_NAME_LENGTH ,pName);
	}


	CErrno ThreadNormalTask::Update( void )
	{ 
		return CErrno::Success(); 
	}


	void ThreadSustainTask::Run( void )
	{
		while (1)
		{
			//5 处理线程退出的信号量的问题.
			ThreadContext<ThreadState> * pThreadContext = (ThreadContext<ThreadState> *)(pthread_getspecific(ThreadContext<ThreadState>::GetThreadKey()));
			if (pThreadContext && (pThreadContext->GetContext()->GetThreadState() == THREAD_STATE_CLOSEING || pThreadContext->GetContext()->GetThreadState() == THREAD_STATE_CLOSED))
			{
				pThreadContext->GetContext()->SetThreadState(THREAD_STATE_CLOSED);
				return ;
			}

			//5 这里处理当线程收到退出的时候.要退出线程.
			sem_t & objSem = ThreadContext<ThreadState>::GetThreadSem(); 
			INT32 nValue = -1;
			sem_getvalue(&objSem , &nValue);
			if (nValue == 1)
			{
				pThreadContext->GetContext()->SetThreadState(THREAD_STATE_CLOSED);
				return ;
			}  

			Update();

			Timer::sleep(1);
// 			pthread_testcancel();
		}
	}

	CErrno ThreadSustainTask::Update( void )
	{

		return CErrno::Success();
	}

}