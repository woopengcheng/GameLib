#include "ServerHandler.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleZDrop_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/  )
{
	INT32 res = -1;

	GameDB::Database * pDB = this->GetDataBase();
	if (table.size() == 0 || !pDB)
	{
		Return(res);
	} 

	GameDB::Operate oper;
	GameDB::SortedSet::ZDrop(*pDB , oper , table );
	if (oper.IsSuccess())
	{
		SyncDataToSlave(oper);

		oper.GetOperateReturns().GetStream() >> res;
		gDebugStream("HDel: table:" << table  << "success.");
	}

	gDebugStream("HandleZDrop_RpcServer "<< res);
	Return(res);
}

