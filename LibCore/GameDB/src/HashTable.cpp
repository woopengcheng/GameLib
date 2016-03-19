#include "GameDB/inc/HashTable.h"
#include "CUtil/inc/DeelxRegexp.h"

namespace GameDB
{
	const char  *  g_szGlobalHashtableSizeName = "__hsize__";

	void HashTable::EncodeKey(const Slice& table,const Slice& key,Slice& outEncodedKey , DEFAULT_STACKCHUNK & sChunk)
	{  
		sChunk.Pushback((void*)&PREFIX_HASHTABLE,1);
		sChunk.Pushback((void*)&SEPARATOR_FIRST,1);
		sChunk.Pushback((void*)(table.data()),(UINT32)table.size());
		sChunk.Pushback((void*)&SEPARATOR_SECOND,1);
		sChunk.Pushback((void*)(key.data()),(UINT32)key.size());

		outEncodedKey = Slice((const char *)(sChunk.Begin()) , sChunk.GetDataLen());
	}

	bool HashTable::DecodeKey(const Slice& encodedName,const Slice& table,Slice& key)
	{
		const char* pFullName = encodedName.data();
		size_t unFullNameSize = encodedName.size();

		size_t separatorSecondPos = table.size() + 1 + 1; //5 传递进来的表名+2个分隔符就是Key开始的位置.

		if(pFullName[0] == PREFIX_HASHTABLE 
			&& pFullName[1] == SEPARATOR_FIRST
			&& unFullNameSize > separatorSecondPos
			&& pFullName[separatorSecondPos] == SEPARATOR_SECOND
			&& memcmp(&pFullName[2],table.data(),table.size()) == 0)
		{
			key = Slice(&pFullName[separatorSecondPos+1],unFullNameSize - separatorSecondPos - 1);

			return true;
		}
		return false; 
	}

	void HashTable::HCount_EncodeKey(const Slice& key,Slice & outKey , DEFAULT_STACKCHUNK & stackChunk)
	{  
		EncodeKey(g_szGlobalHashtableSizeName , key , outKey , stackChunk); 
	}

	bool HashTable::HCount_DecodeKey(const Slice& dbFullKey,Slice& hKey)
	{
		return DecodeKey(dbFullKey,g_szGlobalHashtableSizeName,hKey); 

	}

	void HashTable::HCount_Initial(Database &db,const Slice & countKey,INT64 & llOldCount)
	{
		std::string strOldCount;
		Status status = db.QuickGet(countKey,strOldCount);
		if(status.ok())
			llOldCount = *reinterpret_cast<const INT64*>(strOldCount.c_str());
		else
			llOldCount = 0;
	}

	bool HashTable::HCount_IncrIfNonExists(Database &db,const Slice& dbKey,INT64& size)
	{
		std::string oldVal;
		Status status = db.QuickGet(dbKey,oldVal);
		if(status.IsNotFound())
		{
			++size;
			return true;
		}
		return false;

	}

	bool HashTable::HCount_DecrIfExists(Database &db,const Slice& dbKey,INT64& size)
	{
		std::string oldVal;
		Status status = db.QuickGet(dbKey,oldVal);
		if(status.ok())
		{
			--size;
			return true;
		}
		return false;

	}

	void HashTable::HCount_SaveToDB(const Slice & countKey,INT64 countVal,WriteBatch& batch,Operate & or)
	{
		batch.Put(countKey , Slice((const char *)&countVal , sizeof(countVal)));
		or.GetOperateRecord().Insert(countKey , Slice((const char *)&countVal , sizeof(countVal)));
	}

	HashTable::CHECK_RESULT HashTable::CheckExists(Database &db,const Slice& dbKey,const Slice& newValue)
	{
		std::string strOldValue;
		Status status = db.QuickGet(dbKey,strOldValue);
		if(status.IsNotFound())
			return CHECK_RESULT_NOTEXISTS;
		if(!status.ok())
			return CHECK_RESULT_ERROR;

		if(newValue.size() == strOldValue.length() && memcmp(newValue.data(),strOldValue.c_str(),strOldValue.length()) == 0)
			return CHECK_RESULT_SAME;

		return CHECK_RESULT_EXISTS; 
	}

