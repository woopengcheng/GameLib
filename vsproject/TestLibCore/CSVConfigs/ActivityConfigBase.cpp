// attention dont't change this line:std::string ActivityId;std::string ActivityName;INT32 ActivityLevelLow;INT32 ActivityLevelHigh;bool IsShowEntrance;std::vector<std::string> EntranceIcon;bool IsShowInterface;std::vector<std::string> InterfaceIcon;INT32 StartTimeWeek;INT32 EndTimeWeek;INT32 StartTimeDate;INT32 EndTimeDate;std::string StartTime;std::string EndTime;INT32 Description;std::string RewardIcon;Timer::Date testdate;testDateStructTimer::Date begin;Timer::Date end;std_unordered_map<INT64 , SActionConfig *> actionConfig;std::vector<Timer::Date> dateArray;ActionConfig testConfig;ActionConfig testConfig2;std::map<std::string , INT64> dateCommon;TestStructArraybool test1;INT64 test2;double test3;INT32 test4;std::string test5;std_unordered_map<INT64 , SActionConfig *> test6;TestStructbool test1;INT64 test2;double test3;INT32 test4;std::string test6;std_unordered_map<INT64 , SActionConfig *> test7;
/************************************
FileName	:	ActivityConfigBase.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-22 11:43:23
Description	:	csv读取数据文件实现
************************************/
#include "ActivityConfigBase.h"
#include "LogLib/inc/Log.h"
#include "ActionConfig.h"

namespace Config
{
	bool ActivityConfigBase::LoadFrom(const std::string & filepath)
	{
		if (m_bLoaded)
		{
			return false;
		}
		Config::ActivityConfigLoad loadConfig;
		MsgAssert_Re0(loadConfig.LoadFrom(filepath + "ActivityConfig.tabcsv") , "Error ActivityConfigLoadFrom " << filepath + "ActivityConfig.tabcsv");

		for(size_t i = 0; i < loadConfig.Count(); ++i)
		{
			Config::SActivityConfigLoad& config = loadConfig.Get(i);
			Config::SActivityConfig data;
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
			data.testdate = config.testdate;
			{
				data.testDateStruct.begin = config.testDateStruct.begin;
				data.testDateStruct.end = config.testDateStruct.end;
				data.testDateStruct.actionConfig.insert(std::make_pair(config.testDateStruct.actionConfig , Config::ActionConfig::GetInstance().GetActionConfig(config.testDateStruct.actionConfig , filepath)));
			}
			data.dateArray = config.dateArray;
			data.testConfig.insert(std::make_pair(config.testConfig , Config::ActionConfig::GetInstance().GetActionConfig(config.testConfig , filepath)));
			data.testConfig2.insert(std::make_pair(config.testConfig2 , Config::ActionConfig::GetInstance().GetActionConfig(config.testConfig2 , filepath)));
			data.dateCommon = config.dateCommon;
			{
				std::vector<SActivityConfigLoad::STestStructArray>::iterator iter = config.vecTestStructArray.begin();
				std::vector<SActivityConfigLoad::STestStructArray>::iterator end = config.vecTestStructArray.end();
				for (; iter != end;++iter)
				{
					SActivityConfig::STestStructArray array;
					array.test1 = iter->test1;
					array.test2 = iter->test2;
					array.test3 = iter->test3;
					array.test4 = iter->test4;
					array.test5 = iter->test5;
					array.test6.insert(std::make_pair(iter->test6 , Config::ActionConfig::GetInstance().GetActionConfig(iter->test6 , filepath)));
					data.vecTestStructArray.push_back(array);
				}
			}
			{
				data.TestStruct.test1 = config.TestStruct.test1;
				data.TestStruct.test2 = config.TestStruct.test2;
				data.TestStruct.test3 = config.TestStruct.test3;
				data.TestStruct.test4 = config.TestStruct.test4;
				data.TestStruct.test6 = config.TestStruct.test6;
				data.TestStruct.test7.insert(std::make_pair(config.TestStruct.test7 , Config::ActionConfig::GetInstance().GetActionConfig(config.TestStruct.test7 , filepath)));
			}
			m_mapConfigs.insert(std::make_pair(data.ActivityId,data));
		}

		m_bLoaded = true;
		return true;
	}

