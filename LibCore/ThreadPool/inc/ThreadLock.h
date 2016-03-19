#ifndef __thread_pool_thread_lock_h__
#define __thread_pool_thread_lock_h__

#include "ThreadPool/inc/ThreadCommon.h" 
#include "tbb/spin_mutex.h"
#include "tbb/spin_rw_mutex.h"

namespace ThreadPool
{
	class  ThreadSpinMutex : public tbb::spin_mutex {	};

	class  AutoSpinLock : public ThreadSpinMutex::scoped_lock
	{
	public:
		AutoSpinLock( tbb::spin_mutex & m)
			: ThreadSpinMutex::scoped_lock(m)
		{ 
		}

		AutoSpinLock( ThreadSpinMutex & m )
			: ThreadSpinMutex::scoped_lock(m)
		{

		}
		~AutoSpinLock(){}
	};

	class  ThreadSpinRWMutex : public tbb::spin_rw_mutex {}; 
	 
	class  AutoSpinRWLock : public ThreadSpinRWMutex::scoped_lock
	{
	public:
		AutoSpinRWLock() 
			: ThreadSpinRWMutex::scoped_lock() {}
		AutoSpinRWLock( tbb::spin_rw_mutex & m , bool write = true )
			: ThreadSpinRWMutex::scoped_lock(m , write)
		{ 
		}
		AutoSpinRWLock( ThreadSpinRWMutex & m , bool write = true )
			: ThreadSpinRWMutex::scoped_lock(m , write)
		{

		}
		~AutoSpinRWLock(){} 
	};
}
#endif
