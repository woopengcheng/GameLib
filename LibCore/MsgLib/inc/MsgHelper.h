#ifndef __msg_rpc_msg_helper_h__
#define __msg_rpc_msg_helper_h__
#include "MsgLib/inc/MsgCommon.h" 
#include "MsgLib/inc/RPCMsgCall.h" 
#include "CUtil/inc/ParameterHelper.h" 
#include "CUtil/inc/GenParamHelper.h" 
#include "CUtil/inc/CUtil.h"
#include "CUtil/inc/Chunk.h"
#include "MsgLib/inc/MsgHelper.h"

namespace Msg
{  
	
#define GEN_RPC_CALL_8(pSendInstance , nameOrID , rpcCallName , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8 ,vecTargets , objSrc , pCallback , usPriority , objSyncType , nTimeout ) \
	if (vecTargets.size() == 0)\
	{\
		Assert_ReF1(FALSE);\
	}\
	else\
	{ \
		Msg::RPCMsgCall * pMsg = new(sizeof(Msg::Object)*(UINT32)(vecTargets.size()))Msg::RPCMsgCall;  \
		\
		Msg::MsgHelper::GenMsgCall(*pMsg , FALSE , nTimeout, 0 , objSyncType);\
		Msg::MsgHelper::GenMsgCall(*pMsg , rpcCallName , vecTargets , objSrc , usPriority);\
		CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4 , p5 , p6 , p7);\
		pMsg->SetCallback(Msg::RpcCallbackPtr(pCallback));\
		\
		INT32 nResult = pSendInstance->SendMsg(nameOrID , pMsg );\
		if(nResult == -1)\
		{\
			SAFE_DELETE(pMsg); \
		}\
		return nResult;\
	}\
	\
	return -1;\
	 
#define GEN_RPC_CALL_7(pSendInstance , nameOrID , rpcCallName , p1 , p2 , p3 , p4 , p5 , p6 , p7 , vecTargets , objSrc , pCallback , usPriority , objSyncType , nTimeout ) \
	if (vecTargets.size() == 0)\
	{\
		Assert_ReF1(FALSE);\
	}\
	else\
	{ \
		Msg::RPCMsgCall * pMsg = new(sizeof(Msg::Object)*(UINT32)(vecTargets.size()))Msg::RPCMsgCall;  \
		\
		Msg::MsgHelper::GenMsgCall(*pMsg , FALSE , nTimeout, 0 , objSyncType);\
		Msg::MsgHelper::GenMsgCall(*pMsg , rpcCallName , vecTargets , objSrc , usPriority);\
		CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4 , p5 , p6 , p7);\
		pMsg->SetCallback(Msg::RpcCallbackPtr(pCallback));\
		\
		INT32 nResult = pSendInstance->SendMsg(nameOrID , pMsg );\
		if(nResult == -1)\
		{\
			SAFE_DELETE(pMsg); \
		}\
		return nResult;\
	}\
	\
	return -1;\
	 
#define GEN_RPC_CALL_6(pSendInstance , nameOrID , rpcCallName , p1 , p2 , p3 , p4 , p5 , p6 , vecTargets , objSrc , pCallback , usPriority , objSyncType , nTimeout ) \
	if (vecTargets.size() == 0)\
	{\
		Assert_ReF1(FALSE);\
	}\
	else\
	{ \
		Msg::RPCMsgCall * pMsg = new(sizeof(Msg::Object)*(UINT32)(vecTargets.size()))Msg::RPCMsgCall;  \
		\
		Msg::MsgHelper::GenMsgCall(*pMsg , FALSE , nTimeout, 0 , objSyncType);\
		Msg::MsgHelper::GenMsgCall(*pMsg , rpcCallName , vecTargets , objSrc , usPriority);\
		CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4 , p5 , p6 );\
		pMsg->SetCallback(Msg::RpcCallbackPtr(pCallback));\
		\
		INT32 nResult = pSendInstance->SendMsg(nameOrID , pMsg );\
		if(nResult == -1)\
		{\
			SAFE_DELETE(pMsg); \
		}\
		return nResult;\
	}\
	\
	return -1;\
	 
