#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHMultiSet_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/) 
{ 
	if (res >= 0)
	{
		gOtherStream("MultiSet data count:" << res);
	}
	else
	{
		gWarniStream("MultiSet wrong, res:" << res);
	} 

	std::cout << "HandleHMultiSet_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHMultiSet_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , CUtilChunk & keyvalues/* = CUtil::Chunk()*/) 
{


	std::cout << "HandleHMultiSet_RpcTimeout" << std::endl;
	ReturnNULL;
}

