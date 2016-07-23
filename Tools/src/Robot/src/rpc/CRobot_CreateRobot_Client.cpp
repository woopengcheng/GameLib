#include "CRobot.h"

Msg::ObjectMsgCall * Robot::CRobot::CreateRobot_RpcClient(INT32 nSessionID, Msg::Object objSrc , INT32 res/* = 0*/)
{
	if (res != -1)
	{
		SetRobotIndex(res);
	}

	std::cout << "CRobot::CreateRobot_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Robot::CRobot::CreateRobot_RpcTimeout(INT32 nSessionID, Msg::Object objSrc , std_string & name/* = std::string()*/ , INT32 prof/* = 0*/ , INT32 level/* = 0*/ , INT32 status/* = 0*/ , std_string & note/* = std::string()*/)
{


	std::cout << "CRobot::CreateRobot_RpcTimeout" << std::endl;
	ReturnNULL;
}

