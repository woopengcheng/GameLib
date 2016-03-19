#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleCreateDatabase_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{
	if (res == 0)
	{
		gOtherStream("createdb success.");
	}
	else
	{
		gOtherStream("createdb failure.");
	} 

	std::cout << "HandleCreateDatabase_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleCreateDatabase_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & dbname/* = std::string()*/) 
{


	std::cout << "HandleCreateDatabase_RpcTimeout" << std::endl;
	ReturnNULL;
}

