// TestLibCore.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "CUtil/inc/CUtil.h"
#include "CUtil/inc/DeelxRegexp.h"
#include "CSVConfigs/ConfigManager.h"
#include "NetLib/inc/NetHelper.h"
#include "CSVConfigs/ActionConfig.h"
#include "CSVConfigs/ActivityConfig.h"
#include "Test/LuaTest/test.h"

int _tmain(int argc, _TCHAR* argv[])
{   
	CUtil::Init();
	
	Config::ConfigManager::GetInstance().Init("./csv_config");
	std::cout << "port used=" << Net::NetHelper::IsSocketPortUsed(8002) << std::endl;;

	gDebugStream(Config::ActionConfig::GetInstance().GetActionConfig(1)->TestStruct.test1);
	gDebugStream(Config::ActionConfig::GetInstance().GetActionConfig(1)->TestStruct.test3.begin()->second->vecTestStructArray[0].test6.begin()->second->TestStruct.test6);
	gDebugStream(Config::ActivityConfig::GetInstance().GetActivityConfig("1")->testdate.GetDateStr());
	gDebugStream(Config::ActivityConfig::GetInstance().GetActivityConfig("1")->vecTestStructArray[0].test6.begin()->second->TestStruct.test6);

	test_lua_main();
	while (true)
	{

		Config::ConfigManager::GetInstance().Cleanup();

	}

	CUtil::Cleanup();
	return 0;
}

