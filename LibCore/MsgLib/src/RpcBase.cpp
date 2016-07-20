#include "MsgLib/inc/RpcBase.h"
#include "MsgLib/inc/RpcInterface.h"
#include "MsgLib/inc/RPCMsgCall.h"
#include "MsgLib/inc/RpcManager.h"
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "CUtil/inc/GenParamHelper.h"
#include "CUtil/inc/Parameters.h"

namespace Msg
{  
	INT32 Rpc::ProxySendBack()
	{
		MsgAssert_ReF(m_pRpcMsgCall , "rpc packet is null."); 

		RPCMsgCall * pRpcMsgCall = NULL;
		m_pRpcMsgCall->CopyTo(pRpcMsgCall);
		return m_pCurRpcManager->SendMsg(m_nSessionID , pRpcMsgCall);   
	}  

	BOOL Rpc::CallObjectFunc( RPCMsgCall * pMsg , VecObjectMsgCallT & vecObjectMsgCall)
	{
		Assert_Re0(pMsg && m_pCurRpcManager);

		std::string strMethod = pMsg->m_szMsgMethod;
		switch (pMsg->GetRpcMsgCallType())
		{
		case RPCTYPE_CLIENT:
			strMethod += RPCClient;
			break;
		case RPCTYPE_SERVER_PROXY:
			strMethod += RPCServerProxy;
			break;
		case RPCTYPE_CLIENT_PROXY:
			strMethod += RPCClientProxy;
			break;
		case RPCTYPE_SERVER:
			strMethod += RPCServer;
			break;
		case RPCTYPE_TIMEOUT:
			strMethod += RPCTimeout;
			break;
		default: 
			return FALSE;
		}
		MethodImpl * pMethodImpl = m_pCurRpcManager->GetMethodImpl(strMethod.c_str());
		MsgAssert_Re0(pMethodImpl , "wrong method" << strMethod);

		ParaseMsgCall objParaseMsgCall;
		objParaseMsgCall.m_pMehtodImpl = pMethodImpl;
		objParaseMsgCall.m_pMsgCall = pMsg;  
		objParaseMsgCall.m_pObj = this;
		objParaseMsgCall.m_nSessionID = m_nSessionID;

		IRpcMsgCallableObject * pICallableObject = NULL;

		if (pMsg->GetTargetsCount() != 0) 
		{ 
			for (UINT32 i = 0;i < pMsg->GetTargetsCount();++i)
			{
				pICallableObject = dynamic_cast<IRpcMsgCallableObject *>(m_pCurRpcManager->GetCallableObject(pMsg->m_aTargets[i]));

				MsgAssert_Re0(pICallableObject , "method:" << strMethod << "has no this objectID:" << pMsg->m_aTargets[i].m_llObjID);
				MsgAssert_Re0(pICallableObject->IsHasFunc(pMsg->m_szMsgMethod) , "this object has no this func.");
				if (pICallableObject)
				{ 
					pICallableObject->SetSessionID(m_nSessionID);
					pICallableObject->SetRpcMsgCall(pMsg); 
					objParaseMsgCall.m_pObj = pICallableObject;

					vecObjectMsgCall.push_back(pMethodImpl->m_pMethodImpl(&objParaseMsgCall));
				}  
			}   
		}    

		return TRUE;
	}


	BOOL Rpc::OnClient( RPCMsgCall * pMsg , VecObjectMsgCallT & vecObjectMsgCall )
	{  
		return  CallObjectFunc(pMsg , vecObjectMsgCall); 
	} 


	BOOL Rpc::OnProxy( RPCMsgCall * pMsg , VecObjectMsgCallT & vecObjectMsgCall )
	{
		return CallObjectFunc(pMsg , vecObjectMsgCall); 
	}


	BOOL Rpc::OnServer( RPCMsgCall * pMsg , VecObjectMsgCallT & vecObjectMsgCall )
	{  
		return CallObjectFunc(pMsg , vecObjectMsgCall);  
	}


	BOOL Rpc::OnTimeout( RPCMsgCall * pMsg , VecObjectMsgCallT & vecObjectMsgCall )
	{ 
		return CallObjectFunc(pMsg , vecObjectMsgCall);
	}


	void Rpc::SetTimeout( UINT64 unTimeout )
	{
		m_objTimeout.Start(unTimeout * 1000);
	}


	BOOL Rpc::IsTimeout()
	{
		return  m_objTimeout.IsExpired();
	}

	CUtil::Parameters * Rpc::GetInParams()
	{
		if (m_pRpcMsgCall && m_pCurRpcManager && m_pCurRpcManager->GetRpcInterface())
		{
			UINT64 ullMsgID = m_pRpcMsgCall->m_ullMsgID;
			Rpc * pRpc = m_pCurRpcManager->GetSendRpc(ullMsgID);
			if (pRpc)
			{
				return &(pRpc->GetRpcMsgCall()->m_objParams);
			}
		}

		return NULL;
	}

	RpcCallbackPtr Rpc::GetCallback()
	{
		if (m_pRpcMsgCall)
		{
			return m_pRpcMsgCall->GetCallback();
		}
		return NULL;
	}

	//#include "rpc.crpc"  //5 
}