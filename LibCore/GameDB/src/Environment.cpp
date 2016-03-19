#include "GameDB/inc/Environment.h"
#include "GameDB/inc/Comparator.h"
#include "GameDB/inc/Database.h" 
#include "GameDB/inc/BackupEnvironment.h" 
#include "LogLib/inc/Log.h"
#include "CUtil/inc/CUtil.h"
#include "GameDB/inc/User.h"
#include "GameDB/inc/HashTable.h"

namespace GameDB
{
	 
	Environment::Environment(const std::string strDirectory , Json::Value & objValue)
		: m_strDirectory(strDirectory)
		, m_objDefaultOptions(objValue) 
	{
		m_pComparator = new Comparator;
		m_pBackupEnv = new BackupEnvironment(Env::Default()); 
	
		std::vector<std::string> vecDatabases;
		Env::Default()->CreateDir(m_strDirectory);
		Env::Default()->GetChildren(m_strDirectory , &vecDatabases);

		std::vector<std::string>::iterator iter = vecDatabases.begin();
		for (;iter != vecDatabases.end();++iter)
		{
			if (*iter == "." || *iter == "..")
			{
				continue;
			}

			OpenDatabase(*iter); 
		}
	
		if (!GetDatabase(g_szSystemDatabase))
		{
			Database * pDB = CreateDatabase(g_szSystemDatabase);
			MsgAssert(pDB , "Create database " << g_szSystemDatabase << "failed.");

			std::string strValue;
			GameDB::User objUser;
			objUser.set_name("admin");
			objUser.set_pswd("admin");
			objUser.set_sysuser(true);
			objUser.ToBson(strValue);

			GameDB::Operate oper;
			GameDB::HashTable::HSet(*pDB , oper , GameDB::User::TableName() , "admin" , strValue);
			if (oper.IsSuccess())
			{
				gOtherStream("create user: name:" <<  "admin"  << "success."); 
			}
		}
	}

	Environment::~Environment()
	{
		CollectionDatabasesT::iterator iter = m_mapDatabases.begin();
		for(;iter != m_mapDatabases.end(); ++iter)
		{
			Database* pDB = iter->second;
			pDB->Close();
			delete pDB;
		}
		m_mapDatabases.clear();

		SAFE_DELETE(m_pBackupEnv);
		SAFE_DELETE(m_pComparator); 
	}
	
	Database * Environment::OpenDatabase(const std::string & strName)
	{
		Database * pDatabase = GetDatabase(strName);
		if (pDatabase)
		{
			return pDatabase;
		}

		Options objOptions;
		MakeOptions(objOptions); 
		objOptions.create_if_missing = true;
		objOptions.error_if_exists = true;
		objOptions.comparator = m_pComparator;

		pDatabase = new Database(strName , m_strDirectory , objOptions , m_pBackupEnv);
		pDatabase->Open();
	
		if (pDatabase->GetLevelDB())
		{
			m_mapDatabases.insert(std::make_pair(strName , pDatabase));
		}
		else
		{
			SAFE_DELETE(pDatabase);
			MsgAssert_Re0(pDatabase , "open database error.");
		}

		return pDatabase;
	}

	BOOL Environment::CloseDatabase(const std::string & strName)
	{
		Database * pDatabase = GetDatabase(strName);
		if (pDatabase)
		{
			return FALSE;
		}

		m_mapDatabases.erase(strName);

		pDatabase->Close();
		SAFE_DELETE(pDatabase);

		return TRUE;
	}

	Database*  Environment::CreateDatabase(const std::string & strName)
	{
		Database * pDatabase = GetDatabase(strName);
		if (pDatabase)
		{
			return NULL;
		}

		Options objOptions;
		MakeOptions(objOptions); 
		objOptions.create_if_missing = true;
 		objOptions.error_if_exists = false;
		objOptions.comparator = m_pComparator;

		pDatabase = new Database(strName , m_strDirectory , objOptions , m_pBackupEnv);
		pDatabase->Create();

		if (pDatabase->GetLevelDB())
		{
			m_mapDatabases.insert(std::make_pair(strName , pDatabase));
		}
		else
		{
			SAFE_DELETE(pDatabase);
			MsgAssert_Re0(pDatabase , "open database error.");
		}

		return pDatabase;

	}

	BOOL Environment::RemoveDatabase(const std::string & strName)
	{
		Database * pDatabase = GetDatabase(strName);
		if (!pDatabase)
		{
			return FALSE;
		}

		m_mapDatabases.erase(strName);
		 
		pDatabase->Close();
		pDatabase->RemoveDatabase();
		SAFE_DELETE(pDatabase);

		return TRUE;
	}

	BOOL Environment::RepairDatabase(const std::string & strName)
	{
		std::string strDatabaseDir = m_strDirectory + strName;
		
		Status objStatus = GameDB::RepairDB(strDatabaseDir , Options());
		if (!objStatus.ok())
		{
			gErrorStream("repair db fail." << objStatus.ToString());
		}

		return objStatus.ok();
	}

	Database * Environment::GetDatabase(const std::string & strName)
	{
		CollectionDatabasesT::iterator iter = m_mapDatabases.find(strName);
		if(m_mapDatabases.end() != iter)
		{
			return iter->second;
		}

		return NULL;
	}

	void Environment::GetAllDatabase(std::vector<std::string> & vecDatabases)
	{
		CollectionDatabasesT::iterator iter = m_mapDatabases.begin();
		for(;m_mapDatabases.end() != iter; ++iter)
		{
			vecDatabases.push_back(iter->first);
		}		
	} 

	void Environment::MakeOptions(Options & objOptions)
	{
		std::string  strCompress = "none";
		INT64        llCacheSize = -1;
		INT64        llMaxOpenFiles = -1;
		INT64        llBlockSize = -1;
		INT64        llWriteBufferSize = -1;
		Json::Value  objDefaultIntValue(Json::Int(-1));

		strCompress = m_objDefaultOptions.get("compress" , "none").asString();
		llCacheSize = CUtil::ParseHumanSizeFromJson(m_objDefaultOptions.get("cache_size",objDefaultIntValue));
		llMaxOpenFiles = CUtil::ParseHumanSizeFromJson(m_objDefaultOptions.get("max_open_files",objDefaultIntValue));
		llBlockSize = CUtil::ParseHumanSizeFromJson(m_objDefaultOptions.get("block_size",objDefaultIntValue));
		llWriteBufferSize = CUtil::ParseHumanSizeFromJson(m_objDefaultOptions.get("write_buffer_size",objDefaultIntValue));

		if (llMaxOpenFiles > 0) 
			objOptions.max_open_files = (INT32)llMaxOpenFiles;
		if (llBlockSize > 0)
		{
#ifdef USE_LEVELDB
			objOptions.block_size = llBlockSize;
#endif
		}
		if (llWriteBufferSize > 0) 
			objOptions.write_buffer_size = llWriteBufferSize;
		if (llCacheSize > 0)
		{
#ifdef USE_LEVELDB
			objOptions.block_cache = NewLRUCache(llCacheSize);
#endif
		}
		if (CUtil::stricmp(strCompress.c_str(), "snappy") == 0)
			objOptions.compression = kSnappyCompression;
#ifdef USE_ROCKSDB
		else if (CUtil::stricmp(strCompress.c_str(), "LZ4") == 0)
			objOptions.compression = kLZ4Compression;
#endif
		else
			objOptions.compression = kNoCompression;

		objOptions.env = m_pBackupEnv;
	}

	Env * GetDefaultEnv()
	{ 
		return Env::Default();
	}

}