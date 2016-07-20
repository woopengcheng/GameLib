#include "CRobot.h"


namespace Robot
{
	CRobot::CRobot(Msg::Object id, Msg::RpcManager * pRpcManager)
		: Msg::IRpcMsgCallableObject(id, pRpcManager) 
	{
	}


	CRobot::~CRobot()
	{
	}

	CErrno CRobot::Init(void)
	{
		return CErrno::Success();
	}

	CErrno CRobot::Cleanup(void)
	{
		return CErrno::Success();
	}

	CErrno CRobot::Update(void)
	{
		return CErrno::Success();
	}
}