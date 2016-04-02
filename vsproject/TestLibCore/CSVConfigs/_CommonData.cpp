// attention dont't change this line:INT32 id;std::map<std::string , bool> boolCommon;std::map<std::string , bool> boolCommon2;std::map<std::string , INT32> intCommon;std::map<std::string , INT64> int64Common;std::map<std::string , double> doubleCommon;std::map<std::string , std::string> stringCommon;std::map<std::string , Timer::Date> dateCommon;
/************************************
FileName	:	_CommonData.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-02 12:00:19
Description	:	csv读取数据文件实现
************************************/
#include "_CommonData.h"
#include "LogLib/inc/Log.h"

namespace Config
{
	bool _CommonData::LoadFrom(const std::string & filepath)
	{
		Config::_CommonDataLoad loadConfig;
		MsgAssert_Re0(loadConfig.LoadFrom(filepath) , "Error _CommonDataLoadFrom " << filepath);

		for(size_t i = 0; i < loadConfig.Count(); ++i)
		{
			Config::S_CommonDataLoad& config = loadConfig.Get(i);
			Config::S_CommonData data = {0};
			data.id = config.id;
			data.boolCommon = config.boolCommon;
			data.boolCommon2 = config.boolCommon2;
			data.intCommon = config.intCommon;
			data.int64Common = config.int64Common;
			data.doubleCommon = config.doubleCommon;
			data.stringCommon = config.stringCommon;
			data.dateCommon = config.dateCommon;
			m_mapConfigs.insert(std::make_pair(data.id,data));
		}
		return true;
	}

	S_CommonData * _CommonData::Get_CommonData(INT32 nIndex)
	{
		MapConfigsT::iterator iter = m_mapConfigs.find(nIndex);
		if(iter == m_mapConfigs.end())
		{
			gWarniStream( "_CommonData::Get_CommonData NotFound " << nIndex);
			return NULL;
		}

		return &iter->second;
	}

	_CommonData * g_p_CommonData = NULL;
}

