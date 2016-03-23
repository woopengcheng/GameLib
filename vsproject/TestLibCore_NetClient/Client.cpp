#include "Client.h"

CErrno Client::Init()
{
	if (!m_pNetReactor)
	{
// 		m_pNetReactor = new Net::NetReactorDefault;
		m_pNetReactor = new Net::NetReactorWES;
	}
	m_pNetReactor->Init();

	if (!m_pMsgProcess)
	{
		m_pMsgProcess = new TestMsgProcess();
	}

	if (!m_pNetHandlerClient)
	{
		m_pNetHandlerClient = Net::NetHandlerClientPtr(new Net::NetHandlerClient(m_pNetReactor , new Net::ClientSession("127.0.0.1" , 5555 , "" , "" , -1 , 0 , -1 , 0 , 1) , m_pMsgProcess));
	}

	m_pNetHandlerClient->Init("127.0.0.1" , 5555);
	m_pNetReactor->AddNetHandler(m_pNetHandlerClient);

	return CErrno::Success();
}

CErrno Client::InitUDP()
{
	if (!m_pNetReactor)
	{
		m_pNetReactor = new Net::NetReactorUDP;
	}
	m_pNetReactor->Init();

	if (!m_pMsgProcess)
	{
		m_pMsgProcess = new TestMsgProcess();
	}

	if (!m_pNetHandlerClient)
	{
		Net::ClientSession * pSession = new Net::ClientSession("255.255.255.255", 5555, "");
		m_pNetHandlerClient = Net::NetHandlerClientPtr(new Net::NetHandlerClient(m_pNetReactor , pSession , m_pMsgProcess));
	}

	m_pNetHandlerClient->Init("255.255.255.255", 5555);
	m_pNetReactor->AddNetHandler(m_pNetHandlerClient);

	return CErrno::Success();
}
CErrno Client::InitZMQ()
{
	if (!m_pNetReactor)
	{
		m_pNetReactor = new Net::NetReactorZMQ;
	}
	m_pNetReactor->Init();

	if (!m_pMsgProcess)
	{
		m_pMsgProcess = new TestMsgProcess();
	}

	if (!m_pNetHandlerClient)
	{
		Net::ClientSession * pSession = new Net::ClientSession("127.0.0.1", 5555, "");
		m_pNetHandlerClient = Net::NetHandlerClientPtr(new Net::NetHandlerClient(m_pNetReactor , pSession , m_pMsgProcess));
	}

	m_pNetHandlerClient->Init("127.0.0.1", 5555);
	m_pNetReactor->AddNetHandler(m_pNetHandlerClient);

	return CErrno::Success();
}
CErrno Client::InitRakNet()
{
	if (!m_pNetReactor)
	{
		m_pNetReactor = new Net::NetReactorRakNet;
	}
	m_pNetReactor->Init();

	if (!m_pMsgProcess)
	{
		m_pMsgProcess = new TestMsgProcess();
	}

	if (!m_pNetHandlerClient)
	{
		Net::ClientSession * pSession = new Net::ClientSession("127.0.0.1", 5555, "");
		m_pNetHandlerClient = Net::NetHandlerClientPtr(new Net::NetHandlerClient(m_pNetReactor , pSession , m_pMsgProcess));
	}

	m_pNetHandlerClient->Init("127.0.0.1", 5555);
	m_pNetReactor->AddNetHandler(m_pNetHandlerClient);

	return CErrno::Success();
}
CErrno Client::Cleanup()
{
	SAFE_DELETE(m_pMsgProcess);
	SAFE_DELETE(m_pNetReactor);

	return CErrno::Success();
}

CErrno Client::Update()
{
	while(1)
	{
		m_pNetHandlerClient->Update();
		m_pNetReactor->Update();

		const char * kSendMsg = "client sendmsg.\n";
		UINT32 unLen = strlen(kSendMsg) + 1;
		char pBuf[1024]; 
		((Net::MsgHeader*)pBuf)->unMsgID = 0;
		((Net::MsgHeader*)pBuf)->unMsgLength = unLen + sizeof(Net::MsgHeader);
		memcpy(pBuf + sizeof(Net::MsgHeader) , kSendMsg , unLen);

		if (m_pNetHandlerClient)
		{
			m_pNetHandlerClient->SendMsg(pBuf , unLen + sizeof(Net::MsgHeader));
		}
		Timer::sleep(1); 
	}
	return CErrno::Success();
}


CErrno TestMsgProcess::Process(Net::ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength)
{
	printf("%s\n" , pBuffer);

	return Net::MsgProcess::Process(pSession , unMsgID , pBuffer , unLength);
}
