#pragma once
#include "RobotCommon.h"
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgNameDefine.h"  
#include "RpcDefines.h" 


namespace Robot
{
	class RobotGroup;

	enum ERobotStatue
	{
		STATE_OFFLINE = 0,
		STATE_LOGIN = 0,
		STATE_GAMING = 0,
		STATE_MOVING = 0,

	};

// 	enum EProf
// 	{
// 		STATE_OFFLINE = 0,
// 		STATE_LOGIN = 0,
// 		STATE_GAMING = 0,
// 		STATE_MOVING = 0,
// 
// 	};

	struct RobotInfo
	{
		std::string		strName;
		INT32			nProf;
		INT32			nLevel;
		INT32			nStatue;
		std::string		strNote;

		RobotInfo()
			: nProf(0)
			, nLevel(0)
			, nStatue(STATE_OFFLINE)
		{

		}
	};

	class CRobot: public Msg::IRpcMsgCallableObject
	{
		RPC_DEFINE_CRobot;
	public:
		CRobot( RobotGroup * pRobotGroup , Msg::Object id, Msg::RpcManager * pRpcManager);
		~CRobot();

	public:
		virtual CErrno				Init(void);
		virtual CErrno				Cleanup(void);
		virtual CErrno				Update(void);

	public:
		RobotInfo			&		GetRobotInfo() { return m_objRobotInfo; }
		const RobotInfo		&		GetRobotInfo() const { return m_objRobotInfo; }
		void						SetRobotInfo(const RobotInfo & val) { m_objRobotInfo = val; }
		INT64						GetPeerRobotID() const { return m_llPeerRobotID; }
		void						SetPeerRobotID(INT64 val) { m_llPeerRobotID = val; }
		INT32						GetListCtrlIndex() const { return m_nListViewIndex; }
		void						SetListCtrlIndex(INT32 val) { m_nListViewIndex = val; }
		RobotGroup			*		GetRobotGroup() const { return m_pRobotGroup; }

	protected:
		RobotInfo					m_objRobotInfo;
		RobotGroup			*		m_pRobotGroup;
		INT64						m_llPeerRobotID;	//5 这里需要关联RobotGroup上的index.
		INT32						m_nListViewIndex;	//5 这里需要关联ListCtrl上的index.
	};

}