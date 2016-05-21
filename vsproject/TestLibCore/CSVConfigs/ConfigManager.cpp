/************************************
FileName	:	ConfigManager.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-21 14:51:04
Description	:	ConfigManager数据管理文件实现
************************************/
#include "ConfigManager.h"
#include "LogLib/inc/Log.h"
#include "ActionConfig.h"
#include "ActivityConfig.h"
#include "_ConditionConfig.h"
#include "_CommonData.h"
	

namespace Config
{
	ConfigManager::ConfigManager()
	{
	}

	ConfigManager::~ConfigManager()
	{
	}

	ConfigManager & ConfigManager::GetInstance()
	{
		static ConfigManager instance;
		return instance;
	}

	INT32 ConfigManager::Init(std::string  strCsvPath)
	{
		MsgAssert_ReF1(strCsvPath.length(), "ConfigManager::Init error.");

		if (strCsvPath[strCsvPath.length() - 1] != '/')
		{
			strCsvPath = strCsvPath + "/";
		}

		LoadFrom(strCsvPath);
		RepairLoad(strCsvPath);
	
		return 0;
	}

	INT32 ConfigManager::LoadFrom(std::string  strCsvPath)
	{
		MsgAssert_ReF1(strCsvPath.length(), "ConfigManager::LoadFrom error.");

		if (strCsvPath[strCsvPath.length() - 1] != '/')
		{
			strCsvPath = strCsvPath + "/";
		}

		Config::ActionConfig::GetInstance().LoadFrom(strCsvPath);
		Config::ActivityConfig::GetInstance().LoadFrom(strCsvPath);
// 		Config::_ConditionConfig::GetInstance().LoadFrom(strCsvPath);
		Config::_CommonData::GetInstance().LoadFrom(strCsvPath);
		return 0;
	}

	INT32 ConfigManager::RepairLoad(std::string  strCsvPath)
	{
		MsgAssert_ReF1(strCsvPath.length(), "ConfigManager::RepairLoad error.");

		if (strCsvPath[strCsvPath.length() - 1] != '/')
		{
			strCsvPath = strCsvPath + "/";
		}

		Config::ActionConfig::GetInstance().RepairLoad(strCsvPath);
		Config::ActivityConfig::GetInstance().RepairLoad(strCsvPath);
// 		Config::_ConditionConfig::GetInstance().RepairLoad(strCsvPath);
		Config::_CommonData::GetInstance().RepairLoad(strCsvPath);
		return 0;
	}

	INT32 ConfigManager::Cleanup()
	{
		return 0;
	}

}

