// TestLibCore.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "CUtil/inc/CUtil.h"
#include "LogLib/inc/Log.h"
#include "json/json.h"
#include "Timer/inc/TimerHelp.h"
#include "RobotGroup.h"
#include "RobotManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		return 1;
	}
	system("pause");

	CUtil::Init();

	Json::Value root;
	Json::JsonParase("Robot.conf", root);

	INT32 nServerPort = CUtil::atoi(argv[1]);
	root["robot_group"]["clients"][0]["port"] = nServerPort;
	gDebugStream("server_port=" << root["clients"][0]["port"]);

	Robot::RobotGroup::GetInstance().Init(root);

	while (true)
	{
		Robot::RobotGroup::GetInstance().Update();
		Timer::sleep(1);
	}

	Robot::RobotGroup::GetInstance().Cleanup();

	CUtil::Cleanup();
	return 0;
}

