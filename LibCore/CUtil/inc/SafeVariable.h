//#ifndef __safe_variable_h__
//#define __safe_variable_h__
//#include "NetLib/inc/INetHandler.h"
//
//namespace ThreadPool
//{
//	template<typename T>
//	class SafeVariable 
//	{ 
//	public:
//		SafeVariable(){}
//		~SafeVariable(){}
//
//	public:
//		T & GetObject();
//		void SetObject(T obj);
//	private:
//		T               m_objVariable;
//		ThreadAutoLock  m_objAutoLock;
//	};
//
//	template<typename T>
//	T & ThreadPool::SafeVariable<T>::GetObject()
//	{
//		if(m_objLock.TryLock(false))
//		{
//			return m_objVariable;
//		} 
//	}
//
//}