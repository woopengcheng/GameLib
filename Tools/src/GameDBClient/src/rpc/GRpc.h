/************************************
FileName	:	GlableRpc.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.37.66
Version		:	0.0.1
Description	:	静态函数,无对象时通过这个类处理.
************************************/
#ifndef __Client_global_rpc_h__
#define __Client_global_rpc_h__
#include "MsgLib/inc/IRpcMsgCallableObject.h" 
#include "RpcDefines.h" 

namespace Client
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

