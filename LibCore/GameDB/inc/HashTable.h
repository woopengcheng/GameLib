#ifndef __gamedb_hashtable_h__
#define __gamedb_hashtable_h__ 
#include "GameDB/inc/DBCommon.h"
#include "GameDB/inc/Database.h"
#include "GameDB/inc/Operate.h"
#include "GameDB/inc/OperateReturns.h"

namespace GameDB
{  
	class DLL_EXPORT HashTable
	{
	public:
		static void EncodeKey(const Slice& table,const Slice& key,Slice& outEncodedKey , DEFAULT_STACKCHUNK & sChunk);
		static bool DecodeKey(const Slice& encodedName,const Slice& table,Slice& key);
		  
		static void HCount_EncodeKey(const Slice& key, Slice & outKey, DEFAULT_STACKCHUNK & stackChunk);
		static bool HCount_DecodeKey(const Slice& dbFullKey,Slice& hKey);
		static void HCount_Initial(Database &db,const Slice & countKey,INT64 & llOldCount);
		static bool HCount_IncrIfNonExists(Database &db,const Slice& dbKey,INT64& size);
 		static bool HCount_DecrIfExists(Database &db,const Slice& dbKey,INT64& size);
		static void HCount_SaveToDB(const Slice & countKey,INT64 countVal,WriteBatch& batch,Operate & or);
		

		enum CHECK_RESULT
		{
			CHECK_RESULT_NOTEXISTS,
			CHECK_RESULT_SAME,
			CHECK_RESULT_EXISTS,
			CHECK_RESULT_ERROR,
		};
		static CHECK_RESULT CheckExists(Database &db,const Slice& dbKey,const Slice& newValue);

		static void HSet(Database &db,Operate & oper,const Slice& table,const Slice& key,const Slice& val);
		static void HSetNX(Database &db,Operate & oper,const Slice& table,const Slice& key,const Slice& val);
 		static void HSetOW(Database &db,Operate& oper,const Slice& table,const Slice& key,const Slice& val);  //5 只写入数据,不写入计数
		static void HGet(Database &db,Operate& oper,const Slice& table,const Slice& key);
		static void HDel(Database &db,Operate& oper,const Slice& table,const Slice& key);

		enum EGetAllType
		{ 
			GET_ALL_TYPE_KEYS , 
			GET_ALL_TYPE_VALS , 
			GET_ALL_TYPE_KEYVALS , 
			GET_ALL_TYPE_NUM ,
		};
		static void HGetAll(Database &db,Operate & oper,const Slice& table , EGetAllType objType);
		static void HGetKeys(Database &db,Operate & oper,const Slice& table);
		static void HGetKeyVals(Database &db,Operate & oper,const Slice& table);
		static void HGetVals(Database &db,Operate & oper,const Slice& table);

		static void HDrop(Database &db,Operate& oper,const Slice& table);

 		static void HMultiSet(Database &db,Operate & oper,const Slice& table,const CollectionKeyValsT& keyvals);
 		static void HMultiGet(Database &db,Operate & oper,const Slice& table,const CollectionSlicesT& keys);
 		static void HMultiDel(Database &db,Operate & oper,const Slice& table,const CollectionSlicesT& keys);

		static void HSetIncr(Database &db,Operate & oper,const Slice& table,const Slice& key,INT64 val);
		static void HSetIncrFloat(Database &db,Operate & oper,const Slice& table,const Slice& key,double val);
 
 		static void HScan(Database &db,Operate & oper,const Slice& table,const Slice& start,const Slice& pattern,INT64 limit, EGetAllType objType);
 
		static void HCount(Database &db,Operate & oper,const Slice& table);
		static void HList(Database &db,Operate & oper);
 
// 		static void CrossAll(Database &db,OperationResultMrefkeyval& result,const Slice& name);
// 		static void CrossAll(Database &db,OperationResultCallback& result,const Slice& name);
// 
	protected:
	private:
	};
	  
}

#endif
