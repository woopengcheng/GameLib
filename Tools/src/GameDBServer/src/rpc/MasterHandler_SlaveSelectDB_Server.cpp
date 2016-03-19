#include "MasterHandler.h"
#include "SlaveRecord.h"

Msg::ObjectMsgCall * Server::MasterHandler::SlaveSelectDB_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &dbname/* = std::string()*/  )
{
	INT32 res = 0; 

	SlaveRecord * pRecord = GetSlaveRecord(objSrc);
	if (pRecord && pRecord->GetDBName() == "")
	{
		pRecord->SetDBName(dbname);
	}
	else
	{
		res = -1;
	} 

	std::cout << "SlaveSelectDB_RpcServer " << res << std::endl;
	Return(res);
}

