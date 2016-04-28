/************************************
FileName	:	Orm_TestSlaveFrom.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-29 00:32:33
Description	:	orm²Ù×÷¼¯ºÏ.
************************************/
#ifndef __Orm_Orm_TestSlaveFrom_h__
#define __Orm_Orm_TestSlaveFrom_h__
#include "Orm_OrmStructs.h"
#include "GameDB/inc/Orm.h"

namespace Orm
{
	class TestSlaveFrom : public GameDB::Orm
	{ 
		public:
			TestSlaveFrom();
			virtual ~TestSlaveFrom();
		
		private:
			TestSlaveFrom( const TestSlaveFrom &);
			const TestSlaveFrom& operator=( const TestSlaveFrom &);
		
		public:
			TestSlaveFrom* Clone();

		public:
			static const char* TableName() { return "TestSlaveFrom"; }
			static const INT64 TableHash = 5701140434439;
		
		public:
			virtual std::string GetKey() override;
			virtual std::string GetRawKey() override;
			virtual std::string GetTableName() override;
		
		public:
			bool IsEqual(const TestSlaveFrom & val);
			bool operator == (const TestSlaveFrom & val);
			bool operator != (const TestSlaveFrom & val);
		
		public:
			virtual void		ToBson(std::string & strBuf) override;
			virtual void		ToBson(mongo::BSONObj  & objBson) override;
			virtual void		FromBson(const char * pData , INT32 nSize) override;
			virtual void		FromBson(const mongo::BSONObj  & objBson) override;
			virtual void		ToCompress(std::string & strBuf); 
			virtual void		FromCompress(const char* data,INT32 size) override;
			virtual void		FromCompress(const std::string & strBuf) override;
		
		public:
			virtual INT64		HashMake(INT64 llSeed = DEFAULT_HASH_SEED) override;
			virtual INT64		CurHash() override { return __hash; }
			virtual void		HashUpdate(INT64 llHash) override { __hash = llHash; }
		
		public:
			virtual void		AutoIncrease(INT64 llKey) override ; 
			virtual void		SetMasterID(INT64 llID) override {id = llID; }
			virtual void		SetMasterID(const std::string & strID) override { }
			virtual INT64		GetMasterID() override { return id;}
			virtual std::string	GetMasterStrID() override { return "";}
		
		public:
			INT64 Getid() const;
			INT64 Getvalue() const;
			void Setvalue(INT64& xxValuexx);

		
		public:
			INT64	id;
			INT64	value;
		
		private:
			INT64			__hash;
		
	}; 
 
}//Orm

#endif
