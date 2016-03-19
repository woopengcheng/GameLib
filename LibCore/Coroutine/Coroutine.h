#ifndef __coroutine_coroutine_h__
#define __coroutine_coroutine_h__
#include "CUtil/inc/Common.h"
#include "CUtil/inc/ThreadSpecific.h"

typedef void  (*coroutine_func)(void*);

namespace Coroutine
{
	INT32		CoInit();
	INT32		CoCreate(void ** pCoID, coroutine_func pFunc, void * pArg);
	void		CoResume(void * pCoID);
	void		CoYield(void * pCoID);
	void		CoYieldCur(void);
	void		CoRelease(void * pCoID);
	void	*	CoSelf();
	void		CoCleanup();

#ifdef LINUX
	INT32		CoPoll(stCoEpoll_t *ctx, struct pollfd fds[], nfds_t nfds, INT32 timeout_ms);
	void 		CoEventloop(stCoEpoll_t *ctx, pfn_co_eventloop_t pfn, void *arg);

	//3.specific

	INT32		CoSetspecific(pthread_key_t key, const void *value);
	void	*	CoGetspecific(pthread_key_t key);

	//4.event
	stCoEpoll_t*CoGetEpollCt(); //ct = current thread
								   
	//5.hook syscall ( poll/read/write/recv/send/recvfrom/sendto )
	void 		CoEnableHookSys();
	void		CoDisableHookSys();
	bool		CoIsEnableHookSys();
#endif
}


#endif // !__coroutine_coroutine_h__

