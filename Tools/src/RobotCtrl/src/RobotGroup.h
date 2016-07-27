#pragma once
#include "CRobot.h"
#include "json/json.h"
#include "MsgLib/inc/RpcInterface.h" 
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgNameDefine.h"  
#include "RpcDefines.h" 

namespace Robot
{
	class RobotServer;

	class RobotGroup : public Msg::IRpcMsgCallableObject
	{
		RPC_DEFINE_RobotGroup;
	public:
		typedef std::vector<CRobot*>					VecRobots;

	public:
		RobotGroup(const std::string & val, INT32 nSessionID, RobotServer * pRobotServer , Msg::Object id, Msg::RpcManager * pRpcManager);
		~RobotGroup();

	public:
		virtual CErrno				Init(Json::Value & conf);
		virtual CErrno				Cleanup(void);
		virtual CErrno				Update(void); 

	public:
		INT32						CreateRobot(const RobotInfo & info , Msg::Object objSrc);
		void						OnCreateRobot(CRobot * pRobot);

	public:
		std::string					GetName() const { return m_strName; }
		void						SetName(std::string val) { m_strName = val; }
		INT32						GetRobotSessionID() const { return m_nRobotSessionID; }
		void						SetRobotSessionID(INT32 val) { m_nRobotSessionID = val; }
		INT32						GetRobotTabIndex() const { return m_nRobotTabIndex; }
		void						SetRobotTabIndex(INT32 val) { m_nRobotTabIndex = val; }
		INT32						GetCurRobotCount() const { return m_nCurRobotCount; }
		const VecRobots		&		GetVecRobots() const { return m_vecRobots; } 
		RobotServer			*		GetRobotServer() { return m_pRobotServer;  }
		BOOL						ResortRobots(EListColType type , BOOL bUp = TRUE);

	protected:
		Json::Value					m_objConf;
		std::string					m_strName;
		INT32						m_nRobotSessionID;
		INT32						m_nRobotTabIndex;		//5 这里需要关联界面上的Tab的index,但是这个Index是从1开始的,所以当界面上使用的时候需要减去1.删除用.
		INT32						m_nCurRobotCount;		//5 当前机器人的数量.
		VecRobots					m_vecRobots;
		RobotServer			*		m_pRobotServer;
		EListColType				m_objCurSortType;
	};

}