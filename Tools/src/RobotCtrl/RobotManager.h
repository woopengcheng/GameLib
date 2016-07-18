#pragma once
#include "RobotCommon.h"
#include "RobotServer.h"
#include "json/json.h"
#include "MsgLib/inc/RpcInterface.h" 

class RobotManager : Msg::RpcInterface
{
public:
	RobotManager();
	~RobotManager();

public:
	typedef std::map<INT32, INT32>					MapTabToRobotServer;		//5 建立Tab标签页到RobotServer的映射关系
	typedef std::map<INT32, INT32>					MapRobotServerToTab;
	typedef std_unordered_map<INT32, RobotServer*>	MapRobotServers;

public:
	static RobotManager		&	GetInstance();

public:
	virtual CErrno				Init(Json::Value & conf) override;
	virtual CErrno				Cleanup(void) override;
	virtual CErrno				Update(void) override;

public:
	CErrno						CreateRobotServer(INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false);
	void						OnCreateRobotServer(RobotServer * pRobot);
	CErrno						DeleteRobotServer(INT32 nSessionID);
	void						OnDeleteRobotServer(RobotServer * pRobot);
	RobotServer			*		OnUpdateCtrlServer(INT32 nIndex);
	RobotGroup			*		OnUpdateRobotTab(INT32 nListCtrlIndex , INT32 nRobotTabIndex);
	INT32						GetCurRobotServerCount() const { return m_nCurRobotServerCount; }

public:
	void						DebugConnect();
	void						DebugDisconnect();

protected:
	void						ResetRobotServerIndex();

private:
	INT32						m_nCurRobotServerCount;
	MapRobotServers				m_mapRobotServers;
	MapTabToRobotServer			m_mapTabToRobotServer;
	MapRobotServerToTab			m_mapRobotServerToTab;
};


class RobotServerListener : public Msg::IRpcListener
{
public:
	RobotServerListener(RobotManager * pMaster)
		: m_pManager(pMaster)
	{

	}

public:
	virtual CErrno		OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false) override;
	virtual CErrno		OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID) override;

private:
	RobotManager * m_pManager;
};