	void HashTable::HSet(Database &db,Operate & oper,const Slice& table,const Slice& key,const Slice& val)
	{
		DEFAULT_STACKCHUNK sc , scSizeKey;
		Slice encodedKey; 
		Status status;
		 
		EncodeKey(table,key,encodedKey , sc);

		INT64 sizeVal = -1;
		Slice sizeKey;
		{
			HCount_EncodeKey(table,sizeKey , scSizeKey);
			HCount_Initial(db,sizeKey,sizeVal);
		}

		switch(CheckExists(db,encodedKey,val))
		{
		case CHECK_RESULT_EXISTS:
			break;
		case CHECK_RESULT_NOTEXISTS:
			++sizeVal;
			break;
		case CHECK_RESULT_SAME:
			{
				oper.SetErrorCode(CErrno::Success());
				return ;
			}
			break;
		}

		WriteBatch batch;
		batch.Put(encodedKey,val);
		oper.GetOperateRecord().Insert(encodedKey,val);

		HCount_SaveToDB(sizeKey,sizeVal,batch,oper);

		status = db.QuickWrite(&batch);
		
		oper.SetErrorCode(status); 
	}

	void HashTable::HSetNX(Database &db,Operate & oper,const Slice& table,const Slice& key,const Slice& val)
	{
		DEFAULT_STACKCHUNK sc , scSizeKey;
		Slice	encodedKey; 
		Status status;

		EncodeKey(table,key,encodedKey , sc);

		INT64 sizeVal = -1;
		Slice sizeKey;
		{
			HCount_EncodeKey(table,sizeKey , scSizeKey);
			HCount_Initial(db,sizeKey,sizeVal);
			if(!HCount_IncrIfNonExists(db,encodedKey,sizeVal))
			{
				oper.SetErrorCode(CErrno::ERR_HAS_EXISTS);
				return ;
			}
		}

		WriteBatch batch;
		batch.Put(encodedKey,val);
		HCount_SaveToDB(sizeKey,sizeVal,batch,oper);

		status = db.QuickWrite(&batch);

		//result.MutableOplog()->Put(encodedKey,val);  //5 这里可能会有问题.
		oper.SetErrorCode(status);

	}

	void HashTable::HSetOW(Database &db,Operate& oper,const Slice& table,const Slice& key,const Slice& val)
	{
		DEFAULT_STACKCHUNK sc;
		Slice	encodedKey; 
		Status status;

		EncodeKey(table,key,encodedKey , sc);

		switch(CheckExists(db,encodedKey,val))
		{
		case CHECK_RESULT_SAME:
			{
				oper.SetErrorCode(CErrno::Success());
				return ;
			};
		} 

		status = db.QuickWrite(encodedKey,val);

		oper.GetOperateRecord().Insert(encodedKey,val);  
		oper.SetErrorCode(status); 
	}

	void HashTable::HGet(Database &db,Operate& oper,const Slice& table,const Slice& key)
	{
		DEFAULT_STACKCHUNK sc;
		Slice	encodedKey;
		Status status;

		EncodeKey(table , key , encodedKey , sc);

		std::string strValue; 
		status = db.QuickGet(encodedKey,strValue); 
		oper.GetOperateReturns().GetStream() << strValue;

		oper.SetErrorCode(status); 
	}

	void HashTable::HDel(Database &db,Operate& oper,const Slice& table,const Slice& key)
	{
		DEFAULT_STACKCHUNK sc , scSizeKey;
		Slice	encodedKey; 
		Status status;

		EncodeKey(table,key,encodedKey , sc);

		INT64 sizeVal = -1;
		Slice sizeKey;
		{
			HCount_EncodeKey(table,sizeKey , scSizeKey);
			HCount_Initial(db,sizeKey,sizeVal);
			if(!HCount_DecrIfExists(db,encodedKey,sizeVal))
			{
				oper.SetErrorCode(CErrno::ERR_HAS_EXISTS);
				return ;
			}
		}

		WriteBatch batch;
		batch.Delete(encodedKey);
		HCount_SaveToDB(sizeKey,sizeVal,batch,oper);

		oper.GetOperateRecord().Delete(encodedKey);
		status = db.QuickWrite(&batch);

		oper.SetErrorCode(status); 
	}

