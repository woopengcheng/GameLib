/************************************
FileName	:	ConfigManager.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-03-09 21:42:33
Description	:	ConfigManager数据管理文件实现
************************************/
#include "ConfigManager.h"
#include "LogLib/inc/Log.h"
#include "ActivityConfig.h"
#include "ActionConfig.h"
	

namespace Config
{
	ConfigManager::ConfigManager()
	{
// 		g_pActivityConfig = new Config::ActivityConfig;
		g_pActionConfig = new Config::ActionConfig;
	}

	ConfigManager::~ConfigManager()
	{
// 		SAFE_DELETE(Config::g_pActivityConfig);
		SAFE_DELETE(Config::g_pActionConfig);
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

		return 0;
	}

	INT32 ConfigManager::Cleanup()
	{
		return -1;
	}

}

