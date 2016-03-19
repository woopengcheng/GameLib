/************************************
FileName	:	RPCDefines.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.31.196
Version		:	0.0.1
Description	:	RPCº¯Êý¶¨Òå
************************************/
#ifndef __msg_Server_rpc_defines_h__
#define __msg_Server_rpc_defines_h__
#include "RpcDatas.h" 

#define  RPC_DEFINE_GRpc public:\
	Msg::ObjectMsgCall * testMulitServerNode_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , CUtilChunk & res = CUtil::Chunk());\
	Msg::ObjectMsgCall * testMulitServerNode_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		GRpc::s_setFuncs.insert("testMulitServerNode");\
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

#define  RPC_DEFINE_FooHandler public:\
	Msg::ObjectMsgCall * testMulitServerNode_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * testMulitServerNode_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * testMulitServerNode_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , CUtilChunk & res = CUtil::Chunk());\
	Msg::ObjectMsgCall * testMulitServerNode_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * testParamsAndRpcDatas_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), TestRpcData & rpcData = TestRpcData() , TestRpcData2 & rpcData2 = TestRpcData2() , std::vector<INT32> & Vec = std::vector<INT32>(1));\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		FooHandler::s_setFuncs.insert("testMulitServerNode");\
		FooHandler::s_setFuncs.insert("testParamsAndRpcDatas");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = FooHandler::s_setFuncs.find(strFunc);\
		if (iter != FooHandler::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\

#define  RPC_DEFINE_ProxyHandler public:\
	Msg::ObjectMsgCall * testMulitServerNode_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * testMulitServerNode_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
	Msg::ObjectMsgCall * testMulitServerNode_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , CUtilChunk & res = CUtil::Chunk());\
	Msg::ObjectMsgCall * testParamsAndRpcDatas_RpcClient(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , INT32 res = 0);\
	Msg::ObjectMsgCall * testParamsAndRpcDatas_RpcTimeout(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID) , TestRpcData & rpcData = TestRpcData() , TestRpcData2 & rpcData2 = TestRpcData2() , std::vector<INT32> & Vec = std::vector<INT32>(1));\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		ProxyHandler::s_setFuncs.insert("testMulitServerNode");\
		ProxyHandler::s_setFuncs.insert("testParamsAndRpcDatas");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = ProxyHandler::s_setFuncs.find(strFunc);\
		if (iter != ProxyHandler::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\

#define  RPC_DEFINE_BarHandler public:\
	Msg::ObjectMsgCall * testMulitServerNode_RpcServer(INT32 nSessionID, Msg::Object objSrc = Msg::Object(Msg::DEFAULT_RPC_CALLABLE_ID), std_string & dbname = std::string() , CUtilChunk & value = CUtil::Chunk() , int value2 = 0 , unsigned int value22 = 0 , char valChar = char(0));\
public:\
	static CollectionObjectFuncsT s_setFuncs;\
	static void InitObjectFuncs()\
	{\
		BarHandler::s_setFuncs.insert("testMulitServerNode");\
	}\
	virtual BOOL IsHasFunc(const std::string & strFunc)\
	{\
		CollectionObjectFuncsT::iterator iter = BarHandler::s_setFuncs.find(strFunc);\
		if (iter != BarHandler::s_setFuncs.end())\
		{\
			return TRUE;\
		}\
	return FALSE;\
	}\


#endif

