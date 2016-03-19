#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleZCount_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/  )
{ 
	INT64 res = -1;

	if (table.size() == 0 || !this->GetDataBase())
	{
		Return(res);
	}

	GameDB::Database * pDB = this->GetDataBase(); 

	GameDB::Operate oper;
	GameDB::SortedSet::ZCount(*pDB , oper , table);
	if (oper.IsSuccess())
	{ 
		oper.GetOperateReturns().GetStream() >> res;
		gDebugStream("table:" << table << "key:" << "success."); 
	}

	std::cout << "HandleZCount_RpcServer "<< std::endl;
	Return(res);
}

