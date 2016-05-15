#ifndef __Task_bttask_h__
#define __Task_bttask_h__
#include "CUtil/inc/Common.h"
#include "behaviac/behaviac.h"

namespace Task
{
	class BTTask : public behaviac::Agent
	{
	public:
		DECLARE_BEHAVIAC_AGENT(Task::BTTask, behaviac::Agent);

	public:
		BTTask()
		{
		}

		virtual ~BTTask()
		{
		}

	public:
		virtual CErrno		Init()
		{
			return CErrno::Success();
		}
		virtual CErrno		Cleanup()
		{
			return CErrno::Success();
		}

	public:
		void				SetName(behaviac::string strName);
		INT32				GetCircleTaskCount(INT32 nIndex);
		void				ClearCircleTaskCount(INT32 nIndex);
		void				DecCircleTaskCount(INT32 nIndex, INT32 nCount);
		void				RecordCircleTaskID(INT32 nIndex, INT32 nID);
		void				NotifyClientCircleID(INT32 nIndex);
		bool				IsTaskFinished(INT32 nTaskID);
		void				UpdateCircleTaskCount(INT32 & nIndex);

	private:
		behaviac::string	m_strName;
	};

}



#endif