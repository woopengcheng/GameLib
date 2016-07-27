#include "RobotCommand.h"

  
void RobotCommands::ExportClassToLua(lua_State* L)
{
	lua_tinker::class_add<RobotParams>(L, "RobotParams", true);
	lua_tinker::class_con<RobotParams>(L, lua_tinker::constructor<RobotParams>::invoke);
	lua_tinker::class_mem<RobotParams>(L, "type", &RobotParams::type);
	lua_tinker::class_mem<RobotParams>(L, "default", &RobotParams::default);
	lua_tinker::class_mem<RobotParams>(L, "note", &RobotParams::note);
	lua_tinker::class_mem<RobotParams>(L, "value", &RobotParams::value);

	lua_tinker::class_add<RobotCommand>(L, "RobotCommand", true);
	lua_tinker::class_con<RobotCommand>(L, lua_tinker::constructor<RobotCommand>::invoke);
	lua_tinker::class_mem<RobotCommand>(L, "command", &RobotCommand::command);
	lua_tinker::class_mem<RobotCommand>(L, "params", &RobotCommand::mapRobotParams);
	lua_tinker::class_def<RobotCommand>(L, "SetParams", &RobotCommand::SetParams);
}


 MapRobotCommands RobotCommands::sRobotCommands;
 MapRobotTypesT RobotCommands::sRobotTypes;
 BOOL RobotCommands::Init()
{
	static lua_State * L = NULL;
	if (!L)
	{
		L = luaL_newstate();
		luaL_openlibs(L);
		lua_tinker::init(L);
		ExportClassToLua(L);

		lua_tinker::dofile(L, "lua/RobotCommand.lua");
		sRobotTypes = lua_tinker::get<MapRobotTypesT>(L, "RobotTypes");
		sRobotCommands = lua_tinker::get<MapRobotCommands>(L, "RobotCommands");

		lua_close(L);
	}

	return TRUE;
}
