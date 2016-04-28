// attention dont't change this line:INT32 id;std::map<std::string , bool> boolCommon;std::map<std::string , bool> boolCommon2;std::map<std::string , INT32> intCommon;std::map<std::string , INT64> int64Common;std::map<std::string , double> doubleCommon;std::map<std::string , std::string> stringCommon;std::map<std::string , Timer::Date> dateCommon;
/************************************
FileName	:	_CommonData.cpp
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.40.128
Version		:	0.0.1
Date		:	2016-04-28 14:51:27
Description	:	csv读取数据文件实现
************************************/
#include "_CommonData.h"
#include "LogLib/inc/Log.h"

namespace Config
{
	//tools after data load success , call OnLoad;
	BOOL _CommonData::OnLoad()
	{
		return FALSE;
	}
}

