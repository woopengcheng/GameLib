#pragma once
#include "Lua/ILua.h"

const INT32 cnCommandParamCount = 6;

class CDlgShowRobot;

enum RobotParamType
{
	ROBOT_TYPE_DOUBLE = 0,
	ROBOT_TYPE_STRING = 1

};
struct RobotParams
{
	INT32			type;
	std::string		default;
	std::string		note;
	std::string		value;		//5 这个是在Lua解析之前进行赋值.

	RobotParams()
		: type(ROBOT_TYPE_DOUBLE)
	{

	}
};

typedef std::map<INT32, std::string>				MapRobotTypesT;
typedef std_unordered_map<INT32, RobotParams>		MapRobotParamsT;
struct RobotCommand
{
	INT32			type;
	std::string		command;
	MapRobotParamsT	mapRobotParams;
	RobotCommand()
		: type(0)
	{
		mapRobotParams.reserve(1024);
	}
public:
	MapRobotParamsT		&	GetParams() { return mapRobotParams; }
	void					SetParams(INT32 nIndex, const RobotParams & val) { mapRobotParams.insert(std::make_pair(nIndex, val)); }

};

typedef std::map<INT32, RobotCommand>		MapRobotCommands;
class RobotCommands
{
public:
	static BOOL				Init();
	static void				ExportClassToLua(lua_State* L);

public:
	static MapRobotTypesT	sRobotTypes;
	static MapRobotCommands sRobotCommands;
};  
