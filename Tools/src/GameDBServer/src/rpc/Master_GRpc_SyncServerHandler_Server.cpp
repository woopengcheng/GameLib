#include "GRpc.h"
#include "DBMaster.h"

Msg::ObjectMsgCall * Server::GRpc::SyncServerHandler_RpcServer(INT32 nSessionID, Msg::Object objSrc )
{
	INT32 res = 0;
	Server::DBMaster::GetInstance().SetServerID(objSrc.m_llObjID);


	std::cout << "SyncServerHandler_RpcServer "<< std::endl;
	Return(res);
}

