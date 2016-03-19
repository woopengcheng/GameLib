#include "ServerHandler.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleHList_RpcServer(INT32 nSessionID , Msg::Object objSrc   )
{
	CUtilChunk res = CUtil::Chunk();

	if (!this->GetDataBase())
	{
		Return(res);
	}

	GameDB::Database * pDB = this->GetDataBase(); 

	GameDB::Operate oper;
	GameDB::HashTable::HList(*pDB , oper);
	if (oper.IsSuccess())
	{ 
		Return(oper.GetOperateReturns().GetStream().GetData());
	} 

	std::cout << "HandleHList_RpcServer "<< std::endl;
	Return(res);
}

