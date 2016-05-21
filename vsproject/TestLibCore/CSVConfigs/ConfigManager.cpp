/************************************
FileName	:	ConfigManager.cpp
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-22 01:03:18
Description	:	ConfigManager数据管理文件实现
************************************/
#include "ConfigManager.h"
#include "LogLib/inc/Log.h"
#include "ActionConfig.h"
#include "ActivityConfig.h"
#include "_ConditionConfig.h"
#include "_CommonData.h"
	

namespace Config
{
	ConfigManager::ConfigManager()
	{
	}

	ConfigManager::~ConfigManager()
	{
	}

	ConfigManager & ConfigManager::GetInstance()
	{
		static ConfigManager instance;
		return instance;
	}

	INT32 ConfigManager::Init(std::string  strCsvPath)
	{
		MsgAssert_ReF1(strCsvPath.length(), "ConfigManager::Init error.");

		if (strCsvPath[strCsvPath.length() - 1] != '/')
		{
			strCsvPath = strCsvPath + "/";
		}

		LoadFrom(strCsvPath);
		RepairLoad(strCsvPath);
	
		return 0;
	}

	INT32 ConfigManager::LoadFrom(std::string  strCsvPath)
	{
		MsgAssert_ReF1(strCsvPath.length(), "ConfigManager::LoadFrom error.");

		if (strCsvPath[strCsvPath.length() - 1] != '/')
		{
			strCsvPath = strCsvPath + "/";
		}

		Config::ActionConfig::GetInstance().LoadFrom(strCsvPath);
		Config::ActivityConfig::GetInstance().LoadFrom(strCsvPath);
// 		Config::_ConditionConfig::GetInstance().LoadFrom(strCsvPath);
		Config::_CommonData::GetInstance().LoadFrom(strCsvPath);
		return 0;
	}

	INT32 ConfigManager::RepairLoad(std::string  strCsvPath)
	{
		MsgAssert_ReF1(strCsvPath.length(), "ConfigManager::RepairLoad error.");

		if (strCsvPath[strCsvPath.length() - 1] != '/')
		{
			strCsvPath = strCsvPath + "/";
		}

		Config::ActionConfig::GetInstance().RepairLoad(strCsvPath);
		Config::ActivityConfig::GetInstance().RepairLoad(strCsvPath);
// 		Config::_ConditionConfig::GetInstance().RepairLoad(strCsvPath);
		Config::_CommonData::GetInstance().RepairLoad(strCsvPath);
		return 0;
	}

