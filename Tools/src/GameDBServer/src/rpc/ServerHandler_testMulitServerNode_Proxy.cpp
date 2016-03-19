#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::testMulitServerNode_RpcServerProxy(INT32 nSessionID, Msg::Object objSrc , std_string & dbname/* = std::string()*/ , CUtilChunk & value/* = CUtil::Chunk()*/ , int value2/* = 0*/ , unsigned int value22/* = 0*/ , char valChar/* = char(0)*/)
{
	CUtilChunk res = CUtil::Chunk();


	if(-1 == ProxySendMsg("tcp://127.0.0.1:8002" , 0 , dbname , value , value2 , value22 , valChar))
	{
		Return(res);
	}


	std::cout << "ServerHandler::testMulitServerNode_RpcServerProxy" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Server::ServerHandler::testMulitServerNode_RpcClientProxy(INT32 nSessionID, Msg::Object objSrc  ,CUtilChunk & res/* = CUtil::Chunk()*/)
{




	std::cout << "ServerHandler::testMulitServerNode_RpcClientProxy" << std::endl;
	Return(res);
}

Msg::ObjectMsgCall * Server::ServerHandler::testMulitServerNode_RpcTimeoutProxy(INT32 nSessionID, Msg::Object objSrc,std_string & dbname/* = std::string()*/ , CUtilChunk & value/* = CUtil::Chunk()*/ , int value2/* = 0*/ , unsigned int value22/* = 0*/ , char valChar/* = char(0)*/ )
{


 	std::cout << "ServerHandler::testMulitServerNode_RpcTimeoutProxy" << std::endl;
	ReturnNULL;
}

