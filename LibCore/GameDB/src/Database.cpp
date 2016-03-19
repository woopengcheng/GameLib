#include "GameDB/inc/Database.h"
#include "GameDB/inc/CustomDefinedCacheHandler.h"
#include "GameDB/inc/BackupEnvironment.h"

namespace GameDB
{  
	 
	Database::Database(const std::string& strName,const std::string& strDirectory,const Options& objOptions , BackupEnvironment * pBackupEnv)
		: m_strName(strName)
		, m_strDirectory(strDirectory)
		, m_objOptions(objOptions)
		, m_pLevelDB(NULL) 
		, m_pCustomCache(NULL)
		, m_pBackupEnv(pBackupEnv)
	{ 
		m_strDirectory = strDirectory + m_strName;
//		m_pCustomCache = leveldb::NewLRUCache(500*1024*1024);
	}

	Database::~Database()
	{ 
		Close();
//		SAFE_DELETE(m_pCustomCache);
	}

	void Database::Close()
	{
#ifdef USE_LEVELDB
		SAFE_DELETE(m_objOptions.block_cache);
#endif

		SAFE_DELETE(m_pLevelDB);
	}

	Status Database::QuickGet(const Slice & objKey , std::string & strVal)
	{
		if (m_pCustomCache)
		{
			Cache::Handle * pHandle = m_pCustomCache->Lookup(objKey);
			if (pHandle)
			{
				std::string * pOldValue = (std::string *)m_pCustomCache->Value(pHandle);
				strVal.assign(*pOldValue);
				m_pCustomCache->Release(pHandle);

				return Status::OK();
			}
		}

		//5 如果用户自定义缓存没有或者没有自定义缓存.就去数据库中查找.
		return	m_pLevelDB->Get(ReadOptions() , objKey , &strVal); 
	}

	Status Database::QuickWrite(const Slice & objKey , const Slice & objVal)
	{
		Status objStatus = m_pLevelDB->Put(WriteOptions() , objKey , objVal);

		if (objStatus.ok() && m_pCustomCache)
		{ 
			UpdateCache(objKey , objVal);
		}

		return objStatus;

	}

	Status Database::QuickWrite(WriteBatch * pBatch)
	{ 
		Status objStatus = m_pLevelDB->Write(WriteOptions() , pBatch);

		if (objStatus.ok() && m_pCustomCache)
		{ 
			CustomDefinedCacheHandler objHandler(this);
			pBatch->Iterate(&objHandler); 
		}

		return objStatus;
	}

	Status Database::QuickDel(const Slice & objKey)
	{
		Status objStatus = m_pLevelDB->Delete(WriteOptions() , objKey);
		if (objStatus.ok() && m_pCustomCache)
		{
			m_pCustomCache->Erase(objKey);
		}

		return objStatus;
	}

	void Database::UpdateCache(const Slice & objKey , const Slice & objVal)
	{
		Cache::Handle * pHandle = m_pCustomCache->Lookup(objKey);
		if (pHandle)
		{
			std::string * pOldValue = (std::string *)m_pCustomCache->Value(pHandle);
			pOldValue->clear();
			pOldValue->append(objVal.data() , objVal.size());

			m_pCustomCache->Release(pHandle);
		}
		else
		{
			std::string * pNewValue = new std::string(objVal.data(),objVal.size());
			m_pCustomCache->Insert(objKey , pNewValue , objVal.size() , fnCustomCacheDeleter);
		} 
	}

	BOOL Database::Open()
	{
		Assert_Re0(0 == m_pLevelDB);

		Options objOptions(m_objOptions);
		objOptions.create_if_missing = false;
		objOptions.error_if_exists = false;

		Status objStatus = DB::Open(objOptions , m_strDirectory , &m_pLevelDB);
		if (!objStatus.ok())
		{
			MsgAssert_Re0(objStatus.ok() , objStatus.ToString())
		}

		return m_pLevelDB != 0;
	}

	BOOL Database::Create()
	{
		Assert_Re0(0 == m_pLevelDB);

		Options objOptions(m_objOptions);
		objOptions.create_if_missing = true;
		objOptions.error_if_exists = true;

		Status objStatus = DB::Open(objOptions , m_strDirectory , &m_pLevelDB);
		if (!objStatus.ok())
		{
			MsgAssert_Re0(objStatus.ok() , objStatus.ToString())
		}

		return objStatus.ok();
	}

	BOOL Database::RemoveDatabase()
	{
		Assert_Re0(m_pLevelDB);

		Status objStatus = GameDB::DestroyDB(m_strDirectory,Options());
		if (!objStatus.ok())
		{
			MsgAssert_Re0(objStatus.ok() , objStatus.ToString())
		}

		return objStatus.ok();
	}

	void Database::RemoveDatas()
	{
		this->Close();
		this->RemoveDatabase();
		this->Create(); 
	}

	BOOL Database::Backup(const std::string & strBackupName , std::string & strDir)
	{
		SBackupDB objContext;
		objContext.strSrcDir = m_strDirectory;
		objContext.strDstFileName = strBackupName;
		objContext.strDstDir = strDir;
		objContext.strDBName = m_strName;

		Env::Default()->CreateDir(objContext.strDstDir);
		Env::Default()->CreateDir(objContext.strDstDir + strBackupName);
		Env::Default()->CreateDir(objContext.strDstDir + strBackupName + "/" + m_strName);
		if (m_pBackupEnv)
		{
			m_pBackupEnv->Backup(m_strDirectory , &objContext);
			m_pBackupEnv->TouchFile(strDir + strBackupName + "/" + m_strName + "/LOCK");
		}
		strDir = strDir + strBackupName.c_str();
  		return TRUE;
	}

}