#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHDrop_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT64 res/* = 0*/) 
{
	if (res >= 0)
	{
		gOtherStream("HandleHDrop success: drop count:" << res);
	}
	else
	{
		gOtherStream("HandleHDrop failure. res:" << res);
	}   

	std::cout << "HandleHDrop_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHDrop_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/) 
{


	std::cout << "HandleHDrop_RpcTimeout" << std::endl;
	ReturnNULL;
}

