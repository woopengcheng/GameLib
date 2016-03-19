#include "ServerHandler.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleZDel_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/ , std_string & key/* = std::string()*/  )
{
	INT64 score = -1;

	GameDB::Database * pDB = this->GetDataBase();
	if (key.size() == 0|| table.size() == 0 || !pDB)
	{
		Return(score);
	} 

	GameDB::Operate oper;
	GameDB::SortedSet::ZDel(*pDB , oper , table , key);
	if (oper.IsSuccess())
	{
		rpc_SyncDataToSlave("tcp://127.0.0.1:9001" , 0 , GetObjectID() , m_strDatabaseName , oper.GetOperateRecord().GetData());

		oper.GetOperateReturns().GetStream() >> score ;
		gDebugStream("HDel: table:" << table  << "success.");
	}
	 
	gDebugStream("HandleZDel_RpcServer score"<< score);
	Return(score);
}

