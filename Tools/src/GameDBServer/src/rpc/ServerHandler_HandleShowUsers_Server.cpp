#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleShowUsers_RpcServer(INT32 nSessionID , Msg::Object objSrc )
{
	CUtilChunk res = CUtil::Chunk();

	GameDB::Database * pDB = GetDBServer()->GetEnvironment()->GetDatabase(g_szSystemDatabase);
	if (!pDB)
	{
		Return(res);
	}

	//5 ºÏ≤È»®œﬁ
	if (!m_objAuthInfo.CheckSysPermission())
	{
		Return(res);
	}
	
	std::string strTable = GameDB::User::TableName();
	GameDB::Operate oper;
	GameDB::HashTable::HGetKeys(*pDB , oper , strTable);
	if (oper.IsSuccess())
	{  
		gDebugStream("HGetKeys table:" << strTable << "success."); 
		Return(oper.GetOperateReturns().GetStream().GetData()); 
	} 
	 
	std::cout << "HandleShowUsers_RpcServer "<< std::endl;
	Return(res);
}

