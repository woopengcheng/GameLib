/************************************
FileName	:	RpcRegister.cpp
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.37.66
Version		:	0.0.1
Description	:	注册每个函数.以及检测网络传递的消息是否是正确的参数.
************************************/
#include "MsgLib/inc/RpcManager.h"
#include "MsgLib/inc/RpcCheckParams.h"
#include "CUtil/inc/Chunk.h"
#include "MsgNameDefine.h"
#include "GRpc.h"
#include "RobotGroup.h"
#include "GRpc.h"
#include "CRobot.h"

namespace Robot
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
	static std::vector<INT32> g_rpcDefaultParam_std__vector_INT32_;

	void RobotGroup::OnRegisterRpcs( void )
	{
		Assert_(m_pRpcManager && Msg::RpcCheckParams::GetInstance());	
		static Robot::GRpc g_pGRpc( Msg::DEFAULT_RPC_CALLABLE_ID , m_pRpcManager); 

		//tool SyncRobotServerID generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_INT32);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("SyncRobotServerID", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("SyncRobotServerID", objReturnParams);
		}

		//tool HandleRobotGroup generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_INT32);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("HandleRobotGroup", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("HandleRobotGroup", objReturnParams);
		}

		//tool UpdateRobotStart generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_CUtilChunk);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_CUtilChunk);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("UpdateRobotStart", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("UpdateRobotStart", objReturnParams);
		}

		//tool SyncRobotGroupID generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams );
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("SyncRobotGroupID", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("SyncRobotGroupID", objReturnParams);
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szSyncRobotGroupID_RpcServer , &GRpc::SyncRobotGroupID_RpcServer); 
		}

		//tool CreateRobots generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_INT32);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("CreateRobots", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("CreateRobots", objReturnParams);
			m_pRpcManager->RegisterFunc<GRpc >(Msg::g_szCreateRobots_RpcServer , &GRpc::CreateRobots_RpcServer); 
		}

		//tool CreateRobot generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std_string , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_INT32 , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("CreateRobot", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("CreateRobot", objReturnParams);
			
			m_pRpcManager->RegisterFunc<CRobot >(Msg::g_szCreateRobot_RpcClient , &CRobot::CreateRobot_RpcClient); 
			m_pRpcManager->RegisterFunc<CRobot >(Msg::g_szCreateRobot_RpcTimeout ,&CRobot::CreateRobot_RpcTimeout); 
		}

		//tool SendRobotCommand generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_std__vector_INT32_ , g_rpcDefaultParam_std_string);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("SendRobotCommand", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("SendRobotCommand", objReturnParams);
			m_pRpcManager->RegisterFunc<CRobot >(Msg::g_szSendRobotCommand_RpcServer , &CRobot::SendRobotCommand_RpcServer); 
		}

		//tool UpdateRobotStatue generate default deliver and return check param here
		{
			CUtil::Parameters objDeliverParams , objReturnParams;
			CUtil::GenParamHelper::GenParams(objDeliverParams  , g_rpcDefaultParam_INT32);
			CUtil::GenParamHelper::GenParams(objReturnParams  , g_rpcDefaultParam_INT32);
			Msg::RpcCheckParams::GetInstance()->InsertDeliverParams("UpdateRobotStatue", objDeliverParams);
			Msg::RpcCheckParams::GetInstance()->InsertReturnParams("UpdateRobotStatue", objReturnParams);
			
			m_pRpcManager->RegisterFunc<CRobot >(Msg::g_szUpdateRobotStatue_RpcClient , &CRobot::UpdateRobotStatue_RpcClient); 
			m_pRpcManager->RegisterFunc<CRobot >(Msg::g_szUpdateRobotStatue_RpcTimeout ,&CRobot::UpdateRobotStatue_RpcTimeout); 
		}

		Robot::CRobot::InitObjectFuncs();
	}

	CollectionObjectFuncsT Robot::GRpc::s_setFuncs;
	CollectionObjectFuncsT Robot::CRobot::s_setFuncs;
}//Robot

