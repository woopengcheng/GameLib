#include "stdafx.h"
#include "CRobot.h"
#include "RobotGroup.h"


namespace Robot
{
	CRobot::CRobot(RobotGroup * pRobotGroup , Msg::Object id, Msg::RpcManager * pRpcManager)
		: Msg::IRpcMsgCallableObject(id, pRpcManager)
		, m_llPeerRobotID(-1)
		, m_pRobotGroup(pRobotGroup)
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