#define GEN_RPC_CALL_5(pSendInstance , nameOrID , rpcCallName , p1 , p2 , p3 , p4 , p5 , vecTargets , objSrc , pCallback , usPriority , objSyncType , nTimeout ) \
	if (vecTargets.size() == 0)\
	{\
		Assert_ReF1(FALSE);\
	}\
	else\
	{ \
		Msg::RPCMsgCall * pMsg = new(sizeof(Msg::Object)*(UINT32)(vecTargets.size()))Msg::RPCMsgCall;  \
		\
		Msg::MsgHelper::GenMsgCall(*pMsg , FALSE , nTimeout, 0 , objSyncType);\
		Msg::MsgHelper::GenMsgCall(*pMsg , rpcCallName , vecTargets , objSrc , usPriority);\
		CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4 , p5);\
		pMsg->SetCallback(Msg::RpcCallbackPtr(pCallback));\
		\
		INT32 nResult = pSendInstance->SendMsg(nameOrID , pMsg );\
		if(nResult == -1)\
		{\
			SAFE_DELETE(pMsg); \
		}\
		return nResult;\
	}\
	\
	return -1;\
	 
#define GEN_RPC_CALL_4(pSendInstance , nameOrID , rpcCallName , p1 , p2 , p3 , p4 , vecTargets , objSrc , pCallback , usPriority , objSyncType , nTimeout ) \
	if (vecTargets.size() == 0)\
	{\
		Assert_ReF1(FALSE);\
	}\
	else\
	{ \
		Msg::RPCMsgCall * pMsg = new(sizeof(Msg::Object)*(UINT32)(vecTargets.size()))Msg::RPCMsgCall;  \
		\
		Msg::MsgHelper::GenMsgCall(*pMsg , FALSE , nTimeout, 0 , objSyncType);\
		Msg::MsgHelper::GenMsgCall(*pMsg , rpcCallName , vecTargets , objSrc , usPriority);\
		CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4);\
		pMsg->SetCallback(Msg::RpcCallbackPtr(pCallback));\
		\
		INT32 nResult = pSendInstance->SendMsg(nameOrID , pMsg );\
		if(nResult == -1)\
		{\
			SAFE_DELETE(pMsg); \
		}\
		return nResult;\
	}\
	\
	return -1;\
	 
#define GEN_RPC_CALL_3(pSendInstance , nameOrID , rpcCallName , p1 , p2 , p3 , vecTargets , objSrc , pCallback , usPriority , objSyncType , nTimeout) \
	if (vecTargets.size() == 0)\
	{\
		Assert_ReF1(FALSE);\
	}\
	else\
	{ \
		Msg::RPCMsgCall * pMsg = new(sizeof(Msg::Object)*(UINT32)(vecTargets.size()))Msg::RPCMsgCall;  \
		\
		Msg::MsgHelper::GenMsgCall(*pMsg , FALSE , nTimeout, 0 , objSyncType);\
		Msg::MsgHelper::GenMsgCall(*pMsg , rpcCallName , vecTargets , objSrc , usPriority);\
		CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3);\
		pMsg->SetCallback(Msg::RpcCallbackPtr(pCallback));\
		\
		INT32 nResult = pSendInstance->SendMsg(nameOrID , pMsg );\
		if(nResult == -1)\
		{\
			SAFE_DELETE(pMsg); \
		}\
		return nResult;\
	}\
	\
	return -1;\
	 
#define GEN_RPC_CALL_2(pSendInstance , nameOrID , rpcCallName , p1 , p2 , vecTargets , objSrc , pCallback , usPriority , objSyncType , nTimeout) \
	if (vecTargets.size() == 0)\
	{\
		Assert_ReF1(FALSE);\
	}\
	else\
	{ \
		Msg::RPCMsgCall * pMsg = new(sizeof(Msg::Object)*(UINT32)(vecTargets.size()))Msg::RPCMsgCall;  \
		\
		Msg::MsgHelper::GenMsgCall(*pMsg , FALSE , nTimeout, 0 , objSyncType);\
		Msg::MsgHelper::GenMsgCall(*pMsg , rpcCallName , vecTargets , objSrc , usPriority);\
		CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2);\
		pMsg->SetCallback(Msg::RpcCallbackPtr(pCallback));\
		\
		INT32 nResult = pSendInstance->SendMsg(nameOrID , pMsg );\
		if(nResult == -1)\
		{\
			SAFE_DELETE(pMsg); \
		}\
		return nResult;\
	}\
	\
	return -1;\
	 
