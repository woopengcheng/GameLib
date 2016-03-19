#include "GameDB/inc/RemoteNodeDefine.h"
#include "OrmHelper.h"
#include "RpcCallFuncs.h"
#include "bson/bson.h"

namespace GameDB
{ 
	CErrno OrmHelper::OrmInsert(GameDB::Orm * obj , EORM_MASK objMask /*= ORM_NONE*/)
	{
		MsgAssert_ReF(obj , "error obj.");

		return Commit(obj,OPERATION_INSERT); 
	}

	CErrno OrmHelper::OrmUpdate(GameDB::Orm * obj , EORM_MASK objMask /*= ORM_NONE*/)
	{
		MsgAssert_ReF(obj , "error obj.");

		INT64 llOldHash = obj->CurHash();
		INT64 llNewHash = obj->HashMake(DEFAULT_HASH_SEED);

		if (llOldHash == llNewHash)
		{
			gDebugStream("OrmUpdate table: " << obj->GetTableName() << " key:" << obj->GetKey() << "is same hash.");
			
			return CErrno::Failure();
		}

		obj->HashUpdate(llNewHash);
		return Commit(obj,OPERATION_UPDATE); 
	}

	CErrno OrmHelper::OrmDelete(GameDB::Orm * obj , EORM_MASK objMask /*= ORM_NONE*/)
	{
		MsgAssert_ReF(obj , "error obj.");

		return Commit(obj,OPERATION_DELETE); 
	}

	CErrno OrmHelper::Commit(GameDB::Orm * obj , EORM_OPERATION objOper)
	{
		MsgAssert_ReF(obj , "error obj.");

		switch (objOper)
		{
		case OPERATION_INSERT:
			{
				std::string strTable , strKey , strValue;
				strTable = obj->GetTableName();
				strKey   = obj->GetKey();
				obj->ToBson(strValue);

	//			_bson::bsonobj objValue;
	//			obj->ToBson(objValue);
	//			
	//			CUtil::Chunk objChunk(objValue.objdata(),objValue.objsize()).;

				//5 这里调试直接往服务器写.但是真实情况这么写就会出问题.因为这里会卡住.
				std::vector<Msg::Object> targets;
				targets.push_back(Msg::Object(1));   
				Client::rpc_HandleHSet(g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER] , targets , Msg::Object(0) , strTable , strKey , strValue, NULL, 1);
			}
			break;
		case OPERATION_UPDATE:
			{ 
				std::string strTable , strKey ;
				strTable = obj->GetTableName();
				strKey   = obj->GetKey(); 

				//5 这里调试直接往服务器写.但是真实情况这么写就会出问题.因为这里会卡住.同时value为string应该不好.
				std::vector<Msg::Object> targets;
				targets.push_back(Msg::Object(1));   
				Client::rpc_HandleHGet(g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], targets , Msg::Object(0) , strTable , strKey , NULL , 1);

			}
			break;
		case OPERATION_DELETE:
			break;
		default:
			break;
		}
		
		return CErrno::Success();
	}

}