#include "MasterHandler.h"
#include "GameDB/inc/RemoteNodeDefine.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::MasterHandler::SlaveRequestSync_RpcServerProxy(INT32 nSessionID , Msg::Object objSrc , std_string & dbname/* = std::string()*/, INT32 last_pos/* = -1*/)
{
	std_string newbackdir = std::string();
	INT32 res = -1;
	 
	if (last_pos < 0)
	{
		if (-1 == ProxySendMsg(g_strGameDBNodes[NETNODE_DBMASTER_TO_DBSERVER], Server::DBMaster::GetInstance().GetServerID(), dbname , last_pos))
		{
			Return(newbackdir, res);
		}
	}
	else
	{
		Server::VecSaveCachesT  vec;
		Server::DBMaster::GetInstance().GetSaveCache(dbname , objSrc.m_llObjID , nSessionID , last_pos , vec);
		for (Server::VecSaveCachesT::iterator iter = vec.begin(); iter != vec.end(); ++iter)
		{
			rpc_SyncDataToSlave(nSessionID, 0, GetObjectID(), dbname , *iter , last_pos + 1);
		}
		Return(newbackdir, res);
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

Msg::ObjectMsgCall * Server::MasterHandler::SlaveRequestSync_RpcTimeoutProxy(INT32 nSessionID , Msg::Object objSrc , std_string & dbname/* = std::string()*/, INT32 last_pos/* = -1*/)
{


	std::cout << "SlaveRequestSync_RpcTimeoutProxy" << std::endl;
	ReturnNULL;
}

