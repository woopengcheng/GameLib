#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHCount_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/  )
{
	INT64 res = -1;

	if (table.size() == 0 || !this->GetDataBase())
	{
		Return(res);
	}

	GameDB::Database * pDB = this->GetDataBase(); 

	GameDB::Operate oper;
	GameDB::HashTable::HCount(*pDB , oper , table);
	if (oper.IsSuccess())
	{ 
		oper.GetOperateReturns().GetStream() >> res;
		gDebugStream("table:" << table << "key:" << "success."); 
	}

	gDebugStream("HandleHCount_RpcServer "<< res );
	Return(res);
}

