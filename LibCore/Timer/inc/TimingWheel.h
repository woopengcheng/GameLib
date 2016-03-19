#ifndef __timer_timing_wheel_h__
#define __timer_timing_wheel_h__
#include "Timer/inc/TimerCommon.h"
#include "Timer/inc/TimerNode.h"  
#include "Timer/inc/IStrategy.h"
#include "Timer/inc/TimerHelp.h" 
#include "LogLib/inc/Log.h"

#define TIMER_ROOT_SIZE_MASK_NUM	((UINT32)8)
#define TIMER_OTHER_SIZE_MASK_NUM	((UINT32)6)
#define TIMER_ROOT_SIZE				((UINT32)256)
#define TIMER_OTHER_SIZE			((UINT32)64)
#define TIMER_OTHER_WHEEL_SIZE		((UINT32)4) 
#define TIMER_OTHER_SIZE_MASK		0X3F
#define TIMER_ROOT_SIZE_MASK		0XFF

namespace Timer
{
	class DLL_EXPORT TimerList
	{
	public:
		TimerList()
			: m_pHead(NULL)
		{

		}
	public:
		TimerNode * GetTimerHead() { return m_pHead; }
		void SetTimerHead(TimerNode * pNode) { m_pHead = pNode; }

		void  AddTimerNode(TimerNode * pNode)
		{
			pNode->m_pNext = m_pHead;
			m_pHead = pNode;
		}

	protected:
		TimerNode * m_pHead;
	};

	class DLL_EXPORT TimingWheel : public IStrategy
	{
	public:
		typedef std_unordered_map<UINT32, TimerNode *> MapNodesT;

	public:
		TimingWheel();
		virtual ~TimingWheel();

	public:
		virtual CErrno			Init(void) override;
		virtual CErrno			Cleanup(void) override;

	public:
		virtual CErrno			InsertNode(UINT32 unTimerID, TimerNode * pNode, bool bRemoveSame = true) override;
		virtual CErrno			RemoveNode(UINT32 unTimerID) override;
		virtual TimerNode	*	Update(void) override;

	protected:
		TimerNode			*	GetNode(UINT32 unTimerID);
		CErrno					DispathNode();
		TimerList			*	GetNodePos(UINT64 ullEndTime, INT32 & nSize, UINT32 & unPos);

	protected:
		TimerList				m_objRoot[TIMER_ROOT_SIZE];
		TimerList				m_aTimer[TIMER_OTHER_WHEEL_SIZE][TIMER_OTHER_SIZE];
		UINT64					m_ullCurTime;
		MapNodesT				m_mapNodes;
	};
}


#endif