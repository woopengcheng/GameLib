#include "MasterHandler.h"
#include "GameDB/inc/User.h"
#include "GameDB/inc/RemoteNodeDefine.h"

Msg::ObjectMsgCall * Server::MasterHandler::SlaveStartAuth_RpcServerProxy(INT32 nSessionID , Msg::Object objSrc , std_string &name/* = std::string()*/ , std_string & pwd/* = std::string()*/)
{
	std_string value = std::string();

	CreateSlaveRecord(objSrc);

	if(-1 == ProxySendMsg(g_strGameDBNodes[NETNODE_DBMASTER_TO_DBSERVER], 1 , name , pwd))
	{
		ReturnDelay;
	}
	
	std::cout << "SlaveStartAuth_RpcServerProxy" << std::endl;
	Return(value);
}

Msg::ObjectMsgCall * Server::MasterHandler::SlaveStartAuth_RpcClientProxy(INT32 nSessionID , Msg::Object objSrc  , std_string &value/* = std::string()*/)
{
	if (value != "")
	{
		GameDB::User objUser;
		objUser.FromBson(value.c_str() , (INT32)value.length()); 
		
		SetSlaveRecordInfo(m_pRpcMsgCall->GetProxySrcID() , objUser);
	} 
	else
	{
		DelSlaveRecord(m_pRpcMsgCall->GetProxySrcID());
	}

	std::cout << "SlaveStartAuth_RpcClientProxy" << std::endl;
	Return(value);
}

Msg::ObjectMsgCall * Server::MasterHandler::SlaveStartAuth_RpcTimeoutProxy(INT32 nSessionID , Msg::Object objSrc , std_string &name/* = std::string()*/ , std_string & pwd/* = std::string()*/)
{

 	std::cout << "SlaveStartAuth_RpcTimeoutProxy" << std::endl;
	ReturnNULL;
}

