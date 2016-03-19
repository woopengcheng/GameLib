#ifndef __msg_msg_common_h__
#define __msg_msg_common_h__
#include "CUtil/inc/Common.h"
#include "tbb/concurrent_unordered_map.h"
#include "tbb/concurrent_hash_map.h"
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_priority_queue.h"
#include "vector"
#include "MsgLib/inc/Object.h"

typedef std::set<std::string> CollectionObjectFuncsT;

namespace Msg
{ 
#define tbb_unordered_map     tbb::concurrent_unordered_map 
#define tbb_hash_map          tbb::concurrent_hash_map 
#define tbb_queue             tbb::concurrent_queue 
#define tbb_priority_queue    tbb::concurrent_priority_queue

#define DEFAULT_RPC_CALLABLE_ID          Object(0)    //5 默认RPC的callable ID
#define DEFAULT_RPC_MSG_ID               2    //5 默认的RPC的消息ID

#define DEFAULT_MSG_THREAD_ID            3
#define DEFAULT_RPC_MSG_THREAD_ID        5
#define DEFAULT_MSG_HANDLE_THREAD_ID     4

#define MAX_MSG_METHOD_NAME_LENGTH       60

#define MAX_MSG_TIME_OUT                 15

#define RPCServerProxy "_X" 
#define RPCClientProxy "_Y" 
//#define RPCTimeoutProxy "_Z" 
#define RPCClient "_C"
#define RPCServer "_S"
#define RPCTimeout "_T" 

#define RPC_NAMEDEFINE(RPC_Name , MIDDLE , SUFFIX) static const char * g_sz##RPC_Name##MIDDLE = #RPC_Name#SUFFIX;  

#define RPC_DEFINE(RPC_Name ) static const char * g_sz##RPC_Name##_RpcCall = #RPC_Name; \
	RPC_NAMEDEFINE(RPC_Name , _RpcServer , _S)\
	RPC_NAMEDEFINE(RPC_Name , _RpcClient , _C)\
	RPC_NAMEDEFINE(RPC_Name , _RpcTimeout , _T)\
	RPC_NAMEDEFINE(RPC_Name , _RpcServerProxy , _X)\
	RPC_NAMEDEFINE(RPC_Name , _RpcClientProxy , _Y)\
	RPC_NAMEDEFINE(RPC_Name , _RpcTimeoutProxy , _T)

#define MSG_DEFINE(MSG_NAME)  static const char * g_szLocal_##MSG_NAME = #MSG_NAME; 

#define VECTOR_TARGETS_NULL (std::vector<Msg::Object>()) 
	 
}
#endif