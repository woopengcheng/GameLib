// TestLibCore.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "CUtil/inc/CUtil.h"
#include "CUtil/inc/DeelxRegexp.h"
#include "CSVConfigs/ConfigManager.h"
#include "NetLib/inc/NetHelper.h"
#include "CSVConfigs/ActionConfig.h"
#include "CSVConfigs/ActivityConfig.h"
#include "Test/LuaTest/test.h"

//Config::SActivityConfig * test_lua_member_property(lua_State * L)
std::string test_lua_member_property(lua_State * L)
{
// 	std::string luabuf =
// 		R"(function test_lua_namespace1()
// 					return 11 == NS_TEST.test_function_in_namespace(11);
// 				end
// 			)";
// 	lua_tinker::dostring(L, luabuf.c_str());
// 	lua_tinker::call<bool>(L, "test_lua_namespace1");
// 	return "";
// 
	std::string luabuf =
		R"(function test_lua_member_property()
					local pActivityConfig = Config.ActivityConfig.GetInstancePtr()
					--return pActivityConfig.GetActivityConfig("1").vecTestStructArray[0].test6.begin()->second->TestStruct.test6
					id = "1"
					return pActivityConfig:GetActivityConfig("1").vecTestStructArray[1].test5
					--return "asdf"
				end	
			)";
	lua_tinker::dostring(L, luabuf.c_str());
	//return  lua_tinker::call<Config::SActivityConfig *>(L, "test_lua_member_property");
	return  lua_tinker::call<std::string>(L, "test_lua_member_property");
};
int _tmain(int argc, _TCHAR* argv[])
{   
	CUtil::Init();

	Config::ConfigManager::GetInstance().Init("./csv_config");
	std::cout << "port used=" << Net::NetHelper::IsSocketPortUsed(8002) << std::endl;;

	gDebugStream(Config::ActionConfig::GetInstance().GetActionConfig(1)->TestStruct.test1);
	gDebugStream(Config::ActionConfig::GetInstance().GetActionConfig(1)->TestStruct.test3.begin()->second->vecTestStructArray[0].test6.begin()->second->TestStruct.test6);
	gDebugStream(Config::ActivityConfig::GetInstance().GetActivityConfig("1")->testdate.GetDateStr());
	gDebugStream(Config::ActivityConfig::GetInstance().GetActivityConfig("1")->vecTestStructArray[0].test6.begin()->second->TestStruct.test6);

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_tinker::init(L);

	test_lua_main(L);
	Config::ConfigManager::GetInstance().ExportClassToLua(L);

// 	Config::SActivityConfig * result = test_lua_member_property(L);
	std::string result = test_lua_member_property(L);
	if (lua_gettop(L) != 0)
	{
		printf("unit test stack leak: error\n");
	}
	gDebugStream("result=" << result);
	while (true)
	{

		Config::ConfigManager::GetInstance().Cleanup();

	}
	lua_close(L);
	CUtil::Cleanup();
	return 0;
}

