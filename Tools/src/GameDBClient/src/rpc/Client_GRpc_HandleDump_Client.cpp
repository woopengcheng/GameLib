#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleDump_RpcClient(INT32 nSessionID , Msg::Object objSrc , CUtilChunk & res/* = 0*/) 
{
	std::string strKey, strVal;
	 
	CUtil::CStream objStream;
	objStream.Pushback(res.Begin() , res.GetDataLen());
	if (res.GetDataLen() == 0)
	{
		ReturnNULL;
	}

	INT32 nCount = 0; 
	objStream >> nCount;
	for (int i = 0 ;i < nCount;++i)
	{
		strKey = "";
		strVal = "";
		objStream >> strKey >> strVal;

		std::cout << strKey << strVal << std::endl;
	}

	std::cout << "HandleDump_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleDump_RpcTimeout(INT32 nSessionID , Msg::Object objSrc) 
{


	std::cout << "HandleDump_RpcTimeout" << std::endl;
	ReturnNULL;
}

