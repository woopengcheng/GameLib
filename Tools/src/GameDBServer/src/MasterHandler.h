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
		typedef std::map<std::string , SlaveRecord *> CollectionSlaveRecordsT;

	public:
		MasterHandler(Msg::Object objID , DBMaster * pDBMaster)
			: Msg::IRpcMsgCallableObject(objID , pDBMaster->GetRpcManager())
			, m_pDBMaster(pDBMaster)
		{}
		~MasterHandler();

	public:
		bool						SendFile(INT32 nSessionID , const std::string & strFilePath , std::string & strFileName);
		DBMaster				*	GetDBMaster(){ return m_pDBMaster; }
		void						StartSyncToSlave(std::string strDBDir);
		void						CreateSlaveRecord(const std::string & strDBName, INT32 nSessionID);
		BOOL						SetSlaveRecordInfo(const std::string & strDBName, GameDB::User & objUser);
		BOOL						DelSlaveRecord(const std::string & strDBName);
		SlaveRecord				*	GetSlaveRecord(const std::string & strDBName);
		SlaveRecord				*	GetSlaveRecordBySessionID(INT32 nSessionID);
		SlaveRecord				*	GetSlaveRecordBySlaveID(INT64 nSlaveID);

	private:
		DBMaster				*	m_pDBMaster;
		CollectionSlaveRecordsT		m_mapSlaveRecords;
	}; 
}

#endif