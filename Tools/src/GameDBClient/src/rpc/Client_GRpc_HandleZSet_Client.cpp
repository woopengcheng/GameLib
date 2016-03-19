#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleZSet_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{
	if (res == 0)
	{
		gOtherStream("ZSet data res:" << res);
	}
	else
	{
		gWarniStream("ZSet wrong, res:" << res);
	} 


	std::cout << "HandleZSet_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleZSet_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , std_string & key/* = std::string()*/, INT64 score/* = 0*/ ) 
{


	std::cout << "HandleZSet_RpcTimeout" << std::endl;
	ReturnNULL;
}

