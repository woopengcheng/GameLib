#include "Role.h" 
#include "CUtil/inc/BsonToCpp.h"

namespace Orm
{ 
	Role::Role()
		: name("")
		, pswd("")
		, sysuser(false)
		, __hash(0)
	{
	}

	Role::~Role()
	{
	}
	std::string Role::GetRawKey()
	{
		std::string result;
		result.reserve(64);

		result.append(name.c_str());
		return result;
	}

	std::string Role::GetKey()
	{
		return this->GetRawKey();
	}

	std::string Role::GetTableName()
	{
		return Role::TableName();
	}

	void Role::AutoIncrease(INT64 llKey)
	{
		assert(false);
	}

	Role* Role::Clone()
	{ 
		mongo::BSONObj  obj;
		ToBson(obj);
		Role* pNew = new Role();
		pNew->FromBson(obj);
		return pNew;
	}

	void Role::ToCompress(std::string & __buf)
	{
		mongo::BSONObj  __obj;
		ToBson(__obj);
		CUtil::Compress(__obj.objdata(),__obj.objsize(),__buf);
	}

	void Role::ToBson(std::string& __buf)
	{
		mongo::BSONObj  __obj;
		ToBson(__obj);
		__buf = std::string(__obj.objdata(),__obj.objsize());
	}

	void Role::ToBson(mongo::BSONObj  & __obj)
	{
		mongo::BSONObjBuilder __builder;
		__builder.append("_T",TableName());
		if(name != "")
			__builder.append("name",name);
		if(pswd != "")
			__builder.append("pswd",pswd);
		if(sysuser != false)
			__builder.append("sysuser",sysuser);
		__obj = __builder.obj();
	}

	void Role::FromCompress(const std::string& __inbuf)
	{
		std::string tmpbuf;
		CUtil::Uncompress(__inbuf.c_str(),(UINT32)__inbuf.length(),tmpbuf);
		mongo::BSONObj  __obj(tmpbuf.c_str());
		assert(__obj.objsize() == tmpbuf.length());
		FromBson(__obj);
	}

	void Role::FromCompress(const char* __data,INT32 __size)
	{
		std::string tmpbuf;
		CUtil::Uncompress(__data,__size,tmpbuf);
		mongo::BSONObj  __obj(tmpbuf.c_str());
		assert(__obj.objsize() == tmpbuf.length());
		FromBson(__obj);
	}

	void Role::FromBson(const char* __data,INT32 __size)
	{
		mongo::BSONObj  __obj(__data);
		assert(__obj.objsize() == __size);
		FromBson(__obj);
	}

	void Role::FromBson(const mongo::BSONObj  & __obj)
	{
		mongo::BSONObjIterator  iter(__obj); 
		while(iter.more())
		{
			mongo::BSONElement __be = iter.next(); 
			const char* fieldName = __be.fieldName();
			INT64 hash = CUtil::BKDRHashSum(fieldName);
			switch(hash)
			{
			case 768799158513: // _T
				{
					assert(CUtil::strcmp(__be.valuestr(),TableName()) == 0);
				}
				break;
			case 1791250331439: // name type: string
				{
					CUtil::BsonToCpp(name,__be);
				}
				break;
			case 1915809189412: // pswd type: string
				{
					CUtil::BsonToCpp(pswd,__be);
				}
				break;
			case 3427387595580: // sysuser type: bool
				{
					CUtil::BsonToCpp(sysuser,__be);
				}
				break;
			}
		}
		__hash = HashMake(0);
	}


	INT64 Role::HashMake(INT64 seed)
	{
		INT64 _result = seed;
		_result = CUtil::CityHash(&name,sizeof(name),_result);

		_result = CUtil::CityHash(&pswd,sizeof(pswd),_result);

		_result = CUtil::CityHash(&sysuser,sizeof(sysuser),_result);

		return _result;
	}

	const char* Role::get_name() const
	{
		return name.c_str();
	}

	void Role::set_name(const char* value)
	{
		name = value;
	}

	const char* Role::get_pswd() const
	{
		return pswd.c_str();
	}

	void Role::set_pswd(const char* value)
	{
		pswd = value;
	}

	bool Role::get_sysuser() const
	{
		return sysuser;
	}

	void Role::set_sysuser(bool value)
	{
		sysuser = value;
	}

	void Role::HashUpdate(INT64 llHash)
	{ 
		__hash = llHash; 
	}

	void Role::SetMasterID(INT64 llID)
	{

	}

	void Role::SetMasterID(const char* pID)
	{

	}

	INT64 Role::GetMasterID()
	{
		return 0;
	}

	const char* Role::GetMasterStrID()
	{
		return "";
	}

	INT64 Role::CurHash()
	{
		return __hash;
	}

	const char* Role::TableName()
	{
		return "role"; 
	}

}