#include "GameDB/inc/RemoteNodeDefine.h"
#include "DBServer.h"
#include "ServerHandler.h"
#include "RPCCallFuncs.h"

namespace Server
{   
	DBServer::DBServer(void)
		: ThreadPool::ThreadSustainTask(1, "DBServer")
		, m_nHandlerCount(0)
	{
		m_pRpcListener = new ServerListener(this);
	}

	DBServer::~DBServer(void)
	{
		SAFE_DELETE(m_pRpcListener);

		MapServerHandlersT::iterator iter = m_mapServerHandlers.begin();
		for (; iter != m_mapServerHandlers.end(); ++iter)
		{
			delete iter->second;
		}
		m_mapServerHandlers.clear();
	}

	CErrno DBServer::Update( void )
	{
		const int maxCount = 100;
		static UINT32 unCount = maxCount;

		std::string strDB = "ll";
		while(unCount == 0)
		{
			Msg::VecObjects objTargets;
			objTargets.push_back(0);
			unCount = maxCount;
//			rpc_testTheSameNode(g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER] , objTargets , 1 , strDB , CUtil::Chunk());
		}
		unCount--;

		return DBServerInterface::Update();
	}

	INT32 DBServer::CreateServerHandler(INT32 nSessionID)
	{
		ServerHandler * pHandler = new ServerHandler(++m_nHandlerCount , this);
		if (pHandler)
		{
			MapServerHandlersT::iterator iter = m_mapServerHandlers.find(m_nHandlerCount);
			if (iter == m_mapServerHandlers.end())
			{
				m_mapServerHandlers.insert(std::make_pair(m_nHandlerCount, pHandler));
			}
			else
			{
				SAFE_DELETE(pHandler);
				gErrorStream("AddServerHandler error,same sessionid=" << pHandler->GetSessionID() << ":handlerID=%d" << m_nHandlerCount);
			}
		}

		return m_nHandlerCount;
	}
		
	CErrno ServerListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (m_pDBServer)
		{
			if (strNetNodeName == g_strGameDBNodes[NETNODE_DBSERVER_TO_DBCLIENT])
			{
				INT32 nHandlerID = m_pDBServer->CreateServerHandler(nSessionID);
				rpc_SyncServerHandler(nSessionID, Msg::Object(0), Msg::Object(nHandlerID));
			}
			if (strNetNodeName == g_strGameDBNodes[NETNODE_DBSERVER_TO_DBMASTER])
			{
				INT32 nHandlerID = m_pDBServer->CreateServerHandler(nSessionID);
				rpc_SyncServerHandler(nSessionID, Msg::Object(0), nHandlerID);
			}
		}
		return CErrno::Success();
	}

	CErrno ServerListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{
		return CErrno::Success();
	}

}