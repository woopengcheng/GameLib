// attention dont't change this line:INT32 id;std::map<std::string , bool> boolCommon;std::map<std::string , bool> boolCommon2;std::map<std::string , INT32> intCommon;std::map<std::string , INT64> int64Common;std::map<std::string , double> doubleCommon;std::map<std::string , std::string> stringCommon;std::map<std::string , Timer::Date> dateCommon;
/************************************
FileName	:	_CommonData.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-30 02:32:23
Description	:	csv读取数据文件实现
************************************/
#include "_CommonData.h"
#include "LogLib/inc/Log.h"

namespace Config
{
	_CommonData * g_p_CommonData = NULL;

	//tools after data load success , call OnLoad;
	BOOL _CommonData::OnLoad()
	{
		return FALSE;
	}
}

