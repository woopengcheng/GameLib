// attention dont't change this line:INT32 nConditionID;std::string strServerCondition;std::string strServerAction;std::string strClientCondition;std::string strClientAction;
/************************************
FileName	:	_ConditionConfigBase.cpp
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.40.128
Version		:	0.0.1
Date		:	2016-05-10 12:12:16
Description	:	csv读取数据文件实现
************************************/
#include "_ConditionConfigBase.h"
#include "LogLib/inc/Log.h"

namespace Config
{
	bool _ConditionConfigBase::LoadFrom(const std::string & filepath)
	{
		if (m_bLoaded)
		{
			return false;
		}
		Config::_ConditionConfigLoad loadConfig;
		MsgAssert_Re0(loadConfig.LoadFrom(filepath + "_ConditionConfig.tabcsv") , "Error _ConditionConfigLoadFrom " << filepath + "_ConditionConfig.tabcsv");

		for(size_t i = 0; i < loadConfig.Count(); ++i)
		{
			Config::S_ConditionConfigLoad& config = loadConfig.Get(i);
			Config::S_ConditionConfig data;
			data.nConditionID = config.nConditionID;
			data.strServerCondition = config.strServerCondition;
			data.strServerAction = config.strServerAction;
			data.strClientCondition = config.strClientCondition;
			data.strClientAction = config.strClientAction;
			m_mapConfigs.insert(std::make_pair(data.nConditionID,data));
		}

		m_bLoaded = true;
		return true;
	}

	S_ConditionConfig * _ConditionConfigBase::Get_ConditionConfig(INT32 id , std::string strFilePath/* = ""*/)
	{
		if (!m_bLoaded)
		{
			LoadFrom(strFilePath);
		}
		MapConfigsT::iterator iter = m_mapConfigs.find(id);
		if(iter == m_mapConfigs.end())
		{
			gWarniStream( "_ConditionConfig::Get_ConditionConfig NotFound " << id);
			return NULL;
		}

		return &iter->second;
	}

}

