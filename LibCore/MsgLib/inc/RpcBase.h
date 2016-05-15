#ifndef __msg_rpc_h__
#define __msg_rpc_h__ 
#include "CUtil/inc/GenParamHelper.h"
#include "MsgLib/inc/MsgCommon.h" 
#include "MsgLib/inc/ICallableObject.h"
#include "MsgLib/inc/RPCMsgCall.h"
#include "MsgLib/inc/RpcCheckParams.h"
#include "MsgLib/inc/RpcManager.h"
#include "NetLib/inc/ISession.h"
#include "Timer/inc/TimeCount.h" 

#define ReturnNULL return NULL;
#define ReturnDelay return RPCReturn(Msg::RETURN_TYPE_DELAY); 
#define Return return RPCReturn


#define RPC_MSG_SEND_TYPE_DELIVER "Deliver"
#define RPC_MSG_SEND_TYPE_RETURN  "Return"

#define RPC_GEN_MSG(vecTargets , objSrc)	if (m_pRpcMsgCall->GetRpcMsgCallType() == RPCTYPE_TIMEOUT ||\
		m_pRpcMsgCall->GetRpcMsgCallType() == RPCTYPE_ERROR ||\
		m_pRpcMsgCall->GetRpcMsgCallType() == RPCTYPE_CLIENT)\
		{\
		return NULL;\
		}\
		\
		RPCMsgCall * pMsg = NULL; \
		m_pRpcMsgCall->CopyExcludeParamsAndTargets(pMsg , vecTargets , objSrc);

#define RPC_GEN_RETURN_MSG \
	std::vector<Msg::Object> vecTargets;\
	if(m_pRpcMsgCall->GetRpcMsgCallType() == RPCTYPE_CLIENT_PROXY)\
		vecTargets.push_back(m_pRpcMsgCall->GetProxySrcID());\
	else\
		vecTargets.push_back(m_pRpcMsgCall->m_objSource);\
	Msg::Object objSrc = this->GetObjectID();\
	RPC_GEN_MSG(vecTargets , objSrc) pMsg->m_bClientRequest = TRUE;

#define RPC_GEN_PROXY_MSG(vecTargets) \
	Msg::Object objSrc = this->GetObjectID();\
	RPC_GEN_MSG(vecTargets , objSrc) pMsg->m_bClientRequest = FALSE; \
	pMsg->SetProxySrcID(m_pRpcMsgCall->m_objSource); 

#define CHECK_RPC_RETURN_TYPE 	m_pRpcMsgCall->ResetReturnType();\
	m_pRpcMsgCall->AddReturnType(type);\
	if (type == RETURN_TYPE_DELAY)\
	{\
		m_pRpcMsgCall->AddDelayTarget(m_objID);\
		return NULL;\
	}

namespace Timer
{
	class TimeCount;
}

namespace Msg
{
	
	class RpcManager;  
	class RPCMsgCall;

	class DLL_EXPORT  Rpc : public ICallableObject
	{
	public:
		typedef std::vector<ObjectMsgCall*> VecObjectMsgCallT;

	public:
		Rpc(RpcManager * pRpcManager , UINT64 unTimeout = 0 , Object nID = DEFAULT_RPC_CALLABLE_ID , RPCMsgCall *  pMsg = NULL , INT32 nSessionID = 0)
			: ICallableObject(nID)  
			, m_pRpcMsgCall(pMsg)
			, m_pRpcManager(pRpcManager)
			, m_nSessionID(nSessionID)
		{ 
			m_objTimeout.Start(unTimeout * 1000); 
		} 
		~Rpc( void ){}

	public:
		virtual BOOL			OnTimeout( RPCMsgCall * pMsg , VecObjectMsgCallT & vecObjectMsgCall );
		virtual BOOL			OnServer( RPCMsgCall * pMsg , VecObjectMsgCallT & vecObjectMsgCall ); 
		virtual BOOL			OnProxy( RPCMsgCall * pMsg , VecObjectMsgCallT & vecObjectMsgCall ); 
		virtual BOOL			OnClient( RPCMsgCall * pMsg , VecObjectMsgCallT & vecObjectMsgCall );  

