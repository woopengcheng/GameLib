#include "ServerHandler.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleDeleteUser_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &name/* = std::string()*/  )
{
	INT32 res = -1; 

	GameDB::Database * pDB = this->GetDataBase();
	if (name.size() == 0 || !pDB || !pDB->IsSystemDatabase())
	{
		Return(res);
	} 

	GameDB::Operate oper;
	GameDB::HashTable::HDel(*pDB , oper , GameDB::User::TableName() , name);
	if (oper.IsSuccess())
	{
		rpc_SyncDataToSlave("tcp://127.0.0.1:9001" , 0 , GetObjectID() , m_strDatabaseName , oper.GetOperateRecord().GetData());

		res = 0;
		gDebugStream("delete user: name:" << name  << "success.");
	}

	gDebugStream("HandleDeleteUser_RpcServer "<< res);
	Return(res);
}

