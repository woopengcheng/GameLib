// attention dont't change this line:INT32 ActivityId;std::string ActivityName;INT32 ActivityLevelHigh;bool IsShowEntrance;std::vector<std::string> EntranceIcon;bool IsShowInterface;std::vector<std::string> InterfaceIcon;INT32 StartTimeWeek;INT32 EndTimeWeek;INT32 StartTimeDate;INT32 EndTimeDate;std::string StartTime;std::string EndTime;INT32 Description;std::string RewardIcon;Timer::Date testdate;testDateStructTimer::Date begin;Timer::Date end;std::vector<Timer::Date> dateArray;
/************************************
FileName	:	ActivityConfig.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-03 20:12:41
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
			data.testdate = config.testdate;
			{
				data.testDateStruct.begin = config.testDateStruct.begin;
				data.testDateStruct.end = config.testDateStruct.end;
			}
			data.dateArray = config.dateArray;
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

	bool ActivityConfig::RunUse(INT32 nIndex , CUtil::Player * pPlayer/* = NULL*/ , CUtil::League * pLeague/* = NULL*/ , CUtil::Team * pTeam/* = NULL*/)
	{
		if (GetActivityConfig(nIndex) == NULL)
		{
			gErrorStream("RunUse error. ActivityConfig  no this id.id=" << nIndex);
			return false;
		}

		if (pPlayer != NULL && pLeague != NULL && pTeam != NULL)
		{
			if (CUtil::Condition<CUtil::PLAYER_LEVEL>()(pPlayer,pLeague)>(GetActivityConfig(nIndex)->ActivityLevelHigh)&&(CUtil::Condition<CUtil::VIP_LEVEL>()(pPlayer)<((GetActivityConfig(nIndex)->Description)+(GetActivityConfig(nIndex)->StartTimeDate))||(GetActivityConfig(nIndex)->testdate)==Timer::Date("2016-3-28-3:15:10"))&&(CUtil::Condition<CUtil::pow>()(1221.123,1221.124)>1221.125))
			{
				pPlayer->Mail(pLeague , pTeam , 12);
			}
		}

		if (pPlayer != NULL && pLeague != NULL)
		{
			if (CUtil::Condition<CUtil::PLAYER_LEVEL>()(pPlayer,pLeague)>(GetActivityConfig(nIndex)->ActivityLevelHigh)&&CUtil::Condition<CUtil::VIP_LEVEL>()(pPlayer)>(GetActivityConfig(nIndex)->EndTimeDate)||CUtil::Condition<CUtil::TEST_MULIT_ARGS>()(pPlayer,(GetActivityConfig(nIndex)->IsShowInterface),(GetActivityConfig(nIndex)->Description),1231231,1221.123,(GetActivityConfig(nIndex)->RewardIcon),Timer::Date("2016-3-28-3:15:10")))
			{
				pPlayer->Say(12 , "动态礼包");
			}
		}

		if (pPlayer != NULL && pLeague != NULL)
		{
			if (CUtil::Condition<CUtil::PLAYER_LEVEL>()(pPlayer,pLeague)>(GetActivityConfig(nIndex)->ActivityLevelHigh)&&CUtil::Condition<CUtil::VIP_LEVEL>()(pPlayer)>(GetActivityConfig(nIndex)->EndTimeDate))
			{
			}
		}

		return true;
	}

	bool ActivityConfig::RunUse1(INT32 nIndex , CUtil::Player * pPlayer/* = NULL*/ , CUtil::League * pLeague/* = NULL*/ , CUtil::Team * pTeam/* = NULL*/)
	{
		if (GetActivityConfig(nIndex) == NULL)
		{
			gErrorStream("RunUse error. ActivityConfig  no this id.id=" << nIndex);
			return false;
		}

		if (pPlayer != NULL && pLeague != NULL && pTeam != NULL)
		{
			if (CUtil::Condition<CUtil::PLAYER_LEVEL>()(pPlayer,pLeague)>(GetActivityConfig(nIndex)->ActivityLevelHigh)&&(CUtil::Condition<CUtil::VIP_LEVEL>()(pPlayer)<(GetActivityConfig(nIndex)->Description)||(GetActivityConfig(nIndex)->testdate)==Timer::Date("2016-3-28-3:15:10")))
			{
				pPlayer->Mail(pLeague , pTeam , 12);
			}
		}

		if (pPlayer != NULL && pLeague != NULL)
		{
			if (CUtil::Condition<CUtil::PLAYER_LEVEL>()(pPlayer,pLeague)>(GetActivityConfig(nIndex)->ActivityLevelHigh)&&CUtil::Condition<CUtil::VIP_LEVEL>()(pPlayer)>(GetActivityConfig(nIndex)->EndTimeDate))
			{
				pPlayer->Say(12 , "动态礼包");
			}
		}

		return true;
	}

	ActivityConfig * g_pActivityConfig = NULL;
}

