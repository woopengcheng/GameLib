#ifndef __server_slave_handler_h__
#define __server_slave_handler_h__
#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgLib/inc/RpcManager.h"
#include "MsgLib/inc/Object.h"
#include "GameDB/inc/Database.h"
#include "CUtil/inc/Chunk.h"
#include "MsgNameDefine.h"  
#include "RpcDefines.h" 
#include "DBSlave.h" 

namespace Server
{   
	class SlaveHandler : public Msg::IRpcMsgCallableObject
	{ 
		RPC_DEFINE_SlaveHandler;

	public:
		CollectionObjectFuncsT m_stest;
		
	public:
		SlaveHandler(Msg::Object objID , DBSlave * pDBSlave)
			: Msg::IRpcMsgCallableObject(objID , pDBSlave->GetRpcManager())
			, m_pDBSlave(pDBSlave)
			, m_pDatabase(NULL)
			, m_nMasterSessionID(-1)
			, m_nLastPos(0)
		{
		}

		~SlaveHandler()
		{

			SAFE_DELETE(m_pDatabase);
		}
	public:
		virtual CErrno Update() override { return CErrno::Success(); }

	public:
		void					StartAuth();
		void					RequestSyncData(BOOL bRequestAll = FALSE);
		DBSlave				*	GetDBSlave(){ return m_pDBSlave; }
		bool					RecvFile(std::string strDir , std::string strFileName , std::string strDBName , INT32 nFileSize , INT32 nSendType , const CUtil::Chunk & objChunk);
		bool					RecvFileEnd(std::string strDir , std::string strDBName );
		void					SetSlaveInfo(const GameDB::SDBSlaveInfo & info);
		GameDB::SDBSlaveInfo	GetSlaveInfo(){ return m_objSlaveInfo;}
		void					SetMasterSessionID(INT32 nSessionID) { m_nMasterSessionID = nSessionID;  }
		INT32					GetMasterSessionID() { return m_nMasterSessionID; }
		INT32					GetLastPos() const { return m_nLastPos; }
		void					SetLastPos(INT32 val) { m_nLastPos = val; }

	private:
		DBSlave				*	m_pDBSlave;
		GameDB::Database	*	m_pDatabase; 
		GameDB::SDBSlaveInfo	m_objSlaveInfo;
		INT32					m_nMasterSessionID;
		INT32					m_nLastPos;
	}; 
}

#endif