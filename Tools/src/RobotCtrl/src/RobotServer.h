#pragma once
#include "RobotGroup.h"
#include "json/json.h"
#include "MsgLib/inc/RpcInterface.h" 
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgNameDefine.h"  
#include "RpcDefines.h" 


namespace Robot
{
	class RobotServer :public Msg::RpcInterface, public Msg::IRpcMsgCallableObject
	{
		RPC_DEFINE_RobotServer;
	public:
		typedef std::map<INT32, INT32>					MapTabToRobotGroup;		//5 建立Tab标签页到RobotServer的映射关系
		typedef std::map<INT32, INT32>					MapRobotGroupToTab;
		typedef std_unordered_map<INT32, RobotGroup*>	MapRobotGroups;

	public:
		RobotServer(const std::string & val, INT32 nSessionID, Msg::Object id, Msg::RpcManager * pRpcManager);
		~RobotServer();

	public:
		virtual CErrno				Init(Json::Value & conf) override;
		virtual CErrno				Cleanup(void) override;
		virtual CErrno				Update(void) override;

	public:
		virtual void				OnRegisterRpcs(void) override;

	public:
		CErrno						CreateRobotGroup(INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false);
		void						OnCreateRobotGroup(RobotGroup * pRobot);
		CErrno						DeleteRobotGroup(INT32 nSessionID);
		void						OnDeleteRobotGroup(RobotGroup * pRobot);
		RobotGroup			*		GetRobotGroup(INT32 nRobotTabIndex);
		void						CloseAllRobotGroups();

	public:
		std::string					GetName() const { return m_strName; }
		void						SetName(std::string val) { m_strName = val; }
		INT32						GetRobotSessionID() const { return m_nRobotSessionID; }
		void						SetRobotSessionID(INT32 val) { m_nRobotSessionID = val; }
		INT32						GetListCtrlIndex() const { return m_nListCtrlIndex; }
		void						SetListCtrlIndex(INT32 val) { m_nListCtrlIndex = val; }
		INT32						GetCurRobotGroupCount() const { return m_nCurRobotGroupCount - 1; }
		const MapRobotGroups	&	GetMapRobotGroups() const { return m_mapRobotGroups; }
		UINT32						GetServerPort() const { return m_nServerPort; }
		void						SetServerPort(UINT32 val) { m_nServerPort = val; }

	public:
		void						DebugConnect();
		void						DebugDisconnect();

	protected:
		void						ResetRobotGroupIndex();

	protected:
		INT32						m_nServerPort;
		Json::Value					m_objConf;
		std::string					m_strName;
		INT32						m_nRobotSessionID;
		INT32						m_nListCtrlIndex;		//5 这里需要关联界面上的listbox的index.删除用.
		INT32						m_nCurRobotGroupCount;
		MapRobotGroups				m_mapRobotGroups;
		MapTabToRobotGroup			m_mapTabToRobotGroup;
		MapRobotGroupToTab			m_mapRobotGroupToTab;
	};


	class RobotServerListener : public Msg::IRpcListener
	{
	public:
		RobotServerListener(RobotServer * pMaster)
			: m_pManager(pMaster)
		{

		}

	public:
		virtual CErrno		OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false) override;
		virtual CErrno		OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID) override;

	private:
		RobotServer * m_pManager;
	};
}