#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleShowDatabases_RpcServer(INT32 nSessionID , Msg::Object objSrc   )
{
	CUtilChunk res = CUtil::Chunk();

	GameDB::Environment * pEnv = GetDBServer()->GetEnvironment();
	if (!pEnv)
	{ 
		Return(res);
	} 

	std::vector<std::string> strAllDBS;
	pEnv->GetAllDatabase(strAllDBS); 
	
	CUtil::CStream cs;
	cs << (UINT32)strAllDBS.size();
	std::vector<std::string>::iterator iter = strAllDBS.begin();
	for (;iter != strAllDBS.end();++iter)
	{
		cs << *iter;
	}

	std::cout << "HandleShowDatabases_RpcServer "<< std::endl;
	Return(cs.GetData());
}

