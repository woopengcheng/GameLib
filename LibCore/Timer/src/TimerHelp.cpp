#include "Timer/inc/TimerHelp.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace Timer
{  
#ifdef WIN32
	UINT64 GetTickCount()
	{  
		UINT64  llCount = 0;
		QueryPerformanceCounter((LARGE_INTEGER *)(&llCount));

		return llCount;
	}

	UINT64 GetTickFrequency()
	{
		static UINT64  s_llFrequency = 0;
		if (!s_llFrequency)
		{
			QueryPerformanceFrequency((LARGE_INTEGER *)(&s_llFrequency));
		}

		return s_llFrequency;
	} 
#else
	UINT64 GetTickCount()
	{ 
		timespec t;
		::clock_gettime(CLOCK_MONOTONIC,&t);              //5 CLOCK_REALTIME不同.这个是过去的某个逝去的时间点的次数.
		return (UINT64)t.tv_sec * TIME_MICRO_PRECISE + t.tv_nsec / (1e9 / TIME_MICRO_PRECISE);   //5 微妙级别
	}

	UINT64 GetTickFrequency()
	{  
		return TIME_MICRO_PRECISE;
	}  
#endif

	UINT64 GetTickSecond(UINT64 llTime /*= -1*/ )
	{
		if (llTime == -1)
		{
			return  GetTickCount() / GetTickFrequency(); 
		} 
		else
		{
			return  llTime / GetTickFrequency(); 
		}
	}

	UINT64 GetTickMicroSecond(UINT64 llTime /*= -1*/ )
	{
		if (llTime == -1)
		{
			return  GetTickCount() * 1000  / GetTickFrequency(); 
		} 
		else
		{
			return  llTime * 1000  / GetTickFrequency(); 
		} 
	}

	UINT64 GetTickMilliSecond(UINT64 llTime /*= -1*/ )
	{
		if (llTime == -1)
		{
			return  GetTickCount() * 1000000 / GetTickFrequency(); 
		} 
		else
		{
			return  llTime * 1000000 / GetTickFrequency(); 
		} 
	}

	UINT64 GetMilliSecond(UINT64 llTime )
	{
		return llTime * GetTickFrequency() / 1000000;

	}

	UINT64 GetMicroSecond(UINT64 llTime )
	{
		return llTime * GetTickFrequency() / 1000;
	}

	UINT64 GetSecond(UINT64 llTime )
	{
		return llTime * GetTickFrequency();
	}

	UINT64 DiffMilliSecond(UINT64 llTime1 , UINT64 llTime2 )
	{
		return ::abs((long long)(llTime1 - llTime2)) * 1000000 / GetTickFrequency(); 
	}

	UINT64 DiffMicroSecond(UINT64 llTime1 , UINT64 llTime2 )
	{
		return ::abs((long long)(llTime1 - llTime2)) * 1000 / GetTickFrequency();

	}

	UINT64 DiffSecond(UINT64 llTime1 , UINT64 llTime2 )
	{
		return ::abs((long long)(llTime1 - llTime2)) / GetTickFrequency();
	}

	std::string GetDate(std::string strFormat/* = "%Y-%m-%d"*/)
	{
		time_t t = time(0); 
		char tmp[64]; 
		strftime( tmp, sizeof(tmp), strFormat.c_str(),localtime(&t) );  
		return std::string(tmp); 
	}

	time_t GetTime()
	{
		return time(0);
	}

	void sleep(UINT64 llMillSec)
	{
#ifdef WIN32
		::Sleep((DWORD)llMillSec);
#else  
		struct timespec objTimeSpec;
		objTimeSpec.tv_sec = llMillSec / 1000;
		objTimeSpec.tv_nsec = (llMillSec % 1000) * 1000000L;
		::nanosleep (&objTimeSpec, 0);
#endif
	}
}