#include "CUtil/inc/CUtil.h"
#include "CUtil/inc/Iconv.h"
//#include "UnitTest++/UnitTestPP.h"
#include "gtest/gtest.h"
#pragma execution_character_set("utf-8")

TEST(LibCore , strcmp)
{
	const char * pStr1 = "strcmp";
	const char * pStr2 = "strcmp"; 
	EXPECT_EQ(CUtil::strcmp(pStr1 , pStr2), 0); // succeeds 
	EXPECT_EQ(CUtil::strcmp("" , "") , 0); // succeeds 

	const char * p1 = "strCMP";
	const char * p2 = "strcmp";
	EXPECT_EQ(CUtil::stricmp(p1 , p2) , 0);
} 

TEST(LibCore , atoi)
{
	const char * pStr1 = "-1";
	const char * pStr2 = "4294967295";
	const char * pStr3 = "9223372036854775807";
	std::string str1 = pStr1;
	std::string str2 = pStr2;
	EXPECT_EQ(CUtil::atoi(pStr1), -1); // succeeds 
	EXPECT_EQ(CUtil::atoi(pStr2) , 4294967295); // succeeds 
	EXPECT_EQ(CUtil::atoi(str1), -1); // succeeds 
	EXPECT_EQ(CUtil::atoi(str2) , 4294967295); // succeeds 
	EXPECT_EQ(CUtil::atoi(pStr3) , 0x7FFFFFFFFFFFFFFF); // succeeds 

	char *endptr;
	char npstr[]="-1238abc";
	INT64 ret = CUtil::strtoll(npstr, &endptr, 10);
	EXPECT_EQ(ret , -1238);
	EXPECT_EQ(CUtil::strcmp(endptr , "abc") , 0);

	ret = CUtil::strtoll(npstr, &endptr, 16);
	EXPECT_EQ(ret , -0x1238abc);
	EXPECT_EQ(CUtil::strcmp(endptr , "") , 0);

	ret = CUtil::strtoll(npstr, &endptr, 8);
	EXPECT_EQ(ret , -0123);
	EXPECT_EQ(CUtil::strcmp(endptr , "8abc") , 0);

	ret = CUtil::strtoll(npstr, &endptr, 2);
	EXPECT_EQ(ret , -01);
	EXPECT_EQ(CUtil::strcmp(endptr , "238abc") , 0);

	UINT64 llRet = CUtil::strtoull(&npstr[1], &endptr, 10);
	EXPECT_EQ(llRet , 1238);
	EXPECT_EQ(CUtil::strcmp(endptr , "abc") , 0);

	llRet = CUtil::strtoull(&npstr[1], &endptr, 16);
	EXPECT_EQ(llRet , 0x1238abc);
	EXPECT_EQ(CUtil::strcmp(endptr , "") , 0);

	llRet = CUtil::strtoull(&npstr[1], &endptr, 8);
	EXPECT_EQ(llRet , 0123);
	EXPECT_EQ(CUtil::strcmp(endptr , "8abc") , 0);

	llRet = CUtil::strtoull(&npstr[1], &endptr, 2);
	EXPECT_EQ(llRet , 01);
	EXPECT_EQ(CUtil::strcmp(endptr , "238abc") , 0);

	char * pf = "-1.123456789";
	float f = 0.0f;
	double df = 0.0f;
	std::string strf = pf;
	f = (float)CUtil::atof(pf);
	df = CUtil::atof(pf);
	EXPECT_EQ(df , -1.123456789);
	EXPECT_EQ(f , -1.1234568f);
	f = (float)CUtil::atof(strf);
	EXPECT_EQ(f , -1.1234568f);
	EXPECT_EQ(df , -1.123456789);
} 

TEST(LibCore , itoa)
{
	INT64 ll = 0xffffffffffffffff;
	EXPECT_EQ(CUtil::itoa(ll) , "-1");
 
	float f = -1.123456789f;
	char pf[1024];;
	CUtil::ftoa(pf , f);
	EXPECT_EQ(CUtil::strcmp(pf , "-1.123457") , 0);
} 

TEST(LibCore , floatcmp)
{ 
	EXPECT_EQ(CUtil::floatcmp(0.0f , -0.0f) , false);
	EXPECT_EQ(CUtil::floatcmp(0.1f , 0.1f) , false);
	EXPECT_EQ(CUtil::floatcmp(0.0001f , 0.00001f) , true);
	EXPECT_EQ(CUtil::floatcmp(0.000001f , 0.0000001f) , false);
} 

TEST(LibCore , strncpy)
{ 
	const char * p1 = "0.123456789";
	char p2[1024];
	CUtil::strncpy(p2 , strlen(p1) + 1 , p1);

	EXPECT_EQ(CUtil::strcmp(p2 , p1) , 0); 
} 

TEST(StrlowerFixture, strlower)
{
	std::string pStr1;
	std::string pStr2;
	pStr1 = "Strlower";
	pStr2 = "strLOWER";

	pStr1 = CUtil::strlower(pStr1.c_str());
	pStr2 = CUtil::strlower(pStr2.c_str());
	EXPECT_EQ(pStr2 , pStr1); // succeeds 
}

TEST(LibCore , strupper)
{
	char * pStr1 = "Strupper";
	char * pStr2 = "strUPPER"; 
	EXPECT_EQ(CUtil::strupper(pStr1) , CUtil::strupper(pStr2)); // succeeds 
}

