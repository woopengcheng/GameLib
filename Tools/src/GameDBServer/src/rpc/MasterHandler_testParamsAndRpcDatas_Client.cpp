#include "MasterHandler.h"

Msg::ObjectMsgCall * Server::MasterHandler::testParamsAndRpcDatas_RpcClient(INT32 nSessionID , Msg::Object objSrc , INT32 res/* = 0*/)
{
	CUtil::Parameters * pParams = GetInParams();
	if (pParams)
	{
		TestRpcData   p1 = pParams->GetValue<TestRpcData>(0);
		TestRpcData2  p2 = pParams->GetValue<TestRpcData2>(1);
		std::vector<INT32> &  Vec = pParams->GetValue<std::vector<INT32>>(2);
		std::vector<INT32> Vec2;
		std::vector<INT32> Vec3;
		pParams->GetValue(2 , Vec2);
		pParams->GetValueEx(p1 , p2 , Vec3);
		Vec3 = (*pParams)[2];
		p2 = (*pParams)[1];
	}


	std::cout << "testParamsAndRpcDatas_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Server::MasterHandler::testParamsAndRpcDatas_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , TestRpcData & rpcData/* = TestRpcData()*/ , TestRpcData2 & rpcData2/* = TestRpcData2()*/ , std::vector<INT32> & Vec/* = std::vector<INT32>(1)*/)
{


	std::cout << "testParamsAndRpcDatas_RpcTimeout" << std::endl;
	ReturnNULL;
}

