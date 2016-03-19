#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleCreateUser_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{

	if (res == 0)
	{
		gOtherStream("CreateUser success.");
	}
	else
	{
		gOtherStream("CreateUser failure.");
	}

	std::cout << "HandleCreateUser_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleCreateUser_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & name/* = std::string()*/ , std_string & pwd/* = std::string()*/,SINT8 issys/* = 0*/) 
{


	std::cout << "HandleCreateUser_RpcTimeout" << std::endl;
	ReturnNULL;
}

