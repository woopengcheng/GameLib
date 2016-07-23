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
#include "RobotGroup.h"

namespace Robot
{
	static INT32  rpc_CreateRobot(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & name = std::string() , INT32 prof = 0 , INT32 level = 0 , INT32 status = 0 , std_string & note = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_5(pInterface , pSessionName , Msg::g_szCreateRobot_RpcCall , name , prof , level , status , note, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_CreateRobot(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & name = std::string() , INT32 prof = 0 , INT32 level = 0 , INT32 status = 0 , std_string & note = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_CreateRobot(objInterface , pSessionName ,vecTargets , objSrc , name , prof , level , status , note, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_CreateRobot(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & name = std::string() , INT32 prof = 0 , INT32 level = 0 , INT32 status = 0 , std_string & note = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_5(pInterface, pSessionName , Msg::g_szCreateRobot_RpcCall , name , prof , level , status , note, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_CreateRobot(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , std_string & name = std::string() , INT32 prof = 0 , INT32 level = 0 , INT32 status = 0 , std_string & note = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_CreateRobot(objInterface , pSessionName ,vecTargets , objSrc , name , prof , level , status , note, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_CreateRobot(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , std_string & name = std::string() , INT32 prof = 0 , INT32 level = 0 , INT32 status = 0 , std_string & note = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_5(pInterface , nSessionID , Msg::g_szCreateRobot_RpcCall , name , prof , level , status , note, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_CreateRobot(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , std_string & name = std::string() , INT32 prof = 0 , INT32 level = 0 , INT32 status = 0 , std_string & note = std::string() , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_CreateRobot(objInterface , nSessionID ,vecTargets , objSrc , name , prof , level , status , note , pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_UpdateRobotStatue(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 status = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_1(pInterface , pSessionName , Msg::g_szUpdateRobotStatue_RpcCall , status, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_UpdateRobotStatue(Msg::RpcInterface & objInterface , const char * pSessionName , Msg::Object objTarget, Msg::Object objSrc , INT32 status = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_UpdateRobotStatue(objInterface , pSessionName ,vecTargets , objSrc , status, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_UpdateRobotStatue(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 status = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_1(pInterface, pSessionName , Msg::g_szUpdateRobotStatue_RpcCall , status, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_UpdateRobotStatue(Msg::RpcInterface & objInterface , const std::string & pSessionName , Msg::Object objTarget, Msg::Object objSrc , INT32 status = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_UpdateRobotStatue(objInterface , pSessionName ,vecTargets , objSrc , status, pCallback , usPriority , objSyncType);
	}

	static INT32  rpc_UpdateRobotStatue(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::VecObjects & vecTargets , Msg::Object objSrc , INT32 status = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		Msg::RpcInterface * pInterface = &objInterface;
		GEN_RPC_CALL_1(pInterface , nSessionID , Msg::g_szUpdateRobotStatue_RpcCall , status, vecTargets , objSrc , pCallback , usPriority , objSyncType , 10);
	}

	static INT32  rpc_UpdateRobotStatue(Msg::RpcInterface & objInterface , INT32 nSessionID , Msg::Object objTarget, Msg::Object objSrc , INT32 status = 0 , Msg::RpcCallbackPtr pCallback = NULL , UINT16 usPriority = 0 , Msg::EMSG_SYNC_TYPE objSyncType = Msg::SYNC_TYPE_ASYNC)
	{
		std::vector<Msg::Object> vecTargets;
		vecTargets.push_back(objTarget);
		return rpc_UpdateRobotStatue(objInterface , nSessionID ,vecTargets , objSrc , status , pCallback , usPriority , objSyncType);
	}

}

#endif
