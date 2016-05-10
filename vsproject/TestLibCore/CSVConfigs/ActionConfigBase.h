// attention dont't change this line:INT64 curve_id;std::vector<INT32> validStages;std::string wrapMode;std::vector<std::string> InterfaceIcon;double attr_mod_val;std::vector<double> start_x;INT64 col_1;bool isLocal;TestStructbool test1;INT64 test2;double test3;INT32 test4;std::string test6;TestStructArraybool test1;INT64 test2;double test3;INT32 test4;std::string test5;
/************************************
FileName	:	ActionConfigBase.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.40.128
Version		:	0.0.1
Date		:	2016-05-10 12:12:16
Description	:	csv读取文件
************************************/
#ifndef __Config_ActionConfigBase_define_h__
#define __Config_ActionConfigBase_define_h__
#include "ActionConfigLoad.h"
#include "../Condition.h"
#include "CUtil/inc/CSVConfig.h"

namespace Config
{


	struct SActionConfig
	{
		INT64							curve_id;	//轨迹ID
		std::vector<INT32>				validStages;	//生效场景--不配表示所有场景中生效--否则仅在配置场景中生效
		std::string						wrapMode;	//循环方式
		std::vector<std::string>		InterfaceIcon;	//界面中图标
		double							attr_mod_val;	//属性修改值
		std::vector<double>				start_x;	//起始点
		INT64							col_1;	//备注1
		bool							isLocal;	//坐标系

		//结构体
		struct STestStruct
		{
			bool						test1;
			INT64						test2;
			double						test3;
			INT32						test4;
			std::string					test6;
		}TestStruct;

		//结构体数组
		struct STestStructArray
		{
			bool						test1;
			INT64						test2;
			double						test3;
			INT32						test4;
			std::string					test5;
		};
		std::vector<STestStructArray>		vecTestStructArray;
	};


	class ActionConfigBase: public CUtil::CSVConfig
	{
	public:
		typedef std_unordered_map<INT64 , SActionConfig> MapConfigsT;

	public:
		bool				LoadFrom(const std::string& filepath);
		SActionConfig *	GetActionConfig(INT64 id , std::string strFilePath = "");

	public:


	private:
		MapConfigsT			m_mapConfigs;

	};
}

#endif// end  __Config_ActionConfig_define_h__
