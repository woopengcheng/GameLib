#include "SlaveHandler.h"



Msg::ObjectMsgCall * Server::SlaveHandler::SlaveStartAuth_RpcClient(INT32 nSessionID , Msg::Object objSrc , std_string & value/* = std::string()*/)
{
	if (value != "")
	{
		this->RequestSyncData();
//		this->SelectDB();//5  不需要选择了.
	}

	std::cout << "SlaveStartAuth_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Server::SlaveHandler::SlaveStartAuth_RpcTimeout(INT32 nSessionID , Msg::Object objSrc ,std_string & name/* = std::string()*/ , std_string & pwd/* = std::string()*/)
{


	std::cout << "SlaveStartAuth_RpcTimeout" << std::endl;
	ReturnNULL;
}

