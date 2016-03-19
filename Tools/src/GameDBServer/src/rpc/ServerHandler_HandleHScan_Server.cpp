#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHScan_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/ , std_string & start/* = std::string()*/ ,std_string & regexp/* = std::string()*/ , INT64 limit/* = 10*/  )
{
	CUtilChunk res = CUtil::Chunk();

	if (table.size() == 0 || !this->GetDataBase())
	{
		Return(res);
	}

	GameDB::Database * pDB = this->GetDataBase(); 

	GameDB::Operate oper;
	GameDB::HashTable::HScan(*pDB , oper , table , start , regexp , limit , GameDB::HashTable::GET_ALL_TYPE_KEYS);
	if (oper.IsSuccess())
	{
		gDebugStream("HGetKeys table:" << table << "success."); 
		Return(oper.GetOperateReturns().GetStream().GetData());
	} 
	 
	std::cout << "HandleHScan_RpcServer "<< std::endl;
	Return(res);
}

