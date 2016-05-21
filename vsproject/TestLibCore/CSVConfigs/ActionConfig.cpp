// attention dont't change this line:INT64 curve_id;std::vector<INT32> validStages;std::string wrapMode;std::vector<std::string> InterfaceIcon;double attr_mod_val;std::vector<double> start_x;INT64 col_1;bool isLocal;TestStructbool test1;INT64 test2;std_unordered_map<std::string , SActivityConfig *> test3;INT32 test4;std::string test6;TestStructArraybool test1;INT64 test2;double test3;INT32 test4;std_unordered_map<std::string , SActivityConfig *> test5;
/************************************
FileName	:	ActionConfig.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-21 14:51:04
Description	:	csv读取数据文件实现
************************************/
#include "ActionConfig.h"
#include "LogLib/inc/Log.h"

namespace Config
{
	//tools after data load success , call OnLoad;
	BOOL ActionConfig::OnLoad()
	{
		return FALSE;
	}
}

