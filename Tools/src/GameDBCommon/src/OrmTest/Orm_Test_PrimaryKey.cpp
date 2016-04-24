#include "Orm_Test_PrimaryKey.h"

namespace Orm
{
	Test_PrimaryKey::Test_PrimaryKey()
		: id(0)
		, p2(std::string())
	{
	}

	Test_PrimaryKey::~Test_PrimaryKey()
	{
	}

	bool Test_PrimaryKey::IsEqual(const Test_PrimaryKey & val)
	{ 
		if(
			id == val.id&&
			p2 == val.p2)
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
		if(p2 != std::string())
			builder.append("p2",p2);
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
		if(size == 0 || strcmp(pData , "") == 0)
		{
			return;
		}
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
			case 695784716674: // p2
				{
					CUtil::BsonToCpp( p2 , be);
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
		_result = CUtil::CityHash(&p2,sizeof(p2),_result);
		return _result;
	}

	INT64 Test_PrimaryKey::Getid() const
	{
		return id;
	}

	void Test_PrimaryKey::Setid(INT64 & xxValuexx)
	{
		id = xxValuexx;
	}

	std::string Test_PrimaryKey::Getp2() const
	{
		return p2;
	}

	void Test_PrimaryKey::Setp2(std::string & xxValuexx)
	{
		p2 = xxValuexx;
	}

}//Orm

