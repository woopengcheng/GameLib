#ifndef __cutil_node_h__
#define __cutil_node_h__
#include "CUtil/inc/Common.h"
#include "CUtil/inc/ClassTypeInterface.h" 
#include "ThreadPool/inc/ThreadLock.h"

namespace CUtil
{ 
	template<typename ValueType>
	class  Node : public CUtil::ClassTypeInterface
	{
		CLASS_TYPE_ID(Node<ValueType> , CUtil::ClassTypeInterface , CTID_Node)
	public:
		Node()
			: m_unNodePos(-1)
			, m_objValue(0)
		{

		}
		explicit Node(ValueType objValue)
			: m_unNodePos(-1)
			, m_objValue(objValue)
		{

		}
		virtual ~Node(void)
		{

		}

	public:
		void SetNodePos(UINT32 unNodePos)
		{
			ThreadPool::AutoSpinRWLock objLock(m_objLock); 
			m_unNodePos = unNodePos;   
		}

		INT32 GetNodePos( void )
		{
			ThreadPool::AutoSpinRWLock objLock(m_objLock , false); 
			return m_unNodePos; 
		}

		ValueType   GetValue( void )
		{
			ThreadPool::AutoSpinRWLock objLock(m_objLock , false); 
			return m_objValue;
		} 
		ThreadPool::ThreadSpinRWMutex & GetLock(){ return m_objLock; }
	protected:
		INT32                m_unNodePos;
		ValueType            m_objValue;
		ThreadPool::ThreadSpinRWMutex   m_objLock; 
	}; 
} 
 
 
#endif