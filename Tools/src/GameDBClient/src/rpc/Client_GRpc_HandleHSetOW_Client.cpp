#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHSetOW_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{ 
	if (res == 0)
	{
		gOtherStream("HSetOW data count:" << res);
	}
	else
	{
		gWarniStream("HSetOW wrong, res:" << res);
	} 


	std::cout << "HandleHSetOW_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHSetOW_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , std_string & key/* = std::string()*/ , std_string & value/* = std::string()*/) 
{


	std::cout << "HandleHSetOW_RpcTimeout" << std::endl;
	ReturnNULL;
}

