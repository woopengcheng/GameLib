#include "Orm_TestSlaveCollection.h"

namespace Orm
{
	TestSlaveCollection::TestSlaveCollection()
	{
		m_pTestSlave = NULL;
	}

	TestSlaveCollection::~TestSlaveCollection()
	{
		SAFE_DELETE(m_pTestSlave);
	}

	TestSlave * TestSlaveCollection::GetTestSlave()
	{
		if(m_pTestSlave == NULL)
		{
			m_pTestSlave = new TestSlave();
			m_pTestSlave->SetMasterID( m_vMasterId );
		}
		return m_pTestSlave;
	}

	void TestSlaveCollection::CleanupTestSlave()
	{
		SAFE_DELETE(m_pTestSlave);
	}

	TestSlave2 * TestSlaveCollection::CreateTestSlave2(BOOL bAddToManager /*= TRUE*/)
	{
		TestSlave2* pTestSlave2 = new TestSlave2;
		pTestSlave2->SetMasterID(m_vMasterId);
		if (bAddToManager)
		{
			m_vecTestSlave2.push_back( pTestSlave2);
		}

		return pTestSlave2;
	}

	BOOL TestSlaveCollection::DeleteTestSlave2(TestSlave2 * pValue , bool bFree /*= false*/)
	{
		return m_vecTestSlave2.Remove(pValue , bFree);
	}

	void TestSlaveCollection::LoadTestSlave2(mongo::BSONObj & obj)
	{
		m_vecTestSlave2.Cleanup();
		mongo::BSONObjIterator iter(obj);
		while(iter.more())
		{
			mongo::BSONElement be = iter.next();
			Assert(be.isABSONObj());

			TestSlave2 * pTestSlave2 = CreateTestSlave2();
			pTestSlave2->FromBson(be.Obj());
		}
	}

	void TestSlaveCollection::SaveTestSlave2(mongo::BSONArrayBuilder & bab)
	{
		for (size_t i = 0;i < m_vecTestSlave2.size();++i)
		{
			mongo::BSONObj obj;
			TestSlave2 * pTestSlave2 = m_vecTestSlave2[i];
			pTestSlave2->ToBson(obj);
			bab.append(obj);
		}
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
		if(m_pTestSlave != NULL)
		{
			mongo::BSONObj obj;
			m_pTestSlave->ToBson(obj);
			builder.append(TestSlave::TableName() , obj);
		}

		{
			mongo::BSONArrayBuilder objBuilder;
			this->SaveTestSlave2(objBuilder);
			objBuilder.append(TestSlave2::TableName() , objBuilder.obj());
		}

		obj = builder.obj();

	}

	void TestSlaveCollection::FromBson(std::string & compressedBuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(compressedBuf.c_str(),(UINT32)compressedBuf.length(),tmpbuf);
		this->FromBson(tmpbuf.c_str(),tmpbuf.length());
	}

	void TestSlaveCollection::FromBson(const char* buf,UINT32 len)
	{
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
			case 3965202877593: // TestSlave
				{
					GetTestSlave()->FromBson(tmpobj);
				}break;
			case 4180792914557: // TestSlave2
				{
					LoadTestSlave2(tmpobj);
				}break;
			default:
				{
					MsgAssert(false , " invalid table hash ");
				}break;
			}
		}
	}

}//Orm

