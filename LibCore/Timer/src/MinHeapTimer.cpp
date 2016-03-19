#include "Timer/inc/MinHeapTimer.h"
#include "LogLib/inc/Log.h"

namespace Timer
{
	class MinHeapCompare
	{
	public:
		bool operator ()(TimerNode *e1, TimerNode *e2)
		{
			return e1->GetEndTime() > e2->GetEndTime();
		}
	};

	TimerNode	* MinHeapTimer::Update(void)
	{
		if (m_vecNodes.size() <= 0)
		{
			return NULL;
		}

		return m_vecNodes[0];
	}

	CErrno MinHeapTimer::InsertNode(UINT32 unTimerID, TimerNode * pNode, bool bRemoveSame /*= true*/)
	{
		do 
		{
			if (GetNode(unTimerID))
			{
				if (bRemoveSame)
				{
					RemoveNode(unTimerID);
				}
				else
				{
					gErrorStream("minheaptimer insert sameid.ID=" << unTimerID);
					return CErrno::Failure();
				}
			}
			
			m_vecNodes.push_back(pNode);  //5 相当于放到了最后一个,然后重新建立堆.
			push_heap(m_vecNodes.begin() , m_vecNodes.end() , MinHeapCompare());
		} while (false);

		return CErrno::Success();
	}

	CErrno MinHeapTimer::RemoveNode(UINT32 unTimerID)
	{
		VecTimerNodes::iterator iter = m_vecNodes.begin();
		for (;iter != m_vecNodes.end();++iter)
		{
			TimerNode * pNode = *iter;
			if (pNode && pNode->GetTimerID() == unTimerID)
			{
				pop_heap(m_vecNodes.begin(), m_vecNodes.end() , MinHeapCompare());  //5 相当于放到了最后一个.
				m_vecNodes.pop_back();  //5 在使用的时候才释放内存.

//				gDebugStream("erase timer:Obj=" << pNode->GetObject() << ":id=" << unTimerID << ":value=" << pNode->GetValue());
				return CErrno::Success();
			}
		}

		return CErrno::Success();
	}

	TimerNode * MinHeapTimer::GetNode(UINT32 unTimerID)
	{
		VecTimerNodes::iterator iter = m_vecNodes.begin();
		for (; iter != m_vecNodes.end(); ++iter)
		{
			TimerNode * pNode = *iter;
			if (pNode && pNode->GetTimerID() == unTimerID)
			{
				return pNode;
			}
		}
		return NULL;
	}

}