#ifndef __test_server_object_h__
#define __test_server_object_h__
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgLib/inc/RpcManager.h"
#include "MsgLib/inc/Object.h"
#include "GameDB/inc/UserAuth.h"
#include "GameDB/inc/SortedSet.h"
#include "GameDB/inc/RemoteNodeDefine.h"
#include "RpcDefines.h" 
#include "MsgNameDefine.h"  
#include "DBServer.h" 
#include "RPCCallFuncs.h"

namespace Server
{  
	class ServerHandler : public Msg::IRpcMsgCallableObject
	{ 
		RPC_DEFINE_ServerHandler;
	public:
		ServerHandler(INT32 nObjectID , DBServer * pDBServer)
			: Msg::IRpcMsgCallableObject(Msg::Object(nObjectID) , pDBServer->GetRpcManager())
			, m_pDBServer(pDBServer)
			, m_strDatabaseName("")
		{}
		 
	public:
		GameDB::Database * GetDataBase()
		{
			if (m_pDBServer)
			{
				return m_pDBServer->GetEnvironment()->GetDatabase(m_strDatabaseName);
			}

			return NULL;
		}
		DBServer	*				GetDBServer() { return m_pDBServer; }
		void						SetAuthInfo(const GameDB::UserAuth & val) { m_objAuthInfo = val; }
		const GameDB::UserAuth &	GetAuthInfo() const { return m_objAuthInfo; }
		INT32						SyncDataToSlave(GameDB::Operate & oper)
		{
			return rpc_SyncDataToSlave(g_strGameDBNodes[NETNODE_DBSERVER_TO_DBMASTER], 0, GetObjectID(), m_strDatabaseName, oper.GetOperateRecord().GetData());
		}

	private:
		DBServer				*	m_pDBServer;
		std::string					m_strDatabaseName;
		GameDB::UserAuth			m_objAuthInfo;
	}; 


}

#endif