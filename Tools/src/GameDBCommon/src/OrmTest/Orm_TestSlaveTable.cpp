#include "Orm_TestSlaveTable.h"

namespace Orm
{
	TestSlaveTable::TestSlaveTable()
		: id(0)
		, id2(std::string())
		, id3(TestStruct())
		, id4(0)
	{
	}

	TestSlaveTable::~TestSlaveTable()
	{
	}

	bool TestSlaveTable::IsEqual(const TestSlaveTable & val)
	{ 
		if(
			id == val.id&&
			id2 == val.id2&&
			id3 == val.id3&&
			id4 == val.id4)
		{
			return true;
		}
		return false;
	} 

	bool TestSlaveTable::operator == (const TestSlaveTable & val)
	{ 
		return IsEqual(val);
	} 

	bool  TestSlaveTable::operator != (const TestSlaveTable & val)
	{ 
		return !IsEqual(val);
	} 

	std::string TestSlaveTable::GetRawKey()
	{
		std::string result;
		result.reserve(64);

		result.append(id2);
		return result;
	}

	std::string TestSlaveTable::GetKey()
	{
		std::string result;
		result.reserve(64);

		result.append(id2);
		return result;
	}

	std::string TestSlaveTable::GetTableName()
	{
		std::string result;
		result.reserve(64);
		result.append(TestSlaveTable::TableName());
		result.append(".");
		{
			char tmpbuf[64] = "";
			CUtil::itoa(tmpbuf,(INT64)id);
			result.append(tmpbuf);
		}
		return result;
	}

	void TestSlaveTable::AutoIncrease(INT64 llKey)
	{
		MsgAssert(false , "AutoIncrease key:" << llKey);
	}

	TestSlaveTable * TestSlaveTable::Clone()
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		TestSlaveTable * pNew = new TestSlaveTable();
		pNew->FromBson(obj);
		return pNew;
	}

	void TestSlaveTable::ToCompress(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		CUtil::Compress(obj.objdata(),obj.objsize(),strBuf);
	}

	void TestSlaveTable::ToBson(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		strBuf = std::string(obj.objdata(),obj.objsize());
	}

	void TestSlaveTable::ToBson(mongo::BSONObj  & obj)
	{
		mongo::BSONObjBuilder builder;
		builder.append("_T",TableName());
		if(id != 0)
			builder.append("id",id);
		if(id2 != std::string())
			builder.append("id2",id2);
		if(id3 != TestStruct())
		{
			CUtil::Parameter p(id3);
			builder.appendBinData("id3" , p.GetDataLen() , mongo::bdtParamter , (const char *)(p.GetStreamData())); 
		}
		if(id4 != 0)
			builder.append("id4",id4);
		obj = builder.obj();
	}

	void TestSlaveTable::FromCompress(const std::string& inbuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(inbuf.c_str(),(UINT32)inbuf.length(),tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void TestSlaveTable::FromCompress(const char* pData,INT32 size)
	{
		std::string tmpbuf;
		CUtil::Uncompress(pData,size,tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void TestSlaveTable::FromBson(const char* pData,INT32 size)
	{
		if(size == 0 || strcmp(pData , "") == 0)
		{
			return;
		}
		mongo::BSONObj  obj(pData);
		MsgAssert(obj.objsize() == size , "FromBson error.");
		FromBson(obj);
	}

	void TestSlaveTable::FromBson(const mongo::BSONObj  & obj)
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
			case 1103808410129: // id4
				{
					CUtil::BsonToCpp( id4 , be);
				}break;
			}
		}
		__hash = HashMake(0);
	}

	INT64 TestSlaveTable::HashMake(INT64 seed)
	{
		INT64 _result = seed;
		_result = CUtil::CityHash(&id,sizeof(id),_result);
		return _result;
		_result = CUtil::CityHash(&id2,sizeof(id2),_result);
		return _result;
		_result = CUtil::CityHash(&id3,sizeof(id3),_result);
		return _result;
		_result = CUtil::CityHash(&id4,sizeof(id4),_result);
		return _result;
	}

	INT64 TestSlaveTable::Getid() const
	{
		return id;
	}

	std::string TestSlaveTable::Getid2() const
	{
		return id2;
	}

	void TestSlaveTable::Setid2(std::string & xxValuexx)
	{
		id2 = xxValuexx;
	}

	TestStruct TestSlaveTable::Getid3() const
	{
		return id3;
	}

	void TestSlaveTable::Setid3(TestStruct & xxValuexx)
	{
		id3 = xxValuexx;
	}

	INT64 TestSlaveTable::Getid4() const
	{
		return id4;
	}

	void TestSlaveTable::Setid4(INT64 & xxValuexx)
	{
		id4 = xxValuexx;
	}

	void TestSlaveTable::Plusid4(INT64 & xxValuexx)
	{
		id4 = id4 + xxValuexx;
	}

	void TestSlaveTable::Minusid4(INT64 & xxValuexx)
	{
		id4 = id4 - xxValuexx;
	}

	void TestSlaveTable::id4Include(INT64 & xxValuexx)
	{
		id4 = id4 | xxValuexx;
	}

	void TestSlaveTable::id4Exclude(INT64 & xxValuexx)
	{
		id4 = id4 & (~xxValuexx);
	}

	BOOL TestSlaveTable::Isid4Include(INT64 & xxValuexx)
	{
		return !!id4 & xxValuexx;
	}

}//Orm

