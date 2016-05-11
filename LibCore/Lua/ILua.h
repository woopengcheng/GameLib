#ifndef __Lua_ilua_h__
#define __Lua_ilua_h__
#include "CUtil/inc/Common.h"
#include "Lua/lua_tinker.h"

class ILua
{
public:
	ILua(lua_State* l = NULL)
		: L(l)
	{
	}

	virtual ~ILua()
	{
		if (L)
		{
			lua_close(L);
		}
	}

public:
	virtual CErrno Init()
	{
		if (!L)
		{
			lua_State* L = luaL_newstate();
			luaL_openlibs(L);
			lua_tinker::init(L);
		}
	}
	virtual CErrno Cleanup()
	{

	}

private:
	lua_State* L;
};



#endif