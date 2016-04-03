// attention dont't change this line:INT32 nConditionID;std::string strServerCondition;std::string strServerAction;std::string strClientCondition;std::string strClientAction;
/************************************
FileName	:	_ConditionConfig.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-03 12:46:51
Description	:	csv读取文件
************************************/
#ifndef __Config__ConditionConfig_define_h__
#define __Config__ConditionConfig_define_h__
#include "_ConditionConfigLoad.h"
#include "../Condition.h"

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


	class _ConditionConfig
	{
	public:
		typedef std_unordered_map<INT32 , S_ConditionConfig> MapConfigsT;

	public:
		bool				LoadFrom(const std::string& filepath);
		S_ConditionConfig *	Get_ConditionConfig(INT32 nIndex);

	public:


	private:
		MapConfigsT m_mapConfigs;

	};
	extern _ConditionConfig * g_p_ConditionConfig;
}

#endif// end  __Config__ConditionConfig_define_h__
