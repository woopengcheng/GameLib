// attention dont't change this line:INT32 nConditionID;std::string strServerCondition;std::string strServerAction;std::string strClientCondition;std::string strClientAction;
/************************************
FileName	:	_ConditionConfig.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-03 20:12:41
Description	:	csv读取数据文件实现
************************************/
#include "_ConditionConfig.h"
#include "LogLib/inc/Log.h"

namespace Config
{
	bool _ConditionConfig::LoadFrom(const std::string & filepath)
	{
		Config::_ConditionConfigLoad loadConfig;
		MsgAssert_Re0(loadConfig.LoadFrom(filepath) , "Error _ConditionConfigLoadFrom " << filepath);

		for(size_t i = 0; i < loadConfig.Count(); ++i)
		{
			Config::S_ConditionConfigLoad& config = loadConfig.Get(i);
			Config::S_ConditionConfig data = {0};
			data.nConditionID = config.nConditionID;
			data.strServerCondition = config.strServerCondition;
			data.strServerAction = config.strServerAction;
			data.strClientCondition = config.strClientCondition;
			data.strClientAction = config.strClientAction;
			m_mapConfigs.insert(std::make_pair(data.nConditionID,data));
		}
		return true;
	}

	S_ConditionConfig * _ConditionConfig::Get_ConditionConfig(INT32 nIndex)
	{
		MapConfigsT::iterator iter = m_mapConfigs.find(nIndex);
		if(iter == m_mapConfigs.end())
		{
			gWarniStream( "_ConditionConfig::Get_ConditionConfig NotFound " << nIndex);
			return NULL;
		}

		return &iter->second;
	}

	_ConditionConfig * g_p_ConditionConfig = NULL;
}

