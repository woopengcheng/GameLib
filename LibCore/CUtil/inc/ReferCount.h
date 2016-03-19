#ifndef __cutil_refer_count_h__
#define __cutil_refer_count_h__ 
#include "CUtil/inc/Common.h" 
#include "CUtil/inc/AutomicUInt.h" 

namespace CUtil
{  
	class DLL_EXPORT HoderObject
	{
	public:
		virtual BOOL Release() = 0;
	};

	template<typename T>
	class Holder : boost::noncopyable 
	{
	public:
		explicit Holder(T * t) // this class should never be explicitly created
			: m_pObject(t)
		{
			MsgAssert(m_pObject , "null object in refcount.");
		}
	public:
		T * m_pObject;

		void zero() { refCount.zero(); }

		// these are called automatically by boost::intrusive_ptr
		void intrusive_ptr_add_ref(Holder* h) 
		{ 
			h->refCount++;
		}
		void intrusive_ptr_release(Holder* h) 
		{
#if defined(_DEBUG) // cant use dassert or DEV here
			MsgAssert((int)h->refCount > 0 , " make sure we haven't already freed the buffer");
#endif
			if(--(h->refCount) == 0)
			{
				m_pObject->Release();
			}
		}

		UINT32 GetRefCount()
		{
			return refCount.get();
		}
	private:
		AtomicUInt refCount;
	};
}
#endif
