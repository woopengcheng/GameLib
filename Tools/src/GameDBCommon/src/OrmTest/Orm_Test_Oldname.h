/************************************
FileName	:	Orm_Test_OldName.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.40.128
Version		:	0.0.1
Date		:	2016-04-29 17:20:53
Description	:	orm²Ù×÷¼¯ºÏ.
************************************/
#ifndef __Orm_Orm_Test_OldName_h__
#define __Orm_Orm_Test_OldName_h__
#include "Orm_OrmStructs.h"
#include "GameDB/inc/Orm.h"

namespace Orm
{
	class Test_OldName : public GameDB::Orm
	{ 
		public:
			Test_OldName();
			virtual ~Test_OldName();
		
		private:
			Test_OldName( const Test_OldName &);
			const Test_OldName& operator=( const Test_OldName &);
		
		public:
			Test_OldName* Clone();

		public:
			static const char* TableName() { return "Test_OldName"; }
			static const INT64 TableHash = 5082095997049;
		
		public:
			virtual std::string GetKey() override;
			virtual std::string GetRawKey() override;
			virtual std::string GetTableName() override;
		
		public:
			bool IsEqual(const Test_OldName & val);
			bool operator == (const Test_OldName & val);
			bool operator != (const Test_OldName & val);
		
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
			std::string Getp1() const;
			void Setp1(std::string& xxValuexx);

		
		public:
			INT64	id;
			std::string	p1;
		
		private:
			INT64			__hash;
		
	}; 
 
}//Orm

#endif
