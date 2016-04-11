#include "ServerHandler.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHSetIncr_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/ , std_string & key/* = std::string()*/ , INT64 value/* = 0*/  )
{
	INT64 res = -1;

	if (key.size() == 0 || table.size() == 0 || !this->GetDataBase())
	{
		Return(res);
	}

	GameDB::Database * pDB = this->GetDataBase(); 

	GameDB::Operate oper;
	GameDB::HashTable::HSetIncr(*pDB , oper , table , key , value);
	if (oper.IsSuccess())
	{
		SyncDataToSlave(oper);

		oper.GetOperateReturns().GetStream() >> res;
		gDebugStream("table:" << table << "key:" << key << "value:" << value << "success."); 
	}
	gDebugStream("HandleHSetIncr_RpcServer "<< res);
	Return(res);
}

