#pragma once 
#include "CUtil/inc/Common.h"
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgNameDefine.h"  
#include "RpcDefines.h" 

namespace Robot
{
	class RobotGroup;

	class RobotGroupHandler : public Msg::IRpcMsgCallableObject
	{
		RPC_DEFINE_RobotGroupHandler;
	public:
		RobotGroupHandler(Msg::Object id, Msg::RpcManager * pRpcManager);
		~RobotGroupHandler();

	public:
		virtual CErrno				Init(void);
		virtual CErrno				Cleanup(void);
		virtual CErrno				Update(void);

	public: 
	protected: 
	};

}