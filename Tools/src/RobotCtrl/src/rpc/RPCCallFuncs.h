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
#include "RobotGroup.h"

namespace Robot
{
	static INT32  rpc_SyncRobotServerID(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Robot::RobotManager::GetInstance())) , pSessionName , Msg::g_szSyncRobotServerID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotServerID(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotServerID( pSessionName ,vecTargets , objSrc , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotServerID(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Robot::RobotManager::GetInstance())) , pSessionName , Msg::g_szSyncRobotServerID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotServerID(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotServerID( pSessionName ,vecTargets , objSrc , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotServerID(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Robot::RobotManager::GetInstance())) , nSessionID , Msg::g_szSyncRobotServerID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotServerID(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotServerID( nSessionID ,vecTargets , objSrc  , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotGroupID(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Robot::RobotServer::GetInstance())) , pSessionName , Msg::g_szSyncRobotGroupID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotGroupID(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotGroupID( pSessionName ,vecTargets , objSrc , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotGroupID(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Robot::RobotServer::GetInstance())) , pSessionName , Msg::g_szSyncRobotGroupID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotGroupID(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotGroupID( pSessionName ,vecTargets , objSrc , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotGroupID(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Robot::RobotServer::GetInstance())) , nSessionID , Msg::g_szSyncRobotGroupID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotGroupID(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotGroupID( nSessionID ,vecTargets , objSrc  , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotID(const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Robot::RobotGroup::GetInstance())) , pSessionName , Msg::g_szSyncRobotID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotID(const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotID( pSessionName ,vecTargets , objSrc , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotID(const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Robot::RobotGroup::GetInstance())) , pSessionName , Msg::g_szSyncRobotID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotID(const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotID( pSessionName ,vecTargets , objSrc , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_SyncRobotID(INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		GEN_RPC_CALL_0((&(Robot::RobotGroup::GetInstance())) , nSessionID , Msg::g_szSyncRobotID_RpcCall , vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_SyncRobotID(INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc  , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_SyncRobotID( nSessionID ,vecTargets , objSrc  , pCallback , usPriority , objSyncType);
	}

}

#endif
