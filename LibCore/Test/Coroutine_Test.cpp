#include "Coroutine/Coroutine.h"
// #include "UnitTest++/UnitTestPP.h"
#include "CUtil/inc/CUtil.h"
#include "gtest/gtest.h"

const INT64 CO_NUM = 5;
static INT64 g_curIndex = 0;
static INT64 g_randIndex = 0;
static INT64 g_randIndexRecord[CO_NUM] = { 0 };

struct CoTask
{
	void * pCoID;
	void * pArg;
};

static void fiberProc(void * pArg)
{
	CoTask * pTask = (CoTask *)pArg;
	{
		INT64 nIndex;
		while (1)
		{
			nIndex = (INT64)(pTask->pArg);

			EXPECT_EQ(nIndex, g_curIndex);

			INT64 nPrevIndex = g_randIndex;
			g_randIndexRecord[nIndex] = g_randIndex;
			Coroutine::CoYieldCur();
			EXPECT_EQ(nPrevIndex, g_randIndexRecord[nIndex]);
		}
	}
}

TEST(Coroutine , co)
{
	typedef std::map<INT64, CoTask*> MapCoTasksT;
	MapCoTasksT	mapCoTasks;

	Coroutine::CoInit();
	for (INT64 i = 0; i < CO_NUM; i++)
	{
		CoTask * pTask = new CoTask;
		pTask->pArg = (void*)(i);

		Coroutine::CoCreate(&(pTask->pCoID), fiberProc, pTask);
		mapCoTasks.insert(std::make_pair(i, pTask));
	}
		
	for (INT64 j = 0; j < 2; ++j)
	{
		for (INT64 i = 0;i < CO_NUM;++i)
		{
			g_curIndex = i;
			g_randIndex = CUtil::random();
			Coroutine::CoResume(mapCoTasks[i]->pCoID);
		}
	}

	for (INT64 i = 0; i < CO_NUM; i++)
	{
		CoTask * pTask = mapCoTasks[i];
		if (pTask)
		{
			Coroutine::CoRelease(pTask->pCoID);
		}
		delete pTask;
	}
	mapCoTasks.clear();
	Coroutine::CoCleanup();
}
