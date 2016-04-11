#include "ServerHandler.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHDrop_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/  )
{
	INT64 res = -1;
	 
	GameDB::Database * pDB = this->GetDataBase();
	if (table.size() == 0 || !pDB)
	{
		Return(res);
	} 

	GameDB::Operate oper;
	GameDB::HashTable::HDrop(*pDB , oper , table);
	if (oper.IsSuccess())
	{
		SyncDataToSlave(oper);

		oper.GetOperateReturns().GetStream() >> res;
		gOtherStream("delete table:" << table  << "success.");
	}
	 
	gDebugStream("HandleHDrop_RpcServer "<< res);
	Return(res);
}

