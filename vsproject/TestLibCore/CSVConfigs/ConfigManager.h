/************************************
FileName	:	ConfigManager.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-21 14:51:04
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
		INT32		LoadFrom(std::string  strCsvPath);
		INT32		RepairLoad(std::string  strCsvPath);
		
	private:
		

	};
}

#endif// end  __Config_ConfigManager_define_h__
