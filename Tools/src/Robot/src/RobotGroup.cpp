#include "RobotGroup.h"
#include "LogLib/inc/Log.h"
#include "RobotManager.h"

namespace Robot
{

	RobotGroup::RobotGroup()
		: m_nRobotSessionID(-1)
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

	void RobotGroup::OnConnect(INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect /*= false*/)
	{
		m_nRobotSessionID = nSessionID;
	}

	void RobotGroup::OnDisonnect(INT32 nSessionID, INT32 nPeerSessionID)
	{

	}

	CErrno RobotGroupListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (m_pManager)
		{
			m_pManager->OnConnect(nSessionID, strNetNodeName, bReconnect);
		}

		gDebugStream("connected from sessionID=" << nSessionID);
		return CErrno::Success();
	}

	CErrno RobotGroupListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{
		if (m_pManager)
		{
			m_pManager->OnDisonnect(nSessionID , nPeerSessionID);
		}

		gDebugStream("disconnected from sessionID=" << nPeerSessionID);
		return CErrno::Success();
	}
}