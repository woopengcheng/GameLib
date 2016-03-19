#ifndef __client_dbclient_h__
#define __client_dbclient_h__   
#include "GameDB/inc/DBClientInterface.h"
#include "GameDB/inc/RemoteNodeDefine.h"

namespace Client
{	
	class  DBClient : public GameDB::DBClientInterface
	{ 
	public:
		DBClient(void);
		virtual ~DBClient(void)
		{
			SAFE_DELETE(m_pRpcListener);
		} 

	public:
		static DBClient & GetInstance()
		{
			static DBClient m_sRpcInterface;
			return m_sRpcInterface;
		} 

	public: 
		virtual void	OnRegisterRpcs(void) override;

	public:
		INT32			GetServerSessionID() const { return m_nServerSessionID; }
		void			SetServerSessionID(INT32 nSessionID) { m_nServerSessionID = nSessionID; }
		INT64			GetServerID() const { return m_llServerID; }
		void			SetServerID(INT64 nID) { m_llServerID = nID; }

	private:
		INT32			m_nServerSessionID;
		INT64			m_llServerID;
	};

	class ClientListener : public Msg::IRpcListener
	{
	public:
		ClientListener(DBClient * pClient)
			: m_pDBClient(pClient)
		{

		}

	public:
		virtual CErrno OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect = false) override;
		virtual CErrno OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID) override;

	private:
		DBClient * m_pDBClient;
	};

}

#endif