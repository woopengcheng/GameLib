#ifndef __msg_rpc_interface_h__
#define __msg_rpc_interface_h__  
#include "MsgLib/inc/MsgCommon.h"
#include "json/json.h"

#ifdef WIN32
#pragma warning( disable :4996)
#endif

namespace Net
{
	class NetThread;
}

namespace Msg
{
	class IRpcListener;
	class RPCMsgCall;
	class RpcManager;
	class RpcInterface;

	struct SRpcCoTask
	{
		void		*	pCoID;
		RPCMsgCall	*	pMsg;
		RpcInterface*	pInterface;
		SRpcCoTask()
			: pCoID(NULL)
			, pMsg(NULL)
			, pInterface(NULL)
		{}
	};

	class DLL_EXPORT  RpcInterface
	{
	public:
		typedef std::map<UINT64, SRpcCoTask*>		MapRpcCoTasksT;

	public:
		RpcInterface(void);
		virtual ~RpcInterface(void);

	public:
		virtual CErrno			Init(Json::Value & conf);
		virtual CErrno			Cleanup(void);
		virtual CErrno			Update(void);
		virtual void			OnRegisterRpcs(void) {}

	public:
		virtual INT32			SendMsg(INT32 nSessionID, RPCMsgCall * pMsg, BOOL bAddRpc = TRUE);
		virtual INT32			SendMsg(const std::string & strNodeName, RPCMsgCall * pMsg, BOOL bAddRpc = TRUE);

	public:
		RpcManager			*	GetRpcManager() { return m_pRpcManager; }
		Net::NetThread 		*	GetNetThread() { return m_pNetThread; }
		IRpcListener		*	GetRpcListener() { return m_pRpcListener; }
		void					RegisterRpc(void);
		BOOL					DeleteRpcCoTask(UINT64 ullMsgID);
		BOOL					AddRpcCoTask(SRpcCoTask * pTask);
		BOOL					ResumeRpcCoTask(UINT64 ullMsgID);

	private:
		void					TakeOverSync(RPCMsgCall * pMsg);
		void					HandlerMySelfNode(Json::Value & conf);
		void					EnableRpc(Json::Value & conf);

	protected:
		RpcManager			*	m_pRpcManager;
		Net::NetThread		*	m_pNetThread;
		IRpcListener		*	m_pRpcListener;

		//5 以下是协程相关处理同步的.
	protected:
		MapRpcCoTasksT			m_mapRpcCoTasks;
		BOOL					m_bFirstUpdated;
	};


	/************************************************************************/
	/* 用于监听RPC是否连接成功和关闭的接口。重写即可
	/* 2015年4月22日12:05:00
	/*
	/************************************************************************/
	class DLL_EXPORT  IRpcListener
	{
	public:
		IRpcListener()
		{

		}
	public:
		virtual CErrno OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false) { return CErrno::Success(); }
		virtual CErrno OnDisconnected(RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID) { return CErrno::Success(); }
	};
}


#endif