/************************************
FileName	:	RPCDefines.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.37.66
Version		:	0.0.1
Description	:	RPCº¯Êý¶¨Òå
************************************/
#ifndef __msg_Server_rpc_defines_h__
#define __msg_Server_rpc_defines_h__
#include "RpcDatas.h" 

namespace Server
{

#define  RPC_DEFINE_GRpc public:\
	Msg::ObjectMsgCall * testMulitServerNode_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , CUtilChunk & res = CUtil::Chunk());\
	Msg::ObjectMsgCall * testMulitServerNode_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * SyncServerHandler_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID));\
	Msg::ObjectMsgCall * SyncMasterHandler_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), INT32 id = 0);\
	Msg::ObjectMsgCall * SyncDataToSlave_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , INT32 last_pos = -1);\
	Msg::ObjectMsgCall * SyncDataToSlave_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , INT32 last_pos = -1);\
	Msg::ObjectMsgCall * SyncDataToSlave_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0 , INT32 master_id = 0);\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		GRpc::s_setFuncs.insert("testMulitServerNode");\
		GRpc::s_setFuncs.insert("SyncServerHandler");\
		GRpc::s_setFuncs.insert("SyncMasterHandler");\
		GRpc::s_setFuncs.insert("SyncDataToSlave");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = GRpc::s_setFuncs.find(strFunc);\
		if (iter != GRpc::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\

#define  RPC_DEFINE_ServerHandler public:\
	Msg::ObjectMsgCall * testMulitServerNode_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * testMulitServerNode_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * testMulitServerNode_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , CUtilChunk & res = CUtil::Chunk());\
	Msg::ObjectMsgCall * testParamsAndRpcDatas_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), TestRpcData & rpcData = TestRpcData() , TestRpcData2 & rpcData2 = TestRpcData2() , std::vector<INT32> & Vec = std::vector<INT32>(0));\
	Msg::ObjectMsgCall * SyncServerHandler_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * SyncServerHandler_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) );\
	Msg::ObjectMsgCall * HandleUserAuth_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & name = std::string() , std_string & pwd = std::string());\
	Msg::ObjectMsgCall * HandleSelectDatabase_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & dbname = std::string());\
	Msg::ObjectMsgCall * HandleCreateDatabase_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & dbname = std::string());\
	Msg::ObjectMsgCall * HandleDeleteDatabase_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & dbname = std::string());\
	Msg::ObjectMsgCall * HandleShowDatabases_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID));\
	Msg::ObjectMsgCall * HandleShowUsers_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID));\
	Msg::ObjectMsgCall * HandleCreateUser_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & name = std::string() , std_string & pwd = std::string() , SINT8 issys = 0);\
	Msg::ObjectMsgCall * HandleDeleteUser_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & name = std::string());\
	Msg::ObjectMsgCall * HandleModifyUser_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & name = std::string() , std_string & pwd = std::string() , SINT8 issys = 0);\
	Msg::ObjectMsgCall * HandleHSet_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & key = std::string() , std_string & value = std::string());\
	Msg::ObjectMsgCall * HandleHSetNX_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & key = std::string() , std_string & value = std::string());\
	Msg::ObjectMsgCall * HandleHSetOW_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & key = std::string() , std_string & value = std::string());\
	Msg::ObjectMsgCall * HandleHGet_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & key = std::string());\
	Msg::ObjectMsgCall * HandleHDel_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & key = std::string());\
	Msg::ObjectMsgCall * HandleHSetIncr_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & key = std::string() , INT64 value = 0);\
	Msg::ObjectMsgCall * HandleHSetIncrFloat_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & key = std::string() , double value = 0.0f);\
	Msg::ObjectMsgCall * HandleHGetKeys_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string());\
	Msg::ObjectMsgCall * HandleHGetVals_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string());\
	Msg::ObjectMsgCall * HandleHGetKeyVals_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string());\
	Msg::ObjectMsgCall * HandleHScan_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & start = std::string() , std_string & regexp = std::string() , INT64 limit = 10);\
	Msg::ObjectMsgCall * HandleHCount_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string());\
	Msg::ObjectMsgCall * HandleHDrop_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string());\
	Msg::ObjectMsgCall * HandleHList_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID));\
	Msg::ObjectMsgCall * HandleHMultiSet_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , CUtilChunk & keyvalues = CUtil::Chunk());\
	Msg::ObjectMsgCall * HandleHMultiGet_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , CUtilChunk & keys = CUtil::Chunk());\
	Msg::ObjectMsgCall * HandleHMultiDel_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , CUtilChunk & keys = CUtil::Chunk());\
	Msg::ObjectMsgCall * HandleZSet_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & key = std::string() , INT64 score = 0);\
	Msg::ObjectMsgCall * HandleZGet_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & key = std::string());\
	Msg::ObjectMsgCall * HandleZDel_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , std_string & key = std::string());\
	Msg::ObjectMsgCall * HandleZTop_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , INT64 start = 0 , INT64 end = 0 , INT64 limit = 0);\
	Msg::ObjectMsgCall * HandleZRTop_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string() , INT64 start = 0 , INT64 end = 0 , INT64 limit = 0);\
	Msg::ObjectMsgCall * HandleZDrop_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string());\
	Msg::ObjectMsgCall * HandleZCount_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & table = std::string());\
	Msg::ObjectMsgCall * HandleZList_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID));\
	Msg::ObjectMsgCall * HandleDump_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID));\
	Msg::ObjectMsgCall * SlaveStartAuth_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & name = std::string() , std_string & pwd = std::string());\
	Msg::ObjectMsgCall * SlaveRequestSync_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & dbname = std::string() , INT32 last_pos = -1);\
	Msg::ObjectMsgCall * SyncDataToSlave_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0 , INT32 master_id = 0);\
	Msg::ObjectMsgCall * SyncDataToSlave_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , INT32 last_pos = -1);\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		ServerHandler::s_setFuncs.insert("testMulitServerNode");\
		ServerHandler::s_setFuncs.insert("testParamsAndRpcDatas");\
		ServerHandler::s_setFuncs.insert("SyncServerHandler");\
		ServerHandler::s_setFuncs.insert("HandleUserAuth");\
		ServerHandler::s_setFuncs.insert("HandleSelectDatabase");\
		ServerHandler::s_setFuncs.insert("HandleCreateDatabase");\
		ServerHandler::s_setFuncs.insert("HandleDeleteDatabase");\
		ServerHandler::s_setFuncs.insert("HandleShowDatabases");\
		ServerHandler::s_setFuncs.insert("HandleShowUsers");\
		ServerHandler::s_setFuncs.insert("HandleCreateUser");\
		ServerHandler::s_setFuncs.insert("HandleDeleteUser");\
		ServerHandler::s_setFuncs.insert("HandleModifyUser");\
		ServerHandler::s_setFuncs.insert("HandleHSet");\
		ServerHandler::s_setFuncs.insert("HandleHSetNX");\
		ServerHandler::s_setFuncs.insert("HandleHSetOW");\
		ServerHandler::s_setFuncs.insert("HandleHGet");\
		ServerHandler::s_setFuncs.insert("HandleHDel");\
		ServerHandler::s_setFuncs.insert("HandleHSetIncr");\
		ServerHandler::s_setFuncs.insert("HandleHSetIncrFloat");\
		ServerHandler::s_setFuncs.insert("HandleHGetKeys");\
		ServerHandler::s_setFuncs.insert("HandleHGetVals");\
		ServerHandler::s_setFuncs.insert("HandleHGetKeyVals");\
		ServerHandler::s_setFuncs.insert("HandleHScan");\
		ServerHandler::s_setFuncs.insert("HandleHCount");\
		ServerHandler::s_setFuncs.insert("HandleHDrop");\
		ServerHandler::s_setFuncs.insert("HandleHList");\
		ServerHandler::s_setFuncs.insert("HandleHMultiSet");\
		ServerHandler::s_setFuncs.insert("HandleHMultiGet");\
		ServerHandler::s_setFuncs.insert("HandleHMultiDel");\
		ServerHandler::s_setFuncs.insert("HandleZSet");\
		ServerHandler::s_setFuncs.insert("HandleZGet");\
		ServerHandler::s_setFuncs.insert("HandleZDel");\
		ServerHandler::s_setFuncs.insert("HandleZTop");\
		ServerHandler::s_setFuncs.insert("HandleZRTop");\
		ServerHandler::s_setFuncs.insert("HandleZDrop");\
		ServerHandler::s_setFuncs.insert("HandleZCount");\
		ServerHandler::s_setFuncs.insert("HandleZList");\
		ServerHandler::s_setFuncs.insert("HandleDump");\
		ServerHandler::s_setFuncs.insert("SlaveStartAuth");\
		ServerHandler::s_setFuncs.insert("SlaveRequestSync");\
		ServerHandler::s_setFuncs.insert("SyncDataToSlave");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = ServerHandler::s_setFuncs.find(strFunc);\
		if (iter != ServerHandler::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\

#define  RPC_DEFINE_MasterHandler public:\
	Msg::ObjectMsgCall * testMulitServerNode_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * testMulitServerNode_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * testMulitServerNode_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , CUtilChunk & res = CUtil::Chunk());\
	Msg::ObjectMsgCall * testParamsAndRpcDatas_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * testParamsAndRpcDatas_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , TestRpcData & rpcData = TestRpcData() , TestRpcData2 & rpcData2 = TestRpcData2() , std::vector<INT32> & Vec = std::vector<INT32>(0));\
	Msg::ObjectMsgCall * SyncMasterHandler_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * SyncMasterHandler_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 id = 0);\
	Msg::ObjectMsgCall * SlaveStartAuth_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & name = std::string() , std_string & pwd = std::string());\
	Msg::ObjectMsgCall * SlaveStartAuth_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & name = std::string() , std_string & pwd = std::string());\
	Msg::ObjectMsgCall * SlaveStartAuth_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & value = std::string());\
	Msg::ObjectMsgCall * SlaveRequestSync_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , INT32 last_pos = -1);\
	Msg::ObjectMsgCall * SlaveRequestSync_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , INT32 last_pos = -1);\
	Msg::ObjectMsgCall * SlaveRequestSync_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & newbackdir = std::string() , INT32 res = 0);\
	Msg::ObjectMsgCall * MasterStartSync_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * MasterStartSync_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & filename = std::string() , INT32 filesize = 0 , INT32 sendtype = 0 , CUtilChunk & value = CUtil::Chunk());\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		MasterHandler::s_setFuncs.insert("testMulitServerNode");\
		MasterHandler::s_setFuncs.insert("testParamsAndRpcDatas");\
		MasterHandler::s_setFuncs.insert("SyncMasterHandler");\
		MasterHandler::s_setFuncs.insert("SlaveStartAuth");\
		MasterHandler::s_setFuncs.insert("SlaveRequestSync");\
		MasterHandler::s_setFuncs.insert("MasterStartSync");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = MasterHandler::s_setFuncs.find(strFunc);\
		if (iter != MasterHandler::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\

#define  RPC_DEFINE_SlaveHandler public:\
	Msg::ObjectMsgCall * testMulitServerNode_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * SlaveStartAuth_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & value = std::string());\
	Msg::ObjectMsgCall * SlaveStartAuth_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & name = std::string() , std_string & pwd = std::string());\
	Msg::ObjectMsgCall * SlaveRequestSync_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & newbackdir = std::string() , INT32 res = 0);\
	Msg::ObjectMsgCall * SlaveRequestSync_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , INT32 last_pos = -1);\
	Msg::ObjectMsgCall * MasterStartSync_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & filename = std::string() , INT32 filesize = 0 , INT32 sendtype = 0 , CUtilChunk & value = CUtil::Chunk());\
	Msg::ObjectMsgCall * SyncDataToSlave_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , INT32 last_pos = -1);\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		SlaveHandler::s_setFuncs.insert("testMulitServerNode");\
		SlaveHandler::s_setFuncs.insert("SlaveStartAuth");\
		SlaveHandler::s_setFuncs.insert("SlaveRequestSync");\
		SlaveHandler::s_setFuncs.insert("MasterStartSync");\
		SlaveHandler::s_setFuncs.insert("SyncDataToSlave");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = SlaveHandler::s_setFuncs.find(strFunc);\
		if (iter != SlaveHandler::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\







}

#endif

