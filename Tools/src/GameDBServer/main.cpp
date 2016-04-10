// GenRpcs.cpp : Defines the entry point for the console application.
//
// #ifdef _DEBUG
// #include "vld.h"
// #endif
#include "stdafx.h"
#include "stdlib.h"
#include <fstream>
#include <iostream>

#include "Timer/inc/TimerHelp.h"
#include "CUtil/inc/CUtil.h" 
#include "json/json.h"
#include "ServerHandler.h"  
#include "DBServer.h"
#include "DBMaster.h"
#include "DBSlave.h"
#include "MasterHandler.h"
#include "MsgLib/inc/RpcManager.h"
#include "SlaveHandler.h" 
#include "ThreadPool/inc/ThreadPoolInterface.h"
#include "GameDB/inc/RemoteNodeDefine.h"
#include "MsgLib/inc/MsgHelper.h"

int _tmain(int argc, _TCHAR* argv[])
{  
	CUtil::Init("DBServer"); 

	std::string defaultConf = "./gdbServer.conf";
	if(argc > 1)
		defaultConf = argv[1];
	  
	Json::Value root;
	Json::JsonParase(defaultConf.c_str() , root); 
	
	std::string strRunMode = root.get("mode" , "master").asString(); 

	std::string strName , strPwd; 
	gDebugStream("run mode in " << strRunMode);
	if (strRunMode.compare("master") == 0)
	{
		Json::Value objMaster = root.get("master", Json::Value());
		Server::DBMaster::GetInstance().Init(objMaster);

		Json::Value objDBServer = root.get("server", Json::Value());
		Server::DBServer::GetInstance().Init(objDBServer);

//		static Server::MasterHandler  ObjMasterHandler(10000,&Server::DBMaster::GetInstance()); 
	}
	else if (strRunMode.compare("slave") == 0)
	{
		Json::Value objSlave = root.get("slave" , Json::Value());
		Server::DBSlave::GetInstance().Init(objSlave);
	}
	else
	{
		gErrorStream("wrong mode in " << defaultConf);
		return 0;
	}

	//5 等待slave连接成功,并且请求数据
	if (strRunMode.compare("slave") == 0 )
	{
		std::vector<std::string> vecDBName;
		Json::Value databases = root.get("slave", Json::Value()).get("databases", Json::Value());
		for (size_t i = 0; i < databases.size(); ++i)
		{
			std::string strDBName = databases[(INT32)i].asString();
			vecDBName.push_back(strDBName);
		}
		std::vector<std::string>::iterator iter = vecDBName.begin() , end = vecDBName.end();
		gDebugStream("waiting slave connect master.");
		while(1)
		{
			if (Server::DBSlave::GetInstance().GetMasterID() > 0)
			{
				bool bConnected = true;
				for (iter = vecDBName.begin(); iter != end;++iter)
				{
					std::string strDBName = Msg::MsgHelper::GeneratePeerInfoKey(*iter, "dbmaster");
					if (!Server::DBSlave::GetInstance().GetRpcManager()->IsConnected(strDBName))
					{
						bConnected = false;
					}
				}
				if (bConnected)
				{
					Server::DBSlave::GetInstance().StartAuth();
					gDebugStream("slave connect success.");
					break;
				}
			}
			Server::DBSlave::GetInstance().Update();
		}
	}
	else
	{
		gDebugStream("waiting master connect server.");
		while (1)
		{
			if (Server::DBMaster::GetInstance().GetRpcManager()->IsConnected(g_strGameDBNodes[NETNODE_DBMASTER_TO_DBSERVER]) && 
				Server::DBMaster::GetInstance().GetServerID() > 0)
			{
				gDebugStream("master connect server success.");
				break;
			}
			Server::DBServer::GetInstance().Update();
		}
	}

	while (1)
	{
		if (strRunMode.compare("slave") != 0)
		{
			Server::DBServer::GetInstance().Update();
		}
		else
		{
			Server::DBSlave::GetInstance().Update();
		}

		if(remove("./quit") == 0)
		{
			break;
		}
		Timer::sleep(1);
	}


	//5 等待slave连接成功,并且请求数据
	if (strRunMode.compare("slave") == 0 )
	{
		Server::DBSlave::GetInstance().Cleanup();  
	}
	else
	{
		Server::DBServer::GetInstance().Cleanup();  
		Server::DBMaster::GetInstance().Cleanup();  
	}

	CUtil::Cleanup();
	 
	return 0;
}

