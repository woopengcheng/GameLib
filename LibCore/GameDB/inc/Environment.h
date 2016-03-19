#ifndef __gamedb_environment_h__ 
#define __gamedb_environment_h__
#include "GameDB/inc/DBCommon.h"
#include "json/value.h"

namespace GameDB
{
	class Database;
	class Comparator;
	class BackupEnvironment;

// 	class DLL_EXPORT GameDBEnv : public Env
// 	{
// 
// 	};

	extern DLL_EXPORT Env * GetDefaultEnv();

	class DLL_EXPORT Environment 
	{
	public:
		typedef std_unordered_map<std::string , Database *> CollectionDatabasesT;

	public:
		Environment(const std::string strDirectory , Json::Value & objValue);
		~Environment();
		
	public:
		Database * OpenDatabase(const std::string & strName);
		BOOL       CloseDatabase(const std::string & strName);
		Database * CreateDatabase(const std::string & strName);
		BOOL       RemoveDatabase(const std::string & strName);
		BOOL       RepairDatabase(const std::string & strName);
		Database * GetDatabase(const std::string & strName);
		void       GetAllDatabase(std::vector<std::string> & vecDatabases); 

	public:
		BackupEnvironment * GetBackupEnv() const { return m_pBackupEnv; } 

	protected:
		void       MakeOptions(Options & objOptions);

	protected:
		std::string		  	  m_strDirectory; 
		BackupEnvironment	* m_pBackupEnv;
		Comparator          * m_pComparator;		//5 数据库排序使用的比较函数. 
		Json::Value           m_objDefaultOptions;
		CollectionDatabasesT  m_mapDatabases;
	};
}



#endif
