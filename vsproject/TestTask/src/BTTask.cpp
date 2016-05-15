#include "BTTask.h"
#include "LogLib/inc/Log.h"

namespace Task
{

	void BTTask::SetName(behaviac::string strName)
	{
		m_strName = strName;
		gDebugStream("BTTask::SetName=" << m_strName);
	}

	INT32 BTTask::GetCircleTaskCount(INT32 nIndex)
	{
		return 5;
	}

	void BTTask::ClearCircleTaskCount(INT32 nIndex)
	{
	}

	void BTTask::DecCircleTaskCount(INT32 nIndex , INT32 nCount)
	{
	}

	void BTTask::RecordCircleTaskID(INT32 nIndex, INT32 nID)
	{

	}

	void BTTask::NotifyClientCircleID(INT32 nIndex)
	{

	}

	bool BTTask::IsTaskFinished(INT32 nTaskID)
	{
		static INT32 nCount = 0;
		return  nCount++ % 2 == 0;
	}

	void BTTask::UpdateCircleTaskCount(INT32 & nIndex)
	{
		static INT32 nCount = 0;
		++nCount;
		nIndex = nCount;

	}



	BEGIN_PROPERTIES_DESCRIPTION(BTTask)
	{
		CLASS_DISPLAYNAME(L"BTTask任务")
		CLASS_DESC(L"用来配置任务和任务树")
		REGISTER_METHOD(SetName).DESC(L"设置任务名字").DISPLAYNAME(L"任务名").PARAM_DISPLAY_INFO(L"任务名" , L"输入任务名" , DefaultParam_t(L"结婚专用"));
		REGISTER_METHOD(GetCircleTaskCount).DESC(L"获得循环任务的次数").DISPLAYNAME(L"循环任务").PARAM_DISPLAY_INFO(L"获得循环任务次数", L"输入循环任务类型ID", DefaultParam_t((INT32)0) , 1 , 1000);
		REGISTER_METHOD(ClearCircleTaskCount).DESC(L"清空循环任务的次数").DISPLAYNAME(L"清空循环任务的次数").PARAM_DISPLAY_INFO(L"清空循环任务的次数", L"输入循环任务类型ID", DefaultParam_t((INT32)0), 1, 1000);
		REGISTER_METHOD(DecCircleTaskCount).DESC(L"减少循环任务的次数").DISPLAYNAME(L"减少循环任务的次数").PARAM_DISPLAY_INFO(L"减少循环任务的次数", L"输入循环任务类型ID", DefaultParam_t((INT32)0), 1, 1000);
		REGISTER_METHOD(RecordCircleTaskID).DESC(L"记录随机到的任务ID").DISPLAYNAME(L"记录随机到的任务").PARAM_DISPLAY_INFO(L"记录随机到的任务", L"输入循环任务类型ID", DefaultParam_t((INT32)0), 1, 1000);
		REGISTER_METHOD(NotifyClientCircleID).DESC(L"通知客户端随机到的任务").DISPLAYNAME(L"通知客户端随机到的任务").PARAM_DISPLAY_INFO(L"通知客户端随机到的任务", L"输入循环任务类型ID", DefaultParam_t((INT32)0), 1, 1000);
		REGISTER_METHOD(IsTaskFinished).DESC(L"任务是否完成").DISPLAYNAME(L"任务是否完成").PARAM_DISPLAY_INFO(L"任务ID", L"任务ID", DefaultParam_t((INT32)0), 1, 1000);
		REGISTER_METHOD(UpdateCircleTaskCount).DESC(L"更新任务计次").DISPLAYNAME(L"更新任务计次").PARAM_DISPLAY_INFO(L"任务ID", L"任务ID", DefaultParam_t((INT32)0), 1, 1000);

	}
	END_PROPERTIES_DESCRIPTION()
}