#ifndef __msg_rpc_manager_h__
#define __msg_rpc_manager_h__ 
#include "ThreadPool/inc/ThreadTask.h"
#include "NetLib/inc/NetLib.h"
#include "MsgLib/inc/MsgDispatcher.h"
#include "MsgLib/inc/MsgCommon.h"

namespace Msg
{
	class RPCMsgCall;	
	class Rpc;	
	class RpcInterface;
		
	typedef Net::SPeerInfo SPeerInfo;

	class DLL_EXPORT  RpcManager : public MsgDispatcher
	{
	public:
		typedef tbb_queue<RPCMsgCall *>									CollectionMsgsQueT;
		typedef std::queue<RPCMsgCall *>								StdQueueMsgsQueT;
		typedef tbb_unordered_map<INT32, CollectionMsgsQueT>			CollectionPostMsgsT;
		typedef std_unordered_map<INT32, StdQueueMsgsQueT>				CollectionDelayMsgsT;

		typedef std_unordered_map<INT32, SPeerInfo>						CollectSessionsIDT;
		typedef std_unordered_map<std::string, SPeerInfo>				CollectSessionsStringT;
		typedef std_unordered_map<UINT64 , Rpc *> MapRpcsT;  

	public:
		RpcManager( RpcInterface * pRpcInterface)
			: m_pRpcInterface(pRpcInterface)
		{}
		virtual ~RpcManager(void){} 
		 
	public:
		virtual CErrno				Init(void);
		virtual CErrno				Cleanup(void);
		virtual CErrno				Update(void);

	public:
		virtual INT32				SendMsg(INT32 nSessionID, RPCMsgCall * pMsg, BOOL bAddRpc = TRUE , BOOL bCheck = TRUE);
		virtual INT32				SendMsg(const std::string & strNodeName, RPCMsgCall * pMsg, BOOL bAddRpc = TRUE);
		virtual CErrno				ProxySendMsg(INT32 nSessionID, RPCMsgCall * pMsg) { return CErrno::Failure(); }
		virtual CErrno				PostMsg(INT32 nSessionID, RPCMsgCall * pMsg);
		virtual CErrno				PostMsg(const std::string & strNodeName, RPCMsgCall * pMsg);
		virtual CErrno				PostDelayMsg(INT32 nSessionID, RPCMsgCall * pMsg);

	public:
		virtual CErrno				HandleMsgs(INT32 nSessionID, std::vector<CUtil::Chunk> & vecMsgs);
		virtual CErrno				HandleMsg(INT32 nSessionID, UINT32 unMsgID, const char* pBuffer, UINT32 unLength);
		virtual CErrno				HandleMsg(INT32 nSessionID, RPCMsgCall * pMsg);

	public:
		BOOL						IsConnected(const std::string & strNodeName);
		CErrno						FetchSessions();
		CErrno						FetchMsgs();
		CErrno						UpdateCalls(void);
		CErrno						UpdatePostMsgs(void);
		CErrno						UpdateDelayMsgs(void);
		CErrno						HandleServerMsg(INT32 nSessionID, RPCMsgCall * pMsg);
		CErrno						HandleClientMsg(INT32 nSessionID, RPCMsgCall * pMsg);
		SPeerInfo					GetPeerSessionInfo(INT32 nSessionID);
		INT32						GetPeerSessionIDByNode(const std::string & strNode);
		INT32						GetSessionIDByNode(const std::string & strNode);
		std::string					GetNodeNameByID(INT32 nSessionID);
		INT32						CheckAndHandlePostMsg(const std::string & strNodeName, RPCMsgCall * pMsg, BOOL bAddRpc);

	public:
		void						InsertSendRpc(UINT64 ullRpcMsgID, Rpc * pRpc);
		RpcInterface		*		GetRpcInterface() { return m_pRpcInterface; }
		Rpc					*		GetSendRpc(UINT64 ullMsgID);
		void						InsertSendRpc(RPCMsgCall * pMsg);
		MapRpcsT			&		GetSendRpcs(void) { return m_mapSendRpcs; }

	protected:
		void						EraseAndInsertPeerSession(const SPeerInfo & objInfo);
		void						InsertPostMsg(INT32 nSessionID, RPCMsgCall * pMsg);
		void						InsertDelayMsg(INT32 nSessionID, RPCMsgCall * pMsg);

	protected:  
		MapRpcsT					m_mapSendRpcs;   
		RpcInterface			*	m_pRpcInterface;
		CollectSessionsIDT			m_mapSessions;
		CollectSessionsIDT			m_mapPeerSessions;
		CollectSessionsStringT		m_mapSessionNodes;
		CollectionPostMsgsT			m_mapPostMsgs;  //5 处理同一个进程下的不通线程的的消息转发
		CollectionDelayMsgsT		m_mapDelayMsgs;	//5	处理逻辑条件中的可能需要延迟处理的消息包.实现同PostMsg.为了性能,这里单独写一份.

	};  

}


#endif