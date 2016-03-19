#include "Coroutine/Coroutine.h"
#include "LogLib/inc/Log.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include "Coroutine/libco/co_routine.h"
	#include "Coroutine/libco/co_routine_inner.h"
	#include "Coroutine/libco/coctx.h"
#endif

namespace Coroutine
{
	CUtil::ThreadSpecific<void> g_FiberContext;

#ifdef WIN32
	INT32 CoInit()
	{
		void * pFiberContext = ConvertThreadToFiber(NULL);
		MsgAssert_ReF1(pFiberContext, "Create fiber error.");

		g_FiberContext.set(pFiberContext);
		return 0;
	}
	INT32 CoCreate(void ** pCoID, coroutine_func pFunc, void * pArg)
	{
		*pCoID = CreateFiber(0, pFunc, pArg);
		if (*pCoID == NULL)
		{
			gErrorStream("create fiber failed.\n");
		}

		return 0;
	}

	void CoResume(void * pCoID)
	{
		SwitchToFiber(pCoID);
	}

	void CoYield(void * pCoID)
	{
		SwitchToFiber(pCoID);
	}

	void Coroutine::CoYieldCur(void)
	{
		void * pFiberContext = g_FiberContext.get();
		Coroutine::CoResume(pFiberContext);
	}

	void CoRelease(void * pCoID)
	{
		DeleteFiber(pCoID);
	}

	void * CoSelf()
	{
		return GetCurrentFiber();
	}

	void CoCleanup()
	{
		ConvertFiberToThread();
	}

#else
	INT32 CoInit()
	{

		return 0;
	}
	INT32 CoCreate(void ** pCoID, coroutine_func pFunc, void * pArg)
	{
		return co_create((stCoRoutine_t**)pCoID, NULL, pFunc, pArg);
	}

	void CoResume(void * pCoID)
	{
		co_resume((stCoRoutine_t *)pCoID);
	}

	void CoYield(void * pCoID)
	{
		co_yield((stCoRoutine_t *)pCoID);
	}

	void Coroutine::CoYieldCur(void)
	{
		co_yield_ct();
	}

	void CoRelease(void * pCoID)
	{
		co_release((stCoRoutine_t *)pCoID);
	}

	void * CoSelf()
	{
		return co_self();
	}

	void CoCleanup()
	{
	}


	INT32 CoPoll(stCoEpoll_t * ctx, pollfd fds[], nfds_t nfds, INT32 timeout_ms)
	{
		return co_poll(ctx , fds , nfds , timeout_ms);
	}

	void CoEventloop(stCoEpoll_t * ctx, pfn_co_eventloop_t pfn, void * arg)
	{
		return co_eventloop(ctx, pfn, arg);
	}

	INT32 CoSetspecific(pthread_key_t key, const void * value)
	{
		return co_setspecific(key , value);
	}

	void * CoGetspecific(pthread_key_t key)
	{
		return co_getspecific(key);
	}

	stCoEpoll_t * CoGetEpollCt()
	{
		return co_get_epoll_ct();
	}

	void CoEnableHookSys()
	{
		return co_enable_hook_sys();
	}

	void CoDisableHookSys()
	{
		return co_disable_hook_sys();
	}

	bool CoIsEnableHookSys()
	{
		return co_is_enable_sys_hook();
	}
#endif // !WIN32

}