TEST(LibCore , is_between)
{ 
	EXPECT_EQ(CUtil::is_between(1 , 1 , 2) , FALSE); // succeeds 
	EXPECT_EQ(CUtil::is_between(1 , 2 , 2) , TRUE); // succeeds 
	EXPECT_EQ(CUtil::is_between(1 , 3 , 2) , TRUE); // succeeds 
	EXPECT_EQ(CUtil::is_between(1.0f , 1.0f , 2.0f) , FALSE); // succeeds 
	EXPECT_EQ(CUtil::is_between(1.0f , 1.0f , 1.0f) , TRUE); // succeeds 
	EXPECT_EQ(CUtil::is_between(1.0f , 3.0f , 2.0f) , TRUE); // succeeds 
}

TEST(LibCore , clamp)
{ 
	EXPECT_EQ(CUtil::clamp(2 , 1 , 2) , 2); // succeeds 
	EXPECT_EQ(CUtil::clamp(2 , 1 , 1) , 1); // succeeds  
	EXPECT_EQ(CUtil::clamp(2.0f , 1.0f , 2.0f) , 2.0f); // succeeds 
	EXPECT_EQ(CUtil::clamp(12.0f , 1.0f , 1.0f) , 1.0f); // succeeds  
}

TEST(LibCore , strtobool)
{ 
	EXPECT_EQ(CUtil::strtobool("True") , 1); // succeeds 
	EXPECT_EQ(CUtil::strtobool("False") , 0); // succeeds  
	EXPECT_EQ(CUtil::strtobool("1") , 1); // succeeds 
	EXPECT_EQ(CUtil::strtobool("0") , 0); // succeeds  
	EXPECT_EQ(CUtil::strtobool("2") , 1); // succeeds 
	EXPECT_EQ(CUtil::strtobool("1w") , 1); // succeeds  
}

TEST(LibCore , isNumber)
{ 
	EXPECT_EQ(CUtil::isNumber("123456789") , 1); // succeeds 
	EXPECT_EQ(CUtil::isNumber("0xffffffff") , 0); // succeeds   
}

TEST(LibCore , tohex)
{ 
	INT64 ll = 4294967295;
	char p[1024];
	CUtil::tohex(ll , p , 1024);

	EXPECT_EQ(CUtil::strcmp(p , "ffffffff") , 0); // succeeds 

// 	const char * p1 = "81985529216486895";
// 	char  p2[1024];
// 	INT32 nSize = 0;
// 	CUtil::tohex(p1 , (UINT32)(strlen(p1) + 1) , p2 , nSize);
}

TEST(LibCore , create_token)
{ 
	char p[1024];
	CUtil::create_token(p , 1024);

	bool bSame = false;
	static const char s_objTokenPool[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-=+_";
	for (INT32 i = 0;i < 1024;++i)
	{
		for (INT32 j = 0;j < sizeof(s_objTokenPool);++j)
		{
			if (s_objTokenPool[j] == p[i])
			{
				bSame = true;
				break;
			}			
		}
		EXPECT_EQ(bSame , true);
		bSame = false;
	}
}


TEST(LibCore , strncat)
{ 
	char *p1 = "123456";
	char *p2 = "789";
	char p3[1024] = {0};
	CUtil::strncat(p3 , p1 , (UINT32)(strlen(p1) + 1));
	CUtil::strncat(p3 , p2 , (UINT32)(strlen(p2) + 1));

	EXPECT_EQ(CUtil::strcmp(p3 , "123456789") , 0);
	 
}

TEST(LibCore , ParseHumanSizeFromString)
{ 
	char *p1 = "10K";
	char *p2 = "10M";
	char *p3 = "1G";
	char *p4 = "10240K"; 
	INT64 nSize = CUtil::ParseHumanSizeFromString(p1);
	UINT64 nValue = 1 * 1024 * 1024 * 1024;
	EXPECT_EQ(nSize , 10 * 1024);
	nSize = CUtil::ParseHumanSizeFromString(p2);
	EXPECT_EQ(nSize , 10 * 1024 * 1024 );
	nSize = CUtil::ParseHumanSizeFromString(p3);
	EXPECT_EQ(nSize ,  nValue);
	nSize = CUtil::ParseHumanSizeFromString(p4);
	EXPECT_EQ(nSize , 10 * 1024 * 1024); 
}

TEST(LibCore , Compress)
{ 
	std::string strUncompressed;
	char *p1 = "10Kasdfadfasdfadfadfadfasdfa";
	CUtil::Compress(p1 , (UINT32)(strlen(p1) + 1) , strUncompressed);

	std::string strUncompressed2;
	CUtil::Uncompress(strUncompressed.c_str() , (UINT32)(strUncompressed.length() + 1) , strUncompressed2);
	EXPECT_EQ(strUncompressed2 , std::string(p1 , strlen(p1) + 1)); 
}

TEST(LibCore , IConv)
{
	std::string strUTF8 = R"(UTF8-编码)";
	std::string strGBK = CUtil::UTF8ToGBK(strUTF8);
	std::string strNewUTF8 = CUtil::GBKToUTF8(strGBK);

	EXPECT_EQ(strNewUTF8 , strUTF8);
}