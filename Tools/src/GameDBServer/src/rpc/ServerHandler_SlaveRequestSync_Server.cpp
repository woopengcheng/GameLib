#include "DBServer.h"
#include "ServerHandler.h"
#include "GameDB/inc/Environment.h"
#include "Timer/inc/TimerHelp.h"


Msg::ObjectMsgCall * Server::ServerHandler::SlaveRequestSync_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string & dbname/* = std::string()*/)
{
	std_string newbackdir = std::string();
	INT32 res = 1;

	GameDB::Database * pDB = Server::DBServer::GetInstance().GetEnvironment()->GetDatabase(dbname);
	if (!pDB)
	{
		Return(newbackdir , res);
	}

	std::string strTimeStamp = Timer::GetDate("%Y%m%d%H%M%S");
	pDB->Backup(strTimeStamp , Server::DBServer::GetInstance().GetBackupDir());
	newbackdir = Server::DBServer::GetInstance().GetBackupDir() + strTimeStamp + "/" + dbname;

	res = 0;
	std::cout << "SlaveRequestSync_RpcServer "<< std::endl;
	Return(newbackdir , res);
}

