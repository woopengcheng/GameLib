/************************************
FileName	:	RpcRegister.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Description	:	注册每个函数.以及检测网络传递的消息是否是正确的参数.
************************************/
#include "MsgLib/inc/RpcManager.h"
#include "MsgLib/inc/RpcCheckParams.h"
#include "CUtil/inc/Chunk.h"
#include "MsgNameDefine.h"
#include "GRpc.h"
#include "DBClient.h"
#include "GRpc.h"

namespace Client
{
	//tool defaultParams define here.
	static int g_rpcDefaultParam_int = 0;
	static short g_rpcDefaultParam_short = 0;
	static long g_rpcDefaultParam_long = 0;
	static char g_rpcDefaultParam_char = char(0);
	static unsigned int g_rpcDefaultParam_unsigned_int = 0;
	static unsigned short g_rpcDefaultParam_unsigned_short = 0;
	static unsigned char g_rpcDefaultParam_unsigned_char = 0;
	static std::string g_rpcDefaultParam_std__string = std::string();
	static time_t g_rpcDefaultParam_time_t = 0;
	static SINT8 g_rpcDefaultParam_SINT8 = 0;
	static UINT8 g_rpcDefaultParam_UINT8 = 0;
	static UINT16 g_rpcDefaultParam_UINT16 = 0;
	static INT16 g_rpcDefaultParam_INT16 = 0;
	static INT32 g_rpcDefaultParam_INT32 = 0;
	static UINT32 g_rpcDefaultParam_UINT32 = 0;
	static UINT64 g_rpcDefaultParam_UINT64 = 0;
	static INT64 g_rpcDefaultParam_INT64 = 0;
	static double g_rpcDefaultParam_double = 0.0f;
	static float g_rpcDefaultParam_float = 0.0f;
	static std_string g_rpcDefaultParam_std_string = std::string();
	static CUtilChunk g_rpcDefaultParam_CUtilChunk = CUtil::Chunk();
	static TestRpcData g_rpcDefaultParam_TestRpcData = TestRpcData();
	static TestRpcData2 g_rpcDefaultParam_TestRpcData2 = TestRpcData2();
	static std::vector<INT32> g_rpcDefaultParam_std__vector_INT32_;

	void DBClient::OnRegisterRpcs( void )
	{
		Assert(m_pRpcManager && Msg::RpcCheckParams::GetInstance());	
		static Client::GRpc g_pGRpc( Msg::DEFAULT_RPC_CALLABLE_ID , m_pRpcManager); 

		//tool testMulitServerNode generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_CUtilChunk , g_rpcDefaultParam_int , g_rpcDefaultParam_unsigned_int , g_rpcDefaultParam_char);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("testMulitServerNode", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("testMulitServerNode", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_sztestMulitServerNode_RpcClient , &GRpc::testMulitServerNode_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_sztestMulitServerNode_RpcTimeout ,&GRpc::testMulitServerNode_RpcTimeout); 
		}

