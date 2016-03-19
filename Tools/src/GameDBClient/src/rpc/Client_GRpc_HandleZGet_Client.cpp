#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleZGet_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT64 score/* = 0*/) 
{
	Msg::RPCMsgCall * pMsg = GetRpcMsgCall();
	if (pMsg)
	{
		gDebugStream("zget table:" << pMsg->m_objParams.GetValue<std::string>(0) << "key:" << pMsg->m_objParams.GetValue<std::string>(1) << "score:" << score);
	}

	std::cout << "HandleZGet_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleZGet_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , std_string & key/* = std::string()*/) 
{


	std::cout << "HandleZGet_RpcTimeout" << std::endl;
	ReturnNULL;
}

