// attention dont't change this line:INT32 nConditionID;std::string strServerCondition;std::string strServerAction;std::string strClientCondition;std::string strClientAction;
/************************************
FileName	:	_ConditionConfigBase.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-22 01:03:18
Description	:	csv读取文件
************************************/
#ifndef __Config__ConditionConfigBase_define_h__
#define __Config__ConditionConfigBase_define_h__
#include "_ConditionConfigLoad.h"
#include "../Condition.h"
#include "ConfigDeclare.h"
#include "CUtil/inc/CSVConfig.h"

namespace Config
{

	struct S_ConditionConfig
	{
		INT32							nConditionID;	//条件ID服务器和客户端有一个枚举与之一一对应(这个文件由程序改动.)
		std::string						strServerCondition;	//服务器条件与服务器的枚举值相同.
		std::string						strServerAction;	//服务器执行动作
		std::string						strClientCondition;	//客户端条件与客户端的枚举值相同.
		std::string						strClientAction;	//客户端执行动作
	};


	class _ConditionConfigBase: public CUtil::CSVConfig
	{
	public:
		typedef std_unordered_map<INT32 , S_ConditionConfig> MapConfigsT;

	public:
		bool				LoadFrom(const std::string& filepath);
		bool				RepairLoad(const std::string& filepath);
		S_ConditionConfig	*	Get_ConditionConfig(INT32 id , std::string strFilePath = "");

	public:


	private:
		MapConfigsT			m_mapConfigs;

	};
}

#endif// end  __Config__ConditionConfig_define_h__
