#include "CRobot.h"

Msg::ObjectMsgCall * Robot::CRobot::SendRobotCommand_RpcServer(INT32 nSessionID, Msg::Object objSrc , CUtil::Chunk & command/* = std::string()*/)
{
	INT32 res = 0;

	INT32 nCommandType = 0;
	CUtil::CStream cs(command);
	cs >> nCommandType;


	std::cout << "CRobot::SendRobotCommand_RpcServer id="<< GetObjectID().m_llObjID << ":command=" << nCommandType  << std::endl;
	Return(res);
}

