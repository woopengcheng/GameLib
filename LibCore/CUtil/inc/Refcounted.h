#ifndef __cutil_refcounted_h__
#define __cutil_refcounted_h__
#include "CUtil/inc/Common.h"
#include "CUtil/inc/ReferCount.h"

namespace CUtil
{

	template<bool MultiThread> class Refcounted
	{

	};

	template<> class Refcounted<true>
	{
	public:
		Refcounted()
		{
			mRef = 0;
			pthread_spin_init(&mLock,0);
		}

		virtual ~Refcounted()
		{
			pthread_spin_destroy(&mLock);
		}

		virtual int DoRef()
		{
			pthread_spin_lock(&mLock);
			++mRef;
			pthread_spin_unlock(&mLock);

			return 0;
		}

		virtual int UnRef()
		{
			pthread_spin_lock(&mLock);
			bool result = (--mRef) == 0;
			pthread_spin_unlock(&mLock);

			if(result)
				delete this;
			return 0;
		}
	private:
		int mRef;
		pthread_spinlock_t mLock;
	};

	template<> class Refcounted<false>
	{
	public:
		Refcounted()
		{
			mRef = 0;
		}

		virtual ~Refcounted()
		{
		}

		virtual int DoRef()
		{
			++mRef;

			return 0;
		}

		virtual int UnRef()
		{
			--mRef;

			if(mRef == 0)
				delete this;
			return 0;
		}

	protected:
		int mRef;
	};

	template<class T> class RefcountedPtr
	{
	public:
		RefcountedPtr(T* v = NULL)
			: mObj(v)
		{
			if(mObj != NULL)
				mObj->doref();
		}

		RefcountedPtr(const RefcountedPtr<T>& v)
		{
			mObj = v.mObj;
			if(mObj != NULL)
				mObj->doref();
		}

		~RefcountedPtr()
		{
			if(mObj != NULL)
				mObj->unref();
			mObj = NULL;
		}

		T* get()
		{
			return mObj;
		}

		operator T*()
		{
			return mObj;
		}

		T* operator ->()
		{
			return mObj;
		}

		operator T&()
		{
			return *mObj;
		}

		RefcountedPtr<T>& operator =(const RefcountedPtr<T>& v)
		{
			if(mObj == v.mObj)
				return *this;

			if(mObj != NULL)
				mObj->unref();
			mObj = v.mObj;
			if(mObj != NULL)
				mObj->doref();
			return *this;
		}

	public:
		void reset(T* obj = NULL)
		{
			if(mObj == obj)
				return;
			if(mObj != NULL)
				mObj->unref();
			mObj = obj;
			if(mObj != NULL)
				mObj->doref();
		}
	private:
		T* mObj;
	};

	class OptionalRefcounted : public Refcounted<false>
	{
	public:
		bool use_ref;

		OptionalRefcounted()
			: use_ref(false)
		{
		}

		virtual int doref() 
		{ 
			if(!use_ref) return 0; 
			return Refcounted<false>::DoRef();
		}
		virtual int unref()
		{
			if(!use_ref) return 0; 
			return Refcounted<false>::UnRef();
		}
	}; 
}

#endif // _pw_refcounted_