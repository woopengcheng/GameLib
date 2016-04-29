/************************************
FileName	:	Orm_TestSlaveCollection.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.40.128
Version		:	0.0.1
Date		:	2016-04-29 17:20:53
Description	:	针对orm子表操作的集合类.
************************************/
#ifndef __Orm_Orm_TestSlave_collection_h__
#define __Orm_Orm_TestSlave_collection_h__
#include "GameDB/inc/DBCommon.h"
#include "GameDB/inc/OrmCollection.h"
#include "GameDB/inc/OrmVector.h"
#include "GameDB/inc/DBCommon.h"

#include "Orm_TestSlaveFrom.h"
#include "Orm_TestSlave.h"
#include "Orm_TestSlaveTable.h"

namespace Orm
{
	class TestSlaveCollection : public  GameDB::OrmCollection<INT64>
	{ 
		public:
			TestSlaveCollection();
			virtual ~TestSlaveCollection();
		
		private:
			TestSlaveCollection( const TestSlave &);
			const TestSlaveCollection & operator=( const TestSlaveCollection &);
		
		public:
			virtual void			ToBson(std::string & strBuf) override;
			virtual void			ToBson(mongo::BSONObj  & objBson) override;
			virtual void			FromBson(std::string & compressedBuf) override;
			virtual void			FromBson(const char * pData , UINT32 nSize) override;
			virtual void			LoadBson(std::string & compressedBuf) override;
			virtual void			LoadBson(const char * pData , UINT32 nSize) override;
			virtual GameDB::Orm * GetTable(const std::string & strTable) override;
		
		public:
		template<class VISITOR,class PARAM> void visit(VISITOR visitor,PARAM& param)
		{
			if(!visitor(m_pTestSlaveFrom , param))
				return ;
		
			if(!visitor(m_pTestSlave , param))
				return ;
		
			for(GameDB::OrmVectorEx< TestSlaveTable *>::iterator iter = m_vTestSlaveTable.begin(); iter != m_vTestSlaveTable.end(); ++iter)
			{
				if(!visitor(*iter,param))
					return ;
			}
		
		}
		
		public:
			TestSlaveFrom * GetTestSlaveFrom();
			void CleanupTestSlaveFrom();
			TestSlave * GetTestSlave();
			void CleanupTestSlave();
		
		public:
			GameDB::OrmVectorEx< TestSlaveTable *> & GetTestSlaveTable(){ return m_vecTestSlaveTable; }
			TestSlaveTable * CreateTestSlaveTable(BOOL bAddToManager = TRUE);
			BOOL DeleteTestSlaveTable(TestSlaveTable * pValue , bool bFree = false);
			void LoadTestSlaveTable(mongo::BSONObj & obj);
			void SaveTestSlaveTable(mongo::BSONArrayBuilder & bab);
			TestSlaveTable * GetTestSlaveTable(const std::string & id2);
		
		protected:
			TestSlaveFrom	 * m_pTestSlaveFrom;
			TestSlave	 * m_pTestSlave;
			GameDB::OrmVectorEx<TestSlaveTable *>	 m_vecTestSlaveTable;
		
	}; 
 
}//Orm

#endif
