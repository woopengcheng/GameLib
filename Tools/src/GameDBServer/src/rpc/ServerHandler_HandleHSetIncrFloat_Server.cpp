#include "ServerHandler.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHSetIncrFloat_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/ , std_string & key/* = std::string()*/ , double value/* = 0.0f*/  )
{
	double res = 0.0f;

	if (key.size() == 0 || table.size() == 0 || !this->GetDataBase())
	{
		Return(res);
	}

	GameDB::Database * pDB = this->GetDataBase(); 

	GameDB::Operate oper;
	GameDB::HashTable::HSetIncrFloat(*pDB , oper , table , key , value);
	if (oper.IsSuccess())
	{
		rpc_SyncDataToSlave("tcp://127.0.0.1:9001" , 0 , GetObjectID() , m_strDatabaseName , oper.GetOperateRecord().GetData());

		oper.GetOperateReturns().GetStream() >> res;
		gDebugStream("table:" << table << "key:" << key << "value:" << value << "success."); 
	}

	gDebugStream("HandleHSetIncrFloat_RpcServer "<< res);
	Return(res);
}

