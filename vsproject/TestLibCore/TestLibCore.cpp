// TestLibCore.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "CUtil/inc/CUtil.h"
#include "CUtil/inc/DeelxRegexp.h"
#include "CSVConfigs/ConfigManager.h"

int _tmain(int argc, _TCHAR* argv[])
{   
	CUtil::Init();
	
	Config::ConfigManager::GetInstance().Init("./csv_config");

	while (true)
	{
		Config::ConfigManager::GetInstance().Cleanup();

	}

	CUtil::Cleanup();
	return 0;
}

