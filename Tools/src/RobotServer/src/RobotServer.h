#pragma once 
#include "json/json.h"
#include "MsgLib/inc/RpcInterface.h" 

namespace Robot
{

	class RobotServer : Msg::RpcInterface
	{
	public:
		typedef std::map<INT32, INT32>					MapTabToRobotGroup;		//5 建立Tab标签页到RobotServer的映射关系
		typedef std::map<INT32, INT32>					MapRobotGroupToTab;

	public:
		static RobotServer & GetInstance()
		{
			static RobotServer sRpcInterface;
			return sRpcInterface;
		}

	public:
		RobotServer();
		~RobotServer();

	public:
		virtual CErrno				Init(Json::Value & conf) override;
		virtual CErrno				Cleanup(void) override;
		virtual CErrno				Update(void) override;

	public:
		virtual void				OnRegisterRpcs(void) override;

	public:
		INT32						GetServerSessionID() const { return m_nServerSessionID; }
		void						SetServerSessionID(INT32 nSessionID) { m_nServerSessionID = nSessionID; }
		INT64						GetServerID() const { return m_llServerID; }
		void						SetServerID(INT64 nID) { m_llServerID = nID; }

	private:
		INT32						m_nServerSessionID;
		INT64						m_llServerID;
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