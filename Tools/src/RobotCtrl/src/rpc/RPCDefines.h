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
	Msg::ObjectMsgCall * SyncRobotServerID_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), INT32 server_port = 0);\
	Msg::ObjectMsgCall * HandleRobotGroup_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), INT32 type = 0 , INT32 param1 = 0 , INT32 param2 = 0 , INT32 param3 = 0 , INT32 param4 = 0);\
	Msg::ObjectMsgCall * UpdateRobotStart_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), CUtilChunk & file_list = CUtil::Chunk());\
	Msg::ObjectMsgCall * SyncRobotGroupID_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID));\
	Msg::ObjectMsgCall * CreateRobots_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), INT32 start_pos = 0 , INT32 end_pos = 0);\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		GRpc::s_setFuncs.insert("SyncRobotServerID");\
		GRpc::s_setFuncs.insert("HandleRobotGroup");\
		GRpc::s_setFuncs.insert("UpdateRobotStart");\
		GRpc::s_setFuncs.insert("SyncRobotGroupID");\
		GRpc::s_setFuncs.insert("CreateRobots");\
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
	Msg::ObjectMsgCall * SyncRobotServerID_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 server_port = 0);\
	Msg::ObjectMsgCall * HandleRobotGroup_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * HandleRobotGroup_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 type = 0 , INT32 param1 = 0 , INT32 param2 = 0 , INT32 param3 = 0 , INT32 param4 = 0);\
	Msg::ObjectMsgCall * UpdateRobotStart_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , CUtilChunk & file_list_re = CUtil::Chunk());\
	Msg::ObjectMsgCall * UpdateRobotStart_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , CUtilChunk & file_list = CUtil::Chunk());\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		RobotServer::s_setFuncs.insert("SyncRobotServerID");\
		RobotServer::s_setFuncs.insert("HandleRobotGroup");\
		RobotServer::s_setFuncs.insert("UpdateRobotStart");\
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
	Msg::ObjectMsgCall * CreateRobots_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * CreateRobots_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 start_pos = 0 , INT32 end_pos = 0);\
	Msg::ObjectMsgCall * CreateRobot_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & name = std::string() , INT32 prof = 0 , INT32 level = 0 , INT32 status = 0 , std_string & note = std::string());\
	Msg::ObjectMsgCall * SendRobotCommand_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * SendRobotCommand_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std::vector<INT32> & map_robots = std::vector<INT32>(0) , std_string & command = std::string());\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		RobotGroup::s_setFuncs.insert("SyncRobotGroupID");\
		RobotGroup::s_setFuncs.insert("CreateRobots");\
		RobotGroup::s_setFuncs.insert("CreateRobot");\
		RobotGroup::s_setFuncs.insert("SendRobotCommand");\
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

#define  RPC_DEFINE_CRobot public:\
	Msg::ObjectMsgCall * CreateRobot_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * CreateRobot_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & name = std::string() , INT32 prof = 0 , INT32 level = 0 , INT32 status = 0 , std_string & note = std::string());\
	Msg::ObjectMsgCall * SendRobotCommand_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std::vector<INT32> & map_robots = std::vector<INT32>(0) , std_string & command = std::string());\
	Msg::ObjectMsgCall * UpdateRobotStatue_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * UpdateRobotStatue_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 status = 0);\
	Msg::ObjectMsgCall * UpdateRobotStatue_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), INT32 status = 0);\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		CRobot::s_setFuncs.insert("CreateRobot");\
		CRobot::s_setFuncs.insert("SendRobotCommand");\
		CRobot::s_setFuncs.insert("UpdateRobotStatue");\
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

