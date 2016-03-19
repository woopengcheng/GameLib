// attention dont't change this line:INT32 curve_id;std::vector<INT32> validStages;std::string wrapMode;std::vector<std::string> InterfaceIcon;double attr_mod_val;std::vector<double> start_x;INT64 col_1;bool isLocal;TestStructbool test1;INT64 test2;double test3;INT32 test4;std::string test6;TestStructArraybool test1;INT64 test2;double test3;INT32 test4;std::string test5;
/************************************
FileName	:	ActionConfig.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-03-09 21:35:57
Description	:	csv读取文件
************************************/
#ifndef __Config_ActionConfig_define_h__
#define __Config_ActionConfig_define_h__
#include "ActionConfigLoad.h"

namespace Config
{

	struct SActionConfig
	{
		INT32							curve_id;	//轨迹ID
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


	class ActionConfig
	{
	public:
		typedef std_unordered_map<INT32 , SActionConfig> MapConfigsT;

	public:
		bool LoadFrom(const std::string& filepath);
		SActionConfig * GetActionConfig(INT32 nIndex);

	private:
		MapConfigsT m_mapConfigs;

	};
	extern ActionConfig * g_pActionConfig;
}

#endif// end  __Config_ActionConfig_define_h__
