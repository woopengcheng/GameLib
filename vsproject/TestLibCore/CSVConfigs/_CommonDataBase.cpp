// attention dont't change this line:INT32 id;std::map<std::string , bool> boolCommon;std::map<std::string , bool> boolCommon2;std::map<std::string , INT32> intCommon;std::map<std::string , INT64> int64Common;std::map<std::string , double> doubleCommon;std::map<std::string , std::string> stringCommon;std::map<std::string , Timer::Date> dateCommon;
/************************************
FileName	:	_CommonDataBase.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-21 14:51:04
Description	:	csv读取数据文件实现
************************************/
#include "_CommonDataBase.h"
#include "LogLib/inc/Log.h"

namespace Config
{
	bool _CommonDataBase::LoadFrom(const std::string & filepath)
	{
		if (m_bLoaded)
		{
			return false;
		}
		Config::_CommonDataLoad loadConfig;
		MsgAssert_Re0(loadConfig.LoadFrom(filepath + "_CommonData.tabcsv") , "Error _CommonDataLoadFrom " << filepath + "_CommonData.tabcsv");

		for(size_t i = 0; i < loadConfig.Count(); ++i)
		{
			Config::S_CommonDataLoad& config = loadConfig.Get(i);
			Config::S_CommonData data;
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

		m_bLoaded = true;
		return true;
	}

	bool _CommonDataBase::RepairLoad(const std::string & filepath)
	{
		MsgAssert_Re0(m_bLoaded , "error _CommonData .no load data.")
		MapConfigsT::iterator iterConfig = m_mapConfigs.begin();

		for(; iterConfig != m_mapConfigs.end(); ++iterConfig)
		{
			Config::S_CommonData & data = iterConfig->second;
		}

		return true;
	}

	S_CommonData * _CommonDataBase::Get_CommonData(INT32 id , std::string strFilePath/* = ""*/)
	{
		MapConfigsT::iterator iter = m_mapConfigs.find(id);
		if(iter == m_mapConfigs.end())
		{
			return NULL;
		}

		return &iter->second;
	}

}

