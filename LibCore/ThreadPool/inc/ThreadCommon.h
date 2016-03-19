#ifndef __thread_pool_thread_common_h__
#define __thread_pool_thread_common_h__
#include "CUtil/inc/Common.h"
#include "tbb/concurrent_unordered_map.h"
#include "tbb/concurrent_hash_map.h"
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_vector.h"
#include "tbb/concurrent_unordered_set.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h> 
  
namespace ThreadPool
{ 
#define MAX_THREAD_TASK_NUMBER   0xfffffff
	
#define tbb_unordered_map tbb::concurrent_unordered_map 
#define tbb_hash_map      tbb::concurrent_hash_map 
#define tbb_queue         tbb::concurrent_queue 
#define tbb_unordered_set tbb::concurrent_unordered_set

#define THREAD_SLEEP_COUNT       1000   //5 关闭线程的时候每次需要执行的次数
#define MAX_THREAD_SLEEP_COUNT   100000 //5 关闭线程最多执行的次数.

#define DEFAULT_THREAD_SEM_PATH  "./ThreadSem.sem"

	enum ETHREAD_SIG
	{
		THREAD_SIG_ALIVE = 0   , //5 0是询问线程生死的.
		THREAD_SIG_QUIT  = 250 , //5 要求线程退出
	};

	enum THREAD_PRIORITY
	{ 
		
	};

	struct ThreadFuncParam
	{
		void * pParam;
		UINT32 unPriority;

		ThreadFuncParam(void * p , UINT32 unPrio) : pParam(p) , unPriority(unPrio){}
		~ThreadFuncParam(){}
	};



} //Thread
#endif