#include "ServerHandler.h"
#include "GameDB/inc/DBCommon.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHMultiSet_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &table/* = std::string()*/ , CUtilChunk & keyvalues/* = CUtil::Chunk()*/  )
{
	INT32 res = -1;

	if (table.size() == 0 || !this->GetDataBase())
	{
		Return(res);
	}

	GameDB::Database * pDB = this->GetDataBase(); 

	GameDB::CollectionKeyValsT mapKeys;
	std::map<std::string , std::string> mapStrKeyValues;
	CUtil::CStream cs;
	cs << keyvalues;
	cs >> mapStrKeyValues;
	std::map<std::string , std::string>::iterator iter = mapStrKeyValues.begin();
	for (;iter != mapStrKeyValues.end();++iter)
	{
		std::string first = iter->first;
		GameDB::Slice second = iter->second;
		mapKeys.insert(std::pair<std::string , GameDB::Slice>(first , second));
	}

	GameDB::Operate oper;
	GameDB::HashTable::HMultiSet(*pDB , oper , table ,mapKeys);
	if (oper.IsSuccess())
	{
		rpc_SyncDataToSlave("tcp://127.0.0.1:9001" , 0 , GetObjectID() , m_strDatabaseName , oper.GetOperateRecord().GetData());

		res = 0;
		gDebugStream("HMultiDel table:" << table << "success."); 
	} 

	gDebugStream("HandleHMultiSet_RpcServer "<< res);
	Return(res);
}

