#ifndef __timer_min_heap_timer_h
#define __timer_min_heap_timer_h
/*#include "CUtil/inc/MinHeap.h"*/
#include "Timer/inc/TimerNode.h"
#include "Timer/inc/IStrategy.h"
#include <algorithm>

namespace Timer
{ 
// 	class DLL_EXPORT MinHeapTimer : public CUtil::MinHeap<TimerNode> , public IStrategy
// 	{
// 	public:
// 		MinHeapTimer( void ){}
// 		virtual ~MinHeapTimer( void ){} 
// 
// 	public:
// 		virtual CErrno		Init( void )override { return CErrno::Success(); }
// 		virtual CErrno		Cleanup( void ) override { return CErrno::Success(); }
// 		virtual TimerNode *	Update(void) override
// 		{ 
// 			TimerNode * pTimerNode = NULL;
// 			TimerNode * pNode = CUtil::MinHeap<TimerNode>::Update();
// 			if (pNode)
// 			{
// 				pTimerNode = dynamic_cast<TimerNode *>(pNode);
// 				if (NULL != pTimerNode)
// 				{
// 					CUtil::MinHeap<TimerNode>::RemoveNode(pTimerNode->GetTimerID());
// 				}
// 			} 
// 
// 			return pTimerNode;
// 		}
//  		virtual CErrno		SetTimer(UINT32 unTimeInterval , UINT32 unTimes, UINT32 unStartTime , void * pObj = NULL , TimerCallBackFunc pFunc = NULL){ return CErrno::Success(); }
// 
// 	public:
// 		virtual CErrno		InsertNode(UINT32 unTimerID ,TimerNode * pNode, bool bRemoveSame = true) override
// 		{
// 			return CUtil::MinHeap<TimerNode>::InsertNode(unTimerID , pNode);
// 		}
// 		virtual CErrno		RemoveNode(UINT32 unTimerID) override
// 		{
// 			return CUtil::MinHeap<TimerNode>::RemoveNode(unTimerID);
// 		}
// 
// 		virtual TimerNode * GetNode(UINT32 unTimerID) override
// 		{ 
// 			TimerNode* pNode = CUtil::MinHeap<TimerNode>::GetNode(unTimerID);
// 			if (pNode)
// 			{
// 				TimerNode * pTimerNode = dynamic_cast<TimerNode*>(pNode);
// 				return pTimerNode;
// 			}
// 			return NULL;
// 		}
// 
// 	};


	class DLL_EXPORT MinHeapTimer : public IStrategy
	{
	public:
		MinHeapTimer(void) {}
		virtual ~MinHeapTimer(void) {}

	public:
		typedef std::vector<TimerNode*>					VecTimerNodes;

	public:
		virtual CErrno			Init(void)override { return CErrno::Success(); }
		virtual CErrno			Cleanup(void) override { return CErrno::Success(); }
		virtual TimerNode	*	Update(void) override;
		virtual CErrno			SetTimer(UINT32 unTimeInterval, UINT32 unTimes, UINT32 unStartTime, void * pObj = NULL, TimerCallBackFunc pFunc = NULL) { return CErrno::Success(); }

	public:
		virtual CErrno			InsertNode(UINT32 unTimerID, TimerNode * pNode, bool bRemoveSame = true) override;
		virtual CErrno			RemoveNode(UINT32 unTimerID) override;
		virtual TimerNode	*	GetNode(UINT32 unTimerID);

	private:
		VecTimerNodes			m_vecNodes;
	};
 } 
#endif