#include "CUtil/inc/BsonToCpp.h"
#include "CUtil/inc/Parameter.h"

namespace CUtil
{

	void BsonToCpp(bool& var,mongo::BSONElement& be)
	{
		if(be.isBoolean())
		{
			var = be.boolean();
			return ;
		}
		MsgAssert(false,__FUNCTION__ << " to bool:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp(UINT8& var,mongo::BSONElement& be)
	{
		if(be.isNumber())
		{
			var = (UINT8)be.numberInt();
			return ;
		}
		MsgAssert(false,__FUNCTION__ << " to UINT8:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp(INT16& var,mongo::BSONElement& be)
	{
		if(be.isNumber())
		{
			var = (INT16)be.numberInt();
			return ;
		}
		MsgAssert(false,__FUNCTION__ << " to INT16:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp(float& var,mongo::BSONElement& be)
	{
		if(be.isNumber())
		{
			var = (float)be.numberDouble();
			return ;
		}
		MsgAssert(false,__FUNCTION__ << " to float:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp(double& var,mongo::BSONElement& be)
	{
		if(be.isNumber())
		{
			var = be.numberDouble();
			return ;
		}
		MsgAssert(false,__FUNCTION__ << " to double:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp(INT32& var,mongo::BSONElement& be)
	{
		if(be.isNumber())
		{
			var = be.numberInt();
			return ;
		}
		MsgAssert(false,__FUNCTION__ << " to INT32:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp(INT64& var,mongo::BSONElement& be)
	{
		if(be.isNumber())
		{
			var = be.numberLong();
			return ;
		}
		MsgAssert(false,__FUNCTION__ << " to INT64:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp(std::string& var,mongo::BSONElement& be)
	{
		if(be.type() == mongo::String)
		{
			var = std::string(be.valuestrsafe(),be.valuestrsize()-1);
			return;
		}
		else if(be.type() == mongo::BinData)
		{
			int len = 0;
			const char* data = be.binData(len);
			var.clear();
			var.append(data,len);
			return;
		}
		MsgAssert(false,__FUNCTION__ << " to string:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp(StringExBase& var,mongo::BSONElement& be)
	{
		if(be.type() == mongo::String)
		{
			var.Init(be.valuestrsafe(),be.valuestrsize());
			return;
		}
		MsgAssert(false,__FUNCTION__ << " to fixed_string_base:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp( UINT32& var,mongo::BSONElement& be )
	{
		if(be.isNumber())
		{
			var = (UINT32)be.numberInt();
			return ;
		}
		MsgAssert(false,__FUNCTION__ << " to UINT32:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp( UINT64& var,mongo::BSONElement& be )
	{
		if(be.isNumber())
		{
			var = (UINT64)be.numberInt();
			return ;
		}
		MsgAssert(false,__FUNCTION__ << " to UINT64:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}

	void BsonToCpp( Parameter & var,mongo::BSONElement& be )
	{
		if(be.type() == mongo::BinData)
		{
			if (mongo::bdtParamter == be.binDataType())
			{
				int len = 0;
				const char* data = be.binData(len);
				var.GetParamStream().Pushback((void*)data , len);
			}

			return ;
		}  

		MsgAssert(false,__FUNCTION__ << " to Parameter:" << be.toString());
		Assert(false && "BsonToCpp: invalid convert");
	}
}