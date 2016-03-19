#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleDump_RpcServer(INT32 nSessionID , Msg::Object objSrc   )
{
	CUtilChunk res;

	GameDB::Database * pDB = GetDataBase();
	if (!pDB)
	{
		Return(res);
	}

	int nCount = 0;
	GameDB::Operate oper;
	GameDB::Iterator * iter = pDB->GetLevelDB()->NewIterator(GameDB::ReadOptions());
	iter->SeekToFirst();
	while(iter->Valid())
	{
		GameDB::Slice key = iter->key();
		GameDB::Slice val = iter->value();
		 
		oper.GetOperateReturns().GetStream() << key.ToString() << val.ToString();

		++nCount;
		iter->Next();
	} 
	delete iter;  
	  
	oper.GetOperateReturns().GetStream().Insert(oper.GetOperateReturns().GetStream().Begin() , &nCount , sizeof(nCount));

 	std::cout << "HandleDump_RpcServer "<< std::endl;
	Return(oper.GetOperateReturns().GetStream().GetData());
}

