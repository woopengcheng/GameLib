#include "GameDB/inc/DBSlaveInterface.h"  
#include "NetLib/inc/NetLib.h"
#include "MsgLib/inc/RpcManager.h"
#include "LogLib/inc/Log.h" 
#include "Timer/inc/TimerHelp.h"
#include "json/json.h"
#include <algorithm>

namespace GameDB
{  
	DBSlaveInterface::DBSlaveInterface()   
	{  		
	}

	DBSlaveInterface::~DBSlaveInterface(void)
	{  
	}

	CErrno DBSlaveInterface::Init(Json::Value & conf)
	{
		if (RpcInterface::Init(conf).IsFailure())
			return CErrno::Failure();

		return  InitDB(conf);
	}
	std::string&   replace_all(std::string&   str, const   std::string&   old_value, const   std::string&   new_value)
	{
		while (true) {
			std::string::size_type   pos(0);
			if ((pos = str.find(old_value)) != std::string::npos)
				str.replace(pos, old_value.length(), new_value);
			else   break;
		}
		return   str;
	}
	std::string&   replace_all_distinct(std::string&   str, const   std::string&   old_value, const   std::string&   new_value)
	{
		for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
			if ((pos = str.find(old_value, pos)) != std::string::npos)
				str.replace(pos, old_value.length(), new_value);
			else   break;
		}
		return   str;
	}

	CErrno DBSlaveInterface::InitDB(const Json::Value & conf)
	{  
		std::string timestamp = Timer::GetDate();
		for(size_t i = 0; i < timestamp.length(); ++i)
		{
			if(timestamp[i] == '-' || timestamp[i] == ' ' || timestamp[i] == ':')
				timestamp[i] = '_';
		}
		 
		// strDirectory = directory + timestamp + "/" ; 
		std::string strDirectory = conf.get("slave_dir" , "./slave_db/").asString();
		if(strDirectory[strDirectory.length() - 1] != '/')
			strDirectory = strDirectory + "/";
	
		std::string cmd = "del   /s/q  ";
		char buf[80];
		getcwd(buf, sizeof(buf));
		cmd += buf;

#ifdef WIN32
		replace_all_distinct(strDirectory, "/", "\\");
#endif
		if (strDirectory[0] == '.')
		{
			std::string tmp(strDirectory.begin() + 1, strDirectory.end());
			cmd += tmp;
		}
		else
		{
			cmd += strDirectory;
		}

		SDBSlaveInfo objInfo; 
		objInfo.strDir = strDirectory;
		objInfo.strUser = conf.get("login_user" , "test").asString();
		objInfo.strPswd = conf.get("login_pswd" , "test").asString();

		std::string strType = conf.get("type" , "tcp").asCString();
		std::string strAddress = conf.get("master_addr" , "127.0.0.1").asCString();
		INT32 nPort = conf.get("master_port" , 8001).asInt(); 

		Json::Value databases = conf.get("databases","[]");
		if(!databases.isArray())
		{
			std::cerr << "invalid conf: databases not array" << std::endl;
			return CErrno::Failure();
		}

		if(databases.size() == 0)
		{
			std::cerr << "invalid conf: databases not contain name" << std::endl;
			return CErrno::Failure();
		}

		std::string tmpCmd = cmd;
		for(size_t i = 0; i < databases.size(); ++i)
		{     
			tmpCmd = cmd;
			std::string strDBName = databases[(INT32)i].asString(); 
			objInfo.strDBName = strDBName;  
			OnCreateDatabase(objInfo);

			if (m_pNetThread)
			{
				m_pNetThread->InsertClientsQueue(strDBName, strAddress , nPort);
			}

			tmpCmd += strDBName;
			::system(tmpCmd.c_str());
		} 
		return CErrno::Success();
	} 

	CErrno DBSlaveInterface::Cleanup(void)
	{ 
 
		return RpcInterface::Cleanup();
	}

	CErrno DBSlaveInterface::Update(void)
	{ 
		return RpcInterface::Update();
	}  

}