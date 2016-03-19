#include "GameDB/inc/DBMasterInterface.h"  
#include "NetLib/inc/NetLib.h"
#include "MsgLib/inc/RpcManager.h"
#include "LogLib/inc/Log.h" 

namespace GameDB
{  
	DBMasterInterface::DBMasterInterface()   
	{  		
	}

	DBMasterInterface::~DBMasterInterface(void)
	{  
	}

	CErrno DBMasterInterface::Init(Json::Value & conf)
	{
		return RpcInterface::Init(conf);
	}
	 
	CErrno DBMasterInterface::Cleanup(void)
	{  
		return RpcInterface::Cleanup();
	}

	CErrno DBMasterInterface::Update(void)
	{ 
		return RpcInterface::Update();
	}  

}