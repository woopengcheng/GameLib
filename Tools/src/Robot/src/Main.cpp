// TestLibCore.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "CUtil/inc/CUtil.h"
#include "LogLib/inc/Log.h"
#include "json/json.h"
#include "Timer/inc/TimerHelp.h"
#include "RobotGroup.h"
#include "RobotManager.h"
#include "RobotCommand.h"
#include "CRobot.h"

INT32 g_nClosed = FALSE;
int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
	{
		return 1;
	}

	CUtil::Init();
	RobotCommands::Init();

	Json::Value root;
	Json::JsonParase("Robot.conf", root);

	INT32 nServerPort = (INT32)CUtil::atoi(argv[1]);
	root["robot_group"]["clients"][0]["port"] = nServerPort;
	gDebugStream("server_port=" << root["clients"][0]["port"]);

	Robot::RobotGroup::GetInstance().Init(root);
	Robot::RobotManager::GetInstance().Init();

	if (1)
	{ 
		Robot::CRobot * pRobot = new Robot::CRobot(1, Robot::RobotGroup::GetInstance().GetRpcManager(), Robot::RobotManager::GetInstance().GetNetReactor());
		pRobot->Init();
		double nCommandType = 20.f;
		CUtil::CStream cs;
		std::string str = "asdf";
		cs << nCommandType << str;

		pRobot->HandleRobotCommand(0 , cs);
	}

	while (!g_nClosed)
	{
		Robot::RobotManager::GetInstance().Update();
		Robot::RobotGroup::GetInstance().Update();

		Timer::sleep(1);
	}

	Robot::RobotManager::GetInstance().Cleanup();
	Robot::RobotGroup::GetInstance().Cleanup();

	CUtil::Cleanup();
	return 0;
}

