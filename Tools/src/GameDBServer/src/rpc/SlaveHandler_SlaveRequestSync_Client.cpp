#include "SlaveHandler.h"



Msg::ObjectMsgCall * Server::SlaveHandler::SlaveRequestSync_RpcClient(INT32 nSessionID , Msg::Object objSrc , std_string & newbackdir/* = std::string()*/ , INT32 res/* = 0*/)
{
	if (res < 0)  //5 如果请求失败.请求全部更新.
	{
		RequestSyncData(TRUE);
	}

	std::cout << "SlaveRequestSync_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Server::SlaveHandler::SlaveRequestSync_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & dbname/* = std::string()*/ , INT32 last_pos)
{


	std::cout << "SlaveRequestSync_RpcTimeout" << std::endl;
	ReturnNULL;
}

