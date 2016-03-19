#ifndef __gamedb_sorted_set_h__
#define __gamedb_sorted_set_h__ 
#include "GameDB/inc/DBCommon.h"
#include "GameDB/inc/Database.h"
#include "GameDB/inc/Operate.h"
#include "GameDB/inc/OperateReturns.h"

namespace GameDB
{  
	class DLL_EXPORT SortedSet
	{
	public:
		static void EncodeKey(const Slice& table,const Slice& key,Slice& outEncodedKey , DEFAULT_STACKCHUNK & sChunk);
		static bool DecodeKey(const Slice& encodedName,const Slice& name,Slice& key);
		static void EncodeScoreKey(const Slice& table,const Slice& key , INT64 score,Slice& outEncodedKey , DEFAULT_STACKCHUNK & sChunk);
		static bool DecodeScoreKey(const Slice& dbKeyData,const Slice& table,Slice& outkey,INT64& outscore);
		static void EncodeRScoreKey(const Slice& table,const Slice& key , INT64 score,Slice& outEncodedKey , DEFAULT_STACKCHUNK & sChunk);
		static bool DecodeRScoreKey(const Slice& dbKeyData,const Slice& table,Slice& outkey,INT64& outscore);
		 
		static void ZCount_EncodeKey(const Slice& key, Slice & outKey, DEFAULT_STACKCHUNK & stackChunk);
		static bool ZCount_DecodeKey(const Slice& dbKeyData,Slice& outKey);
		static void ZCount_Initial(Database &db,const Slice & countKey,INT64 & llOldCount);
		static bool ZCount_IncrIfNonExists(Database &db,const Slice& dbKey,INT64& size);
		static bool ZCount_DecrIfExists(Database &db,const Slice& dbKey,INT64& size);
		static void ZCount_SaveToDB(const Slice & countKey,INT64 countVal,WriteBatch& batch,Operate & or);
	 
		static void ZSet(Database &db,Operate& oper,const Slice& table,const Slice& key , INT64 llScore);		 
		static void ZGet(Database &db,Operate & oper,const Slice& table,const Slice& key);	 
		static void ZDel(Database &db,Operate & oper,const Slice& table,const Slice& key);

		static void ZTop(Database &db,Operate & oper,const Slice& table,INT64 llStart , INT64 llEnd , INT64 llLimit);
		static void ZRTop(Database &db,Operate & oper,const Slice& table,INT64 llStart , INT64 llEnd , INT64 llLimit);

		static void ZDrop(Database &db,Operate & oper,const Slice& table);
		static void ZCount(Database &db,Operate & oper,const Slice& table);
		static void ZList(Database &db,Operate & oper);


	};

}

#endif
