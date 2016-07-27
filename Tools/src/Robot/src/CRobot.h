#pragma once 
#include "CUtil/inc/Common.h"
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "NetLib/inc/NetHandlerClient.h"
#include "NetLib/inc/MsgProcess.h"
#include "MsgNameDefine.h"  
#include "RpcDefines.h" 
#include "Lua/ILua.h"

namespace Robot
{
	class RobotGroup;
	class RobotMsgProcess;

	class CRobot : public Msg::IRpcMsgCallableObject
	{
		RPC_DEFINE_CRobot;
	public:
		CRobot(Msg::Object id, Msg::RpcManager * pRpcManager, Net::INetReactor * pReactor);
		~CRobot();

	public:
		virtual CErrno				Init(void);
		virtual CErrno				Cleanup(void);
		virtual CErrno				Update(void);
		virtual CErrno				Process(Net::ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength);

	public:
		INT32						SendMsg(const char * pBuf, UINT32 unSize);

	public:
		INT32						GetRobotIndex() const { return m_nRobotIndex; }
		void						SetRobotIndex(INT32 val) { m_nRobotIndex = val; }
		Net::NetHandlerClientPtr	GetNetHandler() { return m_pNetHandlerClient;  }
		INT32						HandleRobotCommand(INT32 nCommandType, CUtil::CStream & cs);
		INT32 	test(char t) { return 1; }
		INT32  test (UINT8 t) { return 1; }
		INT32  test (bool t) { return t; }

	protected:
		INT32						m_nRobotIndex;		//5 这里需要关联robotctrl上的index.	
		Net::NetHandlerClientPtr    m_pNetHandlerClient;
		Net::INetReactor		*	m_pReactor;
		RobotMsgProcess			*	m_pMsgProcess;
		lua_State				*	m_L;
	};

	class RobotMsgProcess : public Net::MsgProcess
	{
	public:
		RobotMsgProcess(CRobot * pRobot)
			: m_pRobot(pRobot)
		{}

	public:
		virtual CErrno Process(Net::ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength);

	protected:
		CRobot					*	m_pRobot;
	};

}