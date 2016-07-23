#include "RobotGroup.h"

Msg::ObjectMsgCall * Robot::RobotGroup::CreateRobot_RpcServer(INT32 nSessionID, Msg::Object objSrc , std_string & name/* = std::string()*/ , INT32 prof/* = 0*/ , INT32 level/* = 0*/ , INT32 status/* = 0*/ , std_string & note/* = std::string()*/)
{
	INT32 res = 0;

	RobotInfo info;
	info.strName = name;
	info.nLevel = level;
	info.nProf = prof;
	info.nStatue = status;
	info.strNote = note;

	res = CreateRobot(info);

	std::cout << "RobotGroup::CreateRobot_RpcServer res=" << res  << std::endl;
	Return(res);
}

