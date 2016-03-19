#ifndef __timer_time_count_h__
#define __timer_time_count_h__ 
#include "Timer/inc/TimerCommon.h"
#include "ThreadPool/inc/ThreadLock.h"

namespace Timer
{
	class DLL_EXPORT TimeCount
	{
 	public:
 		TimeCount()
 			: m_ullLastTime(0)
 			, m_ullTimeInterval(0)
 		{}
 		~TimeCount(){}
 
 	public:
		virtual CErrno		Start(UINT64 ullTimeInterval);
		virtual CErrno		Cleanup(); 
 
 	public:
 		//************************************
 		// Method:    IsExpired
 		// FullName:  IsExpired
 		// Access:    public 
 		// Returns:   BOOL
 		// Qualifier: 获取当前时间是否满足.
 		// Parameter: UINT64 unCurTime  如果有传递当前时间.就用传递值.如果没有.自动获取.
 		// Parameter: UINT64 * pDiff  通过指针的方式获取相差的值得大小.
 		//************************************
		BOOL				IsExpired(UINT64 unCurTime = 0 , UINT64 * pDiff = NULL);
 		BOOL				IsStarted();
		UINT64				ElapseTicks();
 		UINT64				ResetTime(); 
		
	private:
 		UINT64				m_ullLastTime;
 		UINT64				m_ullTimeInterval;
	};

	
 	class DLL_EXPORT TimeCount_Safe
 	{
 	public:
 		TimeCount_Safe()
 			: m_ullLastTime(0)
 			, m_ullTimeInterval(0)
 		{}
 		~TimeCount_Safe(){}
 
 	public:
 		virtual CErrno		Start(UINT64 ullTimeInterval);
 		virtual CErrno		Cleanup(); 
 
 	public:
 		//************************************
 		// Method:    IsExpired
 		// FullName:  IsExpired
 		// Access:    public 
 		// Returns:   BOOL
 		// Qualifier: 获取当前时间是否满足.
 		// Parameter: UINT64 unCurTime  如果有传递当前时间.就用传递值.如果没有.自动获取.
 		// Parameter: UINT64 * pDiff  通过指针的方式获取相差的值得大小.
 		//************************************
 		BOOL				IsExpired(UINT64 unCurTime = 0 , UINT64 * pDiff = NULL);
 		BOOL				IsStarted();
		UINT64				ElapseTicks();
 		CErrno				ResetTime();
 
 	public:
 		void				SetLastTime(UINT64 ullLastTime);
 		UINT64				GetLastTime();
 		void				SetTimeInterval(UINT64 ullTimeInterval);
 		UINT64				GetTimeInterval();
 
	protected:
 		UINT64				m_ullLastTime;
		UINT64				m_ullTimeInterval;

	protected:
  		ThreadPool::ThreadSpinRWMutex m_objLock;
 	};

}
 
#endif