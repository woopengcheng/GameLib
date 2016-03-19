/************************************
FileName	:	Orm_TestSlave2.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.31.196
Version		:	0.0.1
Date		:	2016-02-09 11:26:27
Description	:	orm²Ù×÷¼¯ºÏ.
************************************/
#ifndef __Orm_Orm_TestSlave2_h__
#define __Orm_Orm_TestSlave2_h__
#include "Orm_OrmStructs.h"
#include "GameDB/inc/Orm.h"

namespace Orm
{
	class TestSlave2 : public GameDB::Orm
	{ 
		public:
			TestSlave2();
			virtual ~TestSlave2();
		
		private:
			TestSlave2( const TestSlave2 &);
			const TestSlave2& operator=( const TestSlave2 &);
		
		public:
			TestSlave2* Clone();

		public:
			static const char* TableName() { return "TestSlave2"; }
			static const INT64 meta_hash = 4180792914557;
		
		public:
			virtual std::string GetKey() override;
			virtual std::string GetRawKey() override;
			virtual std::string GetTableName() override;
		
		public:
			bool IsEqual(const TestSlave2 & val);
			bool operator == (const TestSlave2 & val);
			bool operator != (const TestSlave2 & val);
		
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
			virtual void		SetMasterID(const char* pID) override { }
			virtual INT64		GetMasterID() override { return id;}
			virtual const char*	GetMasterStrID() override { return "";}
		
		public:
			INT64 Getid() const;
			void Setid(INT64& value);

			INT64 Getid2() const;
			void Setid2(INT64& value);

			void Plusid2(INT64 & value);
			void Minusid2(INT64 & value);
			void id2Include(INT64 & value);
			BOOL Isid2Include(INT64 & value);
			void id2Exclude(INT64 & value);
			TestStruct Getid3() const;
			void Setid3(TestStruct& value);

		
		public:
			INT64	id;
			INT64	id2;
			TestStruct	id3;
		
		private:
			INT64			__hash;
		
	}; 
 
}//Orm

#endif
