#include "GRpc.h"
#include "RobotManager.h"

Msg::ObjectMsgCall * Robot::GRpc::CreateRobots_RpcServer(INT32 nSessionID, Msg::Object objSrc , INT32 start_pos/* = 0*/ , INT32 end_pos/* = 0*/)
{
	INT32 res = 0;

	res = Robot::RobotManager::GetInstance().PreCreateRobots(start_pos, end_pos);

	std::cout << "GRpc::CreateRobots_RpcServer "<< std::endl;
	Return(res);
}

