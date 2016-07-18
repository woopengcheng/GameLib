#pragma once
#include "RobotGroup.h"
#include "json/json.h"
#include "MsgLib/inc/RpcInterface.h" 


class RobotServer : Msg::RpcInterface
{
public:
	typedef std::map<INT32, INT32>					MapTabToRobotGroup;		//5 建立Tab标签页到RobotServer的映射关系
	typedef std::map<INT32, INT32>					MapRobotGroupToTab;
	typedef std_unordered_map<INT32, RobotGroup*>	MapRobotGroups;

public:
	RobotServer(const std::string & val , INT32 nSessionID);
	~RobotServer();

public:
	virtual CErrno				Init(Json::Value & conf) override;
	virtual CErrno				Cleanup(void) override;
	virtual CErrno				Update(void) override;
	 
public:
	CErrno						CreateRobotGroup(INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false);
	void						OnCreateRobotGroup(RobotGroup * pRobot);
	CErrno						DeleteRobotGroup(INT32 nSessionID);
	void						OnDeleteRobotGroup(RobotGroup * pRobot);
	RobotGroup			*		OnUpdateRobotTab(INT32 nRobotTabIndex);

public:
	std::string					GetName() const { return m_strName; }
	void						SetName(std::string val) { m_strName = val; }
	INT32						GetSessionID() const { return m_nSessionID; }
	void						SetSessionID(INT32 val) { m_nSessionID = val; }
	INT32						GetListCtrlIndex() const { return m_nListCtrlIndex; }
	void						SetListCtrlIndex(INT32 val) { m_nListCtrlIndex = val; }
	INT32						GetCurRobotGroupCount() const { return m_nCurRobotGroupCount; }
	const MapRobotGroups	&	GetMapRobotGroups() const { return m_mapRobotGroups; }

public:
	void						DebugConnect();
	void						DebugDisconnect();

protected:
	void						ResetRobotGroupIndex();

protected:
	std::string					m_strName;
	INT32						m_nSessionID;
	INT32						m_nListCtrlIndex;		//5 这里需要关联界面上的listbox的index.删除用.
	INT32						m_nCurRobotGroupCount;
	MapRobotGroups				m_mapRobotGroups;
	MapTabToRobotGroup			m_mapTabToRobotGroup;
	MapRobotGroupToTab			m_mapRobotGroupToTab;
};


class RobotGroupListener : public Msg::IRpcListener
{
public:
	RobotGroupListener(RobotServer * pMaster)
		: m_pManager(pMaster)
	{

	}

public:
	virtual CErrno		OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false) override;
	virtual CErrno		OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID) override;

private:
	RobotServer * m_pManager;
};