#include "Orm_Reflection.h"

#include "Orm_Test_Oldname.h"
#include "Orm_Test_PrimaryKey.h"
#include "Orm_TestSlave.h"
#include "Orm_TestSlave1.h"
#include "Orm_TestSlave2.h"

namespace Orm
{
	const char* cst_slaves_of_TestSlave[] = 
 	{
		"TestSlave2",
		NULL 
	};

	INT64 cst_slaves_hash_of_TestSlave[] = 
 	{
		4180792914557,
		NULL 
	};

	GameDB::Orm * CreateObject(const char * data,size_t size)
	{
		mongo::BSONObj obj(data);
		MsgAssert_Re(obj.objsize() == size , NULL , "");
		return CreateObject(obj);
	}

	GameDB::Orm * createObject(bson::bo& obj)
	{
		std::string metaname;
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
		case Orm::TestSlave2::meta_hash:
			result = new Orm::TestSlave2();
		break;
		}

		if(result != NULL)
			result->FromBson(obj);
		return result;
	}
}//Orm

