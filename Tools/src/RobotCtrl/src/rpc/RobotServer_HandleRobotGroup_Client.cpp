#include "RobotServer.h"

Msg::ObjectMsgCall * Robot::RobotServer::HandleRobotGroup_RpcClient(INT32 nSessionID, Msg::Object objSrc , INT32 res/* = 0*/)
{


	std::cout << "RobotServer::HandleRobotGroup_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Robot::RobotServer::HandleRobotGroup_RpcTimeout(INT32 nSessionID, Msg::Object objSrc , INT32 type/* = 0*/ , INT32 param1/* = 0*/ , INT32 param2/* = 0*/ , INT32 param3/* = 0*/ , INT32 param4/* = 0*/)
{


	std::cout << "RobotServer::HandleRobotGroup_RpcTimeout" << std::endl;
	ReturnNULL;
}

