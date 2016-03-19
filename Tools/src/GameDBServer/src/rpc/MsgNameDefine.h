/************************************
FileName	:	MsgNameDefine.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Description	:	用于定义消息的全局唯一名字
************************************/
#ifndef __msg_Server_msg_name_define_h__
#define __msg_Server_msg_name_define_h__
#include "MsgLib/inc/MsgCommon.h" 

namespace Msg
{
	//tool testMulitServerNode declare here
	RPC_DEFINE(testMulitServerNode);

	//tool testParamsAndRpcDatas declare here
	RPC_DEFINE(testParamsAndRpcDatas);

	//tool SyncServerHandler declare here
	RPC_DEFINE(SyncServerHandler);

	//tool HandleUserAuth declare here
	RPC_DEFINE(HandleUserAuth);

	//tool HandleSelectDatabase declare here
	RPC_DEFINE(HandleSelectDatabase);

	//tool HandleCreateDatabase declare here
	RPC_DEFINE(HandleCreateDatabase);

	//tool HandleDeleteDatabase declare here
	RPC_DEFINE(HandleDeleteDatabase);

	//tool HandleShowDatabases declare here
	RPC_DEFINE(HandleShowDatabases);

	//tool HandleShowUsers declare here
	RPC_DEFINE(HandleShowUsers);

	//tool HandleCreateUser declare here
	RPC_DEFINE(HandleCreateUser);

	//tool HandleDeleteUser declare here
	RPC_DEFINE(HandleDeleteUser);

	//tool HandleModifyUser declare here
	RPC_DEFINE(HandleModifyUser);

	//tool HandleHSet declare here
	RPC_DEFINE(HandleHSet);

	//tool HandleHSetNX declare here
	RPC_DEFINE(HandleHSetNX);

	//tool HandleHSetOW declare here
	RPC_DEFINE(HandleHSetOW);

	//tool HandleHGet declare here
	RPC_DEFINE(HandleHGet);

	//tool HandleHDel declare here
	RPC_DEFINE(HandleHDel);

	//tool HandleHSetIncr declare here
	RPC_DEFINE(HandleHSetIncr);

	//tool HandleHSetIncrFloat declare here
	RPC_DEFINE(HandleHSetIncrFloat);

	//tool HandleHGetKeys declare here
	RPC_DEFINE(HandleHGetKeys);

	//tool HandleHGetVals declare here
	RPC_DEFINE(HandleHGetVals);

	//tool HandleHGetKeyVals declare here
	RPC_DEFINE(HandleHGetKeyVals);

	//tool HandleHScan declare here
	RPC_DEFINE(HandleHScan);

	//tool HandleHCount declare here
	RPC_DEFINE(HandleHCount);

	//tool HandleHDrop declare here
	RPC_DEFINE(HandleHDrop);

	//tool HandleHList declare here
	RPC_DEFINE(HandleHList);

	//tool HandleHMultiSet declare here
	RPC_DEFINE(HandleHMultiSet);

	//tool HandleHMultiGet declare here
	RPC_DEFINE(HandleHMultiGet);

	//tool HandleHMultiDel declare here
	RPC_DEFINE(HandleHMultiDel);

	//tool HandleZSet declare here
	RPC_DEFINE(HandleZSet);

	//tool HandleZGet declare here
	RPC_DEFINE(HandleZGet);

	//tool HandleZDel declare here
	RPC_DEFINE(HandleZDel);

	//tool HandleZTop declare here
	RPC_DEFINE(HandleZTop);

	//tool HandleZRTop declare here
	RPC_DEFINE(HandleZRTop);

	//tool HandleZDrop declare here
	RPC_DEFINE(HandleZDrop);

	//tool HandleZCount declare here
	RPC_DEFINE(HandleZCount);

	//tool HandleZList declare here
	RPC_DEFINE(HandleZList);

	//tool HandleDump declare here
	RPC_DEFINE(HandleDump);

	//tool SyncMasterHandler declare here
	RPC_DEFINE(SyncMasterHandler);

	//tool SlaveStartAuth declare here
	RPC_DEFINE(SlaveStartAuth);

	//tool SlaveSelectDB declare here
	RPC_DEFINE(SlaveSelectDB);

	//tool SlaveRequestSync declare here
	RPC_DEFINE(SlaveRequestSync);

	//tool MasterStartSync declare here
	RPC_DEFINE(MasterStartSync);

	//tool SyncDataToSlave declare here
	RPC_DEFINE(SyncDataToSlave);

}

#endif

