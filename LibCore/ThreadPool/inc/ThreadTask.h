#ifndef __thread_pool_thread_task_h__
#define __thread_pool_thread_task_h__
#include "ThreadPool/inc/ThreadCommon.h"

namespace ThreadPool
{
	class DLL_EXPORT ThreadTask
	{
	public:
		ThreadTask(UINT32 unPriority , char * pTaskName = "UnknownName" , BOOL bInStack = TRUE);
		~ThreadTask(void){}

	public:
		virtual void		OnStartThread( void );    //5 线程开始执行的时候调用.
		virtual void		Run( void );
		virtual CErrno		Update( void ) = 0 ;
		virtual void		OnEndThread( void );      //5 线程结束的时候调用. 这里作为释放线程资源的地方.即使线程异常退出也是这里作为释放处.

	public:
		char			*	GetTaskName( void );
		void				SetTaskName(char * pName);
		UINT32				GetThreadPriority( void );
		void				SetThreadPriority(UINT32 unPriority);
		BOOL				GetInStack() const { return m_bInStack; }
		void				SetInStack(BOOL val) { m_bInStack = val; }

	private:
		UINT32				m_unPriority;                    //5 初始化只读.所以线程安全.
		char				m_aTaskName[MAX_NAME_LENGTH];    //5 初始化只读.所以线程安全.
		BOOL				m_bInStack;                      //5 初始化只读.所以线程安全.
	};


	class DLL_EXPORT ThreadNormalTask : public ThreadTask
	{ 
	public:
		ThreadNormalTask(UINT32 unPriority , char * pTaskName = "UnknownName", BOOL bInStack = FALSE):ThreadTask(unPriority , pTaskName , bInStack){}

	public:  
		virtual CErrno		Update( void ) = 0;
	};

	class DLL_EXPORT ThreadSustainTask : public ThreadTask
	{ 
	public:
		ThreadSustainTask(UINT32 unPriority , char * pTaskName = "UnknownName", BOOL bInStack = TRUE):ThreadTask(unPriority , pTaskName , bInStack){}
		~ThreadSustainTask(void){}

	public: 
		virtual void		Run( void ) override;
		virtual CErrno		Update( void ) = 0;  //5 里不需要再在Update里面加死循环.会阻塞程序.
	};
}


#endif