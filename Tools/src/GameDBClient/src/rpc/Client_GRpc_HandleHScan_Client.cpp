#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHScan_RpcClient(INT32 nSessionID , Msg::Object objSrc , CUtilChunk & res/* = CUtil::Chunk()*/) 
{

	if (res.GetDataLen() != 0)
	{
		CUtil::CStream cs;
		cs.Pushback(res.Begin() , res.GetDataLen());

		INT32 nCount = 0;
		cs >> nCount;

		std::string strKey;
		for (INT32 i = 0;i < nCount;++i)
		{
			cs >> strKey;
			gOtherStream("i:" << i << "key:" << strKey);
		}
	}
	else
	{
		gWarniStream("get error.");
	}


	std::cout << "HandleHScan_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHScan_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , std_string & start/* = std::string()*/ , std_string & regexp/* = std::string()*/ , INT64 limit/* = 10*/) 
{


	std::cout << "HandleHScan_RpcTimeout" << std::endl;
	ReturnNULL;
}

