/************************************
FileName	:	Orm_Reflection.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-25 00:28:58
Description	:	针对slave的类产生反射.
************************************/
#ifndef __Orm_Orm__feflection_h__
#define __Orm_Orm__feflection_h__
#include "GameDB/inc/Orm.h"

namespace Orm
{
	extern const char * cst_slaves_of_TestSlave[];
	extern INT64 cst_slaves_hash_of_TestSlave[];

	extern GameDB::Orm * CreateObject(bson::bo & obj , std::string & metaname);
	extern GameDB::Orm * CreateObject(const char * data,size_t size , std::string & metaname);
	extern GameDB::Orm * CreateObject(bson::bo & obj , std::string & metaname);
	extern BOOL GetSlaveTableMasterIDFromBson(const char * data,size_t size , const std::string & strTableName ,const std::string & key , INT64 & intValue , std::string & strValue);
	extern BOOL GetSlaveTableMasterIDFromBson(bson::bo & obj , const std::string & strTableName ,const std::string & key , INT64 & intValue , std::string & strValue);
	
}//Orm

#endif
