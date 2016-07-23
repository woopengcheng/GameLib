#include "GRpc.h"
#include "RobotServer.h"

Msg::ObjectMsgCall * Robot::GRpc::HandleRobotGroup_RpcServer(INT32 nSessionID, Msg::Object objSrc , INT32 type/* = 0*/ , INT32 param1/* = 0*/ , INT32 param2/* = 0*/ , INT32 param3/* = 0*/ , INT32 param4/* = 0*/)
{
	INT32 res = 0;

	Robot::RobotServer::GetInstance().HandleRobotGroup(type, param1, param2, param3, param4);

	std::cout << "GRpc::HandleRobotGroup_RpcServer " << std::endl;
	Return(res);
}

