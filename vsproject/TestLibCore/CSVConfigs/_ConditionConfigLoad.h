/************************************
FileName	:	_ConditionConfigLoad.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-03 01:24:06
Description	:	csv配置文件
************************************/
#ifndef ___ConditionConfigLoad_define_h__
#define ___ConditionConfigLoad_define_h__
#include "CUtil/inc/Common.h "

#include "Timer/inc/Date.h "

namespace Config
{
	struct S_ConditionConfigLoad
	{
		INT32							nConditionID;	//条件ID服务器和客户端有一个枚举与之一一对应(这个文件由程序改动.)
		std::string						strServerCondition;	//服务器条件与服务器的枚举值相同.
		std::string						strServerAction;	//服务器执行动作
		std::string						strClientCondition;	//客户端条件与客户端的枚举值相同.
		std::string						strClientAction;	//客户端执行动作
	};


	class _ConditionConfigLoad
	{
	public:
		typedef std::vector<S_ConditionConfigLoad> CollectionConfigsT;

	public:
		bool LoadFrom(const std::string& filename);

	public:
		S_ConditionConfigLoad & Get(size_t row);

	public:
		inline size_t Count(){ return m_vtConfigs.size(); }

	private:
		CollectionConfigsT m_vtConfigs;
	};
}

#endif// end  ___ConditionConfigLoad_define_h__
