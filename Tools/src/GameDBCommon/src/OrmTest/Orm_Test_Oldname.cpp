#include "Orm_Test_OldName.h"

namespace Orm
{
	Test_OldName::Test_OldName()
		: id(0)
		, p1(std::string())
	{
	}

	Test_OldName::~Test_OldName()
	{
	}

	bool Test_OldName::IsEqual(const Test_OldName & val)
	{ 
		if(
			id == val.id&&
			p1 == val.p1)
		{
			return true;
		}
		return false;
	} 

	bool Test_OldName::operator == (const Test_OldName & val)
	{ 
		return IsEqual(val);
	} 

	bool  Test_OldName::operator != (const Test_OldName & val)
	{ 
		return !IsEqual(val);
	} 

	std::string Test_OldName::GetRawKey()
	{
		std::string result;
		result.reserve(64);

		return result;
	}

	std::string Test_OldName::GetKey()
	{
		std::string result;
		result.reserve(64);

		{
			result.append(CUtil::itoa((INT64)id));
		}
		return result;
	}

	std::string Test_OldName::GetTableName()
	{
		return Test_OldName::TableName();
	}

	void Test_OldName::AutoIncrease(INT64 llKey)
	{
		MsgAssert(false , "AutoIncrease key:" << llKey);
	}

	Test_OldName * Test_OldName::Clone()
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		Test_OldName * pNew = new Test_OldName();
		pNew->FromBson(obj);
		return pNew;
	}

	void Test_OldName::ToCompress(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		CUtil::Compress(obj.objdata(),obj.objsize(),strBuf);
	}

	void Test_OldName::ToBson(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		strBuf = std::string(obj.objdata(),obj.objsize());
	}

	void Test_OldName::ToBson(mongo::BSONObj  & obj)
	{
		mongo::BSONObjBuilder builder;
		builder.append("_T",TableName());
		if(id != 0)
			builder.append("id",id);
		if(p1 != std::string())
			builder.append("p1",p1);
		obj = builder.obj();
	}

	void Test_OldName::FromCompress(const std::string& inbuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(inbuf.c_str(),(UINT32)inbuf.length(),tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void Test_OldName::FromCompress(const char* pData,INT32 size)
	{
		std::string tmpbuf;
		CUtil::Uncompress(pData,size,tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void Test_OldName::FromBson(const char* pData,INT32 size)
	{
		if(size == 0 || strcmp(pData , "") == 0)
		{
			return;
		}
		mongo::BSONObj  obj(pData);
		MsgAssert(obj.objsize() == size , "FromBson error.");
		FromBson(obj);
	}

	void Test_OldName::FromBson(const mongo::BSONObj  & obj)
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
			case 704374651268: // oldName: p4
			case 700079683971: // oldName: p3
			case 691489749377: // p1
				{
					CUtil::BsonToCpp( p1 , be);
				}break;
			}
		}
		__hash = HashMake(0);
	}

	INT64 Test_OldName::HashMake(INT64 seed)
	{
		INT64 _result = seed;
		_result = CUtil::CityHash(&id,sizeof(id),_result);
		return _result;
		_result = CUtil::CityHash(&p1,sizeof(p1),_result);
		return _result;
	}

	INT64 Test_OldName::Getid() const
	{
		return id;
	}

	std::string Test_OldName::Getp1() const
	{
		return p1;
	}

	void Test_OldName::Setp1(std::string & xxValuexx)
	{
		p1 = xxValuexx;
	}

}//Orm

