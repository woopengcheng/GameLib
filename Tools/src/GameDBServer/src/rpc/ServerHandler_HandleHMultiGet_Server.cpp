#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHMultiGet_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/ , CUtilChunk & keys/* = CUtil::Chunk()*/  )
{
	CUtilChunk res = 0;

	if (table.size() == 0 || !this->GetDataBase())
	{
		Return(res);
	}

	GameDB::Database * pDB = this->GetDataBase(); 

	GameDB::CollectionSlicesT vecKeys;
	std::vector<std::string> vecStrKeys;
	CUtil::CStream cs;
	cs << keys;
	cs >> vecStrKeys;
	std::vector<std::string>::iterator iter = vecStrKeys.begin();
	for (;iter != vecStrKeys.end();++iter)
	{
		vecKeys.push_back(*iter);
	}

	GameDB::Operate oper;
	GameDB::HashTable::HMultiGet(*pDB , oper , table ,vecKeys);
	if (oper.IsSuccess())
	{
		gDebugStream("HMultiDel table:" << table << "success."); 
		Return(oper.GetOperateReturns().GetStream().GetData());  
	} 
	 
	std::cout << "HandleHMultiGet_RpcServer "<< std::endl;
	Return(res);
}

