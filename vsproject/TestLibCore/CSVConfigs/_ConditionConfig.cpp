// attention dont't change this line:INT32 nConditionID;std::string strServerCondition;std::string strServerAction;std::string strClientCondition;std::string strClientAction;
/************************************
FileName	:	_ConditionConfig.cpp
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.40.128
Version		:	0.0.1
Date		:	2016-04-28 14:51:27
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

