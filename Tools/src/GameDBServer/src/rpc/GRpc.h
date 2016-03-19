/************************************
FileName	:	GlableRpc.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Description	:	静态函数,无对象时通过这个类处理.
************************************/
#ifndef __Server_global_rpc_h__
#define __Server_global_rpc_h__
#include "MsgLib/inc/IRpcMsgCallableObject.h" 
#include "RpcDefines.h" 

namespace Server
{
	class GRpc : public Msg::IRpcMsgCallableObject
	{
		RPC_DEFINE_GRpc;
	public:
		GRpc(Msg::Object nID , Msg::RpcManager * pRpcManager)
			: Msg::IRpcMsgCallableObject(nID , pRpcManager)
		{
			GRpc::InitObjectFuncs();
		}
	public:
	};

}

#endif

