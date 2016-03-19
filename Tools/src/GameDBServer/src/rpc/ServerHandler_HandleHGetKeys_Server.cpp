#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHGetKeys_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/  )
{
	CUtilChunk res = CUtil::Chunk();

	if (table.size() == 0 || !this->GetDataBase())
	{
		Return(res);
	}

	GameDB::Database * pDB = this->GetDataBase(); 

	GameDB::Operate oper;
	GameDB::HashTable::HGetKeys(*pDB , oper , table);
	if (oper.IsSuccess())
	{
		gDebugStream("HGetKeys table:" << table << "success."); 
		Return(oper.GetOperateReturns().GetStream().GetData()); 
	} 
  
	std::cout << "HandleHGetKeys_RpcServer "<< std::endl;
	Return(res);
}

