#include "Orm_Test_PrimaryKey.h"

namespace Orm
{
	Test_PrimaryKey::Test_PrimaryKey()
		: id(0)
		, p1(std::string())
	{
	}

	Test_PrimaryKey::~Test_PrimaryKey()
	{
	}

	bool Test_PrimaryKey::IsEqual(const Test_PrimaryKey & val)
	{ 
		if(
			id == val.id&&
			p1 == val.p1)
		{
			return true;
		}
		return false;
	} 

	bool Test_PrimaryKey::operator == (const Test_PrimaryKey & val)
	{ 
		return IsEqual(val);
	} 

	bool  Test_PrimaryKey::operator != (const Test_PrimaryKey & val)
	{ 
		return !IsEqual(val);
	} 

	std::string Test_PrimaryKey::GetRawKey()
	{
		std::string result;
		result.reserve(64);

		{
			result.append(CUtil::itoa((INT64)id));
		}
		return result;
	}

	std::string Test_PrimaryKey::GetKey()
	{
		return this->GetRawKey();

	}

	std::string Test_PrimaryKey::GetTableName()
	{
		return Test_PrimaryKey::TableName();
	}

	void Test_PrimaryKey::AutoIncrease(INT64 llKey)
	{
		id = llKey;
	}

	Test_PrimaryKey * Test_PrimaryKey::Clone()
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		Test_PrimaryKey * pNew = new Test_PrimaryKey();
		pNew->FromBson(obj);
		return pNew;
	}

	void Test_PrimaryKey::ToCompress(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		CUtil::Compress(obj.objdata(),obj.objsize(),strBuf);
	}

	void Test_PrimaryKey::ToBson(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		strBuf = std::string(obj.objdata(),obj.objsize());
	}

	void Test_PrimaryKey::ToBson(mongo::BSONObj  & obj)
	{
		mongo::BSONObjBuilder builder;
		builder.append("_T",TableName());
		if(id != 0)
			builder.append("id",id);
		if(p1 != std::string())
			builder.append("p1",p1);
		obj = builder.obj();
	}

	void Test_PrimaryKey::FromCompress(const std::string& inbuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(inbuf.c_str(),(UINT32)inbuf.length(),tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void Test_PrimaryKey::FromCompress(const char* pData,INT32 size)
	{
		std::string tmpbuf;
		CUtil::Uncompress(pData,size,tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void Test_PrimaryKey::FromBson(const char* pData,INT32 size)
	{
		mongo::BSONObj  obj(pData);
		MsgAssert(obj.objsize() == size , "FromBson error.");
		FromBson(obj);
	}

	void Test_PrimaryKey::FromBson(const mongo::BSONObj  & obj)
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
			case 691489749377: // p1
				{
					CUtil::BsonToCpp( p1 , be);
				}break;
			}
		}
		__hash = HashMake(0);
	}

	INT64 Test_PrimaryKey::HashMake(INT64 seed)
	{
		INT64 _result = seed;
		_result = CUtil::CityHash(&id,sizeof(id),_result);
		return _result;
		_result = CUtil::CityHash(&p1,sizeof(p1),_result);
		return _result;
	}

	INT64 Test_PrimaryKey::Getid() const
	{
		return id;
	}

	void Test_PrimaryKey::Setid(INT64 & value)
	{
		id = value;
	}

	std::string Test_PrimaryKey::Getp1() const
	{
		return p1;
	}

	void Test_PrimaryKey::Setp1(std::string & value)
	{
		p1 = value;
	}

}//Orm

