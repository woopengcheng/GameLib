/************************************
FileName	:	MsgNameDefine.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.37.66
Version		:	0.0.1
Description	:	用于定义消息的全局唯一名字
************************************/
#ifndef __msg_Robot_msg_name_define_h__
#define __msg_Robot_msg_name_define_h__
#include "MsgLib/inc/MsgCommon.h" 

namespace Msg
{
	//tool SyncRobotServerID declare here
	RPC_DEFINE(SyncRobotServerID);

	//tool HandleRobotGroup declare here
	RPC_DEFINE(HandleRobotGroup);

	//tool UpdateRobotStart declare here
	RPC_DEFINE(UpdateRobotStart);

	//tool SyncRobotGroupID declare here
	RPC_DEFINE(SyncRobotGroupID);

	//tool CreateRobots declare here
	RPC_DEFINE(CreateRobots);

	//tool CreateRobot declare here
	RPC_DEFINE(CreateRobot);

	//tool SendRobotCommand declare here
	RPC_DEFINE(SendRobotCommand);

	//tool UpdateRobotStatue declare here
	RPC_DEFINE(UpdateRobotStatue);

}

#endif

