/************************************
FileName	:	OrmStructs.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.31.196
Version		:	0.0.1
Date		:	2016-02-09 11:26:27
Description	:	数据库中定义的数据.
************************************/
#ifndef __Orm_orm_structs_h__
#define __Orm_orm_structs_h__
#include "CUtil/inc/Common.h"
#include "CUtil/inc/Chunk.h"
#include "CUtil/inc/Parameter.h"
#include "CUtil/inc/ParameterHelper.h"

namespace Orm
{
	class TestStruct: public CUtil::Marshal
	{ 
		public:
		int p1;
		unsigned int p2;
		char p3;
		unsigned char p4;
		std::vector<INT32> p5;
		long p6;
		time_t p7;
		short p8;
		unsigned short p9;
		double p10;
		float p11;
		std_string p12;
		CUtilChunk p13;

		TestStruct()
			: p1( 111 ) 
			, p2( 21 ) 
			, p3( 0 ) 
			, p4( 0 ) 
			, p5( std::vector<INT32>(1) ) 
			, p6( 0 ) 
			, p7( 0 ) 
			, p8( 0 ) 
			, p9( 0 ) 
			, p10( 1.0f ) 
			, p11( 1.0f ) 
			, p12( std::string() ) 
			, p13( CUtil::Chunk() ) 
			{}
		TestStruct(const TestStruct & val)
		{ 
			p1 = val.p1;
			p2 = val.p2;
			p3 = val.p3;
			p4 = val.p4;
			p5 = val.p5;
			p6 = val.p6;
			p7 = val.p7;
			p8 = val.p8;
			p9 = val.p9;
			p10 = val.p10;
			p11 = val.p11;
			p12 = val.p12;
			p13 = val.p13;
		} 

		bool IsEqual(const TestStruct & val)
		{ 
			if(
				p1 == val.p1&&
				p2 == val.p2&&
				p3 == val.p3&&
				p4 == val.p4&&
				p5 == val.p5&&
				p6 == val.p6&&
				p7 == val.p7&&
				p8 == val.p8&&
				p9 == val.p9&&
				p10 == val.p10&&
				p11 == val.p11&&
				p12 == val.p12&&
				p13 == val.p13)
			{
				return true;
			}
			return false;
		} 

		bool operator == (const TestStruct & val)
		{ 
			return IsEqual(val);
		} 

		bool operator != (const TestStruct & val)
		{ 
			return !IsEqual(val);
		} 

		virtual CUtil::CStream & marshal(CUtil::CStream & cs) const override
		{ 
			cs << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8 << p9 << p10 << p11 << p12 << p13;
			return cs; 
		} 

		virtual CUtil::CStream & unMarshal(CUtil::CStream & cs) override
		{ 
			cs >> p1 >> p2 >> p3 >> p4 >> p5 >> p6 >> p7 >> p8 >> p9 >> p10 >> p11 >> p12 >> p13;
			return cs; 
		} 

	}; 
 
}//Orm

namespace CUtil
{ 
	enum PARAMETER_TYPE_ORM_STRUCTS_USER_DEFINES 
	{ 
		PARAMETER_TYPE_ORM_STRUCTS_USER_DEFINE_FIRST = CUtil::PARAMETER_TYPE_USER_DEFINE + 156 ,
		PARAMETER_TYPE_ORM_STRUCTS_USER_DEFINE_TestStruct,
	}; 
 
	GEN_PARAMTER_HELPER(Orm::TestStruct , PARAMETER_TYPE_ORM_STRUCTS_USER_DEFINE_TestStruct);
}

#endif
