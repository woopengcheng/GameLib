#include "CRobot.h"

Msg::ObjectMsgCall * Robot::CRobot::UpdateRobotStatue_RpcServer(INT32 nSessionID, Msg::Object objSrc , INT32 status/* = 0*/)
{
	INT32 res = 0;


	std::cout << "CRobot::UpdateRobotStatue_RpcServer "<< std::endl;
	Return(res);
}

