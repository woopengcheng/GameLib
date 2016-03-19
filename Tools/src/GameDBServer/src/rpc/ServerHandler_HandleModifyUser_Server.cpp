#include "ServerHandler.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleModifyUser_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &name/* = std::string()*/ , std_string & pwd/* = std::string()*/ , SINT8 issys/* = 0*/  )
{
	INT32 res = -1;

	GameDB::Database * pDB = GetDBServer()->GetEnvironment()->GetDatabase(g_szSystemDatabase);
	if (!pDB)
	{ 
		Return(res);
	} 

	GameDB::Operate oper;
	GameDB::HashTable::HGet(*pDB , oper , GameDB::User::TableName() , name);
	if (!oper.IsSuccess())
	{
		gDebugStream("DB:" <<  g_szSystemDatabase << " table:" <<  g_szSystemDatabase << "name:" << name << "failure.");
		Return(res);
	}  

	std::string value;
	oper.GetOperateReturns().GetStream() >> value;
	if (value.length() <= 0)
	{
		gDebugStream("DB:" << g_szSystemDatabase << "table:" << g_szSystemDatabase << "key:" << name << "auth failure.");
		Return(res);
	} 

	GameDB::User objUser;
	objUser.FromBson(value.c_str() , (INT32)value.length()); 
	objUser.set_pswd(pwd.c_str());
	objUser.set_sysuser(issys != 0);

	std::string strValue;
	objUser.ToBson(strValue);
	GameDB::HashTable::HSet(*pDB , oper , GameDB::User::TableName() , name , strValue);

	if (oper.IsSuccess())
	{
		rpc_SyncDataToSlave("tcp://127.0.0.1:9001" , 0 , GetObjectID() , m_strDatabaseName , oper.GetOperateRecord().GetData());

		res = 0;
	}

	gDebugStream("HandleModifyUser_RpcServer "<< res);
	Return(res);
}