#define GEN_RPC_CALL_1(pSendInstance , nameOrID , rpcCallName , p1 , vecTargets , objSrc , pCallback , usPriority , objSyncType , nTimeout ) \
	if (vecTargets.size() == 0)\
	{\
		Assert_ReF1(FALSE);\
	}\
	else\
	{ \
		Msg::RPCMsgCall * pMsg = new(sizeof(Msg::Object)*(UINT32)(vecTargets.size()))Msg::RPCMsgCall;  \
		\
		Msg::MsgHelper::GenMsgCall(*pMsg , FALSE , nTimeout, 0 , objSyncType);\
		Msg::MsgHelper::GenMsgCall(*pMsg , rpcCallName , vecTargets , objSrc , usPriority);\
		CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1);\
		pMsg->SetCallback(Msg::RpcCallbackPtr(pCallback));\
		\
		INT32 nResult = pSendInstance->SendMsg(nameOrID , pMsg );\
		if(nResult == -1)\
		{\
			SAFE_DELETE(pMsg); \
		}\
		return nResult;\
	}\
	\
	return -1;\
	 
#define GEN_RPC_CALL_0(pSendInstance , nameOrID , rpcCallName , vecTargets , objSrc , pCallback , usPriority , objSyncType , nTimeout ) \
	if (vecTargets.size() == 0)\
	{\
		Assert_ReF1(FALSE);\
	}\
	else\
	{ \
		Msg::RPCMsgCall * pMsg = new(sizeof(Msg::Object)*(UINT32)(vecTargets.size()))Msg::RPCMsgCall;  \
		\
		Msg::MsgHelper::GenMsgCall(*pMsg , FALSE , nTimeout, 0 , objSyncType);\
		Msg::MsgHelper::GenMsgCall(*pMsg , rpcCallName , vecTargets , objSrc , usPriority);\
		CUtil::GenParamHelper::GenParams(pMsg->m_objParams);\
		pMsg->SetCallback(Msg::RpcCallbackPtr(pCallback));\
		\
		INT32 nResult = pSendInstance->SendMsg(nameOrID , pMsg );\
		if(nResult == -1)\
		{\
			SAFE_DELETE(pMsg); \
		}\
		return nResult;\
	}\
	\
	return -1;\
	 
	
#define GEN_MSG_CALL(rpcCallName , p1 , p2 , p3 , p4 , p5 , p6 , p7 , vecTargets , objSrc , usPriority) \
	if (vecTargets.size() == 0)\
	{\
		Assert_ReF(FALSE);\
	}\
	else\
	{ \
		Msg::ObjectMsgCall * pMsg = new(sizeof(Msg::Object)*(UINT32)(vecTargets.size()))Msg::ObjectMsgCall;  \
		\
		Msg::MsgHelper::GenMsgCall(*pMsg , rpcCallName , vecTargets , objSrc , usPriority);\
		CUtil::GenParamHelper::GenParams(pMsg->m_objParams , p1 , p2 , p3 , p4 , p5 , p6 , p7);\
		pMsg->SetCallback(Msg::RpcCallbackPtr(pCallback));\
		\
		INT32 nResult = pSendInstance->SendMsg(nameOrID , pMsg );\
		if(nResult == -1)\
		{\
			SAFE_DELETE(pMsg); \
		}\
		return nResult;\
	}\
	\
	return -1;\

	class DLL_EXPORT MsgHelper
	{
	public: 		
		static BOOL			GenMsgCall(RPCMsgCall & objMsgCall , BOOL bRecvRequest = FALSE , UINT64 ullTimeout = MAX_MSG_TIME_OUT , UINT64 ullMsgID = 0 , EMSG_SYNC_TYPE objSyncType = SYNC_TYPE_NONSYNC);
		static BOOL			GenMsgCall(ObjectMsgCall & objMsgCall , const char * pMsgCallName , UINT16 usPriority = 0);
		static BOOL			GenMsgCall(ObjectMsgCall & objMsgCall , const char * pMsgCallName , std::vector<Object> & vtTargets , Object objSrc , UINT16 usPriority = 0);

		static std::string	GenerateSuffixNodeName(const std::string & strNodeName);
		static BOOL			IsSimilarWithNodeName(const std::string & strNodeName, const std::string & strSuffixNodeName);
		static std::string	GetSuffixNodeName(const std::string & strNodeName);
		static std::string	ExchangeNodeName(const std::string & strNodeName);
		static std::string	GeneratePeerInfoKey(const std::string & strCurNodeName, const std::string & strRemoteNodeName);

	};

}
#endif