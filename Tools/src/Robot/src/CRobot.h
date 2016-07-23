#pragma once 
#include "CUtil/inc/Common.h"
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgNameDefine.h"  
#include "RpcDefines.h" 

namespace Robot
{
	class RobotGroup;

	class CRobot : public Msg::IRpcMsgCallableObject
	{
		RPC_DEFINE_CRobot;
	public:
		CRobot(Msg::Object id, Msg::RpcManager * pRpcManager);
		~CRobot();

	public:
		virtual CErrno				Init(void);
		virtual CErrno				Cleanup(void);
		virtual CErrno				Update(void);

	public:
		INT32						GetRobotIndex() const { return m_nRobotIndex; }
		void						SetRobotIndex(INT32 val) { m_nRobotIndex = val; }

	protected:
		INT32						m_nRobotIndex;		//5 这里需要关联robotctrl上的index.				
	};

}