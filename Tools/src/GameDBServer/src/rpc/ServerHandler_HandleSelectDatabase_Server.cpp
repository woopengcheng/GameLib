#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleSelectDatabase_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &dbname/* = std::string()*/  )
{
	INT32 res = -1; 

	if(!m_objAuthInfo.CheckSysPermission() && dbname == g_szSystemDatabase && m_pDBServer && m_pDBServer->GetEnvironment() && m_pDBServer->GetEnvironment()->GetDatabase(dbname))
	{
		res = -1;
	}
	else
	{
		res = 0;
		m_strDatabaseName = dbname;
	}

	gDebugStream("HandleSelectDatabase_RpcServer "<< res);
	Return(res);
}

