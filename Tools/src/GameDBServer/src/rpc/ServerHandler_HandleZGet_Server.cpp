#include "ServerHandler.h"
#include "GameDB/inc/SortedSet.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleZGet_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/ , std_string & key/* = std::string()*/  )
{
	INT64 llScore = -1; 

	GameDB::Database * pDB = this->GetDataBase();
	if (key.size() == 0 || table.size() == 0 || !pDB)
	{
		Return(llScore);
	} 
	 
	GameDB::Operate oper;
	GameDB::SortedSet::ZGet(*pDB , oper , table , key);
	if (oper.IsSuccess())
	{
		oper.GetOperateReturns().GetStream() >> llScore;
		gDebugStream("table:" << table << "key:" << key << "success."); 
	} 

	gDebugStream("HandleZGet_RpcServer score:"<< llScore);
	Return(llScore);
}

