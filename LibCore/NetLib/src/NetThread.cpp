#include "NetLib/inc/NetThread.h"
#include "NetLib/inc/NetLib.h"
#include "ThreadPool/inc/ThreadPoolInterface.h"

namespace Net
{ 

	NetThread::NetThread(void)
		: ThreadPool::ThreadSustainTask(DEFAULT_THREAD_PRIORITY , "NetThread" , TRUE)
		, m_usServerPort(0)
		, m_pNetReactor(NULL)
		, m_bInitFirst(FALSE)
	{ 
	} 

	NetThread::~NetThread(void)
	{
		Cleanup();
	}

	NetThread & NetThread::GetInstance()
	{
		static NetThread instance;
		return instance;
	}

	CErrno NetThread::Init(Json::Value & conf)
	{   
		if (!m_bInitFirst)
		{
			m_objInitConf = conf;

			if (!m_pNetReactor)
			{
				m_pNetReactor = new Net::NetReactorDefault();
				m_pNetReactor->SetNetThread(this);

				if (CErrno::Success() != m_pNetReactor->Init())
				{
					SAFE_DELETE(m_pNetReactor);
					MsgAssert_ReF(0, "NetReactor fail.");
				}
			}

			Json::Value netThead = conf.get("net_thread", Json::Value());
			INT32 nPriority = netThead.get("priority", DEFAULT_THREAD_PRIORITY).asInt();

			SetThreadPriority(nPriority);
			ThreadPool::ThreadPoolInterface::GetInstance().CreateThread(nPriority);
			ThreadPool::ThreadPoolInterface::GetInstance().AddTask(this);

		}
		else
		{
			Json::Value server = conf.get("server", Json::Value());
			std::string strType = server.get("listen_type", "tcp").asCString();
			std::string strAddress = server.get("listen_address", "127.0.0.1").asCString();
			INT32 nPort = server.get("listen_port", 8003).asInt();
			std::string strNodeName = server.get("net_node_name", "").asCString();

			StartupServer(strNodeName, strType, strAddress, nPort);

			Json::Value clients = conf.get("clients", Json::Value());
			StartupClient(clients);
		}
		return CErrno::Success(); 
	}

	CErrno NetThread::Cleanup( void )
	{
		ThreadPool::ThreadPoolInterface::GetInstance().Closeup();
		if (m_pNetReactor || m_pNetReactor->Cleanup().IsFailure())
		{
			return CErrno::Failure();
		}
		SAFE_DELETE(m_pNetReactor);  

		return CErrno::Success();
	} 

	CErrno NetThread::Update( void )
	{
		if (!m_bInitFirst)
		{
			m_bInitFirst = TRUE;
			Init(m_objInitConf);
		}

		FetchClientsQueue();

		UpdateHandlers();

		CErrno err = DeliverMsg();

		if(m_pNetReactor)
		{
			if (m_pNetReactor->Update().IsSuccess() && err.IsSuccess())
			{
				err = CErrno::Failure();
			}
		}

		return err;
	}

	CErrno NetThread::UpdateHandlers(void)
	{
		if (m_pNetReactor)
		{
			INetReactor::CollectNetHandlersT::const_iterator iter = m_pNetReactor->GetNetHandlers().begin();
			for (; iter != m_pNetReactor->GetNetHandlers().end(); ++iter)
			{
				INetHandlerPtr pHandler = iter->second;
				if (pHandler)
				{
					pHandler->Update();
				}
			}
		}

		return CErrno::Success();
	}

	CErrno NetThread::DeliverMsg()
	{
		CErrno err;
		const INetReactor::CollectNetHandlersT mapHandlers = m_pNetReactor->GetNetHandlers();
		INetReactor::CollectNetHandlersT::const_iterator iter = mapHandlers.begin();
		for (; iter != mapHandlers.end(); ++iter)
		{
			const INetHandlerPtr pHandler = iter->second;
			if (pHandler)
			{
				if (pHandler->DeliverMsg().IsFailure() && err.IsSuccess())
				{
					err = CErrno::Failure();
				}
			}
		}
		return err;
	}

