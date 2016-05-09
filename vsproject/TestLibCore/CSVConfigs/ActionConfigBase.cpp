// attention dont't change this line:INT64 curve_id;std::vector<INT32> validStages;std::string wrapMode;std::vector<std::string> InterfaceIcon;double attr_mod_val;std::vector<double> start_x;INT64 col_1;bool isLocal;TestStructbool test1;INT64 test2;double test3;INT32 test4;std::string test6;TestStructArraybool test1;INT64 test2;double test3;INT32 test4;std::string test5;
/************************************
FileName	:	ActionConfigBase.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-09 23:54:53
Description	:	csv读取数据文件实现
************************************/
#include "ActionConfigBase.h"
#include "LogLib/inc/Log.h"

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
				data.TestStruct.test3 = config.TestStruct.test3;
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
					array.test5 = iter->test5;
					data.vecTestStructArray.push_back(array);
				}
			}
			m_mapConfigs.insert(std::make_pair(data.curve_id,data));
		}

		m_bLoaded = true;
		return true;
	}

	SActionConfig * ActionConfigBase::GetActionConfig(INT64 id , std::string strFilePath/* = ""*/)
	{
		if (!m_bLoaded)
		{
			LoadFrom(strFilePath);
		}
		MapConfigsT::iterator iter = m_mapConfigs.find(id);
		if(iter == m_mapConfigs.end())
		{
			gWarniStream( "ActionConfig::GetActionConfig NotFound " << id);
			return NULL;
		}

		return &iter->second;
	}

}

