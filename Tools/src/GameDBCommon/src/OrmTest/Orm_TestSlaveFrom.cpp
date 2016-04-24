#include "Orm_TestSlaveFrom.h"

namespace Orm
{
	TestSlaveFrom::TestSlaveFrom()
		: id(0)
		, value(0)
	{
	}

	TestSlaveFrom::~TestSlaveFrom()
	{
	}

	bool TestSlaveFrom::IsEqual(const TestSlaveFrom & val)
	{ 
		if(
			id == val.id&&
			value == val.value)
		{
			return true;
		}
		return false;
	} 

	bool TestSlaveFrom::operator == (const TestSlaveFrom & val)
	{ 
		return IsEqual(val);
	} 

	bool  TestSlaveFrom::operator != (const TestSlaveFrom & val)
	{ 
		return !IsEqual(val);
	} 

	std::string TestSlaveFrom::GetRawKey()
	{
		std::string result;
		result.reserve(64);

		return result;
	}

	std::string TestSlaveFrom::GetKey()
	{
		std::string result;
		result.reserve(64);

		{
			result.append(CUtil::itoa((INT64)id));
		}
		return result;
	}

	std::string TestSlaveFrom::GetTableName()
	{
		return TestSlaveFrom::TableName();
	}

	void TestSlaveFrom::AutoIncrease(INT64 llKey)
	{
		MsgAssert(false , "AutoIncrease key:" << llKey);
	}

	TestSlaveFrom * TestSlaveFrom::Clone()
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		TestSlaveFrom * pNew = new TestSlaveFrom();
		pNew->FromBson(obj);
		return pNew;
	}

	void TestSlaveFrom::ToCompress(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		CUtil::Compress(obj.objdata(),obj.objsize(),strBuf);
	}

	void TestSlaveFrom::ToBson(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		strBuf = std::string(obj.objdata(),obj.objsize());
	}

	void TestSlaveFrom::ToBson(mongo::BSONObj  & obj)
	{
		mongo::BSONObjBuilder builder;
		builder.append("_T",TableName());
		if(id != 0)
			builder.append("id",id);
		if(value != 0)
			builder.append("value",value);
		obj = builder.obj();
	}

	void TestSlaveFrom::FromCompress(const std::string& inbuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(inbuf.c_str(),(UINT32)inbuf.length(),tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void TestSlaveFrom::FromCompress(const char* pData,INT32 size)
	{
		std::string tmpbuf;
		CUtil::Uncompress(pData,size,tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void TestSlaveFrom::FromBson(const char* pData,INT32 size)
	{
		if(size == 0 || strcmp(pData , "") == 0)
		{
			return;
		}
		mongo::BSONObj  obj(pData);
		MsgAssert(obj.objsize() == size , "FromBson error.");
		FromBson(obj);
	}

	void TestSlaveFrom::FromBson(const mongo::BSONObj  & obj)
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
			case 2324188493089: // value
				{
					CUtil::BsonToCpp( value , be);
				}break;
			}
		}
		__hash = HashMake(0);
	}

	INT64 TestSlaveFrom::HashMake(INT64 seed)
	{
		INT64 _result = seed;
		_result = CUtil::CityHash(&id,sizeof(id),_result);
		return _result;
		_result = CUtil::CityHash(&value,sizeof(value),_result);
		return _result;
	}

	INT64 TestSlaveFrom::Getid() const
	{
		return id;
	}

	INT64 TestSlaveFrom::Getvalue() const
	{
		return value;
	}

	void TestSlaveFrom::Setvalue(INT64 & xxValuexx)
	{
		value = xxValuexx;
	}

}//Orm

