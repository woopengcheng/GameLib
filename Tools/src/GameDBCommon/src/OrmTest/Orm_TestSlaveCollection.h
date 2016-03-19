/************************************
FileName	:	Orm_TestSlaveCollection.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.31.196
Version		:	0.0.1
Date		:	2016-02-09 11:26:27
Description	:	针对orm子表操作的集合类.
************************************/
#ifndef __Orm_Orm_TestSlave_collection_h__
#define __Orm_Orm_TestSlave_collection_h__
#include "GameDB/inc/DBCommon.h"
#include "GameDB/inc/OrmCollection.h"
#include "GameDB/inc/OrmVector.h"
#include "GameDB/inc/DBCommon.h"

#include "Orm_TestSlave1.h"
#include "Orm_TestSlave.h"
#include "Orm_TestSlave2.h"

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
			virtual void		ToBson(std::string & strBuf) override;
			virtual void		ToBson(mongo::BSONObj  & objBson) override;
			virtual void		FromBson(std::string & compressedBuf) override;
			virtual void		FromBson(const char * pData , UINT32 nSize) override;
		
		public:
		template<class VISITOR,class PARAM> void visit(VISITOR visitor,PARAM& param)
		{
			if(!visitor(m_pTestSlave , param))
				return ;
		
			for(GameDB::OrmVectorEx< TestSlave2 *>::iterator iter = m_vTestSlave2.begin(); iter != m_vTestSlave2.end(); ++iter)
			{
				if(!visitor(*iter,param))
					return ;
			}
		
		}
		
		public:
			TestSlave * GetTestSlave();
			void CleanupTestSlave();
		
		public:
			GameDB::OrmVectorEx< TestSlave2 *> & GetTestSlave2(){ return m_vecTestSlave2; }
			TestSlave2 * CreateTestSlave2(BOOL bAddToManager = TRUE);
			BOOL DeleteTestSlave2(TestSlave2 * pValue , bool bFree = false);
			void LoadTestSlave2(mongo::BSONObj & obj);
			void SaveTestSlave2(mongo::BSONArrayBuilder & bab);
		
		protected:
			TestSlave	 * m_pTestSlave;
			GameDB::OrmVectorEx<TestSlave2 *>	 m_vecTestSlave2;
		
	}; 
 
}//Orm

#endif
