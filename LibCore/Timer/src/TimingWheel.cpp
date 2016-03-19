#include "Timer/inc/TimingWheel.h"

namespace Timer
{

	TimingWheel::TimingWheel()
		: m_ullCurTime(TIMING_WHEEL_PRECISE_FUNC)
	{
		memset(m_objRoot, 0, sizeof(TimerList) * TIMER_ROOT_SIZE);
		memset(m_aTimer, 0, sizeof(TimerList) * TIMER_OTHER_WHEEL_SIZE * TIMER_OTHER_SIZE);
	}

	TimingWheel::~TimingWheel()
	{

	}

	CErrno  TimingWheel::Init(void)
	{ 
		return CErrno::Success();
	}

	CErrno  TimingWheel::Cleanup(void)
	{
		m_mapNodes.clear();

		for (INT32 i = 0; i < TIMER_ROOT_SIZE; ++i)
		{
			TimerList & pList = m_objRoot[i];

			TimerNode * pNode = pList.GetTimerHead();
			TimerNode * pNext = NULL;
			while (pNode)
			{
				pNext = pNode->GetNext();
				SAFE_DELETE(pNode);
				pNode = pNext;
			}
		}

		for (INT32 i = 0; i < TIMER_OTHER_WHEEL_SIZE; ++i)
		{
			for (INT32 j = 0; j < TIMER_OTHER_SIZE; ++j)
			{
				TimerList & pList = m_aTimer[i][j];
				{
					TimerNode * pNode = pList.GetTimerHead();
					TimerNode * pNext = NULL;
					while (pNode)
					{
						pNext = pNode->GetNext();
						SAFE_DELETE(pNode);
						pNode = pNext;
					}
				}
			}
		}

		memset(m_objRoot, 0, sizeof(TimerList) * TIMER_ROOT_SIZE);
		memset(m_aTimer, 0, sizeof(TimerList) * TIMER_OTHER_WHEEL_SIZE * TIMER_OTHER_SIZE);

		return CErrno::Success();
	}

	CErrno  TimingWheel::RemoveNode(UINT32 unTimerID)
	{
		MapNodesT::iterator iter = m_mapNodes.find(unTimerID);
		if (iter != m_mapNodes.end())
		{
			TimerNode * pNode = iter->second;
			if (pNode)
			{
				pNode->SetDelete(TRUE);
				m_mapNodes.erase(iter);
			}
		}

		return CErrno::Success();
	}
	
	TimerNode * TimingWheel::GetNode(UINT32 unTimerID)
	{
		MapNodesT::iterator iter = m_mapNodes.find(unTimerID);
		if (iter != m_mapNodes.end())
		{
			return iter->second;
		}

		return NULL;
	}

	TimerList *	TimingWheel::GetNodePos(UINT64 ullEndTime, INT32 & nSize, UINT32 & unPos)
	{
		TimerList * pCurList = NULL;
		if (m_ullCurTime > ullEndTime)
		{
			return NULL;
		}
		else
		{
			UINT64 ullCurTime = m_ullCurTime;

			if ((ullEndTime >> TIMER_ROOT_SIZE_MASK_NUM) == ((ullCurTime) >> TIMER_ROOT_SIZE_MASK_NUM))
			{
				nSize = -1;
				unPos = ullEndTime & TIMER_ROOT_SIZE_MASK;
				return &(m_objRoot[unPos]);
			}

			nSize = 0;
			ullCurTime >>= TIMER_ROOT_SIZE_MASK_NUM;
			ullEndTime >>= TIMER_ROOT_SIZE_MASK_NUM;  //5 这里代表处于哪个时刻上.
			while (1)
			{
				if ((ullEndTime >> TIMER_OTHER_SIZE_MASK_NUM) == (ullCurTime >> TIMER_OTHER_SIZE_MASK_NUM))
				{
					break;
				}
				ullEndTime >>= TIMER_OTHER_SIZE_MASK_NUM;
				ullCurTime >>= TIMER_OTHER_SIZE_MASK_NUM;
				++nSize;
			}

			unPos = ullEndTime & TIMER_OTHER_SIZE_MASK;
			pCurList = &m_aTimer[nSize][unPos];
		}

		return pCurList;
	}

