#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHDel_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{ 
	if (res == 0)
	{
		gOtherStream("HandleHDel success");
	}
	else
	{
		gOtherStream("HandleHDel failure. res:" << res);
	}  


	std::cout << "HandleHDel_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHDel_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , std_string & key/* = std::string()*/) 
{


	std::cout << "HandleHDel_RpcTimeout" << std::endl;
	ReturnNULL;
}

