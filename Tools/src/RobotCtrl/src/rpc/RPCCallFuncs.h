/************************************
FileName	:	RpcCallFuncs.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.37.66
Version		:	0.0.1
Description	:	客户端调用的rpc.
************************************/
#ifndef __msg_rpc_call_funcs_h__
#define __msg_rpc_call_funcs_h__

#include "MsgLib/inc/MsgHelper.h"
#include "MsgLib/inc/RPCMsgCall.h"
#include "MsgNameDefine.h"
#include "RpcDatas.h"
#include "RobotManager.h"
#include "RobotServer.h"

namespace Robot
{
	static INT32  rpc_SyncRobotServerID(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 server_port = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_1(pInterface , pSessionName , Msg::g_szSyncRobotServerID_RpcCall , server_port, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotServerID(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , INT32 server_port = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotServerID(objInterface , pSessionName ,vecTargets , objSrc , server_port, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotServerID(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 server_port = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_1(pInterface, pSessionName , Msg::g_szSyncRobotServerID_RpcCall , server_port, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotServerID(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , INT32 server_port = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotServerID(objInterface , pSessionName ,vecTargets , objSrc , server_port, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotServerID(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 server_port = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_1(pInterface , nSessionID , Msg::g_szSyncRobotServerID_RpcCall , server_port, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotServerID(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , INT32 server_port = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotServerID(objInterface , nSessionID ,vecTargets , objSrc , server_port , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_HandleRobotGroup(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 type = 0 , INT32 param1 = 0 , INT32 param2 = 0 , INT32 param3 = 0 , INT32 param4 = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_5(pInterface , pSessionName , Msg::g_szHandleRobotGroup_RpcCall , type , param1 , param2 , param3 , param4, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_HandleRobotGroup(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , INT32 type = 0 , INT32 param1 = 0 , INT32 param2 = 0 , INT32 param3 = 0 , INT32 param4 = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_HandleRobotGroup(objInterface , pSessionName ,vecTargets , objSrc , type , param1 , param2 , param3 , param4, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_HandleRobotGroup(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 type = 0 , INT32 param1 = 0 , INT32 param2 = 0 , INT32 param3 = 0 , INT32 param4 = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_5(pInterface, pSessionName , Msg::g_szHandleRobotGroup_RpcCall , type , param1 , param2 , param3 , param4, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_HandleRobotGroup(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , INT32 type = 0 , INT32 param1 = 0 , INT32 param2 = 0 , INT32 param3 = 0 , INT32 param4 = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_HandleRobotGroup(objInterface , pSessionName ,vecTargets , objSrc , type , param1 , param2 , param3 , param4, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_HandleRobotGroup(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 type = 0 , INT32 param1 = 0 , INT32 param2 = 0 , INT32 param3 = 0 , INT32 param4 = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_5(pInterface , nSessionID , Msg::g_szHandleRobotGroup_RpcCall , type , param1 , param2 , param3 , param4, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_HandleRobotGroup(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , INT32 type = 0 , INT32 param1 = 0 , INT32 param2 = 0 , INT32 param3 = 0 , INT32 param4 = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_HandleRobotGroup(objInterface , nSessionID ,vecTargets , objSrc , type , param1 , param2 , param3 , param4 , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_UpdateRobotStart(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , CUtilChunk & file_list = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_1(pInterface , pSessionName , Msg::g_szUpdateRobotStart_RpcCall , file_list, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_UpdateRobotStart(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , CUtilChunk & file_list = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_UpdateRobotStart(objInterface , pSessionName ,vecTargets , objSrc , file_list, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_UpdateRobotStart(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , CUtilChunk & file_list = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_1(pInterface, pSessionName , Msg::g_szUpdateRobotStart_RpcCall , file_list, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_UpdateRobotStart(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , CUtilChunk & file_list = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_UpdateRobotStart(objInterface , pSessionName ,vecTargets , objSrc , file_list, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_UpdateRobotStart(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , CUtilChunk & file_list = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_1(pInterface , nSessionID , Msg::g_szUpdateRobotStart_RpcCall , file_list, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_UpdateRobotStart(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , CUtilChunk & file_list = CUtil::Chunk() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_UpdateRobotStart(objInterface , nSessionID ,vecTargets , objSrc , file_list , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotGroupID(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_0(pInterface , pSessionName , Msg::g_szSyncRobotGroupID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotGroupID(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotGroupID(objInterface , pSessionName ,vecTargets , objSrc , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotGroupID(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_0(pInterface, pSessionName , Msg::g_szSyncRobotGroupID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotGroupID(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotGroupID(objInterface , pSessionName ,vecTargets , objSrc , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotGroupID(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_0(pInterface , nSessionID , Msg::g_szSyncRobotGroupID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotGroupID(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotGroupID(objInterface , nSessionID ,vecTargets , objSrc  , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_CreateRobots(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 start_pos = 0 , INT32 end_pos = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_2(pInterface , pSessionName , Msg::g_szCreateRobots_RpcCall , start_pos , end_pos, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_CreateRobots(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , INT32 start_pos = 0 , INT32 end_pos = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_CreateRobots(objInterface , pSessionName ,vecTargets , objSrc , start_pos , end_pos, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_CreateRobots(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 start_pos = 0 , INT32 end_pos = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_2(pInterface, pSessionName , Msg::g_szCreateRobots_RpcCall , start_pos , end_pos, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_CreateRobots(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , INT32 start_pos = 0 , INT32 end_pos = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_CreateRobots(objInterface , pSessionName ,vecTargets , objSrc , start_pos , end_pos, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_CreateRobots(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 start_pos = 0 , INT32 end_pos = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_2(pInterface , nSessionID , Msg::g_szCreateRobots_RpcCall , start_pos , end_pos, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_CreateRobots(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , INT32 start_pos = 0 , INT32 end_pos = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_CreateRobots(objInterface , nSessionID ,vecTargets , objSrc , start_pos , end_pos , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SendRobotCommand(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std::vector<INT32> & map_robots = std::vector<INT32>(0) , std_string & command = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_2(pInterface , pSessionName , Msg::g_szSendRobotCommand_RpcCall , map_robots , command, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SendRobotCommand(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , std::vector<INT32> & map_robots = std::vector<INT32>(0) , std_string & command = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SendRobotCommand(objInterface , pSessionName ,vecTargets , objSrc , map_robots , command, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SendRobotCommand(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std::vector<INT32> & map_robots = std::vector<INT32>(0) , std_string & command = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_2(pInterface, pSessionName , Msg::g_szSendRobotCommand_RpcCall , map_robots , command, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SendRobotCommand(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , std::vector<INT32> & map_robots = std::vector<INT32>(0) , std_string & command = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SendRobotCommand(objInterface , pSessionName ,vecTargets , objSrc , map_robots , command, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SendRobotCommand(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , std::vector<INT32> & map_robots = std::vector<INT32>(0) , std_string & command = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_2(pInterface , nSessionID , Msg::g_szSendRobotCommand_RpcCall , map_robots , command, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SendRobotCommand(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , std::vector<INT32> & map_robots = std::vector<INT32>(0) , std_string & command = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SendRobotCommand(objInterface , nSessionID ,vecTargets , objSrc , map_robots , command , pCallback , usPriority , objSyncType);
	}

}

#endif
