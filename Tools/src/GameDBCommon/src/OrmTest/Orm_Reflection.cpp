#include "GameDB/inc/OrmHelper.h"
#include "CUtil/inc/BsonToCpp.h"
#include "Orm_Reflection.h"
#include "Orm_Test_OldName.h"
#include "Orm_Test_PrimaryKey.h"
#include "Orm_TestSlave.h"
#include "Orm_TestSlaveFrom.h"
#include "Orm_TestSlaveTable.h"

namespace Orm
{
	const char* cst_slaves_of_TestSlave[] = 
 	{
		"TestSlaveFrom",
		"TestSlaveTable",
		NULL 
	};

	INT64 cst_slaves_hash_of_TestSlave[] = 
 	{
		5701140434439,
		6061817222949,
		NULL 
	};

	GameDB::Orm * CreateObject(const char * data,size_t size , std::string & metaname)
	{
		mongo::BSONObj obj(data);
		MsgAssert_Re(obj.objsize() == size , NULL , "");
		return CreateObject(obj , metaname);
	}

	GameDB::Orm * CreateObject(bson::bo& obj , std::string & metaname)
	{
		mongo::BSONObjIterator iter(obj);
		while(iter.more())
		{
			mongo::BSONElement be = iter.next();
			if(CUtil::strcmp(be.fieldName(),"_T") == 0)
			{
				metaname = be.str();
				break;
			}
		}
		GameDB::Orm * result = NULL;
		INT64 hash = CUtil::BKDRHashSum(metaname.c_str());
		switch(hash)
		{
		case Orm::TestSlaveTable::TableHash:
			result = new Orm::TestSlaveTable();
		break;
		case Orm::TestSlaveFrom::TableHash:
			result = new Orm::TestSlaveFrom();
		break;
		case Orm::TestSlave::TableHash:
			result = new Orm::TestSlave();
		break;
		}

		if(result != NULL)
			result->FromBson(obj);
		return result;
	}
	BOOL GetSlaveTableMasterIDFromBson(const char * data,size_t size, const std::string & strTableName ,const std::string & key , INT64 & intValue , std::string & strValue)
	{
		mongo::BSONObj obj(data);
		MsgAssert_Re(obj.objsize() == size , NULL , "");
		return GetSlaveTableMasterIDFromBson(obj , strTableName , key , intValue , strValue);
	}

	BOOL GetSlaveTableMasterIDFromBson(bson::bo & obj , const std::string & strTableName ,const std::string & key , INT64 & intValue , std::string & strValue)
	{
		std::string strParaseTableName = GameDB::OrmHelper::GetTableNameFromBson(obj);
		if (strParaseTableName != strTableName)
		{
			return NULL;
		}
		mongo::BSONObjIterator  iter(obj); 
		while(iter.more())
		{
			mongo::BSONElement be = iter.next();
			const char* fieldName = be.fieldName();
			INT64 hash = CUtil::BKDRHashSum(fieldName);
			INT64 keyHash = CUtil::BKDRHashSum(key.c_str());
			if(keyHash != hash){ continue; }
			switch(hash)
			{
				case 1095218475535: // TestSlaveTable::id2
				{
					return TRUE;
				}break;
			}
		}
		return FALSE;
	}
}//Orm

