#ifndef __server_db_master_h__
#define __server_db_master_h__  
#include "MsgLib/inc/RpcInterface.h"
#include "GameDB/inc/DBMasterInterface.h"
#include "ThreadPool/inc/ThreadTask.h"
#include "GameDB/inc/HashTable.h"
#include "GameDB/inc/Environment.h"
#include "GameDB/inc/Database.h"
#include "GameDB/inc/Operate.h"
#include "json/json.h"

namespace Server
{  
	class MasterHandler;
	class SlaveRecord;

	class  DBMaster : public GameDB::DBMasterInterface  , ThreadPool::ThreadSustainTask
	{ 
	public:
		typedef std::vector<MasterHandler *> VecMasterHandlersT;

	public:
		DBMaster(void) ;
		virtual ~DBMaster(void);

	public:
		static DBMaster & GetInstance()
		{
			static DBMaster m_sRpcInterface;
			return m_sRpcInterface;
		} 

	public: 
		virtual CErrno			Init(Json::Value & conf) override;
		virtual CErrno			Cleanup(void) override;
		virtual void			OnRegisterRpcs(void) override;
		virtual CErrno			Update(void) override;

	public:
		INT32					CreateMasterHandler(INT32 nSessionID , const std::string & strDBName);
		SlaveRecord			*	GetSlaveRecord(const std::string & strDBName);
		INT64					GetServerID() const { return m_llServerID; }
		void					SetServerID(INT64 nID) { m_llServerID = nID; }

	private: 
		CErrno					InitThread(Json::Value & conf);

	private:
		INT64					m_llServerID;
		INT32					m_nHandlerCount;
		VecMasterHandlersT		m_vecMasterHandlers; 
		MasterHandler		*	m_pMasterHandler;

	};  
	 

	class MasterListener : public Msg::IRpcListener
	{
	public:
		MasterListener(DBMaster * pMaster)
			: m_pDBMaster(pMaster)
		{

		}

	public:
		virtual CErrno OnConnected(Msg::RpcInterface * pRpcInterface , INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false) override;
		virtual CErrno OnDisconnected(Msg::RpcInterface * pRpcInterface , INT32 nSessionID, INT32 nPeerSessionID) override;

	private:
		DBMaster * m_pDBMaster;
	};


}


#endif