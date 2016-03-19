#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleDeleteUser_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{
	if (res == 0)
	{
		gOtherStream("DeleteUser success.");
	}
	else
	{
		gOtherStream("DeleteUser failure. res:" << res);
	}  

	std::cout << "HandleDeleteUser_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleDeleteUser_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & name/* = std::string()*/) 
{


	std::cout << "HandleDeleteUser_RpcTimeout" << std::endl;
	ReturnNULL;
}

