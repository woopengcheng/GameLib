#ifndef __client_client_orm_helper_h__
#define __client_client_orm_helper_h__
#include "GameDB/inc/OrmHelper.h"
#include "GameDB/inc/OrmCollection.h"
#include "RPCCallFuncs.h"
#include "GameDB/inc/RemoteNodeDefine.h"

namespace Client
{
	typedef INT32(*OrmHandleHSetOW)(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & table, std_string & key, std_string & value, Msg::RpcCallbackPtr pCallback/* = NULL*/, UINT16 usPriority/* = 0*/, Msg::EMSG_SYNC_TYPE objSyncType/* = Msg::SYNC_TYPE_SYNC*/);
	typedef INT32(*OrmHandleHSet)(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & table, std_string & key, std_string & value, Msg::RpcCallbackPtr pCallback/* = NULL*/, UINT16 usPriority/* = 0*/, Msg::EMSG_SYNC_TYPE objSyncType /*= Msg::SYNC_TYPE_SYNC*/);
	typedef INT32(*OrmHandleHDel)(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & table, std_string & key, Msg::RpcCallbackPtr pCallback/* = NULL*/, UINT16 usPriority/* = 0*/, Msg::EMSG_SYNC_TYPE objSyncType/* = Msg::SYNC_TYPE_SYNC*/);
	typedef INT32(*OrmHandleHGet)(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & table, std_string & key, Msg::RpcCallbackPtr pCallback/* = NULL*/, UINT16 usPriority/* = 0*/, Msg::EMSG_SYNC_TYPE objSyncType/* = Msg::SYNC_TYPE_SYNC*/);
	typedef INT32(*OrmHandleHGetKeyVals)(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & table, Msg::RpcCallbackPtr pCallback/* = NULL*/, UINT16 usPriority/* = 0*/, Msg::EMSG_SYNC_TYPE objSyncType/* = Msg::SYNC_TYPE_SYNC*/);

	static CErrno OrmInsert(GameDB::Orm * pObj, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		OrmHandleHSetOW pFunc = rpc_HandleHSetOW;
		return GameDB::OrmHelper::OrmInsert<>(pObj, pFunc, g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], DBClient::GetInstance().GetServerID() ,  Msg::Object(0) , objMask);
	}
	static CErrno OrmUpdate(GameDB::Orm * pObj, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		OrmHandleHSet pFunc = rpc_HandleHSet;
		return GameDB::OrmHelper::OrmUpdate<>(pObj, pFunc, g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], DBClient::GetInstance().GetServerID(), Msg::Object(0), objMask);
	}
	static CErrno OrmDelete(GameDB::Orm * pObj, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		OrmHandleHDel pFunc = rpc_HandleHDel;
		return GameDB::OrmHelper::OrmDelete<>(pObj, pFunc, g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], DBClient::GetInstance().GetServerID(), Msg::Object(0), objMask);
	}
	static CErrno OrmQuery(GameDB::OrmCollectionBase * pCollection , GameDB::Orm * pObj, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		if (!pCollection || !pObj || (pObj->GetSyncState() != GameDB::SYNC_STATE_INVALID && pObj->GetSyncState() != GameDB::SYNC_STATE_END))
		{
			return CErrno::Failure();
		}
		else
		{
			pObj->SetSyncState(GameDB::SYNC_STATE_START);
		}

		pCollection->OrmQueryInsertTable(pObj->GetTableName());
		GameDB::CollectionQueryPtr pCallback(new GameDB::CollectionQuery(pCollection));
		OrmHandleHGet pFunc = rpc_HandleHGet;
		return GameDB::OrmHelper::OrmQuery<>(pObj, pFunc, g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], DBClient::GetInstance().GetServerID(), Msg::Object(0), pCallback , objMask);
	}

	static CErrno OrmQuery(GameDB::OrmCollectionBase * pCollection, std::string table , INT64 key, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		if (!pCollection || pCollection->IsTableInOrmQuery(table))
		{
			return CErrno::Failure();
		}
		pCollection->OrmQueryInsertTable(table);
		GameDB::CollectionQueryPtr pCallback(new GameDB::CollectionQuery(pCollection));

		OrmHandleHGetKeyVals pFunc = rpc_HandleHGetKeyVals;
		std::string strTable = table;
		strTable += SLAVE_TABLE_SPECIAL_ID_TAG;
		strTable += CUtil::itoa(key);
		return GameDB::OrmHelper::OrmQuery<>(strTable, pFunc, g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], DBClient::GetInstance().GetServerID(), Msg::Object(0), pCallback, objMask);
	}

	static CErrno OrmQuery(GameDB::OrmCollectionBase * pCollection, std::string table, std::string key, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		if (!pCollection || pCollection->IsTableInOrmQuery(table))
		{
			return CErrno::Failure();
		}
		pCollection->OrmQueryInsertTable(table);
		GameDB::CollectionQueryPtr pCallback(new GameDB::CollectionQuery(pCollection));

		OrmHandleHGetKeyVals pFunc = rpc_HandleHGetKeyVals;
		std::string strTable = table;
		strTable += SLAVE_TABLE_SPECIAL_ID_TAG;
		strTable += key;
		return GameDB::OrmHelper::OrmQuery<>(strTable, pFunc, g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], DBClient::GetInstance().GetServerID(), Msg::Object(0), pCallback, objMask);
	}

	static CErrno OrmQuery(GameDB::OrmCollectionBase * pCollection, const std::string strTable , GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		if (!pCollection)
		{
			return CErrno::Failure();
		}
		
		GameDB::Orm * pTable = pCollection->GetTable(strTable);
		if (pTable)
		{
			return OrmQuery(pCollection, pTable);
		}

		return CErrno::Failure();
	}

	static CErrno OrmQuery(GameDB::OrmCollectionBase * pCollection, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		if (!pCollection)
		{
			return CErrno::Failure();
		}

		//5 查询子表
		CErrno err;
		const std::vector<std::string> & vec = pCollection->GetTables();
		std::vector<std::string>::const_iterator iter = vec.begin();
		for (; iter != vec.end(); ++iter)
		{
			std::string strTable = *iter;
			GameDB::Orm * pTable = pCollection->GetTable(strTable);
			if (pTable)
			{
				err = OrmQuery(pCollection, pTable);
			}
			if (err.IsFailure())
			{
				gErrorStream("query error. name=" << strTable);
			}
		}

		//5 查询子表数组
		const std::vector<std::string> & vecSlaves = pCollection->GetSlaveTables();
		iter = vecSlaves.begin();
		for (; iter != vecSlaves.end(); ++iter)
		{
			std::string strTable = *iter;

			GameDB::OrmCollection<INT64> * pInt64 = dynamic_cast<GameDB::OrmCollection<INT64> *>(pCollection);
			GameDB::OrmCollection<std::string> * pString = dynamic_cast<GameDB::OrmCollection<std::string> *>(pCollection);
			if (pInt64)
			{
				err = OrmQuery(pCollection, strTable, pInt64->GetMasterID());
			}
			else if (pString)
			{
				err = OrmQuery(pCollection, strTable, pString->GetMasterID());
			}

			if (err.IsFailure())
			{
				gErrorStream("query error. name=" << strTable);
			}
		}
	
		return err;
	}
}


#endif