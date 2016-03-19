#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleDeleteDatabase_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &dbname/* = std::string()*/  )
{
	INT32 res = -1;

	GameDB::Environment * pEnv = GetDBServer()->GetEnvironment();
	if (!pEnv)
	{ 
		Return(res);
	}  

	if (pEnv->GetDatabase(dbname) && m_objAuthInfo.CheckSysPermission())
	{
		res = pEnv->RemoveDatabase(dbname) - 1;
	} 

	gDebugStream( "HandleDeleteDatabase_RpcServer " << res);;
	Return(res);
}

