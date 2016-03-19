#include "Timer/inc/GlobalTimer.h"
#include "Timer/inc/GlobalTimerTask.h"
#include "Timer/inc/TimerHelp.h"
#include "CUtil/inc/CUtil.h"
#include "gtest/gtest.h"

static BOOL	 g_nClosedCount = 0;
static INT32 g_nTestTimerOneTimeID = 100;
static INT32 g_nTestTimerMultiTimesID = 101;
static INT32 g_nTestTimerLimitTimeID = 102;
static INT32 g_nTestTimerObjectStaticID = 103;
static INT32 g_nTestTimerStaticID = 104;
static INT32 g_nTestTimerMultiTimes = 3;

class TestTimer : public Timer::TimerTask
{
public:
	TestTimer()
		: test(1)
	{}
	virtual ~TestTimer() {}
	UINT32 test;

public:
	virtual void	OnTimer(UINT32 unTimerID, UINT32 unRemainTimers) override
	{
		if (unTimerID == g_nTestTimerOneTimeID)  //5 测试1次是否生效
		{
			++g_nClosedCount;
			EXPECT_EQ(unRemainTimers, 1);
		}
		else if (unTimerID == g_nTestTimerMultiTimesID)  //5 测试是否多次生效
		{
			static INT32 nTimes = g_nTestTimerMultiTimes;
			EXPECT_EQ(unRemainTimers, nTimes);
			--nTimes;
			++g_nClosedCount;
		}
		else if (unTimerID == g_nTestTimerLimitTimeID)//5 测试无限次是否生效
		{
			EXPECT_EQ(unRemainTimers, 0);
		}
		else
		{
			EXPECT_EQ(unRemainTimers,1);  //5 测试自动生成的ID
			++g_nClosedCount;
		}
	}
	virtual INT32	SetTimer(UINT32 unInterval, UINT32 unTimes = 0, UINT32 unStartTimer = 0, UINT32 unTimerID = 0) override
	{
		return Timer::GlobalTimer::GetInstance().SetTimer(unInterval, unTimes, unStartTimer,  this, NULL , unTimerID);
	}

	static void  RunTimer(void * pObj, UINT32 unTimerID, UINT32 unRemainTimers)
	{
		TestTimer * pTimerTask = (TestTimer *)pObj;
		EXPECT_EQ(!pTimerTask, false);
		EXPECT_EQ(pTimerTask->test, 1);

		EXPECT_EQ(unTimerID, g_nTestTimerObjectStaticID);
		EXPECT_EQ(unRemainTimers, 1);
		++g_nClosedCount;
	}
};

static INT32 g_nExtraID = 1000;
static void  RunTimer(void * pObj, UINT32 unTimerID, UINT32 unRemainTimers)
{
	if (pObj)
	{
		EXPECT_EQ(1, 0);
	}

	if (unTimerID == g_nTestTimerOneTimeID + g_nExtraID)  //5 测试1次是否生效
	{
		EXPECT_EQ(unRemainTimers, 1);
		++g_nClosedCount;
	}
	else if (unTimerID == g_nTestTimerMultiTimesID + g_nExtraID)  //5 测试是否多次生效
	{
		static INT32 nTimes = g_nTestTimerMultiTimes;
		EXPECT_EQ(unRemainTimers, nTimes);
		--nTimes;
		++g_nClosedCount;
	}
	else if (unTimerID == g_nTestTimerLimitTimeID + g_nExtraID)//5 测试无限次是否生效
	{
		EXPECT_EQ(unRemainTimers, 0);
	}
	else
	{
		EXPECT_EQ(unTimerID, g_nTestTimerStaticID);
		++g_nClosedCount;
	}

}


TEST(Timer , Timer_Test)
{
	Timer::GlobalTimer::GetInstance().Init();

	TestTimer test;
	test.SetTimer(1, 1, 0, g_nTestTimerOneTimeID);
	test.SetTimer(2, g_nTestTimerMultiTimes, 0, g_nTestTimerMultiTimesID);
	test.SetTimer(3, 0, 0, g_nTestTimerLimitTimeID);
	test.SetTimer(4, 1, 1);
	Timer::GlobalTimer::GetInstance().SetTimer(1, 1, 1, &test, &TestTimer::RunTimer , g_nTestTimerObjectStaticID);

	Timer::GlobalTimer::GetInstance().SetTimer(1, 1, 0, NULL, RunTimer, g_nTestTimerOneTimeID + g_nExtraID);
	Timer::GlobalTimer::GetInstance().SetTimer(2, g_nTestTimerMultiTimes, 1, NULL, RunTimer, g_nTestTimerMultiTimesID + g_nExtraID);
	Timer::GlobalTimer::GetInstance().SetTimer(3, 0, 1, NULL, RunTimer, g_nTestTimerLimitTimeID + g_nExtraID);
	Timer::GlobalTimer::GetInstance().SetTimer(5, 2, 5, NULL, RunTimer, g_nTestTimerStaticID);
	while (g_nClosedCount != 12)
	{
		Timer::GlobalTimer::GetInstance().Update();
		Timer::sleep(1);
	}

	Timer::GlobalTimer::GetInstance().Cleanup();
}
