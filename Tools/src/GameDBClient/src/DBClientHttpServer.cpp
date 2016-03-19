#include "ThreadPool/inc/ThreadPoolInterface.h" 
#include "NetLib/inc/NetReactorSelect.h."
#include "NetLib/inc/HttpProtocol.h"
#include "DBClientHttpServer.h"
#include "ClientCommands.h"

namespace Client
{
	std::string g_strHtmlResult = "result.";

	CErrno DBClientHttpServer::Init(Json::Value & conf)
	{
		Json::Value http = conf.get("http" , Json::Value()); 
		std::string strType = http.get("type" , "tcp").asCString();
		std::string strAddress = http.get("address" , "127.0.0.1").asCString();
		INT32 nPort = http.get("port" , 8080).asInt();

		return Init(strAddress.c_str() , nPort);
	}

	CErrno DBClientHttpServer::Init(const char * pAddress , INT32 nPort , BOOL bResueAddr /*= TRUE */, INT32 nListenerCount /*= DEFAULT_LISTENER_COUNT*/)
	{
		CErrno err = Net::HttpServer::Init(pAddress , nPort , bResueAddr , nListenerCount);
		if (err.IsSuccess())
		{
			ThreadPool::ThreadPoolInterface::GetInstance().CreateThread(2 , 1);  
			ThreadPool::ThreadPoolInterface::GetInstance().AddTask(this);  
		}

		return err;
	}

	CErrno DBClientHttpServer::Cleanup()
	{

		ThreadPool::ThreadPoolInterface::GetInstance().Cleanup();
		return Net::HttpServer::Cleanup();
	} 

	CErrno DBClientHttpServer::Update(void)
	{ 
		m_pNetReactor->Update();
		return ThreadPool::ThreadSustainTask::Update();
	} 

	DBClientHttpServer::DBClientHttpServer(Net::INetReactor * pNetReactor /*= NULL*/)
		: HttpServer(pNetReactor)
		, ThreadPool::ThreadSustainTask(2 , "DBClientHttpServer" )
	{  
		if (!m_pNetReactor)
		{
			m_pNetReactor = new Net::NetReactorSelect; 

			if(CErrno::Success() != m_pNetReactor->Init())
			{
				SAFE_DELETE(m_pNetReactor);
			}
		}
	}

	CErrno DBClientHttpServer::HttpHandler(Net::HttpSession * pSession , Net::HttpProtocol& request,Net::HttpProtocol& response)
	{
		std::string strURL = request.GetInputUrl();
		
		if (strURL == "index")
		{
			std::string resultContent = "resultresultresultresultresultresultresultresultresultresult";

			std::string result = GetHtmlResult(resultContent);
			response.Reserve((INT32)(result.length()) + 512);
			response.WriteResponseHeader(200,"OK");
			response.WriteHeader(Net::HttpConsts::HEADER_CONNECTION,"keep-alive");
			response.WriteHeader(Net::HttpConsts::HEADER_CONTENT_TYPE,"text/html;charset=utf-8");
			response.WriteHeader(Net::HttpConsts::HEADER_CONTENT_LENGTH, (INT32)(result.length()));
			response.WriteContent(result.c_str(), (INT32)(result.length()));

			return CErrno::Success();
		}
		else if (strURL == "submit")
		{
			std::vector<std::string> vecParams;
			std::map<std::string, std::string> mapContents;
			request.GetContent(mapContents);
			std::string strKey = mapContents["commandName"];
			if (strKey != "None")
			{
				vecParams.push_back(strKey);
				std::string strVal = mapContents["command"];
				if (strVal != "")
				{
					vecParams.push_back(strVal);
				}
				g_pClientCommands->Execute(&Client::DBClient::GetInstance(), vecParams);
			}
			
			std::string result = GetHtmlResult(g_strHtmlResult);

			response.Reserve((INT32)(result.length()) + 512);
			response.WriteResponseHeader(200, "OK");
			response.WriteHeader(Net::HttpConsts::HEADER_CONNECTION, "keep-alive");
			response.WriteHeader(Net::HttpConsts::HEADER_CONTENT_TYPE, "text/html;charset=utf-8");
			response.WriteHeader(Net::HttpConsts::HEADER_CONTENT_LENGTH, (INT32)(result.length()));
			response.WriteContent(result.c_str(), (INT32)(result.length()));

			return CErrno::Success();
		}
		else if(strURL == "pwd")
		{
			std::string strLic = "123";
			std::string strAuth;
			if (request.m_phAuthorization)
			{
				strAuth = request.m_phAuthorization;
			}
			if (!m_objAuths.HasRight(strAuth, strLic))
			{
				response.WriteResponseHeader(401, "Authorization failed");
				response.WriteHeader(Net::HttpConsts::HEADER_CONNECTION, "keep-alive");
				response.WriteHeader(Net::HttpConsts::HEADER_WWW_AUTHORIZATION, "Basic realm=\"woopengcheng.com.cn\"");
				response.WriteContent("Unauthorized.", sizeof("Unauthorized."));
			}
			else
			{
				std::string result = "<HTML>\n				<HEAD>\n				<TITLE>GameDBClient password test</TITLE>\n				</HEAD>\n				</HTML>";

				response.Reserve((INT32)(result.length()) + 512);
				response.WriteResponseHeader(200, "OK");
				response.WriteHeader(Net::HttpConsts::HEADER_CONNECTION, "close");
				response.WriteHeader(Net::HttpConsts::HEADER_CONTENT_TYPE, "text/html;charset=utf-8");
				response.WriteHeader(Net::HttpConsts::HEADER_CONTENT_LENGTH, (INT32)(result.length()));
				response.WriteContent(result.c_str(), (INT32)(result.length()));
			}
			return CErrno::Success();
		}
		else
		{
			return CErrno(CErrno::ERR_INVALID_DATA);
		}

		return CErrno::Failure();
	}

	std::string DBClientHttpServer::GetHtmlResult(std::string strContent)
	{
		static std::string resultHead = "<html lang=\"en\">\
				<head>\
				<meta charset = \"gbk-2312\">\
				<title>GameDBClient</title>\
				</head>\
				<body>\
				<table border = \"1\" width = \"700\">\
				<tr >\
				<td width = \"100\">result</td>\
				<td width = \"600\">";

		static std::string resultTail = "< / td>\
				</tr>\
				<tr>\
				<br>\
				</tr>\
				</table>\
				<br><form action=\"submit\" method=\"post\">\
				<select name = \"commandName\" width = \"300\">";

		std::string result = resultHead + strContent + resultTail;
		std::string strCommands = "<option value=\"None\">please select commands:</option>";
		if (g_pClientCommands)
		{
			Client::ClientCommands::CollectionClientCommandsT & mapCommands = g_pClientCommands->GetClientCommand();
			Client::ClientCommands::CollectionClientCommandsT::iterator iter = mapCommands.begin();
			for (; iter != mapCommands.end(); ++iter)
			{
				strCommands += "<option value=\"";
				strCommands += iter->first;
				strCommands += "\">";
				strCommands += iter->first;
				strCommands += "</option>\n";
			}
		}

		std::string strResult = "<input type=\"text\" name=\"command\" width=\"300\">\
				<input type = \"submit\" value = \"ok\" width = \"100\">\
				</form>\
				</body>\
				</html>";

		result += strCommands + strResult;

		return result;
	}

}