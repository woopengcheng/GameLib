#ifndef __gamedb_backup_environment_h__ 
#define __gamedb_backup_environment_h__
#include "GameDB/inc/DBCommon.h"

namespace GameDB
{
	struct SBackupDB
	{
		std::string strDBName;
		std::string strSrcDir;
		std::string strDstFileName;
		std::string strDstDir;
	};

	class BackupEnvironment : public EnvWrapper
	{
	public:
		typedef std::vector<std::string> CollectionWillDeleteFilesT;

	public:
		BackupEnvironment()
			: EnvWrapper(Env::Default())
			, m_bBackuping(FALSE)
		{

		}
		explicit BackupEnvironment(Env * pEnv)
			: EnvWrapper(pEnv)
			, m_bBackuping(FALSE)
		{

		}
		virtual ~BackupEnvironment(){}

	public:
		virtual Status				Backup(const std::string & strDir , void * arg);
		virtual Status				DeleteFile(const std::string & strFile){ return RemoveFile(strFile); }
		virtual bool				CheckCopyFile(const std::string& strName);
		virtual bool				CanCopyFile(const std::string& strName);
		virtual Status				CloneFile(void * arg , const std::string & strFile , int64_t llFileLength);
		virtual Status				CloneFile(const std::string& strSrc,const std::string& strDst,int64_t llFileLength );
		virtual Status				LinkFile(const std::string & strSrc , const std::string & strDst);
		virtual Status				TouchFile(const std::string & strDir );
 
	public: 
		Status						RemoveFile(const std::string & strFile);  //5 解决windows的DeleteFile检测错误问题

	protected:
		BOOL						m_bBackuping;		//5 是否正在备份,用于删除文件之用
		DBMutex						m_objMutex;
		CollectionWillDeleteFilesT	m_vecWillDeleteFiles;
	}; 
}

#endif