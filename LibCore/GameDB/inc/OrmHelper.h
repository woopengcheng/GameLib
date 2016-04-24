#ifndef __gamedb_orm_helper_h__
#define __gamedb_orm_helper_h__
#include "GameDB/inc/DBCommon.h"
#include "GameDB/inc/Orm.h"

namespace GameDB
{
	//5 特殊用途,还不知道做什么.
	enum EORM_MASK
	{
		ORM_NONE = 0,
	};

	enum EORM_OPERATION
	{
		OPERATION_INSERT,
		OPERATION_UPDATE,
		OPERATION_DELETE,

		OPERATION_MAX,
	};
	class OrmHelper
	{
	public:
		template<typename FUNC, typename NodeName, typename Target, typename Source>
		static CErrno OrmInsert(GameDB::Orm * obj, FUNC func, NodeName nodeName, Target target, Source src, EORM_MASK objMask = ORM_NONE)
		{
			MsgAssert_ReF(obj, "OrmInsert error obj.");

			std::string strTable, strKey, strValue;
			strTable = obj->GetTableName();
			strKey = obj->GetKey();
			obj->ToBson(strValue);

			//5 这里调试直接往服务器写.但是真实情况这么写就会出问题.因为这里会卡住.所以改为异步写
			INT32 nRes = func(nodeName, target, src, strTable, strKey, strValue, NULL, 0, Msg::SYNC_TYPE_ASYNC);
			if (nRes == -1)
			{
				return CErrno::Failure();
			}
			return CErrno::Success();
		}

		template<typename FUNC, typename NodeName, typename Target, typename Source>
		static CErrno OrmUpdate(GameDB::Orm * obj, FUNC func, NodeName nodeName, Target target, Source src, EORM_MASK objMask = ORM_NONE)
		{
			MsgAssert_ReF(obj, "OrmUpdate error obj.");

			INT64 llOldHash = obj->CurHash();
			INT64 llNewHash = obj->HashMake(DEFAULT_HASH_SEED);

			if (llOldHash == llNewHash)
			{
				gDebugStream("OrmUpdate table: " << obj->GetTableName() << " key:" << obj->GetKey() << "is same hash.");

				return CErrno::Failure();
			}

			obj->HashUpdate(llNewHash);  //5 有个潜规则.如果是clone了一份.那么这个hash是一样的.那么则需要强制调用hashupdate(0)才行

			std::string strTable, strKey, strValue;
			strTable = obj->GetTableName();
			strKey = obj->GetKey();
			obj->ToBson(strValue);

			//5 这里调试直接往服务器写.但是真实情况这么写就会出问题.因为这里会卡住.所以改为异步写
			INT32 nRes = func(nodeName, target, src, strTable, strKey, strValue, NULL, 0, Msg::SYNC_TYPE_ASYNC);
			if (nRes == -1)
			{
				return CErrno::Failure();
			}
			return CErrno::Success();
		}

		//5 这里不需要调用底层的commit.因为func不一致
		template<typename FUNC, typename NodeName, typename Target, typename Source>
		static CErrno OrmDelete(GameDB::Orm * obj , FUNC func, NodeName nodeName, Target target, Source src, EORM_MASK objMask = ORM_NONE)
		{
			MsgAssert_ReF(obj, "OrmDelete error obj.");

			std::string strTable, strKey;
			strTable = obj->GetTableName();
			strKey = obj->GetKey();

			//5 这里调试直接往服务器写.但是真实情况这么写就会出问题.因为这里会卡住.所以改为异步写
			func(nodeName, target, src, strTable, strKey, NULL, 0, Msg::SYNC_TYPE_ASYNC);

			return CErrno::Success();
		}
	
		template<typename FUNC, typename NodeName, typename Target, typename Source , typename QFUNC>
		static CErrno OrmQuery(GameDB::Orm * obj , FUNC func, NodeName nodeName, Target target, Source src, QFUNC qfunc, EORM_MASK objMask = ORM_NONE)
		{
			MsgAssert_ReF(obj, "OrmQuery error obj.");

			std::string strTable, strKey;
			strTable = obj->GetTableName();
			strKey = obj->GetKey();

			//5 这里调试直接往服务器写.但是真实情况这么写就会出问题.因为这里会卡住.所以改为异步写
			func(nodeName, target, src, strTable, strKey, qfunc, 0, Msg::SYNC_TYPE_ASYNC);

			return CErrno::Success();
		}

		static std::string GetTableNameFromBson(const char * data, size_t size)
		{
			mongo::BSONObj obj(data);
			MsgAssert_Re(obj.objsize() == size, NULL, "");
			return GetTableNameFromBson(obj);
		}

		static std::string GetTableNameFromBson(bson::bo & obj)
		{
			std::string strTableName;
			mongo::BSONObjIterator iter(obj);
			while (iter.more())
			{
				mongo::BSONElement be = iter.next();
				if (CUtil::strcmp(be.fieldName(), "_T") == 0)
				{
					strTableName = be.str();
					break;
				}
			}
			return strTableName;
		}

	};
}


#endif