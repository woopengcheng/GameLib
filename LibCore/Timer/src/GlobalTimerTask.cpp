#include "Timer/inc/GlobalTimerTask.h"
#include "Timer/inc/GlobalTimer.h"

namespace Timer
{

	GlobalTimerTask::GlobalTimerTask( void )
	{

	}

	GlobalTimerTask::~GlobalTimerTask( void )
	{
		ClearTimer();
	} 

	void GlobalTimerTask::OnTimer( UINT32 unTimerID , UINT32 unTimers )
	{

	}
	 
	INT32 GlobalTimerTask::SetTimer( UINT32 unInterval , UINT32 unTimes , UINT32 unStartTimer /*= 0*/  , UINT32 unTimerID /*= 0*/)
	{
		INT32 nID = GlobalTimer::GetInstance().SetTimer(unInterval , unStartTimer , unTimes ,  this , NULL , unTimerID); 
		if (nID > 0)
		{
			m_vecTimer.push_back(nID);
		}
 		return nID;
	}

	void GlobalTimerTask::ClearTimer()
	{
		CollectTimerIDT::iterator iter = m_vecTimer.begin();
		for (;iter != m_vecTimer.end();++iter)
		{
			RemoveTimer(*iter);
		}

		m_vecTimer.clear();
	}

	void GlobalTimerTask::RemoveTimer(INT32 nID)
	{
		GlobalTimer::GetInstance().RemoveTimer(nID);
	}

	INT32 GlobalTimerTask::SetTimerEx( UINT32 unStartExecuteTime , UINT32 unTimes /*= 0 */, UINT32 unTimerID /*= 0*/ )
	{
// 		INT32 nID = GlobalTimer::GetInstance().SetTimer(this , unStartExecuteTime , unTimes);
// 		if (nID > 0)
// 		{
// 			m_vecTimer.push_back(nID);
// 		}
// 		return nID;
		return 0;

	}

}