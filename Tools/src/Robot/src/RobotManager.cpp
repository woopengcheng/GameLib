#include "RobotManager.h"
#include "LogLib/inc/Log.h"

namespace Robot
{

	RobotManager::RobotManager()
		: m_nServerSessionID(-1)
		, m_llServerID(0)
	{
		m_pRpcListener = new RobotManagerListener(this);
	}


	RobotManager::~RobotManager()
	{
	}

	CErrno RobotManager::Init(Json::Value & conf)
	{
		Json::Value objRobot = conf.get("robot", Json::Value());
		return RpcInterface::Init(conf);
	}

	CErrno RobotManager::Cleanup(void)
	{

		return RpcInterface::Cleanup();
	}

	CErrno RobotManager::Update(void)
	{
		return Msg::RpcInterface::Update();
	}

	CErrno RobotManagerListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (m_pManager)
		{
			//		m_pManager->CreateRobotGroup(nSessionID, strNetNodeName, bReconnect);
		}

		gDebugStream("connected from sessionID=" << nSessionID);
		return CErrno::Success();
	}

	CErrno RobotManagerListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{
		if (m_pManager)
		{
			// 		m_pManager->DeleteRobotGroup(nSessionID);
		}

		gDebugStream("disconnected from sessionID=" << nPeerSessionID);
		return CErrno::Success();
	}
}