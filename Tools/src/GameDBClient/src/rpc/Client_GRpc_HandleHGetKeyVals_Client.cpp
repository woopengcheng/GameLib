#include "GRpc.h"
#include "MsgLib/inc/RPCMsgCall.h"
#include "OrmHelper.h"

Msg::ObjectMsgCall * Client::GRpc::HandleHGetKeyVals_RpcClient(INT32 nSessionID , Msg::Object objSrc , CUtilChunk & res/* = CUtil::Chunk()*/) 
{ 
	if (res.GetDataLen() != 0)
	{
		CUtil::CStream cs;
		cs.Pushback(res.Begin() , res.GetDataLen());

		INT32 nCount = 0;
		cs >> nCount;

		Msg::RpcCallbackPtr rpcCallback = GetCallback();
		std::string strKey;
		std::string strVal;
		for (INT32 i = 0;i < nCount;++i)
		{
			cs >> strKey >> strVal;
			gOtherStream("i:" << i << "key:" << strKey << "value:" << strVal);
			if (rpcCallback)
			{
				GameDB::STableKeyVal obj;
				obj.strTable = GetInParams()->GetValue<std::string>(0);
				obj.strKey = strKey;
				obj.strVal = strVal;
				rpcCallback->OnCall(&obj);
			}
		}
	}
	else
	{
		gWarniStream("get error.");
	}


	std::cout << "HandleHGetKeyVals_RpcClient" << std::endl;
	ReturnNULL;
}

Msg::ObjectMsgCall * Client::GRpc::HandleHGetKeyVals_RpcTimeout(INT32 nSessionID , Msg::Object objSrc , std_string & table/* = std::string()*/) 
{


	std::cout << "HandleHGetKeyVals_RpcTimeout" << std::endl;
	ReturnNULL;
}