		//tool testParamsAndRpcDatas generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_TestRpcData , g_rpcDefaultParam_TestRpcData2 , g_rpcDefaultParam_std__vector_INT32_);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("testParamsAndRpcDatas", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("testParamsAndRpcDatas", objReturnParams);
		}

		//tool SyncServerHandler generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams );
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("SyncServerHandler", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("SyncServerHandler", objReturnParams);
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szSyncServerHandler_RpcServer , &GRpc::SyncServerHandler_RpcServer); 
		}

		//tool HandleUserAuth generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleUserAuth", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleUserAuth", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleUserAuth_RpcClient , &GRpc::HandleUserAuth_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleUserAuth_RpcTimeout ,&GRpc::HandleUserAuth_RpcTimeout); 
		}

		//tool HandleSelectDatabase generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleSelectDatabase", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleSelectDatabase", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleSelectDatabase_RpcClient , &GRpc::HandleSelectDatabase_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleSelectDatabase_RpcTimeout ,&GRpc::HandleSelectDatabase_RpcTimeout); 
		}

		//tool HandleCreateDatabase generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleCreateDatabase", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleCreateDatabase", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleCreateDatabase_RpcClient , &GRpc::HandleCreateDatabase_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleCreateDatabase_RpcTimeout ,&GRpc::HandleCreateDatabase_RpcTimeout); 
		}

		//tool HandleDeleteDatabase generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleDeleteDatabase", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleDeleteDatabase", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleDeleteDatabase_RpcClient , &GRpc::HandleDeleteDatabase_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleDeleteDatabase_RpcTimeout ,&GRpc::HandleDeleteDatabase_RpcTimeout); 
		}

		//tool HandleShowDatabases generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams );
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleShowDatabases", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleShowDatabases", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleShowDatabases_RpcClient , &GRpc::HandleShowDatabases_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleShowDatabases_RpcTimeout ,&GRpc::HandleShowDatabases_RpcTimeout); 
		}

		//tool HandleShowUsers generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams );
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleShowUsers", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleShowUsers", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleShowUsers_RpcClient , &GRpc::HandleShowUsers_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleShowUsers_RpcTimeout ,&GRpc::HandleShowUsers_RpcTimeout); 
		}

		//tool HandleCreateUser generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string , g_rpcDefaultParam_SINT8);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleCreateUser", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleCreateUser", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleCreateUser_RpcClient , &GRpc::HandleCreateUser_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleCreateUser_RpcTimeout ,&GRpc::HandleCreateUser_RpcTimeout); 
		}

		//tool HandleDeleteUser generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleDeleteUser", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleDeleteUser", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleDeleteUser_RpcClient , &GRpc::HandleDeleteUser_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleDeleteUser_RpcTimeout ,&GRpc::HandleDeleteUser_RpcTimeout); 
		}

		//tool HandleModifyUser generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string , g_rpcDefaultParam_SINT8);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleModifyUser", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleModifyUser", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleModifyUser_RpcClient , &GRpc::HandleModifyUser_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleModifyUser_RpcTimeout ,&GRpc::HandleModifyUser_RpcTimeout); 
		}

		//tool HandleHSet generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHSet", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHSet", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHSet_RpcClient , &GRpc::HandleHSet_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHSet_RpcTimeout ,&GRpc::HandleHSet_RpcTimeout); 
		}

		//tool HandleHSetNX generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHSetNX", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHSetNX", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHSetNX_RpcClient , &GRpc::HandleHSetNX_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHSetNX_RpcTimeout ,&GRpc::HandleHSetNX_RpcTimeout); 
		}

		//tool HandleHSetOW generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHSetOW", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHSetOW", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHSetOW_RpcClient , &GRpc::HandleHSetOW_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHSetOW_RpcTimeout ,&GRpc::HandleHSetOW_RpcTimeout); 
		}

		//tool HandleHGet generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_std_string);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHGet", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHGet", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHGet_RpcClient , &GRpc::HandleHGet_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHGet_RpcTimeout ,&GRpc::HandleHGet_RpcTimeout); 
		}

		//tool HandleHDel generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHDel", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHDel", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHDel_RpcClient , &GRpc::HandleHDel_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHDel_RpcTimeout ,&GRpc::HandleHDel_RpcTimeout); 
		}

		//tool HandleHSetIncr generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string , g_rpcDefaultParam_INT64);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT64);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHSetIncr", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHSetIncr", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHSetIncr_RpcClient , &GRpc::HandleHSetIncr_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHSetIncr_RpcTimeout ,&GRpc::HandleHSetIncr_RpcTimeout); 
		}

		//tool HandleHSetIncrFloat generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string , g_rpcDefaultParam_double);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_double);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHSetIncrFloat", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHSetIncrFloat", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHSetIncrFloat_RpcClient , &GRpc::HandleHSetIncrFloat_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHSetIncrFloat_RpcTimeout ,&GRpc::HandleHSetIncrFloat_RpcTimeout); 
		}

		//tool HandleHGetKeys generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHGetKeys", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHGetKeys", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHGetKeys_RpcClient , &GRpc::HandleHGetKeys_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHGetKeys_RpcTimeout ,&GRpc::HandleHGetKeys_RpcTimeout); 
		}

		//tool HandleHGetVals generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHGetVals", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHGetVals", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHGetVals_RpcClient , &GRpc::HandleHGetVals_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHGetVals_RpcTimeout ,&GRpc::HandleHGetVals_RpcTimeout); 
		}

		//tool HandleHGetKeyVals generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHGetKeyVals", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHGetKeyVals", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHGetKeyVals_RpcClient , &GRpc::HandleHGetKeyVals_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHGetKeyVals_RpcTimeout ,&GRpc::HandleHGetKeyVals_RpcTimeout); 
		}

		//tool HandleHScan generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string , g_rpcDefaultParam_INT64);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHScan", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHScan", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHScan_RpcClient , &GRpc::HandleHScan_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHScan_RpcTimeout ,&GRpc::HandleHScan_RpcTimeout); 
		}

		//tool HandleHCount generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT64);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHCount", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHCount", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHCount_RpcClient , &GRpc::HandleHCount_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHCount_RpcTimeout ,&GRpc::HandleHCount_RpcTimeout); 
		}

		//tool HandleHDrop generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT64);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHDrop", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHDrop", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHDrop_RpcClient , &GRpc::HandleHDrop_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHDrop_RpcTimeout ,&GRpc::HandleHDrop_RpcTimeout); 
		}

		//tool HandleHList generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams );
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHList", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHList", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHList_RpcClient , &GRpc::HandleHList_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHList_RpcTimeout ,&GRpc::HandleHList_RpcTimeout); 
		}

		//tool HandleHMultiSet generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_CUtilChunk);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHMultiSet", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHMultiSet", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHMultiSet_RpcClient , &GRpc::HandleHMultiSet_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHMultiSet_RpcTimeout ,&GRpc::HandleHMultiSet_RpcTimeout); 
		}

		//tool HandleHMultiGet generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_CUtilChunk);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHMultiGet", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHMultiGet", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHMultiGet_RpcClient , &GRpc::HandleHMultiGet_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHMultiGet_RpcTimeout ,&GRpc::HandleHMultiGet_RpcTimeout); 
		}

		//tool HandleHMultiDel generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_CUtilChunk);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleHMultiDel", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleHMultiDel", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHMultiDel_RpcClient , &GRpc::HandleHMultiDel_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleHMultiDel_RpcTimeout ,&GRpc::HandleHMultiDel_RpcTimeout); 
		}

		//tool HandleZSet generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string , g_rpcDefaultParam_INT64);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleZSet", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleZSet", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZSet_RpcClient , &GRpc::HandleZSet_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZSet_RpcTimeout ,&GRpc::HandleZSet_RpcTimeout); 
		}

		//tool HandleZGet generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT64);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleZGet", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleZGet", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZGet_RpcClient , &GRpc::HandleZGet_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZGet_RpcTimeout ,&GRpc::HandleZGet_RpcTimeout); 
		}

		//tool HandleZDel generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT64);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleZDel", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleZDel", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZDel_RpcClient , &GRpc::HandleZDel_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZDel_RpcTimeout ,&GRpc::HandleZDel_RpcTimeout); 
		}

		//tool HandleZTop generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_INT64 , g_rpcDefaultParam_INT64 , g_rpcDefaultParam_INT64);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleZTop", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleZTop", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZTop_RpcClient , &GRpc::HandleZTop_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZTop_RpcTimeout ,&GRpc::HandleZTop_RpcTimeout); 
		}

		//tool HandleZRTop generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_INT64 , g_rpcDefaultParam_INT64 , g_rpcDefaultParam_INT64);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleZRTop", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleZRTop", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZRTop_RpcClient , &GRpc::HandleZRTop_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZRTop_RpcTimeout ,&GRpc::HandleZRTop_RpcTimeout); 
		}

		//tool HandleZDrop generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleZDrop", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleZDrop", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZDrop_RpcClient , &GRpc::HandleZDrop_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZDrop_RpcTimeout ,&GRpc::HandleZDrop_RpcTimeout); 
		}

		//tool HandleZCount generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT64);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleZCount", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleZCount", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZCount_RpcClient , &GRpc::HandleZCount_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZCount_RpcTimeout ,&GRpc::HandleZCount_RpcTimeout); 
		}

		//tool HandleZList generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams );
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleZList", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleZList", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZList_RpcClient , &GRpc::HandleZList_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleZList_RpcTimeout ,&GRpc::HandleZList_RpcTimeout); 
		}

		//tool HandleDump generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams );
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleDump", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleDump", objReturnParams);
			
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleDump_RpcClient , &GRpc::HandleDump_RpcClient); 
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szHandleDump_RpcTimeout ,&GRpc::HandleDump_RpcTimeout); 
		}

		//tool SyncMasterHandler generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_INT32);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("SyncMasterHandler", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("SyncMasterHandler", objReturnParams);
		}

		//tool SlaveStartAuth generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_std_string);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("SlaveStartAuth", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("SlaveStartAuth", objReturnParams);
		}

		//tool SlaveRequestSync generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_INT32);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("SlaveRequestSync", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("SlaveRequestSync", objReturnParams);
		}

		//tool MasterStartSync generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_CUtilChunk);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("MasterStartSync", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("MasterStartSync", objReturnParams);
		}

		//tool SyncDataToSlave generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_CUtilChunk , g_rpcDefaultParam_INT32);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("SyncDataToSlave", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("SyncDataToSlave", objReturnParams);
		}

	}

	CollectionObjectFuncsT Client::GRpc::s_setFuncs;
}//Client

