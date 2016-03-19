#include "GameDB/inc/DBClientInterface.h"  
#include "MsgLib/inc/RpcManager.h"
#include "LogLib/inc/Log.h"
#include "json/json.h"
#include "NetLib/inc/NetLib.h"

namespace GameDB
{ 

	DBClientInterface::DBClientInterface()  
	{  
	}

	DBClientInterface::~DBClientInterface(void)
	{  
	}

	CErrno DBClientInterface::Init(Json::Value & conf)
	{
		return RpcInterface::Init(conf);
	}

	CErrno DBClientInterface::Cleanup(void)
	{ 
		return RpcInterface::Cleanup();
	}

	CErrno DBClientInterface::Update(void)
	{ 
		return RpcInterface::Update();
	} 
	
}