	CErrno NetThread::FetchMsgs(INT32 nSessionID , CollectMsgChunksVec & vecMsgs)
	{
		INetHandlerPtr pHandler = m_pNetReactor->GetNetHandlerByID(nSessionID);
		if (pHandler && pHandler->GetSession() && !pHandler->GetSession()->IsClosed())
		{
			return pHandler->FetchMsgs(vecMsgs);
		}

		return CErrno::Failure();
	}

	CErrno NetThread::StartupServer(const std::string & strNetNodeName , const std::string & strType , const std::string & strAddress , INT32 nPort)
	{
		m_usServerPort = nPort;
		m_strNetNodeName = strNetNodeName;
		m_strAddress = strAddress;
		m_strRpcType = strType;

		Net::ISession * pSeesion = new Net::ServerSession(strAddress.c_str() , m_usServerPort , m_strNetNodeName);
		NetHandlerListenerPtr pListener(new NetHandlerListener(m_pNetReactor , pSeesion));
		if (CErrno::Failure() == pListener->Init(strAddress.c_str(), m_usServerPort))
		{
			gErrorStream("NetHandlerListener init failure:" << m_strNetNodeName << ":address=" << strAddress << ":port=" << m_usServerPort);
			return CErrno::Failure();
		}

		if (!(m_pNetReactor->AddNetHandler(pListener, Net::NET_FUNC_ACCEPT_DEFAULT)).IsSuccess())
		{
			gErrorStream("StartupRPCServer AddNetHandler failure:" << strNetNodeName << ":address=" << strAddress << ":port=" << m_usServerPort);
			return CErrno::Failure();
		}

		gDebugStream("StartupRPCServer success:" << strNetNodeName << ":address=" << strAddress << ":port=" << m_usServerPort);
		return CErrno::Success();
	}
	
	CErrno NetThread::StartupClient(const Json::Value & clients)
	{
		INT32 nCount = clients.size(); 
		for (INT32 i = 0 ;i < nCount; ++ i)
		{     
			Json::Value client = clients[i];

			INT32 bReconnect = client.get("reconnect", 1).asInt();
			std::string strType = client.get("type" , "tcp").asCString();
			std::string strAddress = client.get("address", "127.0.0.1").asCString();
			INT32 nPort = client.get("port", 0000).asInt();
			INT32 nSendBuf = client.get("send_buf", DEFAULT_SOCKET_BUFFER_SIZE).asInt();
			INT32 nRecvBuf = client.get("recv_buf", DEFAULT_SOCKET_BUFFER_SIZE).asInt();
			
			INetHandlerPtr pNetHandler = CreateClientHandler(m_strNetNodeName, "" , strAddress.c_str(), nPort);
			if (!pNetHandler)
			{
				return CErrno::Failure();
			}
			ClientSession * pSession = dynamic_cast<ClientSession*>(pNetHandler->GetSession());
			if (pSession)
			{
				pSession->SetReconnect(bReconnect);
				NetSocket objSocket = pSession->GetSocket();
				if (objSocket != -1)
				{
					NetHelper::SetDefaultSocket(objSocket, nSendBuf, nRecvBuf);
					pSession->SetSendBufSize(nSendBuf);
					pSession->SetSendBufSize(nRecvBuf);
				}
			}
		}
		return CErrno::Success();
	} 

