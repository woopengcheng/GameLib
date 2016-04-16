#include "GRpc.h"
#include "MasterHandler.h"


Msg::ObjectMsgCall * Server::GRpc::SyncDataToSlave_RpcServerProxy(INT32 nSessionID , Msg::Object objSrc  ,std_string & dbname/* = std::string()*/ , CUtilChunk & value/* = CUtil::Chunk()*/)
{
	INT32 res = 0;

	Server::DBMaster::VecSlaveRecordsT vecRecords;
	bool bFound = Server::DBMaster::GetInstance().GetSlaveRecord(dbname , vecRecords);
	if (bFound)
	{
		Server::DBMaster::VecSlaveRecordsT::iterator iter = vecRecords.begin();
		for (;iter != vecRecords.end();++iter)
		{
			Server::SlaveRecord * pSlaveRecord = *iter;
			if (pSlaveRecord  && -1 == ProxySendMsg(pSlaveRecord->GetSlaveSessionID(), pSlaveRecord->GetSlaveID(), dbname, value))
			{
				continue;
			}
		}
	}

	std::cout << "SyncDataToSlave_RpcServerProxy" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Server::GRpc::SyncDataToSlave_RpcClientProxy(INT32 nSessionID , Msg::Object objSrc  ,INT32 res/* = 0*/)
{




	std::cout << "SyncDataToSlave_RpcClientProxy" << std::endl;
	Return(res);
}

Msg::ObjectMsgCall * Server::GRpc::SyncDataToSlave_RpcTimeoutProxy(INT32 nSessionID , Msg::Object objSrc,std_string & dbname/* = std::string()*/ , CUtilChunk & value/* = CUtil::Chunk()*/ )
{


 	std::cout << "SyncDataToSlave_RpcTimeoutProxy" << std::endl;
	ReturnNULL;
}

