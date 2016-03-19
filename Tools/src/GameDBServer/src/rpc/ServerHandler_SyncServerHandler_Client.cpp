#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::SyncServerHandler_RpcClient(INT32 nSessionID, Msg::Object objSrc , INT32 res/* = 0*/)
{


	std::cout << "SyncServerHandler_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Server::ServerHandler::SyncServerHandler_RpcTimeout(INT32 nSessionID, Msg::Object objSrc )
{


	std::cout << "SyncServerHandler_RpcTimeout" << std::endl;
	ReturnNULL;
}

