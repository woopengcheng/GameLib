#include "GRpc.h"
#include "DBClientHttpServer.h"

Msg::ObjectMsgCall * Client::GRpc::HandleShowDatabases_RpcClient(INT32 nSessionID , Msg::Object objSrc , CUtilChunk & res/* = CUtil::Chunk()*/) 
{
	CUtil::CStream cs((const char*)res.Begin() , res.GetDataLen());

	UINT32 unCount = 0;
	cs >> unCount;

	MsgAssert_Re0(unCount < 1000000 && unCount >= 0 , "showdatabases wrong count.");

	g_strHtmlResult = "";
	std::string strDBName;
	while(unCount--)
	{
		strDBName.clear();
		cs >> strDBName;

		g_strHtmlResult += "DBName=";
		g_strHtmlResult += strDBName;
		g_strHtmlResult += "\n";

		gOtherStream("num:" << unCount << " dbname: " << strDBName);
	}


	std::cout << "HandleShowDatabases_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleShowDatabases_RpcTimeout(INT32 nSessionID , Msg::Object objSrc) 
{


	std::cout << "HandleShowDatabases_RpcTimeout" << std::endl;
	ReturnNULL;
}

