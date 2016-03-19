#include "SlaveHandler.h"

Msg::ObjectMsgCall * Server::SlaveHandler::SyncMasterHandler_RpcServer(INT32 nSessionID , Msg::Object objSrc ,INT32 id/* = 0*/)
{
	INT32 res = 0;
	
	Server::DBSlave::GetInstance().SetMasterID(objSrc);
	std::cout << "SyncMasterHandler_RpcServer "<< std::endl;
	Return(res);
}

