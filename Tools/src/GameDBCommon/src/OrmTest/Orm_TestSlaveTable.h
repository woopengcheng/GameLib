/************************************
FileName	:	Orm_TestSlaveTable.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.40.128
Version		:	0.0.1
Date		:	2016-04-29 17:20:53
Description	:	orm²Ù×÷¼¯ºÏ.
************************************/
#ifndef __Orm_Orm_TestSlaveTable_h__
#define __Orm_Orm_TestSlaveTable_h__
#include "Orm_OrmStructs.h"
#include "GameDB/inc/Orm.h"

namespace Orm
{
	class TestSlaveTable : public GameDB::Orm
	{ 
		public:
			TestSlaveTable();
			virtual ~TestSlaveTable();
		
		private:
			TestSlaveTable( const TestSlaveTable &);
			const TestSlaveTable& operator=( const TestSlaveTable &);
		
		public:
			TestSlaveTable* Clone();

		public:
			static const char* TableName() { return "TestSlaveTable"; }
			static const INT64 TableHash = 6061817222949;
		
		public:
			virtual std::string GetKey() override;
			virtual std::string GetRawKey() override;
			virtual std::string GetTableName() override;
		
		public:
			bool IsEqual(const TestSlaveTable & val);
			bool operator == (const TestSlaveTable & val);
			bool operator != (const TestSlaveTable & val);
		
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
			std::string Getid2() const;
			void Setid2(std::string& xxValuexx);

			const TestStruct & Getid3() const;
			void Setid3(TestStruct& xxValuexx);

			INT64 Getid4() const;
			void Setid4(INT64& xxValuexx);

			void Plusid4(INT64 & xxValuexx);
			void Minusid4(INT64 & xxValuexx);
			void id4Include(INT64 & xxValuexx);
			BOOL Isid4Include(INT64 & xxValuexx);
			void id4Exclude(INT64 & xxValuexx);
		
		public:
			INT64	id;
			std::string	id2;
			TestStruct	id3;
			INT64	id4;
		
		private:
			INT64			__hash;
		
	}; 
 
}//Orm

#endif
