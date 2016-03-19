#include "ThreadPool/inc/ThreadPool.h"
#include "ThreadPool/inc/ThreadTask.h"
#include "Timer/inc/TimerHelp.h"
#include "ThreadPool/inc/ThreadContext.h"
#include "ThreadPool/inc/ThreadState.h"
#include "LogLib/inc/Log.h"
#include <signal.h> 

namespace ThreadPool
{  
	void SigHandler( INT32 nSigno )
	{
		switch(nSigno)
		{
		case THREAD_SIG_QUIT:
			{
				ThreadContext<ThreadState> * pThreadContext = (ThreadContext<ThreadState> *)(pthread_getspecific(ThreadContext<ThreadState>::GetThreadKey()));
				Assert(pThreadContext);

				pThreadContext->GetContext()->SetThreadState(THREAD_STATE_CLOSEING);
			}break;
		default:
			{
				break;
			}
		}
	}

	IThreadPool::IThreadPool()
		: m_bPirorityStrict(TRUE)
		, m_bInited(FALSE)
	{

	}

	CErrno IThreadPool::Init(std::map<UINT32, UINT32 > & mapPriorityCount, BOOL bPirorityStrict/* = TRUE*/)
	{
		std::map<UINT32 , UINT32 >::iterator iter = mapPriorityCount.begin();
		for (;iter != mapPriorityCount.end();++ iter)
		{
			m_mapThreadPriorityCount.insert(std::make_pair(iter->first , iter->second));
		}

		m_bPirorityStrict = bPirorityStrict;
 //		RegisterSig(THREAD_SIG_QUIT , SigHandler);
		return CErrno::Success();
	}

	CErrno IThreadPool::Cleanup()
	{
		Assert_ReF(KillChildrenThread())
		{
			m_mapThreadPriorityCount.clear();

			MapThreadTasksT::iterator iter = m_mapThreadTasks.begin();
			for (;iter != m_mapThreadTasks.end();++iter)
			{
				ThreadTask * pTask = NULL;
				QueueThreadTasksT & pQueue = iter->second;
				while(pQueue.try_pop(pTask))
				{
					SAFE_DELETE(pTask);
				} 
			}
			m_mapThreadTasks.clear();
		}   

// 		if( sem_close( &ThreadContext<ThreadState>::GetThreadSem() ) != 0 )
// 		{
// 			Assert_Re0(0 && "Close 信号灯失败...\n"); 
// 		}
//
// 		if( sem_unlink( DEFAULT_THREAD_SEM_PATH ) != 0 )
// 		{
// 			Assert(0 && "删除信号灯失败....\n"); 
// 		} 

		return CErrno::Success();
	}

	CErrno IThreadPool::Startup()
	{
		if (!m_bInited)
		{
			ThreadContext<ThreadState>::Init();

			pthread_t pID = pthread_self();
			pthread_detach(pID);

			pthread_t th;
			MapThreadPriorityCountT::iterator iter = m_mapThreadPriorityCount.begin();
			for (; iter != m_mapThreadPriorityCount.end(); ++iter)
			{
				for (UINT32 i = 0; i < iter->second; ++i)
				{
					ThreadFuncParam * pThreadFuncParam;
					{
						pThreadFuncParam = new ThreadFuncParam(this, iter->first);
						CreateThread(th, NULL, &IThreadPool::ThreadFunc, (void*)pThreadFuncParam);

						m_queueThreads.push(th);
						Timer::sleep(1);
					}
				}
			}
			m_bInited = TRUE;
			return CErrno::Success();
		}
		else
		{
			gErrorStream("threadpool init muilt times.");
		}

		return CErrno::Failure();
	}

	CErrno IThreadPool::Closeup()
	{

		return CErrno::Success();
	}
	 
	BOOL IThreadPool::AddTask( ThreadTask * pThreadTask )
	{
		if (pThreadTask && m_mapThreadTasks.size() < MAX_THREAD_TASK_NUMBER)
		{
			UINT32 unPriority = pThreadTask->GetThreadPriority();
			MapThreadTasksT::accessor result; 
			if (!m_mapThreadTasks.find(result , unPriority))
			{ 
				MapThreadTasksT::accessor result2; 
				m_mapThreadTasks.insert(result2 , unPriority);
				 
				result2->second.push(pThreadTask);
			} 
			else
			{ 
				QueueThreadTasksT &pQueue = result->second;
				pQueue.push(pThreadTask);
			}
		}
		else
		{
			SAFE_DELETE(pThreadTask);
		}

		return TRUE;
	}

