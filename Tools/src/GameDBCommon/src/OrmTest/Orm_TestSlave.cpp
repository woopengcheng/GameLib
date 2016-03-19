#include "Orm_TestSlave.h"

namespace Orm
{
	TestSlave::TestSlave()
		: id(0)
	{
	}

	TestSlave::~TestSlave()
	{
	}

	bool TestSlave::IsEqual(const TestSlave & val)
	{ 
		if(
			id == val.id)
		{
			return true;
		}
		return false;
	} 

	bool TestSlave::operator == (const TestSlave & val)
	{ 
		return IsEqual(val);
	} 

	bool  TestSlave::operator != (const TestSlave & val)
	{ 
		return !IsEqual(val);
	} 

	std::string TestSlave::GetRawKey()
	{
		std::string result;
		result.reserve(64);

		return result;
	}

	std::string TestSlave::GetKey()
	{
		std::string result;
		result.reserve(64);

			{
			result.append(CUtil::itoa((INT64)id));
		}
		return result;
	}

	std::string TestSlave::GetTableName()
	{
		return TestSlave::TableName();
	}

	void TestSlave::AutoIncrease(INT64 llKey)
	{
		MsgAssert(false , "AutoIncrease key:" << llKey);
	}

	TestSlave * TestSlave::Clone()
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		TestSlave * pNew = new TestSlave();
		pNew->FromBson(obj);
		return pNew;
	}

	void TestSlave::ToCompress(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		CUtil::Compress(obj.objdata(),obj.objsize(),strBuf);
	}

	void TestSlave::ToBson(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		strBuf = std::string(obj.objdata(),obj.objsize());
	}

	void TestSlave::ToBson(mongo::BSONObj  & obj)
	{
		mongo::BSONObjBuilder builder;
		builder.append("_T",TableName());
		if(id != 0)
			builder.append("id",id);
		obj = builder.obj();
	}

	void TestSlave::FromCompress(const std::string& inbuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(inbuf.c_str(),(UINT32)inbuf.length(),tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void TestSlave::FromCompress(const char* pData,INT32 size)
	{
		std::string tmpbuf;
		CUtil::Uncompress(pData,size,tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void TestSlave::FromBson(const char* pData,INT32 size)
	{
		mongo::BSONObj  obj(pData);
		MsgAssert(obj.objsize() == size , "FromBson error.");
		FromBson(obj);
	}

	void TestSlave::FromBson(const mongo::BSONObj  & obj)
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
			}
		}
		__hash = HashMake(0);
	}

	INT64 TestSlave::HashMake(INT64 seed)
	{
		INT64 _result = seed;
		_result = CUtil::CityHash(&id,sizeof(id),_result);
		return _result;
	}

	INT64 TestSlave::Getid() const
	{
		return id;
	}

	void TestSlave::Setid(INT64 & value)
	{
		id = value;
	}

}//Orm

