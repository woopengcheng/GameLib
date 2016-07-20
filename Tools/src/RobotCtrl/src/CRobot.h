#pragma once
#include "RobotCommon.h"
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgNameDefine.h"  
#include "RpcDefines.h" 


namespace Robot
{
	class RobotGroup;

	class CRobot: public Msg::IRpcMsgCallableObject
	{
		RPC_DEFINE_CRobot;
	public:
		CRobot(const std::string & val, INT32 nSessionID, RobotGroup * pRobotGroup , Msg::Object id, Msg::RpcManager * pRpcManager);
		~CRobot();

	public:
		virtual CErrno				Init(void);
		virtual CErrno				Cleanup(void);
		virtual CErrno				Update(void);

	public:
		std::string					GetName() const { return m_strName; }
		void						SetName(std::string val) { m_strName = val; }
		INT32						GetSessionID() const { return m_nSessionID; }
		void						SetSessionID(INT32 val) { m_nSessionID = val; }
		INT32						GetRobotIndex() const { return m_nRobotIndex; }
		void						SetRobotIndex(INT32 val) { m_nRobotIndex = val; }
		RobotGroup			*		GetRobotGroup() const { return m_pRobotGroup; }

	protected:
		std::string					m_strName;
		INT32						m_nSessionID;
		INT32						m_nRobotIndex;		//5 这里需要关联ShowRobots界面上的index.
		RobotGroup			*		m_pRobotGroup;
	};

}