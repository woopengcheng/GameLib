#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleSelectDatabase_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{
	if (res == 0)
	{
		gOtherStream("selectdb success.");
	}
	else
	{
		gOtherStream("selectdb failure.");
	}

	std::cout << "HandleSelectDatabase_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleSelectDatabase_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & dbname/* = std::string()*/) 
{


	std::cout << "HandleSelectDatabase_RpcTimeout" << std::endl;
	ReturnNULL;
}

