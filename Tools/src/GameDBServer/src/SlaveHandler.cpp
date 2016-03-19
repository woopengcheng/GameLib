#include "GameDB/inc/RemoteNodeDefine.h"
#include "GameDB/inc/Comparator.h"
#include "GameDB/inc/DBCommon.h"
#include "Timer/inc/TimerHelp.h"
#include "SlaveHandler.h"
#include "RPCCallFuncs.h"
#include <fstream> 

namespace Server
{ 
	bool SlaveHandler::RecvFile(std::string strDir , std::string strFileName , std::string strDBName , INT32 nFileSize , INT32 nSendType , const CUtil::Chunk & objChunk)
	{ 
		std::string localfilename = strDir + strDBName + "/" + strFileName;
		GameDB::GetDefaultEnv()->CreateDir(strDir);
		GameDB::GetDefaultEnv()->CreateDir(strDir + strDBName);

		if (nSendType == 0)
		{
			std::fstream f;
			f.open(localfilename.c_str(),std::ios::out | std::ios::binary | std::ios::app); 
			MsgAssert_Re0(f.good() , localfilename << "wrong open");

			gDebugStream("recv file:" << strFileName << "recv size: " << objChunk.GetDataLen() );
			f.write((const char *)objChunk.Begin(),objChunk.GetDataLen());  

			f.close(); 
		}
		else if (nSendType == 1)
		{
			std::vector<std::string> files;
			GameDB::GetDefaultEnv()->GetChildren(strDir + strDBName,&files);
			   
			for(size_t i = 0; i < files.size(); ++i)
			{
				const std::string& filename = files[i];
				if(filename == "." || filename == "..")
					continue;

#ifdef DeleteFile
#undef DeleteFile
				GameDB::GetDefaultEnv()->DeleteFile(strDir + strDBName + "/" + filename);
#endif // DeleteFile

#ifdef UNICODE
#define DeleteFile  DeleteFileW
#else
#define DeleteFile  DeleteFileA
#endif // !UNICODE
			} 
		}
		else if (nSendType == 2)  //5 代表发送完成.
		{
			return RecvFileEnd(strDir , strDBName);
		} 

		return true;
	}

	bool SlaveHandler::RecvFileEnd(std::string strDir , std::string strDBName )
	{
		GameDB::Options objOp;
		objOp.error_if_exists = true;
		objOp.create_if_missing = false;
		objOp.comparator = new GameDB::Comparator();
		objOp.compression = kSnappyCompression;

		m_pDatabase = new GameDB::Database(strDBName,strDir,objOp);
		if(!m_pDatabase->Open() || !m_pDatabase->GetLevelDB())
		{
			delete m_pDatabase;
			m_pDatabase = 0;
			gErrorStream( "slave open database failed: " << strDBName );
			return false;
		}

		gDebugStream(Timer::GetDate("%Y-%m-%d-%H-%M-%S") << " " << "slave sync successful:" << strDBName );

		return true;
	}

	void SlaveHandler::SetSlaveInfo(const GameDB::SDBSlaveInfo & info)
	{
		m_objSlaveInfo = info;
	}

	void SlaveHandler::StartAuth()
	{
		if (m_pDBSlave)
		{
			Server::rpc_SlaveStartAuth(m_nMasterSessionID, m_pDBSlave->GetMasterID() , m_objID  , m_objSlaveInfo.strUser , m_objSlaveInfo.strPswd);
		}
	}

	void SlaveHandler::RequestSyncData()
	{ 
		if (m_pDBSlave)
		{
			Server::rpc_SlaveRequestSync(m_nMasterSessionID, m_pDBSlave->GetMasterID() , m_objID , m_objSlaveInfo.strDBName);
		}
	}

	void SlaveHandler::SelectDB()
	{ 
		if (m_pDBSlave)
		{
			Server::rpc_SlaveSelectDB(m_nMasterSessionID, m_pDBSlave->GetMasterID() , m_objID , m_objSlaveInfo.strDBName , NULL , 1);
		}

	}
}