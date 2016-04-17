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
	{
		m_pRpcListener = new MasterListener(this);
	}

	DBMaster::~DBMaster(void)
	{
		SAFE_DELETE(m_pRpcListener);

		MapMasterHandlersT::iterator iter = m_mapMasterHandlers.begin();
		for (;iter != m_mapMasterHandlers.end();++iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_mapMasterHandlers.clear();
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

	INT32 DBMaster::CreateMasterHandler(INT32 nSessionID)
	{
		++m_nHandlerCount;
		MasterHandler * pMasterHandler = new MasterHandler(m_nHandlerCount, this);
				
		pMasterHandler->SetDBSlaveSessionID(nSessionID);
 		m_mapMasterHandlers.insert(std::make_pair(m_nHandlerCount , pMasterHandler));
		return m_nHandlerCount;
	}

	MasterHandler		* DBMaster::GetMasterHandler(INT32 nID)
	{
		MapMasterHandlersT::iterator iter = m_mapMasterHandlers.find(nID);
		if (iter != m_mapMasterHandlers.end())
		{
			return iter->second;
		}

		return NULL;
	}

	INT32 DBMaster::CreateSlaveRecord(INT32 nMasterID, INT32 nSessionID, const std::string & strDBName)
	{
		MasterHandler * pHandler = GetMasterHandler(nMasterID);
		if (pHandler)
		{
			return pHandler->CreateSlaveRecord(strDBName, nSessionID);
		}
		else
		{
			gErrorStream("CreateSlaveRecord error masterID=" << nMasterID << "sessionID=" << nSessionID << ":dbName=" << strDBName);
		}

		return 0;
	}

	SlaveRecord * DBMaster::GetSlaveRecord(INT32 nMasterID , const std::string & strDBName)
	{
		MasterHandler * pHandler = GetMasterHandler(nMasterID);
		if (pHandler)
		{
			return pHandler->GetSlaveRecord(strDBName);
		}

		return NULL;
	}

	bool DBMaster::GetSlaveRecord(const std::string & strDBName, VecSlaveRecordsT & vecSlaveRecords)
	{
		MapMasterHandlersT::iterator iter = m_mapMasterHandlers.begin();
		for (;iter != m_mapMasterHandlers.end();++iter)
		{
			MasterHandler * pHandler = iter->second;
			if (pHandler)
			{
				SlaveRecord * pRecord = pHandler->GetSlaveRecord(strDBName);
				if (pRecord)
				{
					vecSlaveRecords.push_back(pRecord);
				}
			}
		}

		if (vecSlaveRecords.size() > 0)
		{
			return true;
		}
		return false;
	}

	CErrno DBMaster::Cleanup(void)
	{

		ThreadPool::ThreadPoolInterface::GetInstance().Cleanup();
		return DBMasterInterface::Cleanup();
	}

	INT32 DBMaster::InsertSaveCache(const std::string & strDBName, CUtil::Chunk objValue)
	{
		MapSaveCachesT::iterator iter = m_mapSaveCaches.find(strDBName);
		if (iter != m_mapSaveCaches.end())
		{
			SSaveCaches & cache = iter->second;
			cache.vecSaveCaches.push_back(objValue);
			return (INT32)cache.vecSaveCaches.size() + cache.nCurPos;
		}
		else
		{
			SSaveCaches cache;
			cache.vecSaveCaches.push_back(objValue);
			m_mapSaveCaches.insert(std::make_pair(strDBName, cache));
			return (INT32)cache.vecSaveCaches.size() + cache.nCurPos;
		}

		return -1;
	}

	BOOL DBMaster::RefreshSaveCache(const std::string & strDBName)
	{
		MapSaveCachesT::iterator iter = m_mapSaveCaches.find(strDBName);
		if (iter != m_mapSaveCaches.end())
		{
			SSaveCaches & cache = iter->second;

			INT32  nMinPos = 0x7FFFFFFF;
			Server::DBMaster::VecSlaveRecordsT vecRecords;
			bool bFound = Server::DBMaster::GetInstance().GetSlaveRecord(strDBName, vecRecords);
			if (bFound)
			{
				INT32 nLastPos = cache.nLastPos;
				Server::DBMaster::VecSlaveRecordsT::iterator iter = vecRecords.begin();
				for (; iter != vecRecords.end(); ++iter)
				{
					Server::SlaveRecord * pSlaveRecord = *iter;
					INT32 nPos = pSlaveRecord->GetSaveCachePos();
					if (pSlaveRecord && nPos >= 0 && nPos <= nMinPos)
					{
						nMinPos = nPos;
					}
				}
				if (nLastPos != nMinPos)
				{
					cache.nLastPos = nMinPos;
					INT32 nDiff = nMinPos - nLastPos;
					if (nDiff <= cache.vecSaveCaches.size())
					{
						cache.vecSaveCaches.erase(cache.vecSaveCaches.begin(), cache.vecSaveCaches.begin() + nDiff);
						cache.nCurPos = nMinPos;
						gDebugStream("RefreshSaveCache .name=" << strDBName << ":begin=" << nLastPos << ":end=" << nMinPos << ":diff=" << nDiff);
					}
					else
					{
						gErrorStream("RefreshSaveCache error. name=" << strDBName);
					}
				}
				return TRUE;
			}
		}

		return FALSE;
	}

	BOOL DBMaster::GetSaveCache(const std::string & strDBName, INT32 nSlaveID , INT32 nSessionID , INT32 nLastPos , VecSaveCachesT & vec)
	{
		MapSaveCachesT::iterator iter = m_mapSaveCaches.find(strDBName);
		if (iter != m_mapSaveCaches.end())
		{
			SSaveCaches & cache = iter->second;

			Server::DBMaster::VecSlaveRecordsT vecRecords;
			bool bFound = Server::DBMaster::GetInstance().GetSlaveRecord(strDBName, vecRecords);
			if (bFound)
			{
				Server::DBMaster::VecSlaveRecordsT::iterator iter = vecRecords.begin();
				for (; iter != vecRecords.end(); ++iter)
				{					
					Server::SlaveRecord * pSlaveRecord = *iter;
					if (pSlaveRecord && pSlaveRecord->GetSlaveID() == nSlaveID && 
						pSlaveRecord->GetSlaveSessionID() == nSessionID &&
						pSlaveRecord->GetSaveCachePos() == nLastPos)
					{
						INT32 nDiff = cache.nLastPos - nLastPos;
						vec.assign(cache.vecSaveCaches.begin() + (nLastPos - cache.nCurPos) , cache.vecSaveCaches.begin() + (nLastPos - cache.nCurPos + nDiff));

						return TRUE;
					}
				}
			}
		}

		return FALSE;

	}

	CErrno MasterListener::OnConnected(Msg::RpcInterface * pRpcInterface , INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (m_pDBMaster)
		{

			if (strNetNodeName == g_strGameDBNodes[NETNODE_DBMASTER_TO_DBSERVER])  //5 除去链接DBServer的其他的都是slave的连接.如果未来有其他连接.那么需要加在else的上面.else是特殊处理.
			{
//				INT32 nMasterHandlerID = m_pDBMaster->CreateMasterHandler(nSessionID);
			}
			else if (strNetNodeName == g_strGameDBNodes[NETNODE_DBMASTER_TO_DBSLAVE])
			{
				if (!bReconnect)
				{
					INT32 nMasterHandlerID = m_pDBMaster->CreateMasterHandler(nSessionID);
					rpc_SyncMasterHandler(nSessionID, Msg::Object(0), Msg::Object(nMasterHandlerID), nMasterHandlerID);
				}
			}
			else
			{
				std::string strCurNode;
				std::vector<std::string> vals;
				CUtil::tokenize(strNetNodeName, vals, SLAVE_SPECIAL_SPLIT, "", "\"");
				if (vals.size() < 2)
				{
					return CErrno::Failure(strCurNode);
				}
				INT32 nMasterID = (INT32)CUtil::atoi(vals[1]);
				
				std::string strName = vals[0];
				vals.clear();
				CUtil::tokenize(strName, vals, "_", "", "\"");
				if (vals.size() < 3)
				{
					return CErrno::Failure();
				}
				std::string strDBName = vals[2];
				m_pDBMaster->CreateSlaveRecord(nMasterID , nSessionID , strDBName);
			}
		}
		if (bReconnect == true)
		{
			gDebugStream("ReConnected from sessionID=" << nSessionID << ":node name=" << strNetNodeName);
		}


		return CErrno::Success();
	}

	CErrno MasterListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{
		gDebugStream("disconnected from sessionID=" << nPeerSessionID);
		return CErrno::Success();
	}

}