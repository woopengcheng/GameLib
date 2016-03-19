#include "ServerHandler.h"



Msg::ObjectMsgCall * Server::ServerHandler::testParamsAndRpcDatas_RpcServer(INT32 nSessionID , Msg::Object objSrc , TestRpcData & rpcData/* = TestRpcData()*/ , TestRpcData2 & rpcData2/* = TestRpcData2()*/ , std::vector<INT32> & Vec/* = std::vector<INT32>(1)*/)
{
	INT32 res = 100;

	static INT32 nCount = 100;
	while(nCount--)
	{
		gDebugStream("testParamsAndRpcDatas_RpcServer:count=" << nCount);
		ReturnDelay;
	}

	std::cout << "testParamsAndRpcDatas_RpcServer "<< std::endl;
	Return(res);
}

