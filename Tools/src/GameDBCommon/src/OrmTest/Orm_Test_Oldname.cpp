#include "Orm_Test_Oldname.h"

namespace Orm
{
	Test_Oldname::Test_Oldname()
		: id(0)
		, p1(std::string())
	{
	}

	Test_Oldname::~Test_Oldname()
	{
	}

	bool Test_Oldname::IsEqual(const Test_Oldname & val)
	{ 
		if(
			id == val.id&&
			p1 == val.p1)
		{
			return true;
		}
		return false;
	} 

	bool Test_Oldname::operator == (const Test_Oldname & val)
	{ 
		return IsEqual(val);
	} 

	bool  Test_Oldname::operator != (const Test_Oldname & val)
	{ 
		return !IsEqual(val);
	} 

	std::string Test_Oldname::GetRawKey()
	{
		std::string result;
		result.reserve(64);

		return result;
	}

	std::string Test_Oldname::GetKey()
	{
		std::string result;
		result.reserve(64);

			{
			result.append(CUtil::itoa((INT64)id));
		}
		return result;
	}

	std::string Test_Oldname::GetTableName()
	{
		return Test_Oldname::TableName();
	}

	void Test_Oldname::AutoIncrease(INT64 llKey)
	{
		p1 = llKey;
	}

	Test_Oldname * Test_Oldname::Clone()
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		Test_Oldname * pNew = new Test_Oldname();
		pNew->FromBson(obj);
		return pNew;
	}

	void Test_Oldname::ToCompress(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		CUtil::Compress(obj.objdata(),obj.objsize(),strBuf);
	}

	void Test_Oldname::ToBson(std::string & strBuf)
	{
		mongo::BSONObj  obj;
		ToBson(obj);
		strBuf = std::string(obj.objdata(),obj.objsize());
	}

	void Test_Oldname::ToBson(mongo::BSONObj  & obj)
	{
		mongo::BSONObjBuilder builder;
		builder.append("_T",TableName());
		if(id != 0)
			builder.append("id",id);
		if(p1 != std::string())
			builder.append("p1",p1);
		obj = builder.obj();
	}

	void Test_Oldname::FromCompress(const std::string& inbuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(inbuf.c_str(),(UINT32)inbuf.length(),tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void Test_Oldname::FromCompress(const char* pData,INT32 size)
	{
		std::string tmpbuf;
		CUtil::Uncompress(pData,size,tmpbuf);
		mongo::BSONObj  obj(tmpbuf.c_str());
		MsgAssert(obj.objsize() == tmpbuf.length() , "");
		FromBson(obj);
	}

	void Test_Oldname::FromBson(const char* pData,INT32 size)
	{
		mongo::BSONObj  obj(pData);
		MsgAssert(obj.objsize() == size , "FromBson error.");
		FromBson(obj);
	}

	void Test_Oldname::FromBson(const mongo::BSONObj  & obj)
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

	INT64 Test_Oldname::HashMake(INT64 seed)
	{
		INT64 _result = seed;
		_result = CUtil::CityHash(&id,sizeof(id),_result);
		return _result;
		_result = CUtil::CityHash(&p1,sizeof(p1),_result);
		return _result;
	}

	INT64 Test_Oldname::Getid() const
	{
		return id;
	}

	void Test_Oldname::Setid(INT64 & value)
	{
		id = value;
	}

	std::string Test_Oldname::Getp1() const
	{
		return p1;
	}

	void Test_Oldname::Setp1(std::string & value)
	{
		p1 = value;
	}

}//Orm

