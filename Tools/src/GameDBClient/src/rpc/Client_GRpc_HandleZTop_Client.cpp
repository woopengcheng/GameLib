#include "GRpc.h"

Msg::ObjectMsgCall * Client::GRpc::HandleZTop_RpcClient(INT32 nSessionID , Msg::Object objSrc , CUtilChunk & res/* = 0*/) 
{
	CUtil::CStream objCS;
	objCS.Pushback(res.Begin() , res.GetDataLen());
	
	INT64 llCount = 0;
	objCS >> llCount;

	INT64 llScore = 0;
	for (INT64 i = 0;i < llCount;++i)
	{
		std::string str;
		objCS >> str >> llScore;

		gDebugStream("ztop" << str << "score:" << llScore);
	}

	std::cout << "HandleZTop_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleZTop_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/ , INT64 start/* = 0*/ , INT64 end/* = 0*/ , INT64 limit/* = 0*/) 
{


	std::cout << "HandleZTop_RpcTimeout" << std::endl;
	ReturnNULL;
}

