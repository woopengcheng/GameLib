#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHList_RpcClient(INT32 nSessionID , Msg::Object objSrc , CUtilChunk & res/* = CUtil::Chunk()*/) 
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


	std::cout << "HandleHList_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHList_RpcTimeout(INT32 nSessionID , Msg::Object objSrc) 
{


	std::cout << "HandleHList_RpcTimeout" << std::endl;
	ReturnNULL;
}

