#include "NetLib/inc/NetLib.h"
#include "NetLib/inc/NetHandlerClient.h"
#include "Timer/inc/TimerHelp.h"
#include "NetLib/inc/MsgProcess.h"

class TestMsgProcess : public Net::MsgProcess
{
public:
	virtual CErrno Process(Net::ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength);
};

class Client  
{
public:
	Client()
		: m_pNetReactor(NULL)
		, m_pMsgProcess(NULL)
	{

	}
	~Client(){  }

public:
	virtual CErrno  Init();
	CErrno InitUDP();
	CErrno InitZMQ();
	CErrno InitRakNet();
	virtual CErrno  Cleanup();
	virtual CErrno  Update(); 

private:
	Net::INetReactor         *  m_pNetReactor;
	Net::NetHandlerClientPtr    m_pNetHandlerClient;
	Net::MsgProcess          *  m_pMsgProcess;
	//	Net::NetHandlerClient *  m_pNetHandlerClient;
};