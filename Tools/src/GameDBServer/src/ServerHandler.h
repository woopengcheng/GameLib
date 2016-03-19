#ifndef __test_server_object_h__
#define __test_server_object_h__

#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgLib/inc/RpcManager.h"
#include "MsgLib/inc/Object.h"
#include "RpcDefines.h" 
#include "MsgNameDefine.h"  
#include "DBServer.h" 
#include "GameDB/inc/UserAuth.h"
#include "GameDB/inc/SortedSet.h"

namespace Server
{  
	class ServerHandler : public Msg::IRpcMsgCallableObject
	{ 
		RPC_DEFINE_ServerHandler;
	public:
		ServerHandler(INT32 nObjectID , DBServer * pDBServer , INT32 nSessionID)
			: Msg::IRpcMsgCallableObject(Msg::Object(nObjectID) , pDBServer->GetRpcManager())
			, m_pDBServer(pDBServer)
			, m_strDatabaseName("")
			, m_nClientSessionID(nSessionID)
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
		INT32						GetClientSessionID() const { return m_nClientSessionID; }
		DBServer	*				GetDBServer() { return m_pDBServer; }
		void						SetAuthInfo(const GameDB::UserAuth & val) { m_objAuthInfo = val; }
		const GameDB::UserAuth &	GetAuthInfo() const { return m_objAuthInfo; }

	private:
		DBServer				*	m_pDBServer;
		INT32						m_nClientSessionID;
		std::string					m_strDatabaseName;
		GameDB::UserAuth			m_objAuthInfo;
	}; 
}

#endif