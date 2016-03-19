#include "ServerHandler.h"
#include "GameDB/inc/SortedSet.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleZSet_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table  , std_string & key , INT64 score)
{
	INT32 res = -1; 

	GameDB::Database * pDB = this->GetDataBase();
	if (key.size() == 0 || table.size() == 0 || !pDB)
	{
		Return(res);
	}  

	GameDB::Operate oper;
	GameDB::SortedSet::ZSet(*pDB , oper , table , key , score);
	if (oper.IsSuccess())
	{
		rpc_SyncDataToSlave("tcp://127.0.0.1:9001" , 0 , GetObjectID() , m_strDatabaseName , oper.GetOperateRecord().GetData());

		gDebugStream("table:" << table << "key:" << key << "score:" << score << "success.");
		Return(0);
	} 

	gDebugStream("HandleZSet_RpcServer "<< res);
	Return(res);
}

