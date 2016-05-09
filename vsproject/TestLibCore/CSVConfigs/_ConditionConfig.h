// attention dont't change this line:INT32 nConditionID;std::string strServerCondition;std::string strServerAction;std::string strClientCondition;std::string strClientAction;
/************************************
FileName	:	_ConditionConfig.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-09 23:54:53
Description	:	csv读取文件
************************************/
#ifndef __Config__ConditionConfig_define_h__
#define __Config__ConditionConfig_define_h__
#include "_ConditionConfigBase.h"

namespace Config
{

	class _ConditionConfig: public _ConditionConfigBase
	{
	public:
		virtual BOOL	OnLoad();
		
	private:

	};
	extern _ConditionConfig * g_p_ConditionConfig;
}

#endif// end  __Config__ConditionConfig_define_h__