	INT32 ConfigManager::ExportClassToLua(lua_State* L)
	{
		if (L == NULL)
		{
			return -1;
		}
		
		lua_tinker::namespace_add(L, "Config");
		lua_tinker::namespace_add(L, "CUtil");
		lua_tinker::class_add<CUtil::CSVConfig>(L, "CUtil::CSVConfig", true);
		lua_tinker::class_add<CUtil::CSVConfig>(L, "CUtil::CSVConfig", true);
		lua_tinker::class_def<CUtil::CSVConfig>(L, "IsLoaded", &CUtil::CSVConfig::IsLoaded);
		
		
		lua_tinker::class_add<Config::ActionConfigBase>(L, "Config::ActionConfigBase", true);
		lua_tinker::class_add<Config::ActionConfig>(L, "Config::ActionConfig", true);
		lua_tinker::scope_inner(L, "Config" , "ActionConfig" , "Config::ActionConfig");
		lua_tinker::class_inh<Config::ActionConfig , Config::ActionConfigBase>(L);
		lua_tinker::class_def_static<Config::ActionConfig>(L, "GetInstance", & Config::ActionConfig::GetInstance);
		lua_tinker::class_def_static<Config::ActionConfig>(L, "GetInstancePtr", & Config::ActionConfig::GetInstancePtr);
		lua_tinker::class_def<Config::ActionConfig>(L, "GetActionConfig", & Config::ActionConfig::GetActionConfig , std::string(""));
		
		lua_tinker::class_add<Config::SActionConfig>(L, "Config::SActionConfig", true);
		lua_tinker::scope_inner(L, "Config" , "SActionConfig" , "Config::SActionConfig");
		lua_tinker::class_mem<Config::SActionConfig>(L, "curve_id", &Config::SActionConfig::curve_id);
		lua_tinker::class_mem<Config::SActionConfig>(L, "validStages", &Config::SActionConfig::validStages);
		lua_tinker::class_mem<Config::SActionConfig>(L, "wrapMode", &Config::SActionConfig::wrapMode);
		lua_tinker::class_mem<Config::SActionConfig>(L, "InterfaceIcon", &Config::SActionConfig::InterfaceIcon);
		lua_tinker::class_mem<Config::SActionConfig>(L, "attr_mod_val", &Config::SActionConfig::attr_mod_val);
		lua_tinker::class_mem<Config::SActionConfig>(L, "start_x", &Config::SActionConfig::start_x);
		lua_tinker::class_mem<Config::SActionConfig>(L, "col_1", &Config::SActionConfig::col_1);
		lua_tinker::class_mem<Config::SActionConfig>(L, "isLocal", &Config::SActionConfig::isLocal);
		{
			lua_tinker::class_add<Config::SActionConfig::STestStruct>(L, "Config::SActionConfig::STestStruct", true);
			lua_tinker::scope_inner(L , "Config::SActionConfig","STestStruct" , "Config::SActionConfig::STestStruct");
			lua_tinker::class_mem<Config::SActionConfig::STestStruct>(L, "test1", &Config::SActionConfig::STestStruct::test1);
			lua_tinker::class_mem<Config::SActionConfig::STestStruct>(L, "test2", &Config::SActionConfig::STestStruct::test2);
			lua_tinker::class_mem<Config::SActionConfig::STestStruct>(L, "test3", &Config::SActionConfig::STestStruct::test3);
			lua_tinker::class_mem<Config::SActionConfig::STestStruct>(L, "test4", &Config::SActionConfig::STestStruct::test4);
			lua_tinker::class_mem<Config::SActionConfig::STestStruct>(L, "test6", &Config::SActionConfig::STestStruct::test6);
		}
		lua_tinker::class_mem<Config::SActionConfig>(L, "TestStruct", &Config::SActionConfig::TestStruct);
		{
			lua_tinker::class_add<Config::SActionConfig::STestStructArray>(L, "Config::SActionConfig::STestStructArray", true);
			lua_tinker::scope_inner(L , "Config::SActionConfig","STestStructArray" , "Config::SActionConfig::STestStructArray");
			lua_tinker::class_mem<Config::SActionConfig::STestStructArray>(L, "test1", &Config::SActionConfig::STestStructArray::test1);
			lua_tinker::class_mem<Config::SActionConfig::STestStructArray>(L, "test2", &Config::SActionConfig::STestStructArray::test2);
			lua_tinker::class_mem<Config::SActionConfig::STestStructArray>(L, "test3", &Config::SActionConfig::STestStructArray::test3);
			lua_tinker::class_mem<Config::SActionConfig::STestStructArray>(L, "test4", &Config::SActionConfig::STestStructArray::test4);
			lua_tinker::class_mem<Config::SActionConfig::STestStructArray>(L, "test5", &Config::SActionConfig::STestStructArray::test5);
		}
		lua_tinker::class_mem<Config::SActionConfig>(L, "vecTestStructArray", &Config::SActionConfig::vecTestStructArray);
		
		lua_tinker::class_add<Config::ActivityConfigBase>(L, "Config::ActivityConfigBase", true);
		lua_tinker::class_add<Config::ActivityConfig>(L, "Config::ActivityConfig", true);
		lua_tinker::scope_inner(L, "Config" , "ActivityConfig" , "Config::ActivityConfig");
		lua_tinker::class_inh<Config::ActivityConfig , Config::ActivityConfigBase>(L);
		lua_tinker::class_def_static<Config::ActivityConfig>(L, "GetInstance", & Config::ActivityConfig::GetInstance);
		lua_tinker::class_def_static<Config::ActivityConfig>(L, "GetInstancePtr", & Config::ActivityConfig::GetInstancePtr);
		lua_tinker::class_def<Config::ActivityConfig>(L, "GetActivityConfig", & Config::ActivityConfig::GetActivityConfig , std::string(""));
		
		lua_tinker::class_add<Config::SActivityConfig>(L, "Config::SActivityConfig", true);
		lua_tinker::scope_inner(L, "Config" , "SActivityConfig" , "Config::SActivityConfig");
		lua_tinker::class_mem<Config::SActivityConfig>(L, "ActivityId", &Config::SActivityConfig::ActivityId);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "ActivityName", &Config::SActivityConfig::ActivityName);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "ActivityLevelLow", &Config::SActivityConfig::ActivityLevelLow);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "ActivityLevelHigh", &Config::SActivityConfig::ActivityLevelHigh);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "IsShowEntrance", &Config::SActivityConfig::IsShowEntrance);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "EntranceIcon", &Config::SActivityConfig::EntranceIcon);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "IsShowInterface", &Config::SActivityConfig::IsShowInterface);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "InterfaceIcon", &Config::SActivityConfig::InterfaceIcon);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "StartTimeWeek", &Config::SActivityConfig::StartTimeWeek);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "EndTimeWeek", &Config::SActivityConfig::EndTimeWeek);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "StartTimeDate", &Config::SActivityConfig::StartTimeDate);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "EndTimeDate", &Config::SActivityConfig::EndTimeDate);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "StartTime", &Config::SActivityConfig::StartTime);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "EndTime", &Config::SActivityConfig::EndTime);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "Description", &Config::SActivityConfig::Description);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "RewardIcon", &Config::SActivityConfig::RewardIcon);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "testdate", &Config::SActivityConfig::testdate);
		{
			lua_tinker::class_add<Config::SActivityConfig::StestDateStruct>(L, "Config::SActivityConfig::StestDateStruct", true);
			lua_tinker::scope_inner(L , "Config::SActivityConfig","StestDateStruct" , "Config::SActivityConfig::StestDateStruct");
			lua_tinker::class_mem<Config::SActivityConfig::StestDateStruct>(L, "begin", &Config::SActivityConfig::StestDateStruct::begin);
			lua_tinker::class_mem<Config::SActivityConfig::StestDateStruct>(L, "end", &Config::SActivityConfig::StestDateStruct::end);
			lua_tinker::class_mem<Config::SActivityConfig::StestDateStruct>(L, "actionConfig", &Config::SActivityConfig::StestDateStruct::actionConfig);
		}
		lua_tinker::class_mem<Config::SActivityConfig>(L, "testDateStruct", &Config::SActivityConfig::testDateStruct);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "dateArray", &Config::SActivityConfig::dateArray);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "testConfig", &Config::SActivityConfig::testConfig);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "testConfig2", &Config::SActivityConfig::testConfig2);
		lua_tinker::class_mem<Config::SActivityConfig>(L, "dateCommon", &Config::SActivityConfig::dateCommon);
		{
			lua_tinker::class_add<Config::SActivityConfig::STestStructArray>(L, "Config::SActivityConfig::STestStructArray", true);
			lua_tinker::scope_inner(L , "Config::SActivityConfig","STestStructArray" , "Config::SActivityConfig::STestStructArray");
			lua_tinker::class_mem<Config::SActivityConfig::STestStructArray>(L, "test1", &Config::SActivityConfig::STestStructArray::test1);
			lua_tinker::class_mem<Config::SActivityConfig::STestStructArray>(L, "test2", &Config::SActivityConfig::STestStructArray::test2);
			lua_tinker::class_mem<Config::SActivityConfig::STestStructArray>(L, "test3", &Config::SActivityConfig::STestStructArray::test3);
			lua_tinker::class_mem<Config::SActivityConfig::STestStructArray>(L, "test4", &Config::SActivityConfig::STestStructArray::test4);
			lua_tinker::class_mem<Config::SActivityConfig::STestStructArray>(L, "test5", &Config::SActivityConfig::STestStructArray::test5);
			lua_tinker::class_mem<Config::SActivityConfig::STestStructArray>(L, "test6", &Config::SActivityConfig::STestStructArray::test6);
		}
		lua_tinker::class_mem<Config::SActivityConfig>(L, "vecTestStructArray", &Config::SActivityConfig::vecTestStructArray);
		{
			lua_tinker::class_add<Config::SActivityConfig::STestStruct>(L, "Config::SActivityConfig::STestStruct", true);
			lua_tinker::scope_inner(L , "Config::SActivityConfig","STestStruct" , "Config::SActivityConfig::STestStruct");
			lua_tinker::class_mem<Config::SActivityConfig::STestStruct>(L, "test1", &Config::SActivityConfig::STestStruct::test1);
			lua_tinker::class_mem<Config::SActivityConfig::STestStruct>(L, "test2", &Config::SActivityConfig::STestStruct::test2);
			lua_tinker::class_mem<Config::SActivityConfig::STestStruct>(L, "test3", &Config::SActivityConfig::STestStruct::test3);
			lua_tinker::class_mem<Config::SActivityConfig::STestStruct>(L, "test4", &Config::SActivityConfig::STestStruct::test4);
			lua_tinker::class_mem<Config::SActivityConfig::STestStruct>(L, "test6", &Config::SActivityConfig::STestStruct::test6);
			lua_tinker::class_mem<Config::SActivityConfig::STestStruct>(L, "test7", &Config::SActivityConfig::STestStruct::test7);
		}
		lua_tinker::class_mem<Config::SActivityConfig>(L, "TestStruct", &Config::SActivityConfig::TestStruct);
		
