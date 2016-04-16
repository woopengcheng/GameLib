#include "ThreadPool/inc/ThreadPoolInterface.h"
#include "Timer/inc/TimerHelp.h"
#include "GameDB/inc/RemoteNodeDefine.h"
#include "DBSlave.h" 
#include "SlaveHandler.h"

namespace Server
{
	DBSlave::DBSlave(void)
		: ThreadPool::ThreadSustainTask(102 , "DBSlave" )
		, m_nSlaveCount(0)
	{
		m_pRpcListener = new SlaveListener(this);
	}

	DBSlave::~DBSlave(void)
	{
		SAFE_DELETE(m_pRpcListener);

		CollectionSlavesT::iterator iter = m_mapSalves.begin();
		for (; iter != m_mapSalves.end(); ++iter)
		{
			SAFE_DELETE(iter->second);
		}

		m_mapSalves.clear();
	}

	CErrno DBSlave::Init(Json::Value & conf)
	{
		Json::Value objThreads = conf.get("thread" , Json::Value());
		InitThread(objThreads);  

		return DBSlaveInterface::Init(conf);
	}

	CErrno DBSlave::Update(void)
	{  
		return DBSlaveInterface::Update();
	}

	CErrno DBSlave::InitThread(Json::Value & conf)
	{
		std::map<UINT32 , UINT32> mapThreads;
		INT32 nCount = conf.size(); 
		for (INT32 i = 0 ;i < nCount; ++ i)
		{     
			Json::Value objThread = conf[i]; 
			UINT32 priority = objThread.get("priority", 0).asUInt();
			SetThreadPriority(priority);
			UINT32 count = objThread.get("count" , 1).asUInt(); 

			mapThreads[priority] = count;    
		}

		ThreadPool::ThreadPoolInterface::GetInstance().Init(mapThreads , TRUE);
		ThreadPool::ThreadPoolInterface::GetInstance().Startup();  
		ThreadPool::ThreadPoolInterface::GetInstance().AddTask(this);  

		return CErrno::Success();
	}

	CErrno DBSlave::Cleanup(void)
	{
		ThreadPool::ThreadPoolInterface::GetInstance().Cleanup();
		return DBSlaveInterface::Cleanup();
	}

	void DBSlave::OnCreateDatabase(const GameDB::SDBSlaveInfo & objInfo)
	{
		SlaveHandler * pHandler = new SlaveHandler(++m_nSlaveCount , this);
		if (pHandler)
		{
			pHandler->SetSlaveInfo(objInfo);
			m_mapSalves.insert(std::make_pair(objInfo.strDBName , pHandler));
		}
	}

	SlaveHandler *  DBSlave::GetSlaveHandler(const std::string & strDBName)
	{
		CollectionSlavesT::iterator iter = m_mapSalves.find(strDBName);
		if (iter != m_mapSalves.end())
		{
			return iter->second;
		}

		return NULL;
	}

	void DBSlave::StartAuth()
	{
		CollectionSlavesT::iterator iter = m_mapSalves.begin();
		for (;iter != m_mapSalves.end();++iter)
		{
			SlaveHandler * pHandler = iter->second;
			if (pHandler)
			{
				pHandler->StartAuth();
			}
		}
	}

	void DBSlave::SetSlaveSessionID(const std::string & strDBName, INT32 nSessionID)
	{
		SlaveHandler * pHandler = GetSlaveHandler(strDBName);
		if (pHandler)
		{
			pHandler->SetMasterSessionID(nSessionID);
		}
	}

	void DBSlave::RequestSyncData(const std::string & strDBName)
	{
		SlaveHandler * pHandler = GetSlaveHandler(strDBName);
		if (pHandler)
		{
			pHandler->RequestSyncData();

			return;
		}
		gDebugStream("RequestSyncData error. no this db=" << strDBName);
	}

	CErrno SlaveListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (m_pDBSlave)
		{
			std::string strCurNode;
			std::vector<std::string> vals;
			CUtil::tokenize(strNetNodeName, vals, "_", "", "\"");
			if (vals.size() < 3)
			{
				return CErrno::Failure();
			}
			std::string strDBName = vals[0];
			CUtil::tokenize(strDBName, vals, SLAVE_SPECIAL_SPLIT, "", "\"");
			if (vals.size() >= 2)
			{
				strDBName = vals[0];
				m_pDBSlave->SetSlaveSessionID(strDBName, nSessionID);

				if (bReconnect)
				{
					m_pDBSlave->RequestSyncData(strDBName);
					gDebugStream("slave reconnect:dbname=" << strDBName);
				}
			}
		}

		return CErrno::Success();

	}

	CErrno SlaveListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{

		return CErrno::Success();
	}

}