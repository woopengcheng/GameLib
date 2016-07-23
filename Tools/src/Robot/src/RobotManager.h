#pragma once 
#include "CUtil/inc/Common.h"
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgNameDefine.h"  
#include "RpcDefines.h" 

namespace Robot
{
	class CRobot;
	class RobotGroup;

	class RobotManager
	{
	public:
		typedef std_unordered_map<INT32, CRobot*>		MapRobots;

	public:
		RobotManager(Msg::RpcManager * pRpcManager);
		~RobotManager();

	public:
		static RobotManager		&	GetInstance();

	public:
		virtual CErrno				Init(void);
		virtual CErrno				Cleanup(void);
		virtual CErrno				Update(void);

	public:
		INT32						PreCreateRobots(INT32 nStartPos, INT32  nEndPos);	//5 最后一个不创建.
		INT32						CreateRobots(INT32 nStartPos, INT32  nEndPos);

	protected:
		INT32						m_nStartPos;
		INT32						m_nEndPos;
		Msg::RpcManager		*		m_pRpcManager;
		MapRobots					m_mapRobots;
	};

}