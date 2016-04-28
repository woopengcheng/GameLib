#include "Orm_TestSlaveCollection.h"
#include "Orm_Reflection.h"
#include "GameDB/inc/OrmHelper.h"

namespace Orm
{
	TestSlaveCollection::TestSlaveCollection()
	{
		m_pTestSlaveFrom = NULL;
		m_pTestSlave = NULL;
	}

	TestSlaveCollection::~TestSlaveCollection()
	{
		SAFE_DELETE(m_pTestSlaveFrom);
		SAFE_DELETE(m_pTestSlave);
	}

	TestSlaveFrom * TestSlaveCollection::GetTestSlaveFrom()
	{
		if(m_pTestSlaveFrom == NULL)
		{
			m_pTestSlaveFrom = new TestSlaveFrom();
			m_pTestSlaveFrom->SetMasterID( m_vMasterId );
		}
		return m_pTestSlaveFrom;
	}

	void TestSlaveCollection::CleanupTestSlaveFrom()
	{
		SAFE_DELETE(m_pTestSlaveFrom);
	}

	TestSlave * TestSlaveCollection::GetTestSlave()
	{
		if(m_pTestSlave == NULL)
		{
			m_pTestSlave = new TestSlave();
			m_pTestSlave->Setid( m_vMasterId );
		}
		return m_pTestSlave;
	}

	void TestSlaveCollection::CleanupTestSlave()
	{
		SAFE_DELETE(m_pTestSlave);
	}

	TestSlaveTable * TestSlaveCollection::CreateTestSlaveTable(BOOL bAddToManager /*= TRUE*/)
	{
		TestSlaveTable* pTestSlaveTable = new TestSlaveTable;
		pTestSlaveTable->SetMasterID(m_vMasterId);
		if (bAddToManager)
		{
			m_vecTestSlaveTable.push_back( pTestSlaveTable);
		}

		return pTestSlaveTable;
	}

	BOOL TestSlaveCollection::DeleteTestSlaveTable(TestSlaveTable * pValue , bool bFree /*= false*/)
	{
		return m_vecTestSlaveTable.Remove(pValue , bFree);
	}

	void TestSlaveCollection::LoadTestSlaveTable(mongo::BSONObj & obj)
	{
		m_vecTestSlaveTable.Cleanup();
		mongo::BSONObjIterator iter(obj);
		while(iter.more())
		{
			mongo::BSONElement be = iter.next();
			Assert(be.isABSONObj());

			TestSlaveTable * pTestSlaveTable = CreateTestSlaveTable();
			pTestSlaveTable->FromBson(be.Obj());
		}
	}

	void TestSlaveCollection::SaveTestSlaveTable(mongo::BSONArrayBuilder & bab)
	{
		for (size_t i = 0;i < m_vecTestSlaveTable.size();++i)
		{
			mongo::BSONObj obj;
			TestSlaveTable * pTestSlaveTable = m_vecTestSlaveTable[i];
			pTestSlaveTable->ToBson(obj);
			bab.append(obj);
		}
	}

	TestSlaveTable * TestSlaveCollection::GetTestSlaveTable(const std::string & id2)
	{
		GameDB::OrmVectorEx<TestSlaveTable *>::iterator iter = m_vecTestSlaveTable.begin();
		for (;iter != m_vecTestSlaveTable.end();++iter)
		{
			if ((*iter)->Getid2() == id2)	
			{
				return *iter;
			}
		}
		return NULL;
	}

	void TestSlaveCollection::ToBson(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		CUtil::Compress(obj.objdata(),obj.objsize(),strBuf);
	}

	void TestSlaveCollection::ToBson(mongo::BSONObj  & obj)
	{
		mongo::BSONObjBuilder builder(1 * 1024 * 1024);
		if(m_pTestSlaveFrom != NULL)
		{
			mongo::BSONObj obj;
			m_pTestSlaveFrom->ToBson(obj);
			builder.append(TestSlaveFrom::TableName() , obj);
		}

		if(m_pTestSlave != NULL)
		{
			mongo::BSONObj obj;
			m_pTestSlave->ToBson(obj);
			builder.append(TestSlave::TableName() , obj);
		}

		{
			mongo::BSONArrayBuilder objBuilder;
			this->SaveTestSlaveTable(objBuilder);
			objBuilder.append(TestSlaveTable::TableName() , objBuilder.obj());
		}

		obj = builder.obj();

	}

	void TestSlaveCollection::FromBson(std::string & compressedBuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(compressedBuf.c_str(),(UINT32)compressedBuf.length(),tmpbuf);
		this->FromBson(tmpbuf.c_str(),(UINT32)tmpbuf.length());
	}

	void TestSlaveCollection::FromBson(const char* buf,UINT32 len)
	{
		if(len == 0 || strcmp(buf , "") == 0)
		{
			return;
		}
		mongo::BSONObj obj(buf);
		Assert(obj.objsize() == len);
		mongo::BSONObjIterator  iter(obj); 
		while(iter.more())
		{
			mongo::BSONElement be = iter.next();
			if(!be.isABSONObj()){ continue; }
			mongo::BSONObj tmpobj = be.Obj();
			INT64 hash = CUtil::BKDRHashSum(be.fieldName());
			switch(hash)
			{
			case 5701140434439: // TestSlaveFrom
				{
					GetTestSlaveFrom()->FromBson(tmpobj);
				}break;
			case 3965202877593: // TestSlave
				{
					GetTestSlave()->FromBson(tmpobj);
				}break;
			case 6061817222949: // TestSlaveTable
				{
					LoadTestSlaveTable(tmpobj);
				}break;
			default:
				{
					MsgAssert(false , " invalid table hash ");
				}break;
			}
		}
	}

	void TestSlaveCollection::LoadBson(std::string & compressedBuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(compressedBuf.c_str(),(UINT32)compressedBuf.length(),tmpbuf);
		this->LoadBson(tmpbuf.c_str(),(UINT32)tmpbuf.length());
	}

	void TestSlaveCollection::LoadBson(const char* buf,UINT32 len)
	{
		if(len == 0 || strcmp(buf , "") == 0)
		{
			return;
		}
		mongo::BSONObj obj(buf);
		Assert(obj.objsize() == len);
		{
			std::string metaname = GameDB::OrmHelper::GetTableNameFromBson(obj);
			INT64 hash = CUtil::BKDRHashSum(metaname.c_str());
			switch(hash)
			{
			case 5701140434439: // TestSlaveFrom
				{
					GetTestSlaveFrom()->FromBson(obj);
				}break;
			case 3965202877593: // TestSlave
				{
					GetTestSlave()->FromBson(obj);
				}break;
			case 6061817222949: // TestSlaveTable
				{
					INT64 iID = -1;
					std::string  strValue = "";
					Orm::GetSlaveTableMasterIDFromBson(obj, TestSlaveTable::TableName() ,"id2",  iID, strValue);
					TestSlaveTable * pTable = GetTestSlaveTable(strValue);
					if(pTable)
					{
						pTable->FromBson(obj);
					}
					else
					{
						pTable = CreateTestSlaveTable();
						pTable->FromBson(obj);
					}
				}break;
			default:
				{
					MsgAssert(false , " invalid table hash ");
				}break;
			}
			
		}
	}

}//Orm

