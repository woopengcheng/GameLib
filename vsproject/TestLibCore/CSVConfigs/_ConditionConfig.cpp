﻿// attention dont't change this line:INT32 nConditionID;std::string strServerCondition;std::string strServerAction;std::string strClientCondition;std::string strClientAction;
/************************************
FileName	:	_ConditionConfig.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-22 11:43:23
Description	:	csv读取数据文件实现
************************************/
#include "_ConditionConfig.h"
#include "LogLib/inc/Log.h"

namespace Config
{
	//tools after data load success , call OnLoad;
	BOOL _ConditionConfig::OnLoad()
	{
		return FALSE;
	}
}

