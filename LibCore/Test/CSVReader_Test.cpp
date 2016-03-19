#include "CUtil/inc/CSVReader.h"
//#include "UnitTest++/UnitTestPP.h"
#include "CUtil/inc/CUtil.h"
#include "gtest/gtest.h"

const char * g_strTest = R"("轨迹ID"	"生效场景 --不配表示所有场景中生效 --否则仅在配置场景中生效"	"循环方式"	"界面中图标"	"属性修改值"	"起始点"	"备注1"	"坐标系"	
"curve_id"	"validStages"	"wrapMode"	"InterfaceIcon"	"attr_mod_val"	"start_x"	"col_1"	"isLocal"	
"int"	"int[]"	"string"	"string[]"	"float"	"float[]"	"long"	"bool"	
"c"	"s"	"s"	"cs"	"cs"	"cs"	"cs"	"s"	
"1"	",2003,2005,2006"	"once"	"act_image,molong"	"25.5"	"9.76649,8.890732,-16.13906"	"410"	"1"	
"2"	",2003,2005,2006"	"once"	"act_image,molong"	"50.7"	"9.76649,8.890732,-16.13906"	"5522677700"	"1"	
"3"	",2003,2005,2006"	"once"	"act_image,molong"	"25.6"	"9.76649,8.890732,-16.13906"	"5522677700"	"0"	
)"; 

struct SAction
{
	INT32 curve_id;	// 轨迹ID
	std::vector<INT32> validStages;	// 生效场景 --不配表示所有场景中生效 --否则仅在配置场景中生效
	std::string wrapMode;	// 循环方式
	std::vector<std::string> InterfaceIcon;	// 界面中图标
	float attr_mod_val;	// 属性修改值
	std::vector<float> start_x;	// 起始点
	INT64 col_1;	// 备注1
	bool isLocal;	// 坐标系
};

TEST(CSVReader , read)
{
	CUtil::CSVReader csv;
	EXPECT_EQ(csv.Load(g_strTest) , 0);

	size_t index_curve_id = csv.GetIndex("curve_id", 1);
	MsgAssert(index_curve_id != (size_t)-1 , "error");

	size_t index_validStages = csv.GetIndex("validStages", 1);
	MsgAssert(index_validStages != (size_t)-1 , "error");

	size_t index_wrapMode = csv.GetIndex("wrapMode", 1);
	MsgAssert(index_wrapMode != (size_t)-1 , "error");

	size_t index_InterfaceIcon = csv.GetIndex("InterfaceIcon", 1);
	MsgAssert(index_InterfaceIcon != (size_t)-1 , "error");

	size_t index_attr_mod_val = csv.GetIndex("attr_mod_val", 1);
	MsgAssert(index_attr_mod_val != (size_t)-1 , "error");

	size_t index_start_x = csv.GetIndex("start_x", 1);
	MsgAssert(index_start_x != (size_t)-1 , "error");

	size_t index_col_1 = csv.GetIndex("col_1", 1);
	MsgAssert(index_col_1 != (size_t)-1 , "error");
	
	size_t index_isLocal = csv.GetIndex("isLocal", 1);
	MsgAssert(index_isLocal != (size_t)-1 , "error");
	
	typedef std::vector<SAction> CollectionConfiguresT;
	CollectionConfiguresT vec;
	for (size_t row = 3; row < csv.Count(); ++row)
	{
		SAction conf;
		conf.curve_id = csv.GetInt32(row, index_curve_id);
		{
			std::vector<std::string> vals;
			std::string __tmp = csv.GetString(row, index_validStages);
			CUtil::tokenize(__tmp, vals, ",", "", "\"");;
			for (size_t i = 0; i < vals.size(); ++i)
				conf.validStages.push_back((INT32)CUtil::atoi(vals[i].c_str()));
		}
		conf.wrapMode = csv.GetString(row, index_wrapMode);
		{
			std::string __tmp = csv.GetString(row, index_InterfaceIcon);
			CUtil::tokenize(__tmp, conf.InterfaceIcon, ",", "", "\"");;
		}
		conf.attr_mod_val = (float)csv.GetDouble(row, index_attr_mod_val);
		{
			std::vector<std::string> vals;
			std::string __tmp = csv.GetString(row, index_start_x);
			CUtil::tokenize(__tmp, vals, ",", "", "\"");;
			for (size_t i = 0; i < vals.size(); ++i)
				conf.start_x.push_back((float)CUtil::atof(vals[i].c_str()));
		}
		conf.col_1 = csv.GetInt64(row, index_col_1);
		conf.isLocal = csv.GetBool(row, index_isLocal);
		vec.push_back(conf);
	}

	EXPECT_EQ(vec[0].curve_id, 1);
	EXPECT_EQ(vec[0].validStages[0], 2003);
	EXPECT_EQ(vec[0].validStages[1], 2005);
	EXPECT_EQ(vec[0].validStages[2], 2006);
	EXPECT_EQ(vec[0].wrapMode, "once");
	EXPECT_EQ(vec[0].InterfaceIcon[0], "act_image");
	EXPECT_EQ(vec[0].InterfaceIcon[1], "molong");
// 	EXPECT_EQ(vec[0].attr_mod_val, 25.5);
// 	EXPECT_EQ(vec[1].attr_mod_val, 50.7);
// 	EXPECT_EQ(vec[2].attr_mod_val, 25.6);
// 	EXPECT_EQ(vec[0].start_x[0], 9.76649);
// 	EXPECT_EQ(vec[0].start_x[1], 8.890732);
// 	EXPECT_EQ(vec[0].start_x[2], -16.13906);
	EXPECT_EQ(vec[0].col_1, 410);
	EXPECT_EQ(vec[0].isLocal, true);
	EXPECT_EQ(vec[2].isLocal, false);
}