#ifndef __gamedb_master_handler_h__
#define __gamedb_master_handler_h__

#include "MsgLib/inc/IRpcMsgCallableObject.h"
#include "MsgLib/inc/RpcManager.h"
#include "MsgLib/inc/Object.h"
#include "GameDB/inc/User.h"
#include "RpcDefines.h" 
#include "MsgNameDefine.h"  
#include "DBMaster.h" 
#include "SlaveRecord.h"

namespace Server
{  
	class MasterHandler : public Msg::IRpcMsgCallableObject
	{ 
 		RPC_DEFINE_MasterHandler;
	public:
		typedef std_unordered_map<std::string, SlaveRecord *>		MapSlaveRecordsT;

	public:
		MasterHandler(Msg::Object objID , DBMaster * pDBMaster)
			: Msg::IRpcMsgCallableObject(objID , pDBMaster->GetRpcManager())
			, m_pDBMaster(pDBMaster)
			, m_nRecordCount(0)
			, m_nDBSlaveSessionID(0)
		{}
		~MasterHandler();

	public:
		bool						SendFile(INT32 nSessionID , const std::string & strFilePath , std::string & strFileName);
		DBMaster				*	GetDBMaster(){ return m_pDBMaster; }
		void						StartSyncToSlave(std::string strDBDir);
		INT32						CreateSlaveRecord(const std::string & strDBName, INT32 nSessionID);
		BOOL						SetSlaveRecordInfo(const std::string & strDBName, GameDB::User & objUser);
		BOOL						DelSlaveRecord(const std::string & strDBName);
		SlaveRecord				*	GetSlaveRecord(const std::string & strDBName);
		SlaveRecord				*	GetSlaveRecordBySessionID(INT32 nSessionID);
		SlaveRecord				*	GetSlaveRecordBySlaveID(INT64 nSlaveID);
		void						SetDBSlaveSessionID(INT32 nSessionID) { m_nDBSlaveSessionID = nSessionID; }
		INT32						GetDBSlaveSessionID() { return m_nDBSlaveSessionID; }
		BOOL						SyncDataToSlave(const std::string & strDBName);

	private:
		DBMaster				*	m_pDBMaster;
		MapSlaveRecordsT			m_mapSlaveRecords;
		INT32						m_nRecordCount;
		INT32						m_nDBSlaveSessionID;
	}; 
}

#endif