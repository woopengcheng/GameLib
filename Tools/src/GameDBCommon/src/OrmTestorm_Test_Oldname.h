/************************************
FileName	:	orm_Test_Oldname.h
Author		:	generate by tools
HostName	:	woopengcheng
IP			:	192.168.1.107
Version		:	0.0.1
Date		:	2015-08-15 12:15:48
Description	:	orm操作集合.
************************************/
#ifndef __Orm_orm_Test_Oldname_h__
#define __Orm_orm_Test_Oldname_h__
#include "GameDB/inc/Orm.h"

namespace Orm
{
	class DLL_EXPORT Test_Oldname : public GameDB::Orm
	{ 
		public:
			Test_Oldname();
			virtual ~Test_Oldname();
		
		public:
			Test_Oldname* Clone();

		public:
			static const char* TableName() { return "Test_Oldname"; }
			static const INT64 meta_hash = 1234567;
		
		public:
			virtual std::string GetKey();
			virtual std::string GetRawKey();
			virtual std::string GetTableName();
			virtual bool        IsDefaultValue();
		
		public:
			virtual void		ToBson(std::string & strBuf);
			virtual void		ToBson(mongo::BSONObj  & objBson);
			virtual void		FromBson(const char * pData , INT32 nSize);
			virtual void		FromBson(const mongo::BSONObj  & objBson);
			virtual void		ToCompress(std::string & __buf); 
			virtual void		FromCompress(const char* data,INT32 size)
			virtual void		FromCompress(const std::string & strBuf)
		
		public:
			virtual INT64		HashMake(INT64 llSeed = DEFAULT_HASH_SEED);
			virtual INT64		CurHash(){ return __hash; }
			virtual void		HashUpdate(INT64 llHash);
		
		public:
			virtual void		AutoIncrease(INT64 llKey); 
			virtual void		SetMasterID(INT64 llID);
			virtual INT64		SetMasterID(const char* pID);
			virtual INT64		GetMasterID();
			virtual const char*	GetMasterStrID(); 
		
		public:
/************************************
FileName	:	orm_Test_Oldname.h
Author		:	generate by tools
HostName	:	woopengcheng
IP			:	192.168.1.107
Version		:	0.0.1
Date		:	2015-08-15 12:16:34
Description	:	orm操作集合.
************************************/
#ifndef __Orm_orm_Test_Oldname_h__
#define __Orm_orm_Test_Oldname_h__
#include "GameDB/inc/Orm.h"

namespace Orm
{
	class DLL_EXPORT Test_Oldname : public GameDB::Orm
	{ 
		public:
			Test_Oldname();
			virtual ~Test_Oldname();
		
		public:
			Test_Oldname* Clone();

		public:
			static const char* TableName() { return "Test_Oldname"; }
			static const INT64 meta_hash = 1234567;
		
		public:
			virtual std::string GetKey();
			virtual std::string GetRawKey();
			virtual std::string GetTableName();
			virtual bool        IsDefaultValue();
		
		public:
			virtual void		ToBson(std::string & strBuf);
			virtual void		ToBson(mongo::BSONObj  & objBson);
			virtual void		FromBson(const char * pData , INT32 nSize);
			virtual void		FromBson(const mongo::BSONObj  & objBson);
			virtual void		ToCompress(std::string & __buf); 
			virtual void		FromCompress(const char* data,INT32 size)
			virtual void		FromCompress(const std::string & strBuf)
		
		public:
			virtual INT64		HashMake(INT64 llSeed = DEFAULT_HASH_SEED);
			virtual INT64		CurHash(){ return __hash; }
			virtual void		HashUpdate(INT64 llHash);
		
		public:
			virtual void		AutoIncrease(INT64 llKey); 
			virtual void		SetMasterID(INT64 llID);
			virtual INT64		SetMasterID(const char* pID);
			virtual INT64		GetMasterID();
			virtual const char*	GetMasterStrID(); 
		
		public:
					
		public:
		CUtilChunk			id;
		
		privite:
			INT64 __hash;
		
	}; 
 
}//orm_

#endif
