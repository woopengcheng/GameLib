#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleZCount_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT64 res/* = 0*/) 
{

	if (res >= 0)
	{
		gOtherStream("ZCount:" << res);
	}
	else
	{
		gOtherStream("ZCount failure. res:" << res);
	}  


	std::cout << "HandleZCount_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleZCount_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/) 
{


	std::cout << "HandleZCount_RpcTimeout" << std::endl;
	ReturnNULL;
}

