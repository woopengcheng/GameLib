// attention dont't change this line:INT64 curve_id;std::vector<INT32> validStages;std::string wrapMode;std::vector<std::string> InterfaceIcon;double attr_mod_val;std::vector<double> start_x;INT64 col_1;bool isLocal;TestStructbool test1;INT64 test2;std_unordered_map<std::string , SActivityConfig *> test3;INT32 test4;std::string test6;TestStructArraybool test1;INT64 test2;double test3;INT32 test4;std_unordered_map<std::string , SActivityConfig *> test5;
/************************************
FileName	:	ActionConfigBase.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-22 01:03:18
Description	:	csv读取数据文件实现
************************************/
#include "ActionConfigBase.h"
#include "LogLib/inc/Log.h"
#include "ActivityConfig.h"

namespace Config
{
	bool ActionConfigBase::LoadFrom(const std::string & filepath)
	{
		if (m_bLoaded)
		{
			return false;
		}
		Config::ActionConfigLoad loadConfig;
		MsgAssert_Re0(loadConfig.LoadFrom(filepath + "ActionConfig.tabcsv") , "Error ActionConfigLoadFrom " << filepath + "ActionConfig.tabcsv");

		for(size_t i = 0; i < loadConfig.Count(); ++i)
		{
			Config::SActionConfigLoad& config = loadConfig.Get(i);
			Config::SActionConfig data;
			data.curve_id = config.curve_id;
			data.validStages = config.validStages;
			data.wrapMode = config.wrapMode;
			data.InterfaceIcon = config.InterfaceIcon;
			data.attr_mod_val = config.attr_mod_val;
			data.start_x = config.start_x;
			data.col_1 = config.col_1;
			data.isLocal = config.isLocal;
			{
				data.TestStruct.test1 = config.TestStruct.test1;
				data.TestStruct.test2 = config.TestStruct.test2;
				data.TestStruct.test3.insert(std::make_pair(config.TestStruct.test3 , Config::ActivityConfig::GetInstance().GetActivityConfig(config.TestStruct.test3 , filepath)));
				data.TestStruct.test4 = config.TestStruct.test4;
				data.TestStruct.test6 = config.TestStruct.test6;
			}
			{
				std::vector<SActionConfigLoad::STestStructArray>::iterator iter = config.vecTestStructArray.begin();
				std::vector<SActionConfigLoad::STestStructArray>::iterator end = config.vecTestStructArray.end();
				for (; iter != end;++iter)
				{
					SActionConfig::STestStructArray array;
					array.test1 = iter->test1;
					array.test2 = iter->test2;
					array.test3 = iter->test3;
					array.test4 = iter->test4;
					array.test5.insert(std::make_pair(iter->test5 , Config::ActivityConfig::GetInstance().GetActivityConfig(iter->test5 , filepath)));
					data.vecTestStructArray.push_back(array);
				}
			}
			m_mapConfigs.insert(std::make_pair(data.curve_id,data));
		}

		m_bLoaded = true;
		return true;
	}

	bool ActionConfigBase::RepairLoad(const std::string & filepath)
	{
		MsgAssert_Re0(m_bLoaded , "error ActionConfig .no load data.")
		MapConfigsT::iterator iterConfig = m_mapConfigs.begin();

		for(; iterConfig != m_mapConfigs.end(); ++iterConfig)
		{
			Config::SActionConfig & data = iterConfig->second;
			{
				data.TestStruct.test3.begin()->second = Config::ActivityConfig::GetInstance().GetActivityConfig(data.TestStruct.test3.begin()->first , filepath);
			}
			{
				std::vector<SActionConfig::STestStructArray> & vecTestStructArray = data.vecTestStructArray;
				std::vector<SActionConfig::STestStructArray>::iterator iterTestStructArray = vecTestStructArray.begin();
				std::vector<SActionConfig::STestStructArray>::iterator endTestStructArray = vecTestStructArray.end();
				for (; iterTestStructArray != endTestStructArray;++iterTestStructArray)
				{
					SActionConfig::STestStructArray & array = *iterTestStructArray;
					array.test5.begin()->second = Config::ActivityConfig::GetInstance().GetActivityConfig(array.test5.begin()->first , filepath);
				}
			}
		}

		return true;
	}

	SActionConfig * ActionConfigBase::GetActionConfig(INT64 id , std::string strFilePath/* = ""*/)
	{
		MapConfigsT::iterator iter = m_mapConfigs.find(id);
		if(iter == m_mapConfigs.end())
		{
			return NULL;
		}

		return &iter->second;
	}

}

