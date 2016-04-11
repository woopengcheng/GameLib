#include "ServerHandler.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHDel_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/ , std_string & key/* = std::string()*/  )
{
	INT32 res = -1;

	GameDB::Database * pDB = this->GetDataBase();
	if (key.size() == 0 || table.size() == 0 || !pDB)
	{
		Return(res);
	} 

	GameDB::Operate oper;
	GameDB::HashTable::HDel(*pDB , oper , table , key);
	if (oper.IsSuccess())
	{
		SyncDataToSlave(oper);

		res = 0;
		gDebugStream("HDel: table:" << table  << "success.");
	}

	gDebugStream("HandleHDel_RpcServer "<< res);
	Return(res);
}