	public:
		BOOL					IsTimeout();
		void					SetTimeout(UINT64 unTimeout); 
		INT32					ProxySendBack();   
		CUtil::Parameters	*	GetInParams();
		RpcCallbackPtr			GetCallback();

	public:   
		template<typename NameOrID>
		INT32 ProxySendMsg(NameOrID pName , std::vector<Msg::Object> & vecTargets ) 
		{
			Assert_ReF1("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod ) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_PROXY_MSG(vecTargets);

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams); 

			INT32 nResult = m_pRpcManager->SendMsg(pName, pMsg);
			if (nResult == -1)
			{
				SAFE_DELETE(pMsg);
			}
			return nResult;
		} 

		template<typename NameOrID , typename P1 >
		INT32 ProxySendMsg(NameOrID pName , std::vector<Msg::Object> & vecTargets , P1 &p1) 
		{
			Assert_ReF1("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 ) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_PROXY_MSG(vecTargets);

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1); 

			INT32 nResult = m_pRpcManager->SendMsg(pName, pMsg);
			if (nResult == -1)
			{
				SAFE_DELETE(pMsg);
			}
			return nResult;
		}

		template<typename NameOrID , typename P1 , typename P2 >
		INT32 ProxySendMsg(NameOrID pName , std::vector<Msg::Object> & vecTargets , P1 &p1 , P2 &p2) 
		{
			Assert_ReF1("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_PROXY_MSG(vecTargets);

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2); 

			INT32 nResult = m_pRpcManager->SendMsg(pName, pMsg);
			if (nResult == -1)
			{
				SAFE_DELETE(pMsg);
			}
			return nResult;
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3>
		INT32 ProxySendMsg(NameOrID pName , std::vector<Msg::Object> & vecTargets , P1 &p1 , P2 &p2 , P3 &p3) 
		{
			Assert_ReF1("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 ) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_PROXY_MSG(vecTargets);

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 ); 

			INT32 nResult = m_pRpcManager->SendMsg(pName, pMsg);
			if (nResult == -1)
			{
				SAFE_DELETE(pMsg);
			}
			return nResult;
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3 , typename P4>
		INT32 ProxySendMsg(NameOrID pName , std::vector<Msg::Object> & vecTargets , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4) 
		{
			Assert_ReF1("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 , p4) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_PROXY_MSG(vecTargets); 

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4); 

			INT32 nResult = m_pRpcManager->SendMsg(pName, pMsg);
			if (nResult == -1)
			{
				SAFE_DELETE(pMsg);
			}
			return nResult;
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3 , typename P4 , typename P5>
		INT32 ProxySendMsg(NameOrID pName , std::vector<Msg::Object> & vecTargets , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5) 
		{
			Assert_ReF1("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 , p4 , p5) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_PROXY_MSG(vecTargets);

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4 , p5 ); 

			INT32 nResult = m_pRpcManager->SendMsg(pName, pMsg);
			if (nResult == -1)
			{
				SAFE_DELETE(pMsg);
			}
			return nResult;
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6>
		INT32 ProxySendMsg(NameOrID pName , std::vector<Msg::Object> & vecTargets , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6) 
		{
			Assert_ReF1("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 , p4 , p5 , p6) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_PROXY_MSG(vecTargets);

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4 , p5 , p6); 

			INT32 nResult = m_pRpcManager->SendMsg(pName, pMsg);
			if (nResult == -1)
			{
				SAFE_DELETE(pMsg);
			}
			return nResult;
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7>
		INT32 ProxySendMsg(NameOrID pName , std::vector<Msg::Object> & vecTargets , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7) 
		{
 			Assert_ReF1("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 , p4 , p5 , p6, p7) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_PROXY_MSG(vecTargets);

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4 , p5 , p6 , p7); 

			INT32 nResult = m_pRpcManager->SendMsg(pName, pMsg);
			if (nResult == -1)
			{
				SAFE_DELETE(pMsg);
			}
			return nResult;
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8>
		INT32 ProxySendMsg(NameOrID pName , std::vector<Msg::Object> & vecTargets , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7 , P8 &p8) 
		{
			Assert_ReF1("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 , p4 , p5 , p6, p7 , p8) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_PROXY_MSG(vecTargets);

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8); 

			INT32 nResult = m_pRpcManager->SendMsg(pName, pMsg);
			if (nResult == -1)
			{
				SAFE_DELETE(pMsg);
			}
			return nResult;
		}

		template<typename NameOrID>
		INT32 ProxySendMsg(NameOrID pName , Msg::Object objTarget ) 
		{
			std::vector<Msg::Object> vecTargets;
			vecTargets.push_back(objTarget);

			return  ProxySendMsg(pName , vecTargets ); 
		} 

		template<typename NameOrID , typename P1 >
		INT32 ProxySendMsg(NameOrID pName , Msg::Object objTarget , P1 &p1) 
		{
			std::vector<Msg::Object> vecTargets;
			vecTargets.push_back(objTarget);

			return  ProxySendMsg(pName , vecTargets  , p1); 
		}

		template<typename NameOrID , typename P1 , typename P2 >
		INT32 ProxySendMsg(NameOrID pName , Msg::Object objTarget , P1 &p1 , P2 &p2) 
		{
			std::vector<Msg::Object> vecTargets;
			vecTargets.push_back(objTarget);

			return  ProxySendMsg(pName , vecTargets  , p1 , p2 ); 
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3>
		INT32 ProxySendMsg(NameOrID pName , Msg::Object objTarget , P1 &p1 , P2 &p2 , P3 &p3) 
		{
			std::vector<Msg::Object> vecTargets;
			vecTargets.push_back(objTarget);

			return  ProxySendMsg(pName , vecTargets  , p1 , p2 , p3); 
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3 , typename P4>
		INT32 ProxySendMsg(NameOrID pName , Msg::Object objTarget , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4) 
		{
			std::vector<Msg::Object> vecTargets;
			vecTargets.push_back(objTarget);

			return  ProxySendMsg(pName , vecTargets  , p1 , p2 , p3 , p4 ); 
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3 , typename P4 , typename P5>
		INT32 ProxySendMsg(NameOrID pName , Msg::Object objTarget , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5) 
		{
			std::vector<Msg::Object> vecTargets;
			vecTargets.push_back(objTarget);

			return  ProxySendMsg(pName , vecTargets  , p1 , p2 , p3 , p4 , p5 ); 
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6>
		INT32 ProxySendMsg(NameOrID pName , Msg::Object objTarget , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6) 
		{
			std::vector<Msg::Object> vecTargets;
			vecTargets.push_back(objTarget);

			return  ProxySendMsg(pName , vecTargets  , p1 , p2 , p3 , p4 , p5 , p6 ); 
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7>
		INT32 ProxySendMsg(NameOrID pName , Msg::Object objTarget , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7) 
		{
			std::vector<Msg::Object> vecTargets;
			vecTargets.push_back(objTarget);

			return  ProxySendMsg(pName , vecTargets  , p1 , p2 , p3 , p4 , p5 , p6 , p7 ); 
		}

		template<typename NameOrID , typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8>
		INT32 ProxySendMsg(NameOrID pName , Msg::Object objTarget , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7 , P8 &p8) 
		{
			std::vector<Msg::Object> vecTargets;
			vecTargets.push_back(objTarget);

			return  ProxySendMsg(pName , vecTargets  , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8); 
		}

	public:   
		RPCMsgCall * RPCReturn(ERPCRETURN_TYPE type = RETURN_TYPE_DONE)
		{
			CHECK_RPC_RETURN_TYPE;
			Assert_Re0("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(FALSE , m_pRpcMsgCall->m_szMsgMethod) && "RPC返回参数类型或者数量不对.");

			RPC_GEN_RETURN_MSG;

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams);

			return pMsg;
		}

		template<class P1>
		RPCMsgCall * RPCReturn(P1 p1 , ERPCRETURN_TYPE type = RETURN_TYPE_DONE)
		{ 
			CHECK_RPC_RETURN_TYPE;
 			Assert_Re0("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(FALSE , m_pRpcMsgCall->m_szMsgMethod , p1) && "RPC返回参数类型或者数量不对.");

			RPC_GEN_RETURN_MSG;

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1);

			return pMsg;
		} 

		template<typename P1 , typename P2 >
		RPCMsgCall * RPCReturn(P1 &p1 , P2 &p2 , ERPCRETURN_TYPE type = RETURN_TYPE_DONE) 
		{
			CHECK_RPC_RETURN_TYPE;
			Assert_Re0("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(FALSE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2) && "RPC返回参数类型或者数量不对.");

			RPC_GEN_RETURN_MSG;

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2);

			return pMsg;
		}

		template<typename P1 , typename P2 , typename P3>
		RPCMsgCall * RPCReturn(P1 &p1 , P2 &p2 , P3 &p3 , ERPCRETURN_TYPE type = RETURN_TYPE_DONE) 
		{
			CHECK_RPC_RETURN_TYPE;
			Assert_Re0("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 ) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_RETURN_MSG;

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 );

			return pMsg;
		}

		template<typename P1 , typename P2 , typename P3 , typename P4>
		RPCMsgCall * RPCReturn(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , ERPCRETURN_TYPE type = RETURN_TYPE_DONE) 
		{
			CHECK_RPC_RETURN_TYPE;
			Assert_Re0("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 , p4) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_RETURN_MSG;

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4 );

			return pMsg;
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5>
		RPCMsgCall * RPCReturn(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , ERPCRETURN_TYPE type = RETURN_TYPE_DONE) 
		{
			CHECK_RPC_RETURN_TYPE;
			Assert_Re0("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 , p4 , p5) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_RETURN_MSG;

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 ,p4 , p5);

			return pMsg;
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6>
		RPCMsgCall * RPCReturn(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , ERPCRETURN_TYPE type = RETURN_TYPE_DONE) 
		{
			CHECK_RPC_RETURN_TYPE;
			Assert_Re0("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 , p4 , p5 , p6) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_RETURN_MSG;

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 ,p4 , p5 , p6); 

			return pMsg;
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7>
		RPCMsgCall * RPCReturn(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7 , ERPCRETURN_TYPE type = RETURN_TYPE_DONE) 
		{
			CHECK_RPC_RETURN_TYPE;
			Assert_Re0("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 , p4 , p5 , p6, p7) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_RETURN_MSG;

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 ,p4 , p5 , p6 , p7);

			return pMsg;
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8>
		RPCMsgCall * RPCReturn(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7 , P8 &p8 , ERPCRETURN_TYPE type = RETURN_TYPE_DONE) 
		{ 
			CHECK_RPC_RETURN_TYPE;
			Assert_Re0("msgPacket maybe NULL or Params is not match" && m_pRpcMsgCall && m_pRpcManager && Msg::RpcCheckParams::GetInstance() && Msg::RpcCheckParams::GetInstance()->CheckRpcParams(TRUE , m_pRpcMsgCall->m_szMsgMethod , p1 , p2 , p3 , p4 , p5 , p6, p7 , p8) && "RPC发送参数类型或者数量不对.");

			RPC_GEN_RETURN_MSG;

			CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 ,p4 , p5 , p6 , p7 , p8);

			return pMsg;	
		} 

	public: 
		RPCMsgCall *			GetRpcMsgCall() { return  m_pRpcMsgCall; } 
		void					SetRpcMsgCall(RPCMsgCall * pMsg) { Assert_(pMsg); m_pRpcMsgCall = pMsg; }
		INT32					GetSessionID() const { return m_nSessionID; }
		void					SetSessionID(INT32 val) { m_nSessionID = val; }

	private: 
		BOOL					CallObjectFunc( RPCMsgCall * pMsg , VecObjectMsgCallT & vecObjectMsgCall); 

	protected: 
		RPCMsgCall			*	m_pRpcMsgCall;
		RpcManager			*	m_pRpcManager;
		Timer::TimeCount		m_objTimeout;
		INT32					m_nSessionID;
		Net::ISession		*	m_pSession;
	}; 
	 
}


#endif