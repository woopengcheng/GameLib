#include "GRpc.h"
#include "RobotGroup.h"

Msg::ObjectMsgCall * Robot::GRpc::CloseRobotGroup_RpcServer(INT32 nSessionID, Msg::Object objSrc )
{
	INT32 res = 0;

	g_nClosed = TRUE;

	std::cout << "GRpc::CloseRobotGroup_RpcServer "<< std::endl;
	Return(res);
}

