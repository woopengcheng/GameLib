/************************************
FileName	:	ConfigManager.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.40.128
Version		:	0.0.1
Date		:	2016-05-10 12:12:16
Description	:	ConfigManager文件声明
************************************/
#ifndef __Config_ConfigManager_define_h__
#define __Config_ConfigManager_define_h__
#include "CUtil/inc/Common.h"

namespace Config
{

	class ConfigManager
	{
	public:
		ConfigManager();
		~ConfigManager();

	public:
		static ConfigManager & GetInstance();

	public:
		INT32		Init(std::string  strCsvPath);
		INT32		Cleanup();
	private:
		

	};
}

#endif// end  __Config_ConfigManager_define_h__