	CErrno  TimingWheel::DispathNode()
	{
		TimerList * pCurList = NULL;
		UINT64 ullCurTime = m_ullCurTime;
		UINT64 ullLastTime = m_ullCurTime - 1;
		if ((m_ullCurTime & TIMER_ROOT_SIZE_MASK) != 0)
		{
			return CErrno::Failure();
		}
		else
		{

			ullCurTime >>= TIMER_ROOT_SIZE_MASK_NUM;
			ullLastTime >>= TIMER_ROOT_SIZE_MASK_NUM;
			INT32 nSize = 0;
			while (1)
			{
				if ((ullLastTime >> TIMER_OTHER_SIZE_MASK_NUM) == (ullCurTime >> TIMER_OTHER_SIZE_MASK_NUM))
				{
					break;
				}
				ullLastTime >>= TIMER_OTHER_SIZE_MASK_NUM;
				ullCurTime >>= TIMER_OTHER_SIZE_MASK_NUM;
				++nSize;
			}

			pCurList = &m_aTimer[nSize][ullCurTime & TIMER_OTHER_SIZE_MASK];

			int nCount = -1;
			if (pCurList)
			{
				nCount = 0;
				memset(m_objRoot, 0, sizeof(TimerList) * TIMER_ROOT_SIZE);	//5 客户端不会跳帧.所以不用释放.
				TimerNode * pHead = pCurList->GetTimerHead();
				TimerNode * pNext = pHead;
				while (pHead)
				{
					pNext = pHead->GetNext();
					InsertNode(pHead->GetTimerID(), pHead, false);
					pHead = pNext;
					++nCount;
				}
			}
			m_aTimer[nSize][ullCurTime & TIMER_OTHER_SIZE_MASK].SetTimerHead(NULL);
		}
		return CErrno::Success();
	}

	CErrno  TimingWheel::InsertNode(UINT32 unTimerID, TimerNode * pNode, bool bRemoveSame/* = true*/)
	{
		UINT64 ullEndTime = pNode->GetEndTime();
		if (ullEndTime < m_ullCurTime)
		{
			return CErrno::Failure();
		}
		UINT64 ullFutureTime = ullEndTime;   //5 执行时需要将心跳放在最后.是为了保证每次插入都能执行成功.
		UINT64 ullCurTime = m_ullCurTime;

		MapNodesT::iterator iter = m_mapNodes.find(unTimerID);
		if (bRemoveSame && iter != m_mapNodes.end())
		{
			TimerNode * pOldNode = iter->second;
			pOldNode->SetDelete(TRUE);
			RemoveNode(unTimerID);
		}

		if ((ullFutureTime >> TIMER_ROOT_SIZE_MASK_NUM) == ((m_ullCurTime) >> TIMER_ROOT_SIZE_MASK_NUM))
		{
			m_mapNodes[unTimerID] = pNode;
			m_objRoot[ullFutureTime & TIMER_ROOT_SIZE_MASK].AddTimerNode(pNode);
			//				gDebugStream("insert_root:total=" << pNode->GetEndTime() << ":unTimerID=" << pNode->GetTimerID() << "internal=" << pNode->GetTimeInterval() << ":bDelete=%d" << pNode->IsDelete() << ":curTime=" << m_ullCurTime);

			return CErrno::Success();
		}

		ullCurTime >>= TIMER_ROOT_SIZE_MASK_NUM;
		ullFutureTime >>= TIMER_ROOT_SIZE_MASK_NUM;  //5 这里代表处于哪个时刻上.
		INT32 nSize = 0;
		while (1)
		{
			if ((ullFutureTime >> TIMER_OTHER_SIZE_MASK_NUM) == (ullCurTime >> TIMER_OTHER_SIZE_MASK_NUM))
			{
				break;
			}
			ullFutureTime >>= TIMER_OTHER_SIZE_MASK_NUM;
			ullCurTime >>= TIMER_OTHER_SIZE_MASK_NUM;
			++nSize;
		}

		m_aTimer[nSize][ullFutureTime & TIMER_OTHER_SIZE_MASK].AddTimerNode(pNode);
		m_mapNodes[unTimerID] = pNode;
		//			gDebugStream("insert_other:total=" << pNode->GetEndTime() << ":nTimerPos=" << nSize << ":unTimerID=" << pNode->GetTimerID() << "internal=" << pNode->GetTimeInterval() << ":bDelete=%d" << pNode->IsDelete() << ":curTime=" << m_ullCurTime);

		return CErrno::Success();
	}

	TimerNode * TimingWheel::Update(void)
	{
		TimerList * pCurList = NULL;

		++m_ullCurTime;
		INT32 nTemp = m_ullCurTime & TIMER_ROOT_SIZE_MASK;
		if (nTemp == 0)
		{
			DispathNode();
		}

		pCurList = &m_objRoot[nTemp];

		if (pCurList)
		{
			TimerNode * pNode = pCurList->GetTimerHead();
			m_objRoot[nTemp].SetTimerHead(NULL);
			// 				if (pNode)
			// 				{
			// 					gDebugStream("update:total=" << pNode->GetEndTime() << ":unTimerID=" << pNode->GetTimerID() << "internal=" << pNode->GetTimeInterval() << ":bDelete=%d" << pNode->IsDelete() << ":curTime=" << m_ullCurTime);
			// 				}
			return pNode;
		}
		else
			return NULL;
	}
}