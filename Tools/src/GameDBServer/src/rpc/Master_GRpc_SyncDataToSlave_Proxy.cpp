#include "GRpc.h"
#include "MasterHandler.h"


Msg::ObjectMsgCall * Server::GRpc::SyncDataToSlave_RpcServerProxy(INT32 nSessionID , Msg::Object objSrc  ,std_string & dbname/* = std::string()*/ , CUtilChunk & value/* = CUtil::Chunk()*/, INT32 last_pos/* = -1*/)
{
	INT32 res = 0;

	Server::DBMaster::VecSlaveRecordsT vecRecords;
	bool bFound = Server::DBMaster::GetInstance().GetSlaveRecord(dbname , vecRecords);
	if (bFound)
	{
		INT32 nPos = Server::DBMaster::GetInstance().InsertSaveCache(dbname, value);
		Server::DBMaster::VecSlaveRecordsT::iterator iter = vecRecords.begin();
		for (;iter != vecRecords.end();++iter)
		{
			Server::SlaveRecord * pSlaveRecord = *iter;
			if (pSlaveRecord  && -1 == ProxySendMsg(pSlaveRecord->GetSlaveSessionID(), pSlaveRecord->GetSlaveID(), dbname, value , nPos))
			{
				continue;
			}
		}
	}

	std::cout << "SyncDataToSlave_RpcServerProxy" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Server::GRpc::SyncDataToSlave_RpcClientProxy(INT32 nSessionID , Msg::Object objSrc  ,INT32 res/* = 0*/ , INT32 master_id)
{
	std::string dbname;
	CUtil::Parameters * pParams = GetInParams();
	if (pParams)
	{
		dbname = pParams->GetValue<std::string>(0);
		INT32 last_pos = pParams->GetValue<INT32>(2);

		Server::SlaveRecord * pSlaveRecord = Server::DBMaster::GetInstance().GetSlaveRecord(master_id , dbname);
		if (pSlaveRecord && res == 0)
		{
			pSlaveRecord->SetSaveCachePos(last_pos);
		}
		Server::DBMaster::GetInstance().RefreshSaveCache(dbname);
	}
	
	std::cout << "SyncDataToSlave_RpcClientProxy" << std::endl;
	Return(res , res);
}

Msg::ObjectMsgCall * Server::GRpc::SyncDataToSlave_RpcTimeoutProxy(INT32 nSessionID , Msg::Object objSrc,std_string & dbname/* = std::string()*/ , CUtilChunk & value/* = CUtil::Chunk()*/, INT32 last_pos/* = -1*/)
{


 	std::cout << "SyncDataToSlave_RpcTimeoutProxy" << std::endl;
	ReturnNULL;
}

