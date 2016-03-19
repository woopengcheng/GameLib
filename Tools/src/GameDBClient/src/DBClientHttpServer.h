#ifndef __gamedb_db_client_http_server_h__
#define __gamedb_db_client_http_server_h__  
#include "GameDB/inc/DBClientInterface.h"
#include "NetLib/inc/NetHandlerHttpServer.h"
#include "ThreadPool/inc/ThreadTask.h"

namespace Client
{  
	extern std::string g_strHtmlResult;

	class  DBClientHttpServer : public Net::HttpServer , ThreadPool::ThreadSustainTask
	{ 
	public:
		explicit DBClientHttpServer(Net::INetReactor * pNetReactor = NULL);
		virtual ~DBClientHttpServer(void){} 

	public:
		static DBClientHttpServer & GetInstance()
		{
			static DBClientHttpServer m_sInterface;
			return m_sInterface;
		} 
	public:    
		CErrno				Init(Json::Value & conf);	
		virtual CErrno		Init(const char * pAddress , INT32 nPort , BOOL bResueAddr = TRUE , INT32  nListenerCount = DEFAULT_LISTENER_COUNT) override;
		virtual CErrno		Cleanup() override;
		virtual CErrno		Update(void) override;
		virtual	CErrno		HttpHandler(Net::HttpSession * pSession , Net::HttpProtocol& request,Net::HttpProtocol& response) override;

	protected:
		std::string			GetHtmlResult(std::string strContent);
	};  

} 

#endif