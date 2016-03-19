#include "NetLib/inc/MsgHandler.h"
#include "NetLib/inc/RakNetUDPManager.h" 
#include "NetLib/inc/MsgWrapper.h"
#include "MsgLib/inc/InnerMsg.h"
#include "ThreadPool/inc/ThreadTask.h" 
#include "ThreadPool/inc/ThreadPoolInterface.h" 
#include "Timer/inc/TimerHelp.h"

namespace Net
{ 
	MsgHandler::MsgHandler()
		: m_unClientCount(0)
	{

	}

	MsgHandler::~MsgHandler()
	{

	}

	CErrno MsgHandler::Init()
	{

		return CErrno::Success();
	} 

	CErrno MsgHandler::Cleanup()
	{
		MapMsgHandlerImpT::iterator iter = m_mapMsgHandlerImp.begin();
		for (;iter != m_mapMsgHandlerImp.end();++iter)
		{
			iter->second->Cleanup();
			SAFE_DELETE(iter->second); 
		}
		m_mapMsgHandlerImp.clear();

		return CErrno::Success();
	} 

	INT32 MsgHandler::AddClient( MsgProcess * pMsgProcess , char* pAddress, UINT16 usPort , BOOL bAutoManagerMemeory)
	{
		INT32 nClientIndex = GetClientIndex();

		MsgHandlerImp * pMsgHandlerImp = new MsgHandlerImp;
		
		pMsgHandlerImp->Init(pMsgProcess , bAutoManagerMemeory);
		  
		if(CErrno::Failure() == pMsgHandlerImp->ConnectServer( pAddress , usPort))
		{ 
			pMsgHandlerImp->Cleanup();
			SAFE_DELETE(pMsgHandlerImp);

			return -1;
		}

		ThreadPool::ThreadPoolInterface::GetInstance().AddTask(pMsgHandlerImp);
		 
		MapMsgHandlerImpT::accessor result;
		m_mapMsgHandlerImp.insert(result , nClientIndex);
		result->second = pMsgHandlerImp;

		return nClientIndex;
	}

	INT32 MsgHandler::GetClientIndex(UINT32 unTimes)
	{
		if (unTimes == MAX_RECURSION_TIMERS)    //5 递归的最大深度,其实是没用的.真实的环境是没有这么大的客户端的.
		{
			return -1;
		}

		int nClientIndex = 0;
		if(m_objThreadLock.try_lock())
		{
			nClientIndex = (m_unClientCount ++) % MAX_CLIENT_NUMBER;
		}
		m_objThreadLock.unlock(); 

		MapMsgHandlerImpT::accessor result;
		if (m_mapMsgHandlerImp.find(result , nClientIndex))
		{
			GetClientIndex(++unTimes);
		}
		else
		{
			return nClientIndex;
		}

		return -1;
	}

	CErrno MsgHandler::SendMsg( UINT32 unClientIndex , UINT32 unMsgID, const char* pBuffer, UINT32 unLength )
	{
		MsgHandlerImp * pMsgHandlerImp = GetMsgHandlerImp(unClientIndex);
		if (pMsgHandlerImp)
		{
			return pMsgHandlerImp->SendMsg(unMsgID , pBuffer , unLength);
		}
		
		return CErrno::Failure();
	}

	MsgHandlerImp * MsgHandler::GetMsgHandlerImp( UINT32 unClientIndex )
	{
		MapMsgHandlerImpT::accessor result;
		if (m_mapMsgHandlerImp.find(result , unClientIndex))
		{
			return result->second;
		}

		return NULL;
	}

	BOOL MsgHandler::IsConnected( UINT32 unClientIndex )
	{
		MsgHandlerImp * pMsgHandlerImp = GetMsgHandlerImp(unClientIndex);
		if (pMsgHandlerImp)
		{
			return pMsgHandlerImp->IsConnected();
		}

		return FALSE; 
	}

	CErrno MsgHandlerImp::ConnectServer(char * pAddress , UINT16 usPort )
	{
		if (m_pNet)
		{
			if (pAddress == NULL)
			{
				return m_pNet->Connect("127.0.0.1" , usPort);
			}
			else
				return m_pNet->Connect(pAddress , usPort);
		}

		return CErrno::Failure();
	}
	
	CErrno MsgHandlerImp::Update()
	{ 
		InflowRecvBuffer();
		PorcessMsg(); 
		OutflowSendBuffer(); 

		return CErrno::Success();
	}

	CErrno MsgHandlerImp::Init(MsgProcess * pMsgProcess , BOOL bAutoManagerMemeory)
	{ 
#ifdef USE_RAKNET
		m_pNet = new RaknetUDPManager;
#else
#endif
		return m_pNet->Init();
	}

	CErrno MsgHandlerImp::Cleanup()
	{  
		m_pNet->Cleanup();
		SAFE_DELETE(m_pNet);

		if (m_bAutoManagerMemory)
		{ 
			SAFE_DELETE(m_pMsgProcess);
		}

		return CErrno::Success();
	}

	BOOL MsgHandlerImp::IsConnected()
	{
		if (m_pNet)
		{
			return m_pNet->IsConnected();
		}

		return FALSE;
	}

	CErrno MsgHandlerImp::InflowRecvBuffer( void )
	{
		if (m_pNet)
		{
			MsgWrapper * pMsgWrapper = m_pNet->RecvMsg();
			while (pMsgWrapper)
			{
				m_queueRecvMsgs.push(pMsgWrapper);
				pMsgWrapper = m_pNet->RecvMsg();
				Timer::sleep(1);
			}

			return CErrno::Success();
		}

		return CErrno::Failure();
	}

	CErrno MsgHandlerImp::PorcessMsg()
	{
		MsgWrapper * pMsgWrapper = NULL; 

		if (m_pMsgProcess)
		{
			while (FetchMsgFromRecvBuffer(pMsgWrapper))
			{ 
	//			m_pMsgProcess->Process(pMsgWrapper->GetMsgID() , pMsgWrapper->GetMsg() , pMsgWrapper->GetMsgSize());
				
				const char * pMsg = pMsgWrapper->GetMsg();
				SAFE_DELETE_ARRAY(pMsg);
				SAFE_DELETE(pMsgWrapper);

				Timer::sleep(1);
			}
			return CErrno::Success();
		} 
		
		return CErrno::Failure(); 
	}

	INT32 MsgHandlerImp::FetchMsgFromRecvBuffer( MsgWrapper *& pMsg )
	{
		return m_queueRecvMsgs.try_pop(pMsg);
	}

	CErrno MsgHandlerImp::OutflowSendBuffer()
	{
		CErrno nCodeError = CErrno::Failure();
		if (m_pNet)
		{
			MsgWrapper * pMsgWrapper = NULL;
			while(m_queueSendMsgs.try_pop(pMsgWrapper))
			{
				nCodeError = m_pNet->SendMsg(pMsgWrapper);

				const char * pMsg = pMsgWrapper->GetMsg();
				SAFE_DELETE_ARRAY(pMsg);
				SAFE_DELETE(pMsgWrapper);

				Timer::sleep(1);
			}
		}
		return nCodeError;
	}

	CErrno MsgHandlerImp::SendMsg( UINT32 unMsgID, const char* pBuffer, UINT32 unLength)
	{
		if (m_pNet)
		{
			char * pMsg = new char[unLength];  
			memcpy(pMsg , pBuffer , unLength);
			MsgWrapper * pMsgWrapper = new MsgWrapper(unMsgID , pMsg , unLength); 

			m_queueSendMsgs.push(pMsgWrapper);

			return CErrno::Success();
		} 

		return CErrno::Success();
	}

}