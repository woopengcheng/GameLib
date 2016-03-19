#include "ServerHandler.h"
#include "GameDB/inc/HashTable.h"
#include "GameDB/inc/Environment.h"
#include "GameDB/inc/Database.h"
#include "GameDB/inc/Operate.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHGet_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/ , std_string & key/* = std::string()*/  )
{
	std_string value = std::string();

	if (key.size() == 0 || table.size() == 0 || !this->GetDataBase())
	{
		Return(value);
	}

	GameDB::Database * pDB = this->GetDataBase(); 
	
	GameDB::Operate oper;
	GameDB::HashTable::HGet(*pDB , oper , table , key);
	if (oper.IsSuccess())
	{
		gDebugStream("table:" << table << "key:" << key << "success."); 
		oper.GetOperateReturns().GetStream() >> value;
	} 

	std::cout << "HandleHGet_RpcServer "<< std::endl;
	Return(value);
}

