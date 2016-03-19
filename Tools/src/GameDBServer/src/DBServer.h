#ifndef __msg_rpc_instance_h__
#define __msg_rpc_instance_h__  
#include "ThreadPool/inc/ThreadTask.h"
#include "GameDB/inc/DBServerInterface.h"
#include "GameDB/inc/HashTable.h"
#include "GameDB/inc/Environment.h"
#include "GameDB/inc/Database.h"
#include "GameDB/inc/Operate.h"
#include "json/json.h"


namespace Server
{  
	class ServerHandler;

	class  DBServer : public GameDB::DBServerInterface , ThreadPool::ThreadSustainTask
	{ 
	public:
		typedef std::unordered_map<INT32, ServerHandler *> MapServerHandlersT;

	public:
		DBServer(void);
		virtual ~DBServer(void);

	public:
		static DBServer & GetInstance()
		{
			static DBServer m_sRpcInterface;
			return m_sRpcInterface;
		} 

	public:  
		virtual void		OnRegisterRpcs(void) override;
		virtual CErrno		Update(void) override;

	public:
		INT32				CreateServerHandler(INT32 nSessionID);

	private:
		INT32				m_nHandlerCount;
		MapServerHandlersT	m_mapServerHandlers;
	};  
	 

	class ServerListener : public Msg::IRpcListener
	{
	public:
		ServerListener(DBServer * pDBServer)
			: m_pDBServer(pDBServer)
		{

		}

	public:
		virtual CErrno OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false) override;
		virtual CErrno OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID) override;

	private:
		DBServer * m_pDBServer;
	};
}


#endif