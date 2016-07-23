#include "RobotGroup.h"

Msg::ObjectMsgCall * Robot::RobotGroup::SendRobotCommand_RpcClient(INT32 nSessionID, Msg::Object objSrc , INT32 res/* = 0*/)
{


	std::cout << "RobotGroup::SendRobotCommand_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Robot::RobotGroup::SendRobotCommand_RpcTimeout(INT32 nSessionID, Msg::Object objSrc , std::vector<INT32> & map_robots/* = std::vector<INT32>(0)*/ , std_string & command/* = std::string()*/)
{


	std::cout << "RobotGroup::SendRobotCommand_RpcTimeout" << std::endl;
	ReturnNULL;
}