	ThreadTask * IThreadPool::FetchTask(  UINT32 unThreadPriority )
	{
		ThreadTask * pThreadTask = NULL;

		if (m_mapThreadTasks.size() != 0)
		{ 
			if (m_bPirorityStrict)
			{
				MapThreadTasksT::accessor result;
				if(m_mapThreadTasks.find(result , unThreadPriority)) 
				{
					QueueThreadTasksT & pQueue = result->second;
					if(!pQueue.try_pop(pThreadTask))
					{
						return NULL;
					}  
				}
			}
			else
			{
				pThreadTask = FetchLowerBoundTask(unThreadPriority);
			}
		}

		return pThreadTask;
	}

	ThreadTask * IThreadPool::FetchLowerBoundTask( UINT32 unThreadPriority )
	{
		ThreadTask * pThreadTask = NULL;

		if (m_mapThreadTasks.size() != 0)
		{
			MapThreadTasksT::iterator iter = m_mapThreadTasks.begin();
			for (;iter != m_mapThreadTasks.end();++ iter)
			{
				if (iter->first >= unThreadPriority)
				{
					QueueThreadTasksT &pQueue = iter->second;
					if(pQueue.try_pop(pThreadTask))
					{ 
						return pThreadTask;
					}
				}
			} 
		}

		return pThreadTask;
	}

	UINT32 IThreadPool::GetThreadsCount()
	{
		UINT32 unCount = 0;
		MapThreadPriorityCountT::iterator iter = m_mapThreadPriorityCount.begin();
		for (;iter != m_mapThreadPriorityCount.end();++ iter)
		{
			unCount += iter->second;
		}

		return unCount;

	} 

	void * IThreadPool::ThreadFunc( void * pParam )
	{
		ThreadFuncParam * pThreadFuncParam = (ThreadFuncParam * )pParam;
		UINT32       unThreadPriority = pThreadFuncParam->unPriority;
		IThreadPool * pThreadPool      = (IThreadPool *)(pThreadFuncParam->pParam);
		SAFE_DELETE(pThreadFuncParam);

//		pthread_detach( pthread_self() );   //5 以防pthread_join等待线程结束.获取线程的返回值.而当前线程还没有释放资源.

		ThreadContext<ThreadState> *pThreadContext = new ThreadContext<ThreadState>;
		Assert_Re0(pThreadContext && pThreadContext->GetContext());
		pthread_setspecific(ThreadContext<ThreadState>::GetThreadKey() , pThreadContext);
		pThreadContext->GetContext()->SetThreadState(THREAD_STATE_RUNNING);

		while(true)
		{
			try
			{
				//5 这里暂时使用pthread自带的sem进行信号检测.
				if (pThreadContext->GetContext()->GetThreadState() == THREAD_STATE_CLOSEING ||
					pThreadContext->GetContext()->GetThreadState() == THREAD_STATE_CLOSED)
				{
					pThreadContext->GetContext()->SetThreadState(THREAD_STATE_CLOSED);
//					return NULL;
				}

				sem_t & objSem = ThreadContext<ThreadState>::GetThreadSem(); 
				INT32 nValue = -1;
				sem_getvalue(&objSem , &nValue);
				if (nValue == 1)
				{
					pThreadContext->GetContext()->SetThreadState(THREAD_STATE_CLOSED); 
//					return NULL;
				} 

				ThreadTask * pThreadTask = pThreadPool->FetchTask( unThreadPriority );
				if (pThreadTask)
				{
					pthread_cleanup_push(ThreadContext<ThreadState>::ThreadCleanup , (void *)pThreadTask);

					pThreadTask->OnStartThread();
					pThreadTask->Run();
					pThreadTask->OnEndThread();

					if (!pThreadTask->GetInStack())
					{
						SAFE_DELETE(pThreadTask);
					}

					pthread_cleanup_pop(0);    //5 这里如果没有异常就弹出栈顶的处理函数. 
				}

				pthread_testcancel();
			} 
			catch ( ... )
			{
			}
		}

		SAFE_DELETE(pThreadContext);

		return NULL;
	}

