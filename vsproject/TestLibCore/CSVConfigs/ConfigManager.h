/************************************
FileName	:	ConfigManager.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-22 11:43:23
Description	:	ConfigManager文件声明
************************************/
#ifndef __Config_ConfigManager_define_h__
#define __Config_ConfigManager_define_h__
#include "CUtil/inc/Common.h"
#include "Lua/lua_tinker.h"

namespace Config
{

	class ConfigManager
	{
	public:
		ConfigManager();
		virtual ~ConfigManager();

	public:
		static ConfigManager & GetInstance();

	public:
		virtual INT32		Init(std::string  strCsvPath);
		virtual INT32		Cleanup();
		virtual INT32		LoadFrom(std::string  strCsvPath);
		virtual INT32		RepairLoad(std::string  strCsvPath);
		virtual INT32		ExportClassToLua(lua_State* L);
		
	public:
	private:
		

	};
}

#endif// end  __Config_ConfigManager_define_h__
