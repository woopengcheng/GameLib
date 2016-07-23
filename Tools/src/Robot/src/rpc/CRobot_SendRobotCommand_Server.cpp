#include "CRobot.h"

Msg::ObjectMsgCall * Robot::CRobot::SendRobotCommand_RpcServer(INT32 nSessionID, Msg::Object objSrc , std::vector<INT32> & map_robots/* = std::vector<INT32>(0)*/ , std_string & command/* = std::string()*/)
{
	INT32 res = 0;


	std::cout << "CRobot::SendRobotCommand_RpcServer "<< std::endl;
	Return(res);
}

