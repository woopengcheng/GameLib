#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHMultiGet_RpcClient(INT32 nSessionID , Msg::Object objSrc , CUtilChunk & res/* = 0*/) 
{


	std::cout << "HandleHMultiGet_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHMultiGet_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , CUtilChunk & keys/* = CUtil::Chunk()*/) 
{


	std::cout << "HandleHMultiGet_RpcTimeout" << std::endl;
	ReturnNULL;
}

