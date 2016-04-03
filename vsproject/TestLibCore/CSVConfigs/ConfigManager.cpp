/************************************
FileName	:	ConfigManager.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-03 12:46:51
Description	:	ConfigManager数据管理文件实现
************************************/
#include "ConfigManager.h"
#include "LogLib/inc/Log.h"
#include "ActivityConfig.h"
#include "ActionConfig.h"
#include "_ConditionConfig.h"
#include "_CommonData.h"
	

namespace Config
{
	ConfigManager::ConfigManager()
	{
// 		g_pActivityConfig = new Config::ActivityConfig;
		g_pActionConfig = new Config::ActionConfig;
// 		g_p_ConditionConfig = new Config::_ConditionConfig;
		g_p_CommonData = new Config::_CommonData;
	}

	ConfigManager::~ConfigManager()
	{
// 		SAFE_DELETE(Config::g_pActivityConfig);
		SAFE_DELETE(Config::g_pActionConfig);
// 		SAFE_DELETE(Config::g_p_ConditionConfig);
		SAFE_DELETE(Config::g_p_CommonData);
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

// 		MsgAssert_ReF1(Config::g_pActivityConfig , "ConfigManager not Init")
// 		Config::g_pActivityConfig->LoadFrom(strCsvPath + "ActivityConfig.tabcsv");

		MsgAssert_ReF1(Config::g_pActionConfig , "ConfigManager not Init")
		Config::g_pActionConfig->LoadFrom(strCsvPath + "ActionConfig.tabcsv");

// 		MsgAssert_ReF1(Config::g_p_ConditionConfig , "ConfigManager not Init")
// 		Config::g_p_ConditionConfig->LoadFrom(strCsvPath + "_ConditionConfig.tabcsv");

		MsgAssert_ReF1(Config::g_p_CommonData , "ConfigManager not Init")
		Config::g_p_CommonData->LoadFrom(strCsvPath + "_CommonData.tabcsv");

		return 0;
	}

	INT32 ConfigManager::Cleanup()
	{
		return -1;
	}

}

