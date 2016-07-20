#include "stdafx.h"
#include "RobotServer.h"
#include "LogLib/inc/Log.h"

namespace Robot
{

	RobotServer::RobotServer()
		: m_nServerSessionID(-1)
		, m_llServerID(0)
	{
		m_pRpcListener = new RobotServerListener(this);
	}


	RobotServer::~RobotServer()
	{
	}

	CErrno RobotServer::Init(Json::Value & conf)
	{
		Json::Value objRobot = conf.get("robot_server", Json::Value());
		return RpcInterface::Init(objRobot);
	}

	CErrno RobotServer::Cleanup(void)
	{

		return RpcInterface::Cleanup();
	}

	CErrno RobotServer::Update(void)
	{
		return Msg::RpcInterface::Update();
	}

	CErrno RobotServerListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (m_pManager)
		{
			//		m_pManager->CreateRobotGroup(nSessionID, strNetNodeName, bReconnect);
		}

		gDebugStream("connected from sessionID=" << nSessionID);
		return CErrno::Success();
	}

	CErrno RobotServerListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{
		if (m_pManager)
		{
			// 		m_pManager->DeleteRobotGroup(nSessionID);
		}

		gDebugStream("disconnected from sessionID=" << nPeerSessionID);
		return CErrno::Success();
	}
}