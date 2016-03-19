#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHSetIncr_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT64 res/* = 0*/) 
{ 
	if (res >= 0)
	{
		gOtherStream("HSetIncr data count:" << res);
	}
	else
	{
		gWarniStream("HSetIncr wrong, res:" << res);
	} 

	std::cout << "HandleHSetIncr_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHSetIncr_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , std_string & key/* = std::string()*/ , INT64 value/* = 0*/) 
{


	std::cout << "HandleHSetIncr_RpcTimeout" << std::endl;
	ReturnNULL;
}

