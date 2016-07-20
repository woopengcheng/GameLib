#include "CRobot.h"
#include "RobotGroupHandler.h"


namespace Robot
{
	RobotGroupHandler::RobotGroupHandler(Msg::Object id, Msg::RpcManager * pRpcManager)
		: Msg::IRpcMsgCallableObject(id, pRpcManager)
	{
	}


	RobotGroupHandler::~RobotGroupHandler()
	{
	}

	CErrno RobotGroupHandler::Init(void)
	{
		return CErrno::Success();
	}

	CErrno RobotGroupHandler::Cleanup(void)
	{
		return CErrno::Success();
	}

	CErrno RobotGroupHandler::Update(void)
	{
		return CErrno::Success();
	}
}