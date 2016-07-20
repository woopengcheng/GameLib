#include "RobotGroup.h"
#include "LogLib/inc/Log.h"

namespace Robot
{

	RobotGroup::RobotGroup()
		: m_nServerSessionID(-1)
		, m_llServerID(0)
	{
		m_pRpcListener = new RobotGroupListener(this);
	}


	RobotGroup::~RobotGroup()
	{
	}

	CErrno RobotGroup::Init(Json::Value & conf)
	{
		Json::Value objRobot = conf.get("robot_group", Json::Value());
		return RpcInterface::Init(objRobot);
	}

	CErrno RobotGroup::Cleanup(void)
	{

		return RpcInterface::Cleanup();
	}

	CErrno RobotGroup::Update(void)
	{
		return Msg::RpcInterface::Update();
	}

	CErrno RobotGroupListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (m_pManager)
		{
			//		m_pManager->CreateRobotGroup(nSessionID, strNetNodeName, bReconnect);
		}

		gDebugStream("connected from sessionID=" << nSessionID);
		return CErrno::Success();
	}

	CErrno RobotGroupListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{
		if (m_pManager)
		{
			// 		m_pManager->DeleteRobotGroup(nSessionID);
		}

		gDebugStream("disconnected from sessionID=" << nPeerSessionID);
		return CErrno::Success();
	}
}