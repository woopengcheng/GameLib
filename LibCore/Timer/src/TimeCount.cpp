#include "Timer/inc/TimeCount.h"
#include "Timer/inc/TimerHelp.h"

namespace Timer
{  
	CErrno TimeCount::Start( UINT64 ullTimeInterval )
	{
		m_ullLastTime = Timer::GetTickMicroSecond();
		m_ullTimeInterval = ullTimeInterval;

		return CErrno::Success();
	}

	CErrno TimeCount::Cleanup()
	{
		m_ullLastTime = 0;
		m_ullTimeInterval = 0;

		return CErrno::Success();
	}

	BOOL TimeCount::IsExpired(UINT64 unCurTime/* = 0 */, UINT64 * pDiff/* = NULL*/)
	{
		if (!m_ullTimeInterval)
		{
			return FALSE;
		}
		else
		{
			if (!unCurTime)
			{
				unCurTime = Timer::GetTickMicroSecond();
			}
			if ((m_ullLastTime + m_ullTimeInterval) < unCurTime)
			{
				if (pDiff)
				{
					*pDiff = unCurTime - m_ullLastTime;
				}
				m_ullLastTime = unCurTime;
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL TimeCount::IsStarted()
	{
		return m_ullTimeInterval != 0;
	} 

	UINT64 TimeCount::ElapseTicks()
	{ 
		return Timer::GetMicroSecond(__max(0LL,Timer::GetTickMicroSecond() - m_ullLastTime));
	}

	UINT64 TimeCount::ResetTime()
	{
		return m_ullLastTime = Timer::GetTickMicroSecond();
	}









	void TimeCount_Safe::SetLastTime( UINT64 ullLastTime )
	{
		ThreadPool::AutoSpinRWLock objLock(m_objLock);
		m_ullLastTime = ullLastTime; 
	}

	UINT64 TimeCount_Safe::GetLastTime()
	{
		ThreadPool::AutoSpinRWLock objLock(m_objLock , false);
		return m_ullLastTime; 
	}

	void TimeCount_Safe::SetTimeInterval( UINT64 ullTimeInterval )
	{
		ThreadPool::AutoSpinRWLock objLock(m_objLock);
		m_ullTimeInterval = ullTimeInterval;
	}

	UINT64 TimeCount_Safe::GetTimeInterval()
	{
		ThreadPool::AutoSpinRWLock objLock(m_objLock , false);
		return m_ullTimeInterval; 
	}

	BOOL TimeCount_Safe::IsExpired( UINT64 unCurTime /*= 0 */, UINT64 * pDiff /*= NULL*/ )
	{
		UINT64 ullTimeInterval = GetTimeInterval();
		UINT64 ullLastTime = GetLastTime();
		if (!ullTimeInterval)
		{
			return FALSE;
		}
		else
		{
			if (!unCurTime)
			{
				unCurTime = Timer::GetTickMicroSecond();
			}
			if ((ullLastTime + ullTimeInterval) <= unCurTime)
			{
				if (pDiff)
				{
					*pDiff = unCurTime - ullLastTime;
				}
				SetLastTime(unCurTime);
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL TimeCount_Safe::IsStarted()
	{
		UINT64 ullTimeInterval = GetTimeInterval();
		return ullTimeInterval != 0; 
	}

	CErrno TimeCount_Safe::Start( UINT64 ullTimeInterval )
	{
		SetTimeInterval(ullTimeInterval);
		SetLastTime(Timer::GetTickMicroSecond());

		return CErrno::Success();
	}

	CErrno TimeCount_Safe::Cleanup()
	{
		SetLastTime(0);
		SetTimeInterval(0);

		return CErrno::Success();
	}

	UINT64 TimeCount_Safe::ElapseTicks()
	{ 
		return Timer::GetMicroSecond(__max(0LL,Timer::GetTickMicroSecond() - GetLastTime()));
	}

	CErrno TimeCount_Safe::ResetTime()
	{
		 SetLastTime(Timer::GetTickMicroSecond()) ;
		 return CErrno::Success();
	}
}