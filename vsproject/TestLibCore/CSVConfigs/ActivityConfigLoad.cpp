/************************************
FileName	:	ActivityConfigLoad.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-03 12:47:58
Description	:	csv读取文件实现
************************************/
#include "ActivityConfigLoad.h"
#include "CUtil/inc/CUtil.h"
#include "CUtil/inc/CSVReader.h"

namespace Config
{
	bool ActivityConfigLoad::LoadFrom(const std::string & filepath)
	{
		CUtil::CSVReader csv;
		if(csv.Load(filepath) != 0)
			return false;

		size_t index_ActivityId = csv.GetIndex("ActivityId", 2);
		MsgAssert_Re0(index_ActivityId != (size_t)-1 , "error ActivityId");

		size_t index_ActivityName = csv.GetIndex("ActivityName", 2);
		MsgAssert_Re0(index_ActivityName != (size_t)-1 , "error ActivityName");

		size_t index_ActivityLevelHigh = csv.GetIndex("ActivityLevelHigh", 2);
		MsgAssert_Re0(index_ActivityLevelHigh != (size_t)-1 , "error ActivityLevelHigh");

		size_t index_IsShowEntrance = csv.GetIndex("IsShowEntrance", 2);
		MsgAssert_Re0(index_IsShowEntrance != (size_t)-1 , "error IsShowEntrance");

		size_t index_EntranceIcon = csv.GetIndex("EntranceIcon", 2);
		MsgAssert_Re0(index_EntranceIcon != (size_t)-1 , "error EntranceIcon");

		size_t index_IsShowInterface = csv.GetIndex("IsShowInterface", 2);
		MsgAssert_Re0(index_IsShowInterface != (size_t)-1 , "error IsShowInterface");

		size_t index_InterfaceIcon = csv.GetIndex("InterfaceIcon", 2);
		MsgAssert_Re0(index_InterfaceIcon != (size_t)-1 , "error InterfaceIcon");

		size_t index_StartTimeWeek = csv.GetIndex("StartTimeWeek", 2);
		MsgAssert_Re0(index_StartTimeWeek != (size_t)-1 , "error StartTimeWeek");

		size_t index_EndTimeWeek = csv.GetIndex("EndTimeWeek", 2);
		MsgAssert_Re0(index_EndTimeWeek != (size_t)-1 , "error EndTimeWeek");

		size_t index_StartTimeDate = csv.GetIndex("StartTimeDate", 2);
		MsgAssert_Re0(index_StartTimeDate != (size_t)-1 , "error StartTimeDate");

		size_t index_EndTimeDate = csv.GetIndex("EndTimeDate", 2);
		MsgAssert_Re0(index_EndTimeDate != (size_t)-1 , "error EndTimeDate");

		size_t index_StartTime = csv.GetIndex("StartTime", 2);
		MsgAssert_Re0(index_StartTime != (size_t)-1 , "error StartTime");

		size_t index_EndTime = csv.GetIndex("EndTime", 2);
		MsgAssert_Re0(index_EndTime != (size_t)-1 , "error EndTime");

		size_t index_Description = csv.GetIndex("Description", 2);
		MsgAssert_Re0(index_Description != (size_t)-1 , "error Description");

		size_t index_RewardIcon = csv.GetIndex("RewardIcon", 2);
		MsgAssert_Re0(index_RewardIcon != (size_t)-1 , "error RewardIcon");

		size_t index_testdate = csv.GetIndex("testdate", 2);
		MsgAssert_Re0(index_testdate != (size_t)-1 , "error testdate");

		size_t index_testDateStruct = csv.GetIndex("testDateStruct[begin,end]", 2);
		MsgAssert_Re0(index_testDateStruct != (size_t)-1 , "error testDateStruct[begin,end]");

		size_t index_dateArray = csv.GetIndex("dateArray", 2);
		MsgAssert_Re0(index_dateArray != (size_t)-1 , "error dateArray");

		for (size_t row = 4; row < csv.Count(); ++row)
		{
			SActivityConfigLoad conf;

			conf.ActivityId = csv.GetInt32(row , index_ActivityId);
			conf.ActivityName = csv.GetString(row , index_ActivityName);
			conf.ActivityLevelHigh = csv.GetInt32(row , index_ActivityLevelHigh);
			conf.IsShowEntrance = csv.GetBool(row , index_IsShowEntrance);
			{
				std::string __tmp = csv.GetString(row, index_EntranceIcon);
				CUtil::tokenize(__tmp, conf.EntranceIcon, ",", "", "\"");
			}

			conf.IsShowInterface = csv.GetBool(row , index_IsShowInterface);
			{
				std::string __tmp = csv.GetString(row, index_InterfaceIcon);
				CUtil::tokenize(__tmp, conf.InterfaceIcon, ",", "", "\"");
			}

			conf.StartTimeWeek = csv.GetInt32(row , index_StartTimeWeek);
			conf.EndTimeWeek = csv.GetInt32(row , index_EndTimeWeek);
			conf.StartTimeDate = csv.GetInt32(row , index_StartTimeDate);
			conf.EndTimeDate = csv.GetInt32(row , index_EndTimeDate);
			conf.StartTime = csv.GetString(row , index_StartTime);
			conf.EndTime = csv.GetString(row , index_EndTime);
			conf.Description = csv.GetInt32(row , index_Description);
			conf.RewardIcon = csv.GetString(row , index_RewardIcon);
			conf.testdate = csv.GetDate(row , index_testdate);
			{
				std::vector<std::string> vals;
				std::string __tmp = csv.GetString(row, index_testDateStruct);
				CUtil::tokenize(__tmp, vals, ",", "", "\"");
				for (size_t i = 0; i < vals.size(); ++i)
				{
					if(i == 0)
					{
						Timer::Date val(vals[i]);
						conf.testDateStruct.begin = val;
					}
					if(i == 1)
					{
						Timer::Date val(vals[i]);
						conf.testDateStruct.end = val;
					}
				}
			}

			{
				std::vector<std::string> vals;
				std::string __tmp = csv.GetString(row, index_dateArray);
				CUtil::tokenize(__tmp, vals, ",", "", "\"");
				for (size_t i = 0; i < vals.size(); ++i)
					conf.dateArray.push_back(Timer::Date(vals[i]));
			}

			m_vtConfigs.push_back(conf);
		}

		return true;
	}

	SActivityConfigLoad & ActivityConfigLoad::Get(size_t row)
	{
		return m_vtConfigs.at(row);
	}

}

