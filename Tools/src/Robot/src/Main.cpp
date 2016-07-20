// TestLibCore.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "CUtil/inc/CUtil.h"
#include "json/json.h"
#include "Timer/inc/TimerHelp.h"
#include "RobotGroup.h"
#include "RobotManager.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CUtil::Init();

	Json::Value root;
	Json::JsonParase("Robot.conf", root);

	Robot::RobotGroup::GetInstance().Init(root);
	Robot::RobotManager::GetInstance().Init(root);

	while (true)
	{
		Robot::RobotGroup::GetInstance().Update();
		Robot::RobotManager::GetInstance().Update();
		Timer::sleep(1);
	}

	Robot::RobotGroup::GetInstance().Cleanup();
	Robot::RobotManager::GetInstance().Cleanup();

	CUtil::Cleanup();
	return 0;
}

