/************************************
FileName	:	MsgNameDefine.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.1.104
Version		:	0.0.1
Description	:	用于定义消息的全局唯一名字
************************************/
#ifndef __msg_Server_msg_name_define_h__
#define __msg_Server_msg_name_define_h__
#include "MsgLib/inc/MsgCommon.h" 

namespace Msg
{
	//tool BarHandler::testMulitServerNode declare here
	RPC_DEFINE(testMulitServerNode);
	
	//tool ProxyHandler::testParamsAndRpcDatas declare here
	RPC_DEFINE(testParamsAndRpcDatas);

}

#endif

