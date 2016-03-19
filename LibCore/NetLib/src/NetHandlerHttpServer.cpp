#include "NetLib/inc/NetHandlerHttpServer.h"
#include "NetLib/inc/INetReactor.h"
#include "NetLib/inc/NetHelper.h"
#include "NetLib/inc/ServerSession.h"
#include "NetLib/inc/HttpProtocol.h"
#include "CUtil/inc/CUtil.h"
#include "Timer/inc/TimerHelp.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>

namespace Net
{

	void HttpServerListener::OnAccept(NetSocket socket , sockaddr_in * addr)
	{
		if (m_pHttpServer)
		{
			m_pHttpServer->OnAccept(socket , addr);
		}
	}

	HttpServer::HttpServer(INetReactor * pNetReactor)
		: m_pListener(NULL)
		, m_pNetReactor(pNetReactor)
	{ 
	}

	HttpServer::~HttpServer(void)
	{

	}

	CErrno HttpServer::Init(const char * pAddress , INT32 nPort , BOOL bResueAddr /*= TRUE */, INT32 nListenerCount /*= DEFAULT_LISTENER_COUNT*/)
	{
		CErrno err;
		if (m_pListener == NULL)
		{
			m_pListener = HttpServerListenerPtr(new HttpServerListener(this , m_pNetReactor , new ServerSession(pAddress , nPort , "")));
			err = m_pListener->Init(pAddress , nPort , bResueAddr , nListenerCount);

			if (err.IsSuccess())
			{
				m_pListener->GetSession()->SetClosed(FALSE);
				m_pNetReactor->AddNetHandler(m_pListener , NET_FUNC_ACCEPT_DEFAULT);
			}
		}
		return err;
	}

	CErrno HttpServer::Cleanup()
	{
		return CErrno::Success();
	}

	void HttpServer::OnAccept(NetSocket socket , sockaddr_in * addr)
	{
		if (m_pNetReactor)
		{
			char szAddress[MAX_NAME_LENGTH];
			UINT16 usPort = 0;
			Net::NetHelper::GetAddressAndPortByAddrIn(addr , szAddress , usPort);

			ServerSession * pServerSession = new HttpSession(szAddress , usPort , "" , "", -1 , NET_STATE_CONNECTED , socket);
			NetHandlerHttpServerPtr pServer( new NetHandlerHttpServer(this , m_pNetReactor , pServerSession) ); 
			m_pNetReactor->AddNetHandler(pServer); 
		} 
	}
	 
	 
	CErrno NetHandlerHttpServer::OnMsgRecving(const char * pBuf , UINT32 unSize)
	{
		INT32 remaineLen = unSize;
		HttpProtocol objRequest;
		while(remaineLen > 0)
		{
			if(objRequest.Parse(&pBuf[unSize - remaineLen],remaineLen,&remaineLen))
			{
				HttpProtocol response;
				CErrno result = m_pServer->HttpHandler((HttpSession *)m_pSession, objRequest, response);
				if (!result.IsSuccess())
				{
					if (result.GetCode() == CErrno::ERR_INVALID_DATA)
					{
						return CErrno::Success();
					}
					return CErrno::Failure();
				}
				objRequest.Clear();
				Send(response);
			}
			else
			{
				break;
			}
		}

		return CErrno::Success();
	}

	INT32 NetHandlerHttpServer::Send(HttpProtocol& response)
	{
		return SendMsg(response.GetBuf(),response.GetLen());
	}


	bool HttpAuth::InitPassword(const std::string & strLicense, void * pPublicKey)
	{
		bool res = false;
		if (pPublicKey)
		{
			BIO * pBio = BIO_new_mem_buf(pPublicKey, (INT32)(strlen((const char *)pPublicKey)));
			res = InitPassword(strLicense , pBio);
			BIO_vfree(pBio);
		}

		return res;
	}

