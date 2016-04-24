/************************************
FileName	:	RpcCallFuncs.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Description	:	客户端调用的rpc.
************************************/
#ifndef __msg_rpc_call_funcs_h__
#define __msg_rpc_call_funcs_h__

#include "MsgLib/inc/MsgHelper.h"
#include "MsgLib/inc/RPCMsgCall.h"
#include "MsgNameDefine.h"
#include "RpcDatas.h"
#include "DBServer.h"
#include "DBMaster.h"
#include "DBSlave.h"

namespace Server
{
	static INT32  rpc_SyncServerHandler(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Server::DBServer::GetInstance())) , pSessionName , Msg::g_szSyncServerHandler_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncServerHandler(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncServerHandler( pSessionName ,vecTargets , objSrc , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncServerHandler(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Server::DBServer::GetInstance())) , pSessionName , Msg::g_szSyncServerHandler_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncServerHandler(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncServerHandler( pSessionName ,vecTargets , objSrc , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncServerHandler(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Server::DBServer::GetInstance())) , nSessionID , Msg::g_szSyncServerHandler_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncServerHandler(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncServerHandler( nSessionID ,vecTargets , objSrc  , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncDataToSlave(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_3((&(Server::DBServer::GetInstance())) , pSessionName , Msg::g_szSyncDataToSlave_RpcCall , dbname , value , last_pos, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncDataToSlave(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncDataToSlave( pSessionName ,vecTargets , objSrc , dbname , value , last_pos, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncDataToSlave(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_3((&(Server::DBServer::GetInstance())) , pSessionName , Msg::g_szSyncDataToSlave_RpcCall , dbname , value , last_pos, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncDataToSlave(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncDataToSlave( pSessionName ,vecTargets , objSrc , dbname , value , last_pos, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncDataToSlave(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_3((&(Server::DBServer::GetInstance())) , nSessionID , Msg::g_szSyncDataToSlave_RpcCall , dbname , value , last_pos, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncDataToSlave(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncDataToSlave( nSessionID ,vecTargets , objSrc , dbname , value , last_pos , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_testParamsAndRpcDatas(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , TestRpcData & rpcData = TestRpcData() , TestRpcData2 & rpcData2 = TestRpcData2() , std::vector<INT32> & Vec = std::vector<INT32>(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_3((&(Server::DBMaster::GetInstance())) , pSessionName , Msg::g_sztestParamsAndRpcDatas_RpcCall , rpcData , rpcData2 , Vec, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_testParamsAndRpcDatas(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , TestRpcData & rpcData = TestRpcData() , TestRpcData2 & rpcData2 = TestRpcData2() , std::vector<INT32> & Vec = std::vector<INT32>(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_testParamsAndRpcDatas( pSessionName ,vecTargets , objSrc , rpcData , rpcData2 , Vec, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_testParamsAndRpcDatas(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , TestRpcData & rpcData = TestRpcData() , TestRpcData2 & rpcData2 = TestRpcData2() , std::vector<INT32> & Vec = std::vector<INT32>(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_3((&(Server::DBMaster::GetInstance())) , pSessionName , Msg::g_sztestParamsAndRpcDatas_RpcCall , rpcData , rpcData2 , Vec, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_testParamsAndRpcDatas(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , TestRpcData & rpcData = TestRpcData() , TestRpcData2 & rpcData2 = TestRpcData2() , std::vector<INT32> & Vec = std::vector<INT32>(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_testParamsAndRpcDatas( pSessionName ,vecTargets , objSrc , rpcData , rpcData2 , Vec, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_testParamsAndRpcDatas(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , TestRpcData & rpcData = TestRpcData() , TestRpcData2 & rpcData2 = TestRpcData2() , std::vector<INT32> & Vec = std::vector<INT32>(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_3((&(Server::DBMaster::GetInstance())) , nSessionID , Msg::g_sztestParamsAndRpcDatas_RpcCall , rpcData , rpcData2 , Vec, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_testParamsAndRpcDatas(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , TestRpcData & rpcData = TestRpcData() , TestRpcData2 & rpcData2 = TestRpcData2() , std::vector<INT32> & Vec = std::vector<INT32>(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_testParamsAndRpcDatas( nSessionID ,vecTargets , objSrc , rpcData , rpcData2 , Vec , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncMasterHandler(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 id = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_1((&(Server::DBMaster::GetInstance())) , pSessionName , Msg::g_szSyncMasterHandler_RpcCall , id, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncMasterHandler(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , INT32 id = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncMasterHandler( pSessionName ,vecTargets , objSrc , id, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncMasterHandler(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 id = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_1((&(Server::DBMaster::GetInstance())) , pSessionName , Msg::g_szSyncMasterHandler_RpcCall , id, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncMasterHandler(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , INT32 id = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncMasterHandler( pSessionName ,vecTargets , objSrc , id, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncMasterHandler(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 id = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_1((&(Server::DBMaster::GetInstance())) , nSessionID , Msg::g_szSyncMasterHandler_RpcCall , id, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncMasterHandler(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , INT32 id = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncMasterHandler( nSessionID ,vecTargets , objSrc , id , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_MasterStartSync(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & filename = std::string() , INT32 filesize = 0 , INT32 sendtype = 0 , CUtilChunk & value = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_4((&(Server::DBMaster::GetInstance())) , pSessionName , Msg::g_szMasterStartSync_RpcCall , filename , filesize , sendtype , value, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_MasterStartSync(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & filename = std::string() , INT32 filesize = 0 , INT32 sendtype = 0 , CUtilChunk & value = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_MasterStartSync( pSessionName ,vecTargets , objSrc , filename , filesize , sendtype , value, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_MasterStartSync(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & filename = std::string() , INT32 filesize = 0 , INT32 sendtype = 0 , CUtilChunk & value = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_4((&(Server::DBMaster::GetInstance())) , pSessionName , Msg::g_szMasterStartSync_RpcCall , filename , filesize , sendtype , value, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_MasterStartSync(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & filename = std::string() , INT32 filesize = 0 , INT32 sendtype = 0 , CUtilChunk & value = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_MasterStartSync( pSessionName ,vecTargets , objSrc , filename , filesize , sendtype , value, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_MasterStartSync(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & filename = std::string() , INT32 filesize = 0 , INT32 sendtype = 0 , CUtilChunk & value = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_4((&(Server::DBMaster::GetInstance())) , nSessionID , Msg::g_szMasterStartSync_RpcCall , filename , filesize , sendtype , value, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_MasterStartSync(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , std_string & filename = std::string() , INT32 filesize = 0 , INT32 sendtype = 0 , CUtilChunk & value = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_MasterStartSync( nSessionID ,vecTargets , objSrc , filename , filesize , sendtype , value , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_testMulitServerNode(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_5((&(Server::DBSlave::GetInstance())) , pSessionName , Msg::g_sztestMulitServerNode_RpcCall , dbname , value , value2 , value22 , valChar, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_testMulitServerNode(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_testMulitServerNode( pSessionName ,vecTargets , objSrc , dbname , value , value2 , value22 , valChar, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_testMulitServerNode(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_5((&(Server::DBSlave::GetInstance())) , pSessionName , Msg::g_sztestMulitServerNode_RpcCall , dbname , value , value2 , value22 , valChar, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_testMulitServerNode(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_testMulitServerNode( pSessionName ,vecTargets , objSrc , dbname , value , value2 , value22 , valChar, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_testMulitServerNode(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_5((&(Server::DBSlave::GetInstance())) , nSessionID , Msg::g_sztestMulitServerNode_RpcCall , dbname , value , value2 , value22 , valChar, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_testMulitServerNode(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0) , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_testMulitServerNode( nSessionID ,vecTargets , objSrc , dbname , value , value2 , value22 , valChar , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SlaveStartAuth(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & name = std::string() , std_string & pwd = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_2((&(Server::DBSlave::GetInstance())) , pSessionName , Msg::g_szSlaveStartAuth_RpcCall , name , pwd, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SlaveStartAuth(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & name = std::string() , std_string & pwd = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SlaveStartAuth( pSessionName ,vecTargets , objSrc , name , pwd, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SlaveStartAuth(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & name = std::string() , std_string & pwd = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_2((&(Server::DBSlave::GetInstance())) , pSessionName , Msg::g_szSlaveStartAuth_RpcCall , name , pwd, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SlaveStartAuth(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & name = std::string() , std_string & pwd = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SlaveStartAuth( pSessionName ,vecTargets , objSrc , name , pwd, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SlaveStartAuth(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & name = std::string() , std_string & pwd = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_2((&(Server::DBSlave::GetInstance())) , nSessionID , Msg::g_szSlaveStartAuth_RpcCall , name , pwd, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SlaveStartAuth(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , std_string & name = std::string() , std_string & pwd = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SlaveStartAuth( nSessionID ,vecTargets , objSrc , name , pwd , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SlaveRequestSync(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & dbname = std::string() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_2((&(Server::DBSlave::GetInstance())) , pSessionName , Msg::g_szSlaveRequestSync_RpcCall , dbname , last_pos, vecTargets , objSrc , pCallback , usPriority , objSyncType , 3600);
	}

	static INT32  rpc_SlaveRequestSync(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & dbname = std::string() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SlaveRequestSync( pSessionName ,vecTargets , objSrc , dbname , last_pos, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SlaveRequestSync(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & dbname = std::string() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_2((&(Server::DBSlave::GetInstance())) , pSessionName , Msg::g_szSlaveRequestSync_RpcCall , dbname , last_pos, vecTargets , objSrc , pCallback , usPriority , objSyncType , 3600);
	}

	static INT32  rpc_SlaveRequestSync(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & dbname = std::string() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SlaveRequestSync( pSessionName ,vecTargets , objSrc , dbname , last_pos, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SlaveRequestSync(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & dbname = std::string() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_2((&(Server::DBSlave::GetInstance())) , nSessionID , Msg::g_szSlaveRequestSync_RpcCall , dbname , last_pos, vecTargets , objSrc , pCallback , usPriority , objSyncType , 3600);
	}

	static INT32  rpc_SlaveRequestSync(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , std_string & dbname = std::string() , INT32 last_pos = -1 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SlaveRequestSync( nSessionID ,vecTargets , objSrc , dbname , last_pos , pCallback , usPriority , objSyncType);
	}

}

#endif
