#include "SlaveHandler.h"



Msg::ObjectMsgCall * Server::SlaveHandler::SlaveRequestSync_RpcClient(INT32 nSessionID , Msg::Object objSrc , std_string & newbackdir/* = std::string()*/ , INT32 res/* = 0*/)
{


	std::cout << "SlaveRequestSync_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Server::SlaveHandler::SlaveRequestSync_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & dbname/* = std::string()*/)
{


	std::cout << "SlaveRequestSync_RpcTimeout" << std::endl;
	ReturnNULL;
}

