/************************************
FileName	:	RpcDatas.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.31.196
Version		:	0.0.1
Description	:	网络消息的数据域.
************************************/
#ifndef __Server_rpc_datas_h__
#define __Server_rpc_datas_h__
#include "CUtil/inc/Common.h"
#include "CUtil/inc/Chunk.h"
#include "CUtil/inc/Parameter.h"
#include "CUtil/inc/ParameterHelper.h"

class TestRpcData : public CUtil::Marshal
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

	TestRpcData()
		: p1(111)
		, p2(21)
		, p3(0)
		, p4(0)
		, p5(std::vector<INT32>(0))
		, p6(0)
		, p7(0)
		, p8(0)
		, p9(0)
		, p10(1.0f)
		, p11(1.0f)
		, p12(std::string())
		, p13(CUtil::Chunk())
	{}
	TestRpcData(const TestRpcData & val)
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

class TestRpcData2 : public CUtil::Marshal
{
public:
	SINT8 p1;
	UINT8 p2;
	INT16 p3;
	UINT16 p4;
	INT32 p6;
	UINT32 p7;
	INT64 p8;
	UINT64 p9;

	TestRpcData2()
		: p1(0)
		, p2(0)
		, p3(0)
		, p4(0)
		, p6(0)
		, p7(0)
		, p8(0)
		, p9(0)
	{}
	TestRpcData2(const TestRpcData2 & val)
	{
		p1 = val.p1;
		p2 = val.p2;
		p3 = val.p3;
		p4 = val.p4;
		p6 = val.p6;
		p7 = val.p7;
		p8 = val.p8;
		p9 = val.p9;
	}

	virtual CUtil::CStream & marshal(CUtil::CStream & cs) const override
	{
		cs << p1 << p2 << p3 << p4 << p6 << p7 << p8 << p9;
		return cs;
	}

	virtual CUtil::CStream & unMarshal(CUtil::CStream & cs) override
	{
		cs >> p1 >> p2 >> p3 >> p4 >> p6 >> p7 >> p8 >> p9;
		return cs;
	}

};


namespace CUtil
{
	enum PARAMETER_TYPE_USER_DEFINES
	{
		PARAMETER_TYPE_USER_DEFINE_FIRST = CUtil::PARAMETER_TYPE_USER_DEFINE,
		PARAMETER_TYPE_USER_DEFINE_TestRpcData,
		PARAMETER_TYPE_USER_DEFINE_TestRpcData2,
	};

	GEN_PARAMTER_HELPER(TestRpcData, PARAMETER_TYPE_USER_DEFINE_TestRpcData);
	GEN_PARAMTER_HELPER(TestRpcData2, PARAMETER_TYPE_USER_DEFINE_TestRpcData2);
}

#endif
