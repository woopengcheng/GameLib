#include "GRpc.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Client::GRpc::HandleUserAuth_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{
	if (res == 0)
	{
		gDebugStream("HandleUserAuth_RpcClient success" );
	}
	else
	{ 
		gDebugStream("HandleUserAuth_RpcClient failure" );
	}

	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleUserAuth_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & name/* = std::string()*/ , std_string & pwd/* = std::string()*/) 
{


	std::cout << "HandleUserAuth_RpcTimeout" << std::endl;
	ReturnNULL;
}

