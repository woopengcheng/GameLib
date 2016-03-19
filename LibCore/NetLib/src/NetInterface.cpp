#include "NetLib/inc/NetInterface.h"
#include "NetLib/inc/MsgHandler.h"
#include "MsgLib/inc/InnerMsg.h" 
#include "ThreadPool/inc/ThreadPoolInterface.h"

namespace Net
{
	NetInterface::NetInterface( void )
		: m_pMsgHandler(NULL)
	{

	}

	NetInterface::~NetInterface( void )
	{

	}

	NetInterface & NetInterface::GetInstance()
	{
		static NetInterface sNetInterface;

		return sNetInterface;
	}

	CErrno NetInterface::Init( void )
	{
 		ThreadPool::ThreadPoolInterface::GetInstance().CreateThread(DEFAULT_THREAD_PRIORITY , DEFAULT_THREAD_PRIORITY_COUNT);
		
		m_pMsgHandler = new MsgHandler;
		m_pMsgHandler->Init();

		return CErrno::Success();
	}

	CErrno NetInterface::Cleanup( void )
	{
		m_pMsgHandler->Cleanup();
		SAFE_DELETE(m_pMsgHandler);
		 
		return CErrno::Success();
	}

	INT32 NetInterface::AddClient( MsgProcess * pMsgProcess , char* pAddress, UINT16 usPort ,  BOOL bAutoManagerMemeory )
	{
		if (m_pMsgHandler)
		{
			return m_pMsgHandler->AddClient(pMsgProcess , pAddress , usPort , bAutoManagerMemeory); 
		}

		return -1;
	}

	BOOL NetInterface::IsConnected( UINT32 unClientIndex )
	{
		if (m_pMsgHandler)
		{
			return m_pMsgHandler->IsConnected(unClientIndex);
		}

		return FALSE;
	}

	CErrno NetInterface::SendMsg( UINT32 unClientIndex , UINT32 unMsgID, const char* pBuffer, UINT32 unLength )
	{
		if (m_pMsgHandler)
		{
			return m_pMsgHandler->SendMsg(unClientIndex , unMsgID , pBuffer , unLength);
		}
		
		return CErrno::Failure();
	}


}

