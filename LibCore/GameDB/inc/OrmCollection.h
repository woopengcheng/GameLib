#ifndef __gamedb_orm_collection_h__
#define __gamedb_orm_collection_h__
#include "GameDB/inc/DBCommon.h"
#include "GameDB/inc/OrmHelper.h"
#include "MsgLib/inc/RPCMsgCall.h"
#include <string>
#include "bson/bson.h"

namespace GameDB
{

	class OrmCollectionBase
	{
	public:
		typedef std_unordered_map<std::string, GameDB::EORM_SYNC_STATE>	MapCollecteStatesT;
		typedef std::vector<std::string>	VecTablesT;

	public:
		OrmCollectionBase()
			: m_syncState(SYNC_STATE_INVALID)
		{
		}

	public:
		virtual void			ToBson(std::string & strBuf) {};
		virtual void			ToBson(mongo::BSONObj  & objBson) {};
		virtual void			FromBson(std::string & compressedBuf) {};
		virtual void			FromBson(const char * pData, UINT32 nSize) {};
		virtual void			LoadBson(std::string & compressedBuf) {};
		virtual void			LoadBson(const char * pData, UINT32 nSize) {};
		virtual	GameDB::Orm	*	GetTable(const std::string & strTable) { return NULL; };

	public:
		const VecTablesT	&	GetTables() { return m_vecTables; }
		const VecTablesT	&	GetSlaveTables() { return m_vecSlaveTables; }
		EORM_SYNC_STATE			GetSyncState() const { return m_syncState; }
		void					SetSyncState(GameDB::EORM_SYNC_STATE val) { m_syncState = val; }
		BOOL					IsTableInOrmQuery(const std::string & table)
		{
			MapCollecteStatesT::iterator iter = m_mapCollectStates.find(table);
			if (iter != m_mapCollectStates.end())
			{
				return TRUE;
			}
			return FALSE;
		}

		BOOL				OrmQueryInsertTable(const std::string & table)
		{
			MapCollecteStatesT::iterator iter = m_mapCollectStates.find(table);
			if (iter != m_mapCollectStates.end())
			{
				return FALSE;
			}
			else
			{
				m_mapCollectStates.insert(std::make_pair(table, GameDB::SYNC_STATE_START));
			}
			return TRUE;
		}

		BOOL				OrmQueryDeleteTable(const std::string & table)
		{
			MapCollecteStatesT::iterator iter = m_mapCollectStates.find(table);
			if (iter != m_mapCollectStates.end())
			{
				m_mapCollectStates.erase(iter);
				return TRUE;
			}
			return FALSE;
		}

	protected:
		EORM_SYNC_STATE		m_syncState;
		MapCollecteStatesT	m_mapCollectStates;
		VecTablesT			m_vecTables;
		VecTablesT			m_vecSlaveTables;
	};


	class CollectionQuery : public Msg::RpcCallback
	{
	public:
		explicit CollectionQuery(OrmCollectionBase * pCollection)
			: m_pCollection(pCollection)
		{
		}
		~CollectionQuery()
		{
		}

	public:
		virtual INT32 OnCall(void * pContext)
		{
			if (m_pCollection)
			{
				STableKeyVal * pObj = (STableKeyVal *)pContext;
				if (m_pCollection != NULL && pObj != NULL)
				{
					m_pCollection->LoadBson(pObj->strVal.c_str(), (UINT32)pObj->strVal.length());

					mongo::BSONObj obj(pObj->strVal.c_str());
					Assert_ReF1(obj.objsize() == pObj->strVal.length());
					std::string metaname = GameDB::OrmHelper::GetTableNameFromBson(obj);
					m_pCollection->OrmQueryDeleteTable(metaname);
					GameDB::Orm * pOrm = m_pCollection->GetTable(metaname);
					if (pOrm)
					{
						pOrm->SetSyncState(SYNC_STATE_END);
					}
				}
			}
			return 0;
		}

	public:
		OrmCollectionBase * m_pCollection;
	};
	DECLARE_BOOST_POINTERS(CollectionQuery);

	//5 这里MasterType只能为INT64或者string,其他类型需要更改工具。
	template<typename MasterType>
	class OrmCollection : public OrmCollectionBase
	{
	public:
		OrmCollection()
		{ 
		}
	};

	
	template<>
	class OrmCollection<INT64> : public OrmCollectionBase
	{
	public:
		OrmCollection()
			: m_vMasterId(0)
		{ 
		}

	public:
		void				SetMasterID(INT64 id) { m_vMasterId = id; }
		INT64				GetMasterID() { return m_vMasterId; } 

	protected:
		INT64				m_vMasterId;
	};

	template<>
	class OrmCollection<std::string> : public OrmCollectionBase
	{
	public:
		OrmCollection()
			: m_vMasterId("")
		{ 
		}

	public:
		void				SetMasterID(std::string id) { m_vMasterId = id; }
		std::string			GetMasterID() { return m_vMasterId; }

	protected:
		std::string			m_vMasterId;
	};


}

#endif
