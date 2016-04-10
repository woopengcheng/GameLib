#include "ThreadPool/inc/ThreadPoolInterface.h"
#include "GameDB/inc/RemoteNodeDefine.h"
#include "DBMaster.h"
#include "MasterHandler.h"
#include "RPCCallFuncs.h"
#include "SlaveRecord.h"

namespace Server
{
	DBMaster::DBMaster(void)
		: ThreadPool::ThreadSustainTask(101 , "DBMaster" )
		, m_nHandlerCount(0)
		, m_llServerID(-1)
		, m_pMasterHandler(NULL)
	{
		m_pRpcListener = new MasterListener(this);
	}

	DBMaster::~DBMaster(void)
	{
		SAFE_DELETE(m_pRpcListener);

		VecMasterHandlersT::iterator iter = m_vecMasterHandlers.begin();
		for (;iter != m_vecMasterHandlers.end();++iter)
		{
			SAFE_DELETE(*iter);
		}
		m_vecMasterHandlers.clear();
	}

	CErrno DBMaster::Init(Json::Value & conf)
	{
		Json::Value objThreads = conf.get("thread" , Json::Value());
		InitThread(objThreads); 
		
		return DBMasterInterface::Init(conf);
	}

	CErrno DBMaster::Update(void)
	{  
		return DBMasterInterface::Update();
	}
	 
	CErrno DBMaster::InitThread(Json::Value & conf)
	{
		std::map<UINT32 , UINT32> mapThreads;
		INT32 nCount = conf.size(); 
		for (INT32 i = 0 ;i < nCount; ++ i)
		{     
			Json::Value objThread = conf[i]; 
			UINT32 priority = objThread.get("priority" , 0).asUInt();
			SetThreadPriority(priority);
			UINT32 count = objThread.get("count" , 1).asUInt(); 

			mapThreads[priority] = count;    
		}

		ThreadPool::ThreadPoolInterface::GetInstance().Init(mapThreads , TRUE);
		ThreadPool::ThreadPoolInterface::GetInstance().Startup();  
		ThreadPool::ThreadPoolInterface::GetInstance().AddTask(this);  

		return CErrno::Success();
	}

	INT32 DBMaster::CreateMasterHandler(INT32 nSessionID , const std::string & strDBName)
	{
		if (!m_pMasterHandler)
		{
			m_pMasterHandler = new MasterHandler(++m_nHandlerCount, this);
		}
		
		m_pMasterHandler->CreateSlaveRecord(strDBName, nSessionID);
// 		MasterHandler * pMasterHandler = new MasterHandler(++m_nHandlerCount , nSessionID , this);
// 		m_vecMasterHandlers.push_back(pMasterHandler);
		return m_nHandlerCount;
	}

	SlaveRecord * DBMaster::GetSlaveRecord(const std::string & strDBName)
	{
		VecMasterHandlersT::iterator iter = m_vecMasterHandlers.begin();
		for (;iter != m_vecMasterHandlers.end();++iter)
		{
			MasterHandler * pMasterHandler = *iter;
			if (pMasterHandler)
			{
				SlaveRecord * pRecord = pMasterHandler->GetSlaveRecord(strDBName); 
				if (pRecord)
				{
					return pRecord;
				}
			}
		}

		return NULL;
	}

	CErrno DBMaster::Cleanup(void)
	{

		ThreadPool::ThreadPoolInterface::GetInstance().Cleanup();
		return DBMasterInterface::Cleanup();
	}

	CErrno MasterListener::OnConnected(Msg::RpcInterface * pRpcInterface , INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (m_pDBMaster)
		{

			if (strNetNodeName == g_strGameDBNodes[NETNODE_DBMASTER_TO_DBSERVER])  //5 除去链接DBServer的其他的都是slave的连接.如果未来有其他连接.那么需要加在else的上面.else是特殊处理.
			{
//				INT32 nMasterHandlerID = m_pDBMaster->CreateMasterHandler(nSessionID);
			}
			else
			{
				std::string strCurNode;
				std::vector<std::string> vals;
				CUtil::tokenize(strNetNodeName, vals, "_", "", "\"");
				if (vals.size() < 3)
				{
					return CErrno::Failure();
				}
				std::string strDBName = vals[2];

				INT32 nMasterHandlerID = m_pDBMaster->CreateMasterHandler(nSessionID, strDBName);
				rpc_SyncMasterHandler(nSessionID, Msg::Object(1), Msg::Object(nMasterHandlerID), nMasterHandlerID);
			}
		}

		return CErrno::Success();
	}

	CErrno MasterListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{

		return CErrno::Success();
	}

}