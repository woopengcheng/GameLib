#include "ServerHandler.h"
#include "GameDB/inc/HashTable.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHMultiDel_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/ , CUtilChunk & keys/* = CUtil::Chunk()*/  )
{
	INT32 res = -1;

	if (table.size() == 0 || !this->GetDataBase())
	{
		Return(res);
	}

	GameDB::Database * pDB = this->GetDataBase(); 

	GameDB::CollectionSlicesT vecKeys;
	std::vector<std::string> vecStrKeys;
	CUtil::CStream cs;
	cs << keys;
	cs >> vecStrKeys;
	std::vector<std::string>::iterator iter = vecStrKeys.begin();
	for (;iter != vecStrKeys.end();++iter)
	{
		vecKeys.push_back(*iter);
	}

	GameDB::Operate oper;
	GameDB::HashTable::HMultiDel(*pDB , oper , table ,vecKeys);
	if (oper.IsSuccess())
	{
		rpc_SyncDataToSlave("tcp://127.0.0.1:9001" , 0 , GetObjectID() , m_strDatabaseName , oper.GetOperateRecord().GetData());

		gDebugStream("HMultiDel table:" << table << "success.");  
		oper.GetOperateReturns().GetStream() >> res; 
	} 

	gDebugStream("HandleHMultiDel_RpcServer "<< res);
	Return(res);
}

