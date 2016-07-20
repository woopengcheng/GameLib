/************************************
FileName	:	RPCDefines.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.37.66
Version		:	0.0.1
Description	:	RPCº¯Êý¶¨Òå
************************************/
#ifndef __msg_Robot_rpc_defines_h__
#define __msg_Robot_rpc_defines_h__
#include "RpcDatas.h" 

namespace Robot
{

#define  RPC_DEFINE_GRpc public:\
	Msg::ObjectMsgCall * SyncRobotServerID_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID));\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		GRpc::s_setFuncs.insert("SyncRobotServerID");\
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

#define  RPC_DEFINE_RobotServer public:\
	Msg::ObjectMsgCall * SyncRobotServerID_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * SyncRobotServerID_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) );\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		RobotServer::s_setFuncs.insert("SyncRobotServerID");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = RobotServer::s_setFuncs.find(strFunc);\
		if (iter != RobotServer::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\

#define  RPC_DEFINE_RobotGroup public:\
	Msg::ObjectMsgCall * SyncRobotGroupID_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * SyncRobotGroupID_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) );\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		RobotGroup::s_setFuncs.insert("SyncRobotGroupID");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = RobotGroup::s_setFuncs.find(strFunc);\
		if (iter != RobotGroup::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\

#define  RPC_DEFINE_RobotGroupHandler public:\
	Msg::ObjectMsgCall * SyncRobotGroupID_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID));\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		RobotGroupHandler::s_setFuncs.insert("SyncRobotGroupID");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = RobotGroupHandler::s_setFuncs.find(strFunc);\
		if (iter != RobotGroupHandler::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\

#define  RPC_DEFINE_CRobot public:\
	Msg::ObjectMsgCall * SyncRobotID_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * SyncRobotID_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) );\
	Msg::ObjectMsgCall * SyncRobotID_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID));\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		CRobot::s_setFuncs.insert("SyncRobotID");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = CRobot::s_setFuncs.find(strFunc);\
		if (iter != CRobot::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\







}

#endif

