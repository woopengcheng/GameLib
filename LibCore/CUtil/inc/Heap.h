#ifndef __cutil_heap_h__
#define __cutil_heap_h__
#include "CUtil/inc/Common.h"

namespace CUtil
{
#define MAX_NODE_SIZE 10000                   //5 定义100000个计时器.

	template <typename NodeType>
	class Heap
	{
	public:
		typedef tbb_hash_map<INT32 , NodeType *> MapNodesT;      //5 保证线程安全.

	public:
		Heap(void){}
		virtual ~Heap(void){ m_mapNodes.clear(); }

	public:
		virtual  CErrno Init(void)
		{
			return CErrno::Success();
		}

		virtual  CErrno Cleanup(void)
		{
			MapNodesT::iterator iter = m_mapNodes.begin();
			for (; iter != m_mapNodes.end(); ++iter)
			{
				SAFE_DELETE(iter->second);
			}
			m_mapNodes.clear();

			return CErrno::Success();
		}

		virtual  NodeType	*	Update(void)
		{
			return NULL;
		}

	public:
		virtual  CErrno		InsertNode(UINT32 unNodeID , NodeType * pNode){ return CErrno::Success(); }
		virtual  CErrno		RemoveNode(UINT32 unNodeID) { return CErrno::Success(); }
		virtual  NodeType * GetNode(UINT32 unNodeID)
		{
			if (unNodeID < GetHeapSize())
			{
				ThreadPool::AutoSpinRWLock objLock(m_objLock, false);
				if (m_pNodes)
				{
					return m_pNodes[unNodeID];
				}
			}

			return NULL;
		}

	protected:
		virtual  CErrno		HeapRebuildDown(NodeType * pRebuildNode){ return CErrno::Success(); }
		virtual  CErrno		HeapRebuildUp(NodeType  * pRebuildNode) { return CErrno::Success(); }

	public:
		INT32	GetHeapParentPos(UINT32 unPos)
		{
			return unPos == 0 ? 0 : ((unPos - 1) / 2);
		}

		INT32	GetHeapLeftPos(UINT32 unPos)
		{
			return unPos * 2 + 1;
		}

		INT32	GetHeapRightPos(UINT32 unPos)
		{
			return unPos * 2 + 2;
		}

		UINT32	GetHeapSize(void)
		{
			return (UINT32)(m_mapNodes.size());
		}

		INT32	SwitchNode(NodeType  * pMoveNode, UINT32 unMoveNodePos)
		{
			if (pMoveNode)
			{
				SetNode(unMoveNodePos, pMoveNode);
				return TRUE;
			}

			return FALSE;
		}

		void	SetNode(UINT32 unNodePos, NodeType * pNode)
		{
			if (pNode && unNodePos < GetHeapSize())
			{
				ThreadPool::AutoSpinRWLock objLock(m_objLock);
				if (m_pNodes)
				{
					m_pNodes[unNodePos] = pNode;
					pNode->SetNodePos(unNodePos);
				}
			}
		}

	protected:
		MapNodesT			m_mapNodes;					//5 主要是用来查找Node所处的位置,然后进行删除.
		NodeType		*	m_pNodes[MAX_NODE_SIZE];	//5 采用数组的方式来实现堆. 
		ThreadPool::ThreadSpinRWMutex   m_objLock;		//5 用来锁堆这个类的.
	};   
	  
} 
#endif