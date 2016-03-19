#include "Orm_TestSlave2.h"

namespace Orm
{
	TestSlave2::TestSlave2()
		: id(0)
		, id2(0)
		, id3(TestStruct())
	{
	}

	TestSlave2::~TestSlave2()
	{
	}

	bool TestSlave2::IsEqual(const TestSlave2 & val)
	{ 
		if(
			id == val.id&&
			id2 == val.id2&&
			id3 == val.id3)
		{
			return true;
		}
		return false;
	} 

	bool TestSlave2::operator == (const TestSlave2 & val)
	{ 
		return IsEqual(val);
	} 

	bool  TestSlave2::operator != (const TestSlave2 & val)
	{ 
		return !IsEqual(val);
	} 

	std::string TestSlave2::GetRawKey()
	{
		std::string result;
		result.reserve(64);

		return result;
	}

	std::string TestSlave2::GetKey()
	{
		std::string result;
		result.reserve(64);

			{
			result.append(CUtil::itoa((INT64)id));
		}
		return result;
	}

	std::string TestSlave2::GetTableName()
	{
		return TestSlave2::TableName();
	}

	void TestSlave2::AutoIncrease(INT64 llKey)
	{
		MsgAssert(false , "AutoIncrease key:" << llKey);
	}

	TestSlave2 * TestSlave2::Clone()
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		TestSlave2 * pNew = new TestSlave2();
		pNew->FromBson(obj);
		return pNew;
	}

	void TestSlave2::ToCompress(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		CUtil::Compress(obj.objdata(),obj.objsize(),strBuf);
	}

	void TestSlave2::ToBson(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		strBuf = std::string(obj.objdata(),obj.objsize());
	}

	void TestSlave2::ToBson(mongo::BSONObj  & obj)
	{
		mongo::BSONObjBuilder builder;
		builder.append("_T",TableName());
		if(id != 0)
			builder.append("id",id);
		if(id2 != 0)
			builder.append("id2",id2);
		if(id3 != TestStruct())
		{
			CUtil::Parameter p(id3);
			builder.appendBinData("id3" , p.GetDataLen() , mongo::bdtParamter , (const char *)(p.GetStreamData())); 
		}
		obj = builder.obj();
	}

	void TestSlave2::FromCompress(const std::string& inbuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(inbuf.c_str(),(UINT32)inbuf.length(),tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void TestSlave2::FromCompress(const char* pData,INT32 size)
	{
		std::string tmpbuf;
		CUtil::Uncompress(pData,size,tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void TestSlave2::FromBson(const char* pData,INT32 size)
	{
		mongo::BSONObj  obj(pData);
		MsgAssert(obj.objsize() == size , "FromBson error.");
		FromBson(obj);
	}

	void TestSlave2::FromBson(const mongo::BSONObj  & obj)
	{
		mongo::BSONObjIterator  iter(obj); 
		while(iter.more())
		{
			mongo::BSONElement be = iter.next();
			const char* fieldName = be.fieldName();
			INT64 hash = CUtil::BKDRHashSum(fieldName);
			switch(hash)
			{
			case 768799158513: // _T
				{
					MsgAssert(CUtil::strcmp(be.valuestr(), TableName()) == 0 , "FromBson error.");
				}break;
			case 880468309535: // id
				{
					CUtil::BsonToCpp( id , be);
				}break;
			case 1095218475535: // id2
				{
					CUtil::BsonToCpp( id2 , be);
				}break;
			case 1099513442832: // id3
				{
					CUtil::Parameter p;
					CUtil::BsonToCpp( p , be);
					id3 = p;
				}break;
			}
		}
		__hash = HashMake(0);
	}

	INT64 TestSlave2::HashMake(INT64 seed)
	{
		INT64 _result = seed;
		_result = CUtil::CityHash(&id,sizeof(id),_result);
		return _result;
		_result = CUtil::CityHash(&id2,sizeof(id2),_result);
		return _result;
		_result = CUtil::CityHash(&id3,sizeof(id3),_result);
		return _result;
	}

	INT64 TestSlave2::Getid() const
	{
		return id;
	}

	void TestSlave2::Setid(INT64 & value)
	{
		id = value;
	}

	INT64 TestSlave2::Getid2() const
	{
		return id2;
	}

	void TestSlave2::Setid2(INT64 & value)
	{
		id2 = value;
	}

	void TestSlave2::Plusid2(INT64 & value)
	{
		id2 = id2 + value;
	}

	void TestSlave2::Minusid2(INT64 & value)
	{
		id2 = id2 - value;
	}

	void TestSlave2::id2Include(INT64 & value)
	{
		id2 = id2 | value;
	}

	void TestSlave2::id2Exclude(INT64 & value)
	{
		id2 = id2 & (~value);
	}

	BOOL TestSlave2::Isid2Include(INT64 & value)
	{
		return id2 & value;
	}

	TestStruct TestSlave2::Getid3() const
	{
		return id3;
	}

	void TestSlave2::Setid3(TestStruct & value)
	{
		id3 = value;
	}

}//Orm

