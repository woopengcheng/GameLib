#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHMultiDel_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{
	if (res >= 0)
	{
		gOtherStream("HandleHMultiDel data count:" << res);
	}
	else
	{
		gWarniStream("HandleHMultiDel wrong, res:" << res);
	} 

	std::cout << "HandleHMultiDel_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHMultiDel_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , CUtilChunk & keys/* = CUtil::Chunk()*/) 
{


	std::cout << "HandleHMultiDel_RpcTimeout" << std::endl;
	ReturnNULL;
}

