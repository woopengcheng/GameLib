#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHSet_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{ 
	if (res == 0)
	{
		gOtherStream("HSet data count:" << res);
	}
	else
	{
		gWarniStream("HSet wrong, res:" << res);
	} 

	std::cout << "HandleHSet_RpcClient resCode:" << res << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHSet_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , std_string & value/* = std::string()*/ , std_string & key/* = std::string()*/) 
{


	std::cout << "HandleHSet_RpcTimeout" << std::endl;
	ReturnNULL;
}

