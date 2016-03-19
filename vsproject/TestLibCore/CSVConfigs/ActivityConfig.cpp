// attention dont't change this line:INT32 ActivityId;std::string ActivityName;INT32 ActivityLevelLow;INT32 ActivityLevelHigh;bool IsShowEntrance;std::vector<std::string> EntranceIcon;bool IsShowInterface;std::vector<std::string> InterfaceIcon;INT32 StartTimeWeek;INT32 EndTimeWeek;INT32 StartTimeDate;INT32 EndTimeDate;std::string StartTime;std::string EndTime;INT32 Description;std::string RewardIcon;
/************************************
FileName	:	ActivityConfig.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-03-09 21:35:57
Description	:	csv读取数据文件实现
************************************/
#include "ActivityConfig.h"
#include "LogLib/inc/Log.h"

namespace Config
{
	bool ActivityConfig::LoadFrom(const std::string & filepath)
	{
		Config::ActivityConfigLoad loadConfig;
		MsgAssert_Re0(loadConfig.LoadFrom(filepath) , "Error ActivityConfigLoadFrom " << filepath);

		for(size_t i = 0; i < loadConfig.Count(); ++i)
		{
			Config::SActivityConfigLoad& config = loadConfig.Get(i);
			Config::SActivityConfig data = {0};
			data.ActivityId = config.ActivityId;
			data.ActivityName = config.ActivityName;
			data.ActivityLevelLow = config.ActivityLevelLow;
			data.ActivityLevelHigh = config.ActivityLevelHigh;
			data.IsShowEntrance = config.IsShowEntrance;
			data.EntranceIcon = config.EntranceIcon;
			data.IsShowInterface = config.IsShowInterface;
			data.InterfaceIcon = config.InterfaceIcon;
			data.StartTimeWeek = config.StartTimeWeek;
			data.EndTimeWeek = config.EndTimeWeek;
			data.StartTimeDate = config.StartTimeDate;
			data.EndTimeDate = config.EndTimeDate;
			data.StartTime = config.StartTime;
			data.EndTime = config.EndTime;
			data.Description = config.Description;
			data.RewardIcon = config.RewardIcon;
			m_mapConfigs.insert(std::make_pair(data.ActivityId,data));
		}
		return true;
	}

	SActivityConfig * ActivityConfig::GetActivityConfig(INT32 nIndex)
	{
		MapConfigsT::iterator iter = m_mapConfigs.find(nIndex);
		if(iter == m_mapConfigs.end())
		{
			gWarniStream( "ActivityConfig::GetActivityConfig NotFound " << nIndex);
			return NULL;
		}

		return &iter->second;
	}

	ActivityConfig * g_pActivityConfig = NULL;
}