	INetHandlerPtr NetThread::CreateClientHandler(std::string strNodeName, std::string  strUUID, const char * pAddress, UINT16 usPort, Net::NetSocket socket /*= 0*/, void * context/* = NULL*/)
	{
		if (!pAddress)
		{
			gErrorStream("createClientHandler error.");
			return NULL;
		}
		
		if (strUUID == "")
		{
			char buf[256] = { 0 };
			CUtil::GenerateUUIDBySys(buf);
			strUUID = buf;
		}
		Net::ClientSession * pSession = new Net::ClientSession(pAddress, usPort, strNodeName.c_str());
		NetHandlerClientPtr pNetHandler(new NetHandlerClient(m_pNetReactor, pSession));
		pSession->SetPeerUUID(strUUID.c_str());

		CErrno nResult = pNetHandler->Init();
//		if (nResult.IsSuccess())
		{
			m_pNetReactor->AddNetHandler(pNetHandler);
		}

		SPeerKeey objCreateInfo;
		objCreateInfo.strAddress = pAddress;
		objCreateInfo.strNodeName = strNodeName;
		objCreateInfo.strUUID = strUUID;
		objCreateInfo.usPort = usPort;

		SPeerInfo objPeer;
		objPeer.nSessionID = pSession->GetSessionID();
		objPeer.nPeerSessionID = -1;
		objPeer.usPeerPort = usPort;
		objPeer.nState = PING_STATE_PINGING;
		objPeer.strAddress = pAddress;
		objPeer.strCurNodeName = strNodeName;
		objPeer.bReconect = false;

		AddPeerSession(objCreateInfo , objPeer);
		return pNetHandler;
	}

	INT32 NetThread::SendMsg(INT32 nSessionID , const char * pBuffer, UINT32 unLength)
	{
		INetHandlerPtr pHandler = m_pNetReactor->GetNetHandlerByID(nSessionID);
		if (pHandler)
		{
			 return pHandler->SendMsg(pBuffer, unLength);
		}

		return -1;
	}
	
	CErrno NetThread::FetchSession(std::vector<SPeerInfo> & vecSessions)
	{
		SPeerInfo sessions;
		while (m_queAceeptSessions.try_pop(sessions))
		{
			vecSessions.push_back(sessions);
		}

		return CErrno::Success();
	}

	CErrno NetThread::AddPeerSession(const SPeerKeey & objKey,const SPeerInfo & objPeerInfo)
	{
		MapPeerSessionT::iterator iter = m_mapPeerSessions.find(objKey);
		if (iter != m_mapPeerSessions.end())
		{
			if (iter->second.nState != PING_STATE_VALID)
			{
				if (objPeerInfo.nSessionID == -1)
				{
					iter->second.nPeerSessionID = objPeerInfo.nPeerSessionID;
				}
				if (objPeerInfo.nPeerSessionID == -1)
				{
					iter->second.nSessionID = objPeerInfo.nSessionID;
				}
				iter->second.strRemoteNodeName = objPeerInfo.strRemoteNodeName;
				iter->second.nState = PING_STATE_PINGED;
				iter->second.bReconect = objPeerInfo.bReconect;
				m_queAceeptSessions.push(iter->second);
			}
		}
		else
		{
			m_mapPeerSessions.insert(std::make_pair(objKey, objPeerInfo));
		}

		return CErrno::Success();
	}

	CErrno NetThread::FetchClientsQueue()
	{
		SPeerKeey key;
		while (m_queCreateClients.try_pop(key))
		{
			INetHandlerPtr pNetHandler = CreateClientHandler(key.strNodeName, key.strUUID ,key.strAddress.c_str(), key.usPort);
			if (!pNetHandler)
			{
				continue;
			}
		}

		return CErrno::Success();
	}

	CErrno NetThread::InsertClientsQueue(const std::string & strNodeName, const std::string & strAddress, UINT16 usPort)
	{
		char buf[256] = { 0 };
		CUtil::GenerateUUIDBySys(buf);
		std::string strUUID = buf;
		m_queCreateClients.push(SPeerKeey(strNodeName , strUUID , strAddress , usPort));

		return CErrno::Success();
	}

	SPeerInfo		NetThread::GetPeerInfo(const SPeerKeey & objInfo)
	{
		MapPeerSessionT::iterator iter = m_mapPeerSessions.find(objInfo);
		if (iter != m_mapPeerSessions.end())
		{
			return iter->second;
		}

		return SPeerInfo();
	}

}