	bool ActivityConfigBase::RepairLoad(const std::string & filepath)
	{
		MsgAssert_Re0(m_bLoaded , "error ActivityConfig .no load data.")
		MapConfigsT::iterator iterConfig = m_mapConfigs.begin();

		for(; iterConfig != m_mapConfigs.end(); ++iterConfig)
		{
			Config::SActivityConfig & data = iterConfig->second;
			{
				data.testDateStruct.actionConfig.begin()->second = Config::ActionConfig::GetInstance().GetActionConfig(data.testDateStruct.actionConfig.begin()->first , filepath);
			}
			data.testConfig.begin()->second = Config::ActionConfig::GetInstance().GetActionConfig(data.testConfig.begin()->first , filepath);
			data.testConfig2.begin()->second = Config::ActionConfig::GetInstance().GetActionConfig(data.testConfig2.begin()->first , filepath);
			{
				std::vector<SActivityConfig::STestStructArray> & vecTestStructArray = data.vecTestStructArray;
				std::vector<SActivityConfig::STestStructArray>::iterator iterTestStructArray = vecTestStructArray.begin();
				std::vector<SActivityConfig::STestStructArray>::iterator endTestStructArray = vecTestStructArray.end();
				for (; iterTestStructArray != endTestStructArray;++iterTestStructArray)
				{
					SActivityConfig::STestStructArray & array = *iterTestStructArray;
					array.test6.begin()->second = Config::ActionConfig::GetInstance().GetActionConfig(array.test6.begin()->first , filepath);
				}
			}
			{
				data.TestStruct.test7.begin()->second = Config::ActionConfig::GetInstance().GetActionConfig(data.TestStruct.test7.begin()->first , filepath);
			}
		}

		return true;
	}

	SActivityConfig * ActivityConfigBase::GetActivityConfig(std::string id , std::string strFilePath/* = ""*/)
	{
		MapConfigsT::iterator iter = m_mapConfigs.find(id);
		if(iter == m_mapConfigs.end())
		{
			return NULL;
		}

		return &iter->second;
	}

	bool ActivityConfigBase::RunUse(std::string id , CUtil::Player * pPlayer/* = NULL*/ , CUtil::League * pLeague/* = NULL*/ , CUtil::Team * pTeam/* = NULL*/)
	{
		if (GetActivityConfig(id) == NULL)
		{
			gErrorStream("RunUse error. ActivityConfig  no this id.id=" << id);
			return false;
		}

		if (pPlayer != NULL && pLeague != NULL && pTeam != NULL)
		{
			if (CUtil::Condition<CUtil::PLAYER_LEVEL>()(pPlayer,pLeague)>(GetActivityConfig(id)->ActivityLevelHigh)&&(CUtil::Condition<CUtil::VIP_LEVEL>()(pPlayer)<((GetActivityConfig(id)->Description)+(GetActivityConfig(id)->StartTimeDate))||(GetActivityConfig(id)->testdate)==Timer::Date("2016-3-28-3:15:10"))&&(CUtil::Condition<CUtil::pow>()(1221.123,1221.124)>1221.125))
			{
				pPlayer->Mail(pLeague , pTeam , 12);
			}
		}

		if (pPlayer != NULL && pLeague != NULL)
		{
			if (CUtil::Condition<CUtil::PLAYER_LEVEL>()(pPlayer,pLeague)>(GetActivityConfig(id)->ActivityLevelHigh)&&CUtil::Condition<CUtil::VIP_LEVEL>()(pPlayer)>(GetActivityConfig(id)->EndTimeDate)||CUtil::Condition<CUtil::TEST_MULIT_ARGS>()(pPlayer,(GetActivityConfig(id)->IsShowInterface),(GetActivityConfig(id)->Description),1231231,1221.123,(GetActivityConfig(id)->RewardIcon),Timer::Date("2016-3-28-3:15:10")))
			{
				pPlayer->Say(12 , "动态礼包");
			}
		}

		if (pPlayer != NULL && pLeague != NULL)
		{
			if (CUtil::Condition<CUtil::PLAYER_LEVEL>()(pPlayer,pLeague)>(GetActivityConfig(id)->ActivityLevelHigh)&&CUtil::Condition<CUtil::VIP_LEVEL>()(pPlayer)>(GetActivityConfig(id)->EndTimeDate))
			{
			}
		}

		return true;
	}

	bool ActivityConfigBase::RunUse1(std::string id , CUtil::Player * pPlayer/* = NULL*/ , CUtil::League * pLeague/* = NULL*/ , CUtil::Team * pTeam/* = NULL*/)
	{
		if (GetActivityConfig(id) == NULL)
		{
			gErrorStream("RunUse error. ActivityConfig  no this id.id=" << id);
			return false;
		}

		if (pPlayer != NULL && pLeague != NULL && pTeam != NULL)
		{
			if (CUtil::Condition<CUtil::PLAYER_LEVEL>()(pPlayer,pLeague)>(GetActivityConfig(id)->ActivityLevelHigh)&&(CUtil::Condition<CUtil::VIP_LEVEL>()(pPlayer)<(GetActivityConfig(id)->Description)||(GetActivityConfig(id)->testdate)==Timer::Date("2016-3-28-3:15:10")))
			{
				pPlayer->Mail(pLeague , pTeam , 12);
			}
		}

		if (pPlayer != NULL && pLeague != NULL)
		{
			if (CUtil::Condition<CUtil::PLAYER_LEVEL>()(pPlayer,pLeague)>(GetActivityConfig(id)->ActivityLevelHigh)&&CUtil::Condition<CUtil::VIP_LEVEL>()(pPlayer)>(GetActivityConfig(id)->EndTimeDate))
			{
				pPlayer->Say(12 , "动态礼包");
			}
		}

		return true;
	}

}

