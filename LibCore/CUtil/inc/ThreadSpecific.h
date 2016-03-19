#ifndef __cutil_thead_specific_h__
#define __cutil_thead_specific_h__

#include "CUtil/inc/Common.h"
#include "pthread.h"  
#include "semaphore.h"
#include <fcntl.h>

namespace CUtil
{
	//5 注意.在多线程中,每一个值都是唯一,例如初始化这种.可能是由主线程初始化.那么调用新建的线程可能就会有问题.即.只能在当前线程使用.
	template<typename T> 
	class ThreadSpecific
	{
	public:
		ThreadSpecific()
		{
			pthread_key_create(&mKey, ThreadSpecific<T>::destructor);
		}

		~ThreadSpecific()
		{
			pthread_key_delete(mKey);
		}
	public:
		void set(T* value)
		{
			pthread_setspecific(mKey, value);
		}

		T* get()
		{
			return (T*)pthread_getspecific(mKey);
		}
	private:
		static void destructor(void*)
		{

		}
	private:
		pthread_key_t mKey;
	};

}


#endif // _cutil_thead_specific_h__