	CErrno IThreadPool::CreateThread( UINT32 unPriority , UINT32 unThreadCount /*= 1*/ )
	{
		if (HasPriorityThread(unPriority))
		{
			gErrorStream("create thread failed. repeat priority , priorityID=" << unPriority);
			return CErrno::Failure();
		}
		else
		{
			m_mapThreadPriorityCount.insert(std::make_pair(unPriority , unThreadCount));
		}

		pthread_t th; 
		for (UINT32 i = 0;i < unThreadCount; ++i)
		{  
			ThreadFuncParam * pThreadFuncParam;
			{
				pThreadFuncParam = new ThreadFuncParam(this , unPriority); 
				CreateThread( th, NULL, &IThreadPool::ThreadFunc, (void*)pThreadFuncParam); 

				m_queueThreads.push(th); 
				Timer::sleep(1);
			}  
		} 

		return CErrno::Success();
	}

	CErrno IThreadPool::CreateThread(pthread_t &th , pthread_attr_t * attr , thread_fun  pFunc , void * pParam )
	{ 
		pthread_attr_t objAtrr;
		if (attr == NULL)
		{ 
			pthread_attr_init(&objAtrr);   
			pthread_attr_setscope (&objAtrr, PTHREAD_SCOPE_SYSTEM);   
			pthread_attr_setdetachstate (&objAtrr, PTHREAD_CREATE_JOINABLE);   
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL);
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL);
			pthread_create( &th, &objAtrr, pFunc, (void*)pParam); 
			return  CErrno::Success();
		}
		else 
		{
			pthread_create( &th, attr, pFunc, (void*)pParam);     //5 创建成功返回0 
			return CErrno::Success();
		}
	}

	BOOL IThreadPool::HasPriorityThread( UINT32 unPriority )
	{ 
		MapThreadPriorityCountT::iterator iter = m_mapThreadPriorityCount.find(unPriority); 
		if (iter == m_mapThreadPriorityCount.end())
		{
			return FALSE;
		}

		return TRUE;
	} 

	BOOL IThreadPool::KillChildrenThread()
	{ 
 		sem_post(&ThreadContext<ThreadState>::GetThreadSem()); 

		UINT32  nCount = 0;
		pthread_t th;
		while(m_queueThreads.try_pop(th))
		{  
			MsgAssert_Re0(!pthread_cancel(th) , "pthread_cancel fail");

			void * pValue = 0;
			if(pthread_join(th , (void**)(&pValue)))
			{
				Assert_Re0(0 && "thread is not quit.");
			}

//			INT32 nRe = pthread_kill(th , THREAD_SIG_ALIVE);
// 			if (nRe == 0)
// 			{
// #ifdef WIN32
// 				raise(THREAD_SIG_QUIT);
// #else
//				pthread_kill(th , THREAD_SIG_QUIT);
//#endif
//				m_queueThreads.push(th);
//  		}
// 			else if (nRe == ESRCH)
// 			{
// 				Timer::sleep(1);
// 			}
// 			else
// 				Assert_Re0(0);
// 			++nCount;
// 
// 			if (nCount % THREAD_SLEEP_COUNT == 0)
// 			{
// 				Timer::sleep(1);
// 			}
// 			if (nCount >= MAX_THREAD_SLEEP_COUNT)
// 			{
// 				return FALSE;     //5 这里执行10万次还没有将所有的进程杀掉.就宕掉.
// 			}
		}

		return TRUE;
	}

	void IThreadPool::RegisterSig( int nSigno , FuncSigHandler  pSigHandler )
	{
#ifndef WIN32
		struct sigaction  objAction;
		memset(&objAction , 0 , sizeof(struct sigaction));

		objAction.sa_flags = 0;
		objAction.sa_handler = SigHandler;
		sigaction(nSigno , &objAction , NULL);
#else
//		signal(nSigno , SigHandler);
#endif
	}

}