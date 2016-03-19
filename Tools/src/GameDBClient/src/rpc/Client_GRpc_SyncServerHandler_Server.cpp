#include "GRpc.h"
#include "DBClient.h"

Msg::ObjectMsgCall * Client::GRpc::SyncServerHandler_RpcServer(INT32 nSessionID, Msg::Object objSrc )
{
	INT32 res = 0;

	Client::DBClient::GetInstance().SetServerID(objSrc.m_llObjID);
	std::cout << "SyncServerHandler_RpcServer "<< std::endl;
	Return(res);
}

