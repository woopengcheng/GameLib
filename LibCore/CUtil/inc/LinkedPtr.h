#ifndef __cutil_linked_ptr_h__
#define __cutil_linked_ptr_h__

namespace CUtil
{
	template<typename T>
	class LinkedPtr
	{
	public:
		LinkedPtr()
			: m_pObject(NULL)
			, m_pPrev(NULL)
			, m_pNext(NULL)
		{

		}
		LinkedPtr(const LinkedPtr<T> & pMaster)
			: m_pObject(NULL)
			, m_pPrev(NULL)
			, m_pNext(NULL)
		{
			_Link(pMaster);
		}
		~LinkedPtr()
		{
			Reset();
		}

	public:
		T  * operator ->()
		{
			return m_pObject;
		}
		T  & operator *()
		{
			return *m_pObject;
		}
		operator T* ()
		{
			return m_pObject;
		}
		operator void operator = (const LinkedPtr<T> & pMaster)
		{
			_Link(pMaster);
		}
		operator bool operator == (const LinkedPtr<T> & pOther)
		{
			return pOther.m_pObject == m_pObject;
		}

		T * GetObject()
		{
			return m_pObject;
		}

		//5 引用计数不考虑第一次母体出来.只考虑引用的次数.
		INT32 GetRefCount()
		{
			INT32 nCount = 0;
			LinkedPtr<T> * pNext = m_pNext;
			LinkedPtr<T> * pPrev = m_pPrev;

			while(pNext != NULL)
			{
				++nCount;
				pNext = pNext->GetNext();
			}

			while(pPrev != NULL)
			{
				++nCount;
				pPrev = pPrev->GetPrev();
			}
		}

	public:
		void  Init(T * pObject)
		{
			m_pObject = pObject;
			m_pPrev = NULL;
			m_pNext = NULL;
		}

		void  Reset()
		{
			if (m_pPrev == NULL)
			{
				while(m_pNext)
				{
					m_pNext->Reset();
				}
			}

			if (m_pPrev)
			{
				m_pPrev->m_pNext = m_pNext;
			}
			if (m_pNext)
			{
				m_pNext->m_pPrev = m_pPrev;
			}

			m_pPrev = NULL;
			m_pNext = NULL;
			m_pObject = NULL;	
		} 

	protected: 
		void _Link(const LinkedPtr<T> & pMaster)
		{
			if (pMaster.m_pObject == NULL || pMaster.m_pObject == m_pObject)
			{
				return ;
			}

			Reset();

			m_pPrev = &pMaster;
			m_pNext = pMaster.m_pNext;
			if (m_pNext)
			{
				m_pNext.m_pPrev = this;
			}
			m_pObject = pMaster.m_pObject;

			LinkedPtr<T>& pMasterTmp = const_cast<LinkedPtr<T>&>(pMaster);
			pMasterTmp.m_pNext = this;
		}

	public:
		LinkedPtr<T> * m_pPrev;
		LinkedPtr<T> * m_pNext; 
		T			 * m_pObject;
	};
}



#endif