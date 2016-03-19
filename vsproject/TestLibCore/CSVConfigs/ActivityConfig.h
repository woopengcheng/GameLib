// attention dont't change this line:INT32 ActivityId;std::string ActivityName;INT32 ActivityLevelLow;INT32 ActivityLevelHigh;bool IsShowEntrance;std::vector<std::string> EntranceIcon;bool IsShowInterface;std::vector<std::string> InterfaceIcon;INT32 StartTimeWeek;INT32 EndTimeWeek;INT32 StartTimeDate;INT32 EndTimeDate;std::string StartTime;std::string EndTime;INT32 Description;std::string RewardIcon;
/************************************
FileName	:	ActivityConfig.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-03-09 21:35:57
Description	:	csv读取文件
************************************/
#ifndef __Config_ActivityConfig_define_h__
#define __Config_ActivityConfig_define_h__
#include "ActivityConfigLoad.h"

namespace Config
{

	struct SActivityConfig
	{
		INT32							ActivityId;	//活动id
		std::string						ActivityName;	//活动名称
		INT32							ActivityLevelLow;	//显示等级下限,不到等级（不包括自身）不显示
		INT32							ActivityLevelHigh;	//显示等级上限，超过等级（不包括自身）不显示
		bool							IsShowEntrance;	//是否显示入口图标，入口图标和界面图标是互斥的；入口图标的形式适合单独放在外面的活动，界面图标就是通用活动中的图标
		std::vector<std::string>		EntranceIcon;	//入口图标
		bool							IsShowInterface;	//是否界面中显示，在这里显示，就是通用活动的形式；否则就是在入口图标中显示，显示为单独活动的形式
		std::vector<std::string>		InterfaceIcon;	//界面中图标
		INT32							StartTimeWeek;	//活动开始时间周，即每周的哪一天；这里填写的值优先级最大，与具体开始时间天，结束时间天无关
		INT32							EndTimeWeek;	//活动结束时间周
		INT32							StartTimeDate;	//活动开始时间天，不在此时间内，则活动不会显示
		INT32							EndTimeDate;	//活动结束时间天
		std::string						StartTime;	//活动开始时间点，不在此时间内则活动不显示
		std::string						EndTime;	//活动结束时间点
		INT32							Description;	//活动说明，通用活动界面中的活动描述
		std::string						RewardIcon;	//奖励图标，领奖界面中该活动的领奖图标
	};


	class ActivityConfig
	{
	public:
		typedef std_unordered_map<INT32 , SActivityConfig> MapConfigsT;

	public:
		bool LoadFrom(const std::string& filepath);
		SActivityConfig * GetActivityConfig(INT32 nIndex);

	private:
		MapConfigsT m_mapConfigs;

	};
	extern ActivityConfig * g_pActivityConfig;
}

#endif// end  __Config_ActivityConfig_define_h__
