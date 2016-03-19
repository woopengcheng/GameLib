#include "DBClient.h"

namespace Client
{
	DBClient::DBClient(void)
		: m_nServerSessionID(-1)
		, m_llServerID(0)
	{
		m_pRpcListener = new ClientListener(this);
	}

	CErrno ClientListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (strNetNodeName == g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER] && m_pDBClient)
		{
			m_pDBClient->SetServerSessionID(nSessionID);
		}

		return CErrno::Success();
	}
	CErrno ClientListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{
		return CErrno::Success();
	}

}