	void HashTable::HDrop(Database &db,Operate& oper,const Slice& table)
	{
		DEFAULT_STACKCHUNK sc , scSizeKey;
		Slice	encodedKey; 
		std::string minkey = "\0";
		Status status;

		EncodeKey(table,minkey,encodedKey , sc);
		
		WriteBatch batch;

		Iterator* iter = db.GetLevelDB()->NewIterator(ReadOptions());
		iter->Seek(encodedKey);

		INT64 llCount = 0;
		while(iter->Valid())
		{
			Slice dbname = iter->key();
			Slice val = iter->value();
			Slice key;
			 
			if(!DecodeKey(dbname,table,key))
				break;

			batch.Delete(dbname);
			oper.GetOperateRecord().Delete(encodedKey);

			++llCount;
			iter->Next();
		}
		delete iter;
		 
		Slice sizeKey; 
		HCount_EncodeKey(table,sizeKey , scSizeKey); 
		oper.GetOperateRecord().Delete(sizeKey);
		batch.Delete(sizeKey);

		status = db.QuickWrite(&batch);

		oper.GetOperateReturns().GetStream() << llCount;

		oper.SetErrorCode(status); 

	}

	void HashTable::HGetAll(Database &db,Operate & oper,const Slice& table , EGetAllType objType)
	{
		DEFAULT_STACKCHUNK sc;
		Slice	encodedKey; 
		std::string minkey = "\0";
		Status status;

		EncodeKey(table,minkey,encodedKey , sc);

		WriteBatch batch;

		Iterator* iter = db.GetLevelDB()->NewIterator(ReadOptions());
		iter->Seek(encodedKey);

		INT32 nCount[GET_ALL_TYPE_NUM] = {0};
		CUtil::CStream cs;
		while(iter->Valid())
		{
			Slice dbname = iter->key();
			Slice val = iter->value();
			Slice key;
			 
			if(!DecodeKey(dbname,table,key))
				break;

			std::string strKey = key.ToString();
			std::string strValue = val.ToString();
			switch(objType)
			{
			case GET_ALL_TYPE_KEYS:
				{
					cs << strKey;
				}break;
			case GET_ALL_TYPE_KEYVALS:
				{
					cs << strKey << strValue;
				}break;
			case GET_ALL_TYPE_VALS:
				{
					cs << strValue;
				}break; 
			default:
				{
					MsgAssert(0 , "unkonwn getall type.");
				}break;
			}
			++nCount[objType];

			iter->Next();
		}
		delete iter;

		oper.GetOperateReturns().GetStream() << nCount[objType]; 
		oper.GetOperateReturns().GetStream().Pushback(cs.Begin() , cs.GetDataLen()); 
		
		oper.SetErrorCode(status); 
	}

	void HashTable::HGetKeys(Database &db,Operate & oper,const Slice& table)
	{
		EGetAllType objType = GET_ALL_TYPE_KEYS;
		HGetAll(db , oper , table , objType);
	}

	void HashTable::HGetVals(Database &db,Operate & oper,const Slice& table)
	{
		EGetAllType objType = GET_ALL_TYPE_VALS;
		HGetAll(db , oper , table , objType); 
	}

	void HashTable::HGetKeyVals(Database &db,Operate & oper,const Slice& table)
	{
		EGetAllType objType = GET_ALL_TYPE_KEYVALS;
		HGetAll(db , oper , table , objType); 
	}

	void HashTable::HMultiSet(Database &db,Operate & oper,const Slice& table,const CollectionKeyValsT & keyvals)
	{
		DEFAULT_STACKCHUNK sc , scSizeKey;
		Slice encodedKey; 
		Status status;
		 
		INT64 sizeVal = -1;
		Slice sizeKey;
		{
			HCount_EncodeKey(table,sizeKey , scSizeKey);
			HCount_Initial(db,sizeKey,sizeVal);
		}

		WriteBatch batch;
		CollectionKeyValsT::const_iterator iter = keyvals.begin();
		for (;iter != keyvals.end();++iter)
		{
			Slice key = iter->first;
			Slice val = iter->second;

			sc.Clear();
			EncodeKey(table,key,encodedKey , sc);
			switch(CheckExists(db,encodedKey,val))
			{
			case CHECK_RESULT_NOTEXISTS:
				++sizeVal;
				break;
			case CHECK_RESULT_SAME:
				{
					continue;
				}
				break;
			}

			batch.Put(encodedKey,val);
			oper.GetOperateRecord().Insert(encodedKey,val);  
		}

		HCount_SaveToDB(sizeKey,sizeVal,batch,oper);

		status = db.QuickWrite(&batch);

		oper.SetErrorCode(status); 
	}

	void HashTable::HMultiGet(Database &db,Operate & oper,const Slice& table,const CollectionSlicesT& keys)
	{
		DEFAULT_STACKCHUNK sc;
		Slice encodedKey; 
		Status status;
		  
		INT64 llCount = 0;
		CUtil::CStream cs;
		CollectionSlicesT::const_iterator iter = keys.begin();
		for (;iter != keys.end();++iter)
		{
			Slice key = *iter;

			sc.Clear();
			EncodeKey(table,key,encodedKey , sc); 

			std::string strValue;
			status = db.QuickGet(encodedKey , strValue);
			cs << strValue;

			++llCount;
		} 

		oper.GetOperateReturns().GetStream() <<llCount;
		oper.GetOperateReturns().GetStream().Pushback(cs.Begin() , cs.GetDataLen());
		oper.SetErrorCode(CErrno::Success()); 

	}

	void HashTable::HMultiDel(Database &db,Operate & oper,const Slice& table,const CollectionSlicesT& keys)
	{ 
		DEFAULT_STACKCHUNK sc , scSizeKey;
		Slice encodedKey; 
		Status status;

		INT64 sizeVal = -1;
		Slice sizeKey;
		{
			HCount_EncodeKey(table,sizeKey , scSizeKey);
			HCount_Initial(db,sizeKey,sizeVal);
		}

		INT32 nCount = 0; 
		WriteBatch batch;
		CollectionSlicesT::const_iterator iter = keys.begin();
		for (;iter != keys.end();++iter)
		{
			Slice key = *iter;

			sc.Clear();
			EncodeKey(table,key,encodedKey , sc); 

			if (HCount_DecrIfExists(db , encodedKey , sizeVal))
			{
				batch.Delete(encodedKey);
				oper.GetOperateRecord().Delete(encodedKey);
				++nCount;
			}
		} 

		HCount_SaveToDB(sizeKey , sizeVal , batch , oper);
		status = db.QuickWrite(&batch);
		
		oper.GetOperateReturns().GetStream() << nCount;
		oper.SetErrorCode(CErrno::Success()); 
	}

	void HashTable::HSetIncr(Database &db,Operate & oper,const Slice& table,const Slice& key,INT64 val)
	{
		DEFAULT_STACKCHUNK sc , scSizeKey;
		Slice encodedKey; 
		Status status;

		EncodeKey(table,key,encodedKey , sc);

		INT64 sizeVal = -1;
		Slice sizeKey;
		{
			HCount_EncodeKey(table,sizeKey , scSizeKey);
			HCount_Initial(db,sizeKey,sizeVal);
		}
		 
		std::string strVal;
		status = db.QuickGet(encodedKey , strVal);

		WriteBatch batch;
		INT64 llValue = 0;
		if (status.ok())
		{
			if (strVal.length() != sizeof(INT64))
			{
				oper.SetErrorCode(CErrno::ERR_INVALID_DATA);
				return;
			}

			llValue = *reinterpret_cast<const INT64*>(strVal.c_str());
		}
		else
		{
			++sizeVal;
			HCount_SaveToDB(sizeKey , sizeVal , batch , oper);
		}
		llValue += val;

		Slice value((const char*)&llValue , sizeof(llValue));

		batch.Put(encodedKey,value);
		oper.GetOperateRecord().Insert(encodedKey,value); 

		status = db.QuickWrite(&batch);

		oper.GetOperateReturns().GetStream() << llValue;
		oper.SetErrorCode(status);  
	}

	void HashTable::HSetIncrFloat(Database &db,Operate & oper,const Slice& table,const Slice& key,double val)
	{
		DEFAULT_STACKCHUNK sc , scSizeKey;
		Slice encodedKey; 
		Status status;

		EncodeKey(table,key,encodedKey , sc);

		INT64 sizeVal = -1;
		Slice sizeKey;
		{
			HCount_EncodeKey(table,sizeKey , scSizeKey);
			HCount_Initial(db,sizeKey,sizeVal);
		}

		std::string strVal;
		status = db.QuickGet(encodedKey , strVal);

		WriteBatch batch;
		double dValue = 0.0f;
		if (status.ok())
		{
			if (strVal.length() != sizeof(INT64))
			{
				oper.SetErrorCode(CErrno::ERR_INVALID_DATA);
				return;
			}

			dValue = *reinterpret_cast<const double*>(strVal.c_str());
		}
		else
		{
			++sizeVal;
			HCount_SaveToDB(sizeKey , sizeVal , batch , oper);
		}
		dValue += val;

		Slice value((const char*)&dValue , sizeof(dValue));

		batch.Put(encodedKey,value);
		oper.GetOperateRecord().Insert(encodedKey,value); 

		status = db.QuickWrite(&batch);
		oper.GetOperateReturns().GetStream() << dValue;

		oper.SetErrorCode(status);  
	}

	void HashTable::HScan(Database &db,Operate & oper,const Slice& table,const Slice& start,const Slice& pattern,INT64 limit, EGetAllType objType)
	{
		if (limit < 0)
		{
			oper.SetErrorCode(CErrno::ERR_INVALID_DATA);
			return;
		}

		DEFAULT_STACKCHUNK sc;
		Slice	encodedKey; 
		std::string minkey = "\0";
		Status status;
		
		if (start.size() == 0)
		{
			EncodeKey(table,minkey,encodedKey , sc);
		}
		else
		{
			EncodeKey(table,start,encodedKey , sc);
		}

		Iterator* iter = db.GetLevelDB()->NewIterator(ReadOptions());
		iter->Seek(encodedKey);

		CRegexpT<char> regexp(pattern.data());

		INT32 nCount[GET_ALL_TYPE_NUM] = {0};
		CUtil::CStream cs;
		while(iter->Valid())
		{
			Slice dbname = iter->key();
			Slice val = iter->value();
			Slice key;

			if(!DecodeKey(dbname,table,key))
				break;

			if (regexp.MatchExact(key.data(), (int)key.size()))
			{
				std::string strKey = key.ToString();
				std::string strValue = val.ToString();
				switch(objType)
				{
				case GET_ALL_TYPE_KEYS:
					{
						cs << strKey;
					}break;
				case GET_ALL_TYPE_KEYVALS:
					{
						cs << strKey << strValue;
					}break;
				case GET_ALL_TYPE_VALS:
					{
						cs << strValue;
					}break; 
				default:
					{
						MsgAssert(0 , "unkonwn getall type.");
					}break;
				}
				++nCount[objType];
				--limit;
			}

			iter->Next();
		}
		delete iter;

		oper.GetOperateReturns().GetStream() << nCount[objType];
		oper.GetOperateReturns().GetStream().Pushback(cs.Begin() , cs.GetDataLen());

		oper.SetErrorCode(CErrno::Success()); 
	}

	void HashTable::HCount(Database &db,Operate & oper,const Slice& table)
	{
		DEFAULT_STACKCHUNK sc , scSizeKey;
		Status status;

		INT64 sizeVal = -1;
		Slice sizeKey;
		{
			HCount_EncodeKey(table,sizeKey , scSizeKey);
			HCount_Initial(db,sizeKey,sizeVal);
		}

		oper.GetOperateReturns().GetStream() << sizeVal;
		oper.SetErrorCode(CErrno::Success());
	}

	void HashTable::HList(Database &db,Operate & oper)
	{
		DEFAULT_STACKCHUNK sc , scSizeKey;
		Status status;

		std::string minkey("\0");  
		Slice sizeKey;
		{
			HCount_EncodeKey(minkey,sizeKey , scSizeKey); 
		}

		INT64 llCount = 0;
		CUtil::CStream cs;
		Iterator* iter = db.GetLevelDB()->NewIterator(ReadOptions());
		iter->Seek(sizeKey);
		while (iter->Valid())
		{
			Slice dbname = iter->key(); 
			Slice key;

			if(!HCount_DecodeKey(dbname,key))
				break;

			std::string strKey = key.ToString();
			cs << strKey;
			
			++llCount;
			iter->Next();
		}

		oper.GetOperateReturns().GetStream() << llCount;
		oper.GetOperateReturns().GetStream().Pushback(cs.Begin() , cs.GetDataLen());
		oper.SetErrorCode(CErrno::Success());
	}

}