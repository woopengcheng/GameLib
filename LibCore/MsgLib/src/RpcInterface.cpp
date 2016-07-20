#include "MsgLib/inc/RpcInterface.h"
#include "MsgLib/inc/RpcManager.h"
#include "MsgLib/inc/NetNode.h"
#include "MsgLib/inc/RPCMsgCall.h"
#include "Coroutine/Coroutine.h"
#include "Timer/inc/TimerHelp.h"
#include "NetLib/inc/NetThread.h"

namespace Msg
{ 

	RpcInterface::RpcInterface(void)
		: m_pRpcManager(NULL)
		, m_pRpcListener(NULL)
		, m_bFirstUpdated(FALSE)
		, m_pNetThread(NULL)
	{
	} 

	RpcInterface::~RpcInterface(void)
	{
	}

	void RpcInterface::RegisterRpc( void )
	{
		OnRegisterRpcs();
	}
	
	BOOL RpcInterface::DeleteRpcCoTask(UINT64 ullMsgID)
	{
		MapRpcCoTasksT::iterator iter = m_mapRpcCoTasks.find(ullMsgID);
		if (iter != m_mapRpcCoTasks.end())
		{
			m_mapRpcCoTasks.erase(iter);

			return TRUE;
		}
		else
		{
			gErrorStream("DeleteRpcCoTask error. no this msgid.ID=" << ullMsgID);
		}

		return FALSE;
	}

	BOOL RpcInterface::AddRpcCoTask(SRpcCoTask * pTask)
	{
		if (pTask && pTask->pMsg)
		{
			MapRpcCoTasksT::iterator iter = m_mapRpcCoTasks.find(pTask->pMsg->m_ullMsgID);
			if (iter == m_mapRpcCoTasks.end())
			{
				m_mapRpcCoTasks.insert(std::make_pair(pTask->pMsg->m_ullMsgID, pTask));

				return TRUE;
			}
		}		
		if (pTask->pMsg)
		{
			gErrorStream("AddRpcCoTask error. no this msgid.ID=" << pTask->pMsg->m_ullMsgID << ":msgMethod=" << pTask->pMsg->m_szMsgMethod);
		}

		return FALSE;

	}

	BOOL RpcInterface::ResumeRpcCoTask(UINT64 ullMsgID)
	{
		MapRpcCoTasksT::iterator iter = m_mapRpcCoTasks.find(ullMsgID);
		if (iter != m_mapRpcCoTasks.end())
		{
			SRpcCoTask * pTask = iter->second;
			if (pTask)
			{
				Coroutine::CoResume(pTask->pCoID);
			}

			return TRUE;
		}
		else
		{
			gErrorStream("ResumeRpcCoTask error. no this msgid.ID=" << ullMsgID);
		}

		return FALSE;

	}

	CErrno RpcInterface::Init(Json::Value & conf)
	{   
		HandlerMySelfNode(conf);

		if (!m_pRpcManager)
		{
			m_pRpcManager = new RpcManager(this);
			m_pRpcManager->Init();
		}
		if (!m_pNetThread)
		{
			m_pNetThread = new Net::NetThread;  //5 作为一个Task线程会自动释放.以及调用cleanup.
			m_pNetThread->Init(conf);
		}

		RegisterRpc();

		EnableRpc(conf);
		return CErrno::Success(); 
	}

	void RpcInterface::EnableRpc(Json::Value & conf)
	{
		Json::Value rpcs = conf.get("enable_rpc", Json::Value());
		INT32 nCount = rpcs.size();
		for (INT32 i = 0; i < nCount; ++i)
		{
			Json::Value rpc = rpcs[i];

			std::string strName = rpc.get("name", "").asCString();
			INT32 enable = rpc.get("enable", TRUE).asBool();
			if (m_pRpcManager)
			{
				MethodImpl * pMethod = m_pRpcManager->GetMethodImpl(strName + RPCClient);
				if (pMethod)
				{
					pMethod->m_bEnable = enable;
				}
			}
		}
	}
	void RpcInterface::HandlerMySelfNode(Json::Value & conf)
	{
		Json::Value server = conf.get("server", Json::Value());
		std::string strNodeName = server.get("net_node_name", "").asCString();

		NetNode::GetInstance().InsertMyselfNodes(strNodeName , this);
	}

	CErrno RpcInterface::Cleanup( void )
	{
		if (m_pRpcManager)
		{
			m_pRpcManager->Cleanup();
			SAFE_DELETE(m_pRpcManager);
		}

		Coroutine::CoCleanup();
		return CErrno::Success();
	} 
	
	CErrno RpcInterface::Update( void )
	{ 
		if (!m_bFirstUpdated)
		{
// 			Coroutine::CoInit();
			m_bFirstUpdated = TRUE;
		}

		if (m_pRpcManager)
		{
			m_pRpcManager->Update();
		}

		Timer::sleep(1);
		return CErrno::Success();
	}	

	INT32 RpcInterface::SendMsg( INT32 nSessionID , RPCMsgCall * pMsg  , BOOL bAddRpc/* = TRUE*/)
	{   
		if (m_pRpcManager)
		{  
			INT32 nResult = m_pRpcManager->SendMsg(nSessionID , pMsg , bAddRpc);
			if (nResult >= 0)
			{
				TakeOverSync(pMsg);
			}
			return nResult;
		}

		return -1;
	} 
	
	INT32 RpcInterface::SendMsg(const std::string & strNodeName , RPCMsgCall * pMsg , BOOL bAddRpc /*= TRUE*/)
	{
		if (m_pRpcManager)
		{
			INT32 nResult = m_pRpcManager->SendMsg(strNodeName, pMsg, bAddRpc);
			if (nResult >= 0)
			{
				TakeOverSync(pMsg);
			}
			return nResult;
		}

		return -1; 
	}

	static void fiberProc(void * pArg)
	{
		SRpcCoTask * pTask = (SRpcCoTask *)pArg;
		if(pTask && pTask->pMsg)
		{
			RPCMsgCall * pMsg = pTask->pMsg;
			while (pMsg->GetSyncResult() == SYNC_RESULT_START_RETURN)
			{
				gErrorStream("MsgName=" << pMsg->m_szMsgMethod << "sync error. state is wrong.state=" << SYNC_RESULT_START_RETURN);
				Coroutine::CoYieldCur();
			}
			
			if (pTask->pInterface)
			{
				pTask->pInterface->DeleteRpcCoTask(pMsg->m_ullMsgID);
			}

			void * pCoID = pTask->pCoID;
			SAFE_DELETE(pTask);
			SAFE_DELETE(pMsg);
			Coroutine::CoRelease(pCoID);
		}
	}

	void RpcInterface::TakeOverSync(RPCMsgCall * pMsg)
	{
		if (pMsg->GetSyncType() == SYNC_TYPE_SYNC)
		{ 
// 			SRpcCoTask * pTask = new SRpcCoTask;
// 			pTask->pMsg = pMsg;
// 			pTask->pInterface = this;
// 
// 			Coroutine::CoCreate(&(pTask->pCoID), fiberProc, pTask);
// 			if (pTask->pCoID)
// 			{
// 				AddRpcCoTask(pTask);
// 				Coroutine::CoResume(pTask->pCoID);
// 			}
			while (pMsg->GetSyncResult() == SYNC_RESULT_START_RETURN)
			{
				Update();
			}
			SAFE_DELETE(pMsg);
		}
	}

}