//		lua_tinker::class_add<Config::_ConditionConfigBase>(L, "Config::_ConditionConfigBase", true);
//		lua_tinker::class_add<Config::_ConditionConfig>(L, "Config::_ConditionConfig", true);
//		lua_tinker::scope_inner(L, "Config" , "_ConditionConfig" , "Config::_ConditionConfig");
//		lua_tinker::class_inh<Config::_ConditionConfig , Config::_ConditionConfigBase>(L);
//		lua_tinker::class_def_static<Config::_ConditionConfig>(L, "GetInstance", & Config::_ConditionConfig::GetInstance);
//		lua_tinker::class_def_static<Config::_ConditionConfig>(L, "GetInstancePtr", & Config::_ConditionConfig::GetInstancePtr);
//		lua_tinker::class_def<Config::S_ConditionConfig>(L, "Get_ConditionConfig", & Config::S_ConditionConfig::Get_ConditionConfig);
//		
//		lua_tinker::class_add<Config::S_ConditionConfig>(L, "Config::S_ConditionConfig", true);
//		lua_tinker::scope_inner(L, "Config" , "S_ConditionConfig" , "Config::S_ConditionConfig");
		
		lua_tinker::class_add<Config::_CommonDataBase>(L, "Config::_CommonDataBase", true);
		lua_tinker::class_add<Config::_CommonData>(L, "Config::_CommonData", true);
		lua_tinker::scope_inner(L, "Config" , "_CommonData" , "Config::_CommonData");
		lua_tinker::class_inh<Config::_CommonData , Config::_CommonDataBase>(L);
		lua_tinker::class_def_static<Config::_CommonData>(L, "GetInstance", & Config::_CommonData::GetInstance);
		lua_tinker::class_def_static<Config::_CommonData>(L, "GetInstancePtr", & Config::_CommonData::GetInstancePtr);
		lua_tinker::class_def<Config::_CommonData>(L, "Get_CommonData", & Config::_CommonData::Get_CommonData , std::string(""));
		
		lua_tinker::class_add<Config::S_CommonData>(L, "Config::S_CommonData", true);
		lua_tinker::scope_inner(L, "Config" , "S_CommonData" , "Config::S_CommonData");
		lua_tinker::class_mem<Config::S_CommonData>(L, "id", &Config::S_CommonData::id);
		lua_tinker::class_mem<Config::S_CommonData>(L, "boolCommon", &Config::S_CommonData::boolCommon);
		lua_tinker::class_mem<Config::S_CommonData>(L, "boolCommon2", &Config::S_CommonData::boolCommon2);
		lua_tinker::class_mem<Config::S_CommonData>(L, "intCommon", &Config::S_CommonData::intCommon);
		lua_tinker::class_mem<Config::S_CommonData>(L, "int64Common", &Config::S_CommonData::int64Common);
		lua_tinker::class_mem<Config::S_CommonData>(L, "doubleCommon", &Config::S_CommonData::doubleCommon);
		lua_tinker::class_mem<Config::S_CommonData>(L, "stringCommon", &Config::S_CommonData::stringCommon);
		lua_tinker::class_mem<Config::S_CommonData>(L, "dateCommon", &Config::S_CommonData::dateCommon);
		return 0;
	}


	INT32 ConfigManager::Cleanup()
	{
		return 0;
	}

}

