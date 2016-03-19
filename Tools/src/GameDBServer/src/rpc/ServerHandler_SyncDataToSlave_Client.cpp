#include "ServerHandler.h"



Msg::ObjectMsgCall * Server::ServerHandler::SyncDataToSlave_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/)
{


	std::cout << "SyncDataToSlave_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Server::ServerHandler::SyncDataToSlave_RpcTimeout(INT32 nSessionID , Msg::Object objSrc ,std_string & dbname/* = std::string()*/ , CUtilChunk & value/* = CUtil::Chunk()*/)
{


	std::cout << "SyncDataToSlave_RpcTimeout" << std::endl;
	ReturnNULL;
}

