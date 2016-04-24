#ifndef __net_net_common_h__
#define __net_net_common_h__
#include "tbb/concurrent_unordered_map.h"
#include "tbb/concurrent_hash_map.h"
#include "tbb/concurrent_queue.h"
#include "CUtil/inc/Common.h"
#include "CUtil/inc/BoostHelper.h"
#include "LogLib/inc/Log.h"


#ifdef WIN32
#pragma warning(disable : 4710)
#endif

namespace Net
{    
#define ID_DEFAULT_RAKNET_USER_PACKET 	254

#define MAX_ADDRESS_LENGTH				20
#define DEFAULT_THREAD_PRIORITY			0
#define DEFAULT_THREAD_PRIORITY_COUNT	1
#define MAX_CLIENT_NUMBER				0xfffffff
#define MAX_RECURSION_TIMERS			1000
#define MAX_MESSAGE_LENGTH				(14 * 1024) 

#define DEFAULT_SESSION_TIME_OUT		5 * 1000   //5 session的超时时间最大为5秒.
#define DEFAULT_MAX_CONNECTION_COUNT	20000      //5 定义最大连接客户端的数量
#define DEFAULT_LISTENER_COUNT			10
#define DEFAULT_SOCKET_BUFFER_SIZE		(40 * 1024)
#define DEFAULT_CIRCLE_BUFFER_SIZE		(DEFAULT_SOCKET_BUFFER_SIZE * 10)

#define DEFAULT_MSG_PING_ID				1
#define DEFAULT_PING_TIME_OUT			5000

#ifndef INVALID_NET_SOCKET
#define INVALID_NET_SOCKET -1
#endif

const INT32 kMAX_EPOOL_WAIT_TIME_OUT = 25;
const INT32 kSTART_RAND_UNUSED_PORT = 10000;
const INT32 kEND_RAND_UNUSED_PORT = 20000;

#define tbb_unordered_map tbb::concurrent_unordered_map 
#define tbb_hash_map      tbb::concurrent_hash_map  
	  
#ifdef WIN32 
	typedef INT32 NetSocket;
#endif

#define NET_TYPE_TCP					"tcp"
#define NET_TYPE_UDP					"udp"
#define NET_TYPE_UNIX					"unix"

	enum EReactorType
	{
		REACTOR_TYPE_VAILID,
		REACTOR_TYPE_SELECT,
		REACTOR_TYPE_EPOLL,
		REACTOR_TYPE_EPOLLEX,
		REACTOR_TYPE_WES,
		REACTOR_TYPE_IOCP,
		REACTOR_TYPE_ZMQ,
		REACTOR_TYPE_UDP,
		REACTOR_TYPE_UDS,
		REACTOR_TYPE_UDSEX,
		REACTOR_TYPE_RAKNET,
	};

	struct MsgHeader 
	{
		UINT32 unMsgLength;    //5 第一个必须是长度
		UINT32 unMsgID;

	public:
		MsgHeader() : unMsgLength(0) , unMsgID(0){}
		MsgHeader(UINT32 msgLength , UINT32 msgID) : unMsgLength(msgLength) , unMsgID(msgID){}
		~MsgHeader(){}
	};

	extern INT32 DLL_EXPORT InitNet() ; 
	extern INT32 DLL_EXPORT CleanNet(); 

} //Net

#endif