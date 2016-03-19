#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::SlaveStartAuth_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &name/* = std::string()*/ , std_string & pwd/* = std::string()*/  )
{
	std_string value = std::string();

	GameDB::Database * pDB = GetDBServer()->GetEnvironment()->GetDatabase(g_szSystemDatabase);
	if (!pDB)
	{ 
		Return(value);
	} 

	GameDB::Operate oper;
	GameDB::HashTable::HGet(*pDB , oper , GameDB::User::TableName() , name);
	if (!oper.IsSuccess())
	{
		gDebugStream("DB:" <<  g_szSystemDatabase << " table:" <<  g_szSystemDatabase << "name:" << name << "failure.");
		Return(value);
	}  
	 
	oper.GetOperateReturns().GetStream() >> value;
	if (value.length() <= 0)
	{
		gDebugStream("DB:" << g_szSystemDatabase << "table:" << g_szSystemDatabase << "key:" << name << "failure.");
		Return(std_string());
	} 

	GameDB::User objUser;
	objUser.FromBson(value.c_str() , (INT32)value.length()); 

	if(objUser.get_pswd() != pwd)
	{ 
		gDebugStream("DB:" << g_szSystemDatabase << "table:" << g_szSystemDatabase << "key:" << name << "password wrong.");
		Return(std_string()); 
	}
	 
	std::cout << "SlaveStartAuth_RpcServer "<< std::endl;
	Return(value);
}

