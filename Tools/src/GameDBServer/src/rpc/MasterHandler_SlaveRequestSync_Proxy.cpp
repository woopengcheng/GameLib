#include "MasterHandler.h"
#include "GameDB/inc/RemoteNodeDefine.h"

Msg::ObjectMsgCall * Server::MasterHandler::SlaveRequestSync_RpcServerProxy(INT32 nSessionID , Msg::Object objSrc , std_string & dbname/* = std::string()*/)
{
	std_string newbackdir = std::string();
	INT32 res = 0;
	 
	if(-1 == ProxySendMsg(g_strGameDBNodes[NETNODE_DBMASTER_TO_DBSERVER], 1 , dbname))
	{
		Return(newbackdir , res);
	}


	std::cout << "SlaveRequestSync_RpcServerProxy" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Server::MasterHandler::SlaveRequestSync_RpcClientProxy(INT32 nSessionID , Msg::Object objSrc , std_string &newbackdir/* = std::string()*/ , INT32 res/* = 0*/)
{
	if (res == 0)
	{
		StartSyncToSlave(newbackdir);
	}

	std::cout << "SlaveRequestSync_RpcClientProxy" << std::endl;
	Return(newbackdir , res);
}

Msg::ObjectMsgCall * Server::MasterHandler::SlaveRequestSync_RpcTimeoutProxy(INT32 nSessionID , Msg::Object objSrc , std_string & dbname/* = std::string()*/)
{


	std::cout << "SlaveRequestSync_RpcTimeoutProxy" << std::endl;
	ReturnNULL;
}