	bool HttpAuth::InitPassword(const std::string & strLicense, const char * pFile)
	{
		bool res = false;
		if (pFile)
		{
			BIO * pBio = BIO_new_file(pFile, "r");
			res = InitPassword(strLicense, pBio);
			BIO_vfree(pBio);
		}

		return res;
	}

	bool HttpAuth::InitPassword(const std::string & strLicense, BIO * pBio)
	{
		if (pBio)
		{
			RSA * pRsa = NULL;
			PEM_read_bio_RSA_PUBKEY(pBio, &pRsa, NULL, NULL);
			if (!pRsa)
			{
				return false;
			}

			std::string strDecodeLic;
			CUtil::Base64DecodeBySSL(strDecodeLic, strLicense);

			unsigned char szBuf[1024] = {0};
			INT32 ret = RSA_public_decrypt((INT32)(strDecodeLic.size()), (const unsigned char *)strDecodeLic.data(), szBuf, pRsa, RSA_PKCS1_PADDING);
			RSA_free(pRsa);
			if (ret <= 0)
			{
				return false;
			}
			
			char  szNamePwdTime[3][256];
			if (sscanf_s((const char *)szBuf , "%[^:]:%[^:]:%s" , szNamePwdTime[0] , szNamePwdTime[1] , szNamePwdTime[2]) != 3)
			{
				return false;
			}

			time_t now = Timer::GetTime();
			for (CollectionEntrysT::iterator iter = m_vecEntries.begin();iter != m_vecEntries.end();++iter)
			{
				if(iter->nExpireTime < now || CUtil::stricmp(iter->strName.c_str() , szNamePwdTime[0]) == 0)
				{
					iter = m_vecEntries.erase(iter);
				}
				else
				{
					++iter;
				}
			}

			INT64 llExpireTime = CUtil::atoi(szNamePwdTime[2]);
			if (llExpireTime == 0 || llExpireTime > now)
			{
				Entry entry;
				entry.strName = szNamePwdTime[0];
				entry.strPWD = szNamePwdTime[1];
				entry.nExpireTime = llExpireTime;

				m_vecEntries.push_back(entry);

				return true;
			}
		}

		return false;
	}

	bool HttpAuth::HasRight(const std::string & strHeadAuth , const std::string & strLicFile)
	{
		if (strHeadAuth.size() >= 256)
		{
			return false;
		}
		char  szNamePwd[3][256];
		if (sscanf_s(strHeadAuth.c_str(), "%s%s" , szNamePwd[0] , szNamePwd[1]) != 2 ||
			CUtil::stricmp(szNamePwd[0] , "basic"))
		{
			return false;
		}

		std::string strDecode;
		CUtil::Base64DecodeBySSL(strDecode, std::string(szNamePwd[1]));
		CUtil::strncpy(szNamePwd[1], strDecode.size(), strDecode.data());
		szNamePwd[1][strDecode.length()] = 0;

		if (sscanf_s(szNamePwd[1] , "%[^:]:%s" , szNamePwd[0] , szNamePwd[2])!= 2)
		{
			return false;
		}

		if (CUtil::stricmp(szNamePwd[0] , "InitPassword") == 0)
		{
			InitPassword(szNamePwd[2], strLicFile.c_str());
			return false;
		}
		if (CUtil::stricmp(szNamePwd[0], "ClearPassword") == 0)
		{
			m_vecEntries.clear();
			return false;
		}
		if (CUtil::stricmp(szNamePwd[0], "LoadPassword") == 0)
		{
			return false;
		}

		if (m_vecEntries.size() == 0)
		{
			return false;
		}

		time_t now = Timer::GetTime();
		for (CollectionEntrysT::iterator iter = m_vecEntries.begin(); iter != m_vecEntries.end(); ++iter)
		{
			if (((iter->nExpireTime && iter->nExpireTime >= now) || iter->nExpireTime == 0)&&
				CUtil::stricmp(iter->strName.c_str(), szNamePwd[0]) == 0 &&
				CUtil::stricmp(iter->strPWD.c_str(), szNamePwd[2]) == 0)
			{
				return true;
			}
		}
		return false;
	}
}
