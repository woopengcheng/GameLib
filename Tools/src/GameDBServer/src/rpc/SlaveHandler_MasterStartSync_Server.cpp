#include "SlaveHandler.h"

Msg::ObjectMsgCall * Server::SlaveHandler::MasterStartSync_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &filename/* = std::string()*/ , INT32 filesize/* = 0*/ , INT32 sendtype/* = 0*/ , CUtilChunk & value/* = CUtil::Chunk()*/  )
{
	INT32 res = 0;
		
	RecvFile(m_objSlaveInfo.strDir , filename , m_objSlaveInfo.strDBName , filesize , sendtype , value ); 

	std::cout << "MasterStartSync_RpcServer "<< std::endl;
	Return(res);
}

