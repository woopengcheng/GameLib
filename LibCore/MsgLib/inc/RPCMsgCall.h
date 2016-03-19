#ifndef __msg_rpc_msg_call_h__
#define __msg_rpc_msg_call_h__ 
#include "MsgLib/inc/MsgCommon.h"  
#include "MsgLib/inc/ObjectMsgCall.h"   

namespace Msg
{
	class DLL_EXPORT RpcCallback
	{
	public:
		virtual ~RpcCallback()
		{}
		virtual INT32 OnCall(void * pContext)
		{
			return -1;
		}
	protected:
	private:
	};
	DECLARE_BOOST_POINTERS(RpcCallback);

	enum ERPC_MSGCALL_TYPE
	{
		RPCTYPE_ERROR = 0 ,
		RPCTYPE_CLIENT  , 
		RPCTYPE_SERVER_PROXY  ,
		RPCTYPE_CLIENT_PROXY  , 
		RPCTYPE_SERVER  , 
		RPCTYPE_TIMEOUT , 
	};

	enum EMSG_SYNC_TYPE
	{
		SYNC_TYPE_SYNC = 0,
		SYNC_TYPE_NONSYNC = 1,
	};

	enum EMSG_SYNC_RESULT
	{
		SYNC_RESULT_START_RETURN = -1,
		SYNC_RESULT_SUCCESS = 0,
		SYNC_RESULT_FALSE = 1,
	};

	enum ERPCRETURN_TYPE
	{
		RETURN_TYPE_DONE = 1, 
		RETURN_TYPE_DELAY = 2, //5 延迟返回,等待某些事件发生的时候再调用.
		RETURN_TYPE_IGNORE = 4	, //5 忽略这个包,同完成.
	};

	class DLL_EXPORT  RPCMsgCall : public ObjectMsgCall
	{ 
		typedef std::set<Msg::Object> CollectionTargetsT;

	public:
		RPCMsgCall()
			: m_bClientRequest(FALSE)
			, m_ullTimeout(MAX_MSG_TIME_OUT)  
			, m_objProxySrcID(DEFAULT_RPC_CALLABLE_ID)
			, m_nRpcMsgCallType(RPCTYPE_ERROR)
			, m_objSyncType(SYNC_TYPE_NONSYNC)
			, m_objSyncResult(SYNC_RESULT_START_RETURN)
			, m_nReturnType(RETURN_TYPE_DONE)  //5 默认完成.
			, m_nProxySessionID(0)
		{  
			RefreshTargets();
		}
		virtual ~RPCMsgCall()
		{ 
			m_setDelayTargets.clear();
		}

	public:
		static void * operator new(size_t size , UINT32 unExtra)throw()
		{
			return malloc(size + unExtra); 
		}

		static void operator delete(void * p)throw()
		{
			free(p);
		}

		static void operator delete(void * p, UINT32 unExtra)throw()
		{
			free(p);
		}

	public:   
		virtual UINT32				RefreshSize() override;
		virtual void				RefreshTargets() override;
		virtual UINT32				GetPacketSize( void ) override;

	public:
		CErrno						CopyTo(RPCMsgCall *& pMsg); //5 将当前对象拷贝给pMsg使用
		CErrno						CopyExcludeNetDatas(RPCMsgCall *& pMsg);
		CErrno						CopyExcludeParams(RPCMsgCall *& pMsg);
		CErrno						CopyExcludeParamsAndTargets(RPCMsgCall *& pMsg ,const std::vector<Msg::Object> & vecTargets , Msg::Object objSrc);
		
	public:
		INT32						GetProxySessionID() const { return m_nProxySessionID; }
		void						SetProxySessionID(INT32 val) { m_nProxySessionID = val; }
		void						SetProxySrcID(Object objProxySrcID){ m_objProxySrcID = objProxySrcID; }
		Object						GetProxySrcID(){ return m_objProxySrcID; }
		INT32						GetRpcMsgCallType() const { return m_nRpcMsgCallType; }
		void						SetRpcMsgCallType(INT32 val) { m_nRpcMsgCallType = val; }
		void						SetSyncType(EMSG_SYNC_TYPE val) { m_objSyncType = val; }
		void						SetSyncResult(EMSG_SYNC_RESULT val) { m_objSyncResult = val; }
		INT32						GetReturnType() const { return m_nReturnType; }
		void						AddReturnType(Msg::ERPCRETURN_TYPE val) { m_nReturnType = m_nReturnType | val; }
		void						ResetReturnType() { m_nReturnType = RETURN_TYPE_DONE; }
		void						AddDelayTarget(Msg::Object obj);
		void						ReplaceDelayTarget();
		EMSG_SYNC_TYPE				GetSyncType(void) { return m_objSyncType; }
		EMSG_SYNC_RESULT			GetSyncResult(void) { return m_objSyncResult; }
		RpcCallbackPtr				GetCallback() { return m_pCallback; }
		void						SetCallback(RpcCallbackPtr val) { m_pCallback = val; }

	public: 
		virtual CUtil::CStream &	marshal(CUtil::CStream & cs) const override;
		virtual CUtil::CStream &	unMarshal(CUtil::CStream & cs) override;

	public:    
		BOOL						m_bClientRequest;
		UINT64						m_ullTimeout;   

		//5 下面的参数是不参与网络传输的.
	protected:
		INT32						m_nProxySessionID;
		Object						m_objProxySrcID;
		INT32						m_nRpcMsgCallType;
		INT32						m_nReturnType;
		EMSG_SYNC_TYPE				m_objSyncType;
		EMSG_SYNC_RESULT			m_objSyncResult;
		CollectionTargetsT			m_setDelayTargets;
		RpcCallbackPtr				m_pCallback;
	};  

}
#endif