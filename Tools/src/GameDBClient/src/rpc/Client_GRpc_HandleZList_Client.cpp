#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleZList_RpcClient(INT32 nSessionID , Msg::Object objSrc , CUtilChunk & res/* = 0*/) 
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

	std::cout << "HandleZList_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleZList_RpcTimeout(INT32 nSessionID , Msg::Object objSrc ) 
{


	std::cout << "HandleZList_RpcTimeout" << std::endl;
	ReturnNULL;
}

