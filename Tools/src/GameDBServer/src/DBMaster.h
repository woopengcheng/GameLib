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
	
	typedef std::vector<CUtil::Chunk>						VecSaveCachesT;
	struct SSaveCaches
	{
		VecSaveCachesT	vecSaveCaches;
		INT32			nLastPos;		//5 最后一次更新的位置.
		INT32			nCurPos;		//5 从第一次开始记录的位置

		SSaveCaches()
			: nLastPos(0)
			, nCurPos(0)
		{
			vecSaveCaches.clear();
		}
	};

	class  DBMaster : public GameDB::DBMasterInterface  , ThreadPool::ThreadSustainTask
	{ 
	public:
		typedef std::vector<SlaveRecord *>							VecSlaveRecordsT;
		typedef std_unordered_map<INT32, MasterHandler *>			MapMasterHandlersT;
		typedef std_unordered_map<std::string, SSaveCaches>			MapSaveCachesT;  //5 给每一数据库一个积存池.用来同步slave使用.


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
		INT32					CreateMasterHandler(INT32 nSessionID);
		MasterHandler		*	GetMasterHandler(INT32 nID);
		INT32					CreateSlaveRecord(INT32 nMasterID, INT32 nSessionID, const std::string & strDBName);
		SlaveRecord			*	GetSlaveRecord(INT32 nMasterID, const std::string & strDBName);
		bool					GetSlaveRecord(const std::string & strDBName , VecSlaveRecordsT & vecSlaveRecords);
		INT64					GetServerID() const { return m_llServerID; }
		void					SetServerID(INT64 nID) { m_llServerID = nID; }
		INT32					InsertSaveCache(const std::string & strDBName, CUtil::Chunk objValue);
		BOOL					RefreshSaveCache(const std::string & strDBName);
		BOOL					GetSaveCache(const std::string & strDBName, INT32 nSlaveID, INT32 nSessionID, INT32 nLastPos, VecSaveCachesT & vec);

	private: 
		CErrno					InitThread(Json::Value & conf);

	private:
		INT64					m_llServerID;
		INT32					m_nHandlerCount;
		MapMasterHandlersT		m_mapMasterHandlers;
		MapSaveCachesT			m_mapSaveCaches;
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