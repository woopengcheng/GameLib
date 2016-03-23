#include "NetLib/inc/NetReactorSelect.h"
#include "NetLib/inc/NetHandlerClient.h"
#include "Timer/inc/TimerHelp.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace Net
{ 
	NetReactorSelect::NetReactorSelect(UINT32 unMaxConnectionCount , NetThread * pThread/* = NULL*/)
		: INetReactor(REACTOR_TYPE_SELECT , pThread)
		, m_nNetHandlerCount(0)
		, m_unMaxConnectionCount(unMaxConnectionCount) 
		, m_pFdSetReads(NULL)
		, m_pFdSetWrites(NULL)
		, m_pFdSetExcepts(NULL)
	{  

		size_t  size = unMaxConnectionCount * sizeof(INT32) + sizeof(fd_set);
		m_pFdSetReads = malloc(size);
		m_pFdSetWrites = malloc(size);
		m_pFdSetExcepts = malloc(size);

		memset(m_pFdSetReads , 0 , size);
		memset(m_pFdSetWrites , 0 , size);
		memset(m_pFdSetExcepts , 0 , size);
	}

	NetReactorSelect::~NetReactorSelect( void )
	{  
		free(m_pFdSetReads);
		free(m_pFdSetWrites);
		free(m_pFdSetExcepts); 
	}

	CErrno NetReactorSelect::Init( void )
	{

		return CErrno::Success();
	}

	CErrno NetReactorSelect::Cleanup( void )
	{  
		m_mapNetHandlers.clear();
		m_mapNetEvents.clear();

		return INetReactor::Cleanup();
	}

	CErrno NetReactorSelect::Update( void )
	{
		if (m_nNetHandlerCount == 0)
		{
			return CErrno::Failure();
		}

		fd_set* pFdSetWrites = (fd_set*)m_pFdSetWrites;
		fd_set* pFdSetReads =  (fd_set*)m_pFdSetReads;
		fd_set* pFdSetExcepts = (fd_set*)m_pFdSetExcepts;

		FD_ZERO(pFdSetExcepts);
		FD_ZERO(pFdSetReads);
		FD_ZERO(pFdSetWrites); 

		{ 
			MapNetEventsT::iterator iterEvents = m_mapNetEvents.begin();
			MapNetHandlersT::iterator iterHandlers = m_mapNetHandlers.begin();
			for (;iterEvents != m_mapNetEvents.end() && iterHandlers != m_mapNetHandlers.end() ;++iterEvents , ++ iterHandlers)
			{
				NetSocket socket = -1;
				INetHandlerPtr pINetHandler = iterHandlers->second;
				if (pINetHandler)
				{
					socket = iterHandlers->second->GetSession()->GetSocket();
					if (iterEvents->second & (FD_READ | FD_ACCEPT)) 
						FD_SET(socket , pFdSetReads); 
					if (iterEvents->second & FD_WRITE) 
						FD_SET(socket , pFdSetWrites); 
					if (iterEvents->second & FD_CLOSE) 
						FD_SET(socket , pFdSetExcepts);   
				}
			}
		}

		struct timeval objTimeout = {0};
		objTimeout.tv_usec = 100;
		objTimeout.tv_sec = 0;

		int nResult = select(m_nNetHandlerCount , pFdSetReads , pFdSetWrites , pFdSetExcepts , &objTimeout);
		if (nResult > 0)
		{
			MapNetHandlersT::iterator iterHandlers = m_mapNetHandlers.begin();
			for (; iterHandlers != m_mapNetHandlers.end();)
			{
				BOOL  bClosed = FALSE;
				INetHandlerPtr  pNetHandler  = iterHandlers->second;
				if (pNetHandler)
				{
					NetSocket socket = pNetHandler->GetSession()->GetSocket(); 

					if (FD_ISSET(socket , pFdSetReads))
					{
						bClosed = !pNetHandler->OnMsgRecving().IsSuccess() || bClosed;
					}

					if (FD_ISSET(socket , pFdSetWrites))
					{
						if (pNetHandler->GetSession()->GetNetState() == NET_STATE_CONNECTING)
						{
							INT32 nError = 0;
							INT32 unErrorLen = sizeof(nError);
							nResult = getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*)&nError, &unErrorLen);
							if (nResult == -1 || nError)
							{
								bClosed = TRUE;
							}
							else
							{
								pNetHandler->GetSession()->SetClosed(FALSE);
								pNetHandler->GetSession()->SetNetState(Net::NET_STATE_CONNECTED);
							}
						}
						else
						{
							bClosed = !pNetHandler->OnMsgSending().IsSuccess() || bClosed;
						}
					}

					if (FD_ISSET(socket , pFdSetExcepts))
					{
						bClosed = TRUE;
					}

					ISession * pSession = pNetHandler->GetSession();
					if (pSession && pSession->GetObjTimeout().IsExpired() || pSession->IsClosed())
					{
						bClosed = TRUE;
					}

					if (bClosed)
					{
						gDebugStream("delete curNodeName=" << pNetHandler->GetSession()->GetCurNodeName() << ":remoteName=" << pNetHandler->GetSession()->GetRemoteName() << ":address=" << pNetHandler->GetSession()->GetAddress() << ":port=" << pNetHandler->GetSession()->GetPort());
						DelNetHandler(pNetHandler , FALSE);
						m_mapNetHandlers.erase(iterHandlers++); 
					}  
					else
					{
						++ iterHandlers;
					}
				}
			}
		} 
		Timer::sleep(1);

		return CErrno::Success();
	}

	CErrno NetReactorSelect::AddNetHandler( INetHandlerPtr  pNetHandler  , ENetHandlerFuncMask objMask /*= NET_FUNC_DEFAULT*/ )
	{
		Assert_ReF(m_unMaxConnectionCount > m_nNetHandlerCount && pNetHandler->GetSession());
	
		INT32  nFDMask = 0; 
		if(objMask & NET_FUNC_READ) 
			nFDMask |= FD_READ;
		if(objMask & NET_FUNC_WRITE)  
			nFDMask |= FD_WRITE;
		if(objMask & NET_FUNC_ACCEPT)  
			nFDMask |= FD_ACCEPT;
		if(objMask & NET_FUNC_EXCEPT)  
			nFDMask |= FD_CLOSE; 
		 
		INT32 nSessionID = pNetHandler->GetSession()->GetSessionID();
		if (m_mapNetHandlers.find(nSessionID) == m_mapNetHandlers.end())
		{
			m_mapNetHandlers.insert(std::make_pair(nSessionID, pNetHandler));
			m_mapNetEvents.insert(std::make_pair(nSessionID, nFDMask));
			++m_nNetHandlerCount;
		}
		else
		{
			gErrorStream("NetReactorSelect::AddNetHandler error. nodeName=" << pNetHandler->GetSession()->GetCurNodeName() << ":address=" << pNetHandler->GetSession()->GetAddress() << ":port=" << pNetHandler->GetSession()->GetPort());

			return CErrno::Failure();
		}

		return INetReactor::AddNetHandler(pNetHandler , objMask);
	}

	CErrno NetReactorSelect::DelNetHandler( INetHandlerPtr  pNetHandler , BOOL bEraseHandler/* = TRUE */)
	{
		if (pNetHandler.get())
		{
			MapNetEventsT::iterator iterEvents = m_mapNetEvents.find(pNetHandler->GetSession()->GetSessionID());
			if (iterEvents != m_mapNetEvents.end())
			{
				m_mapNetEvents.erase(iterEvents);
			}
			else
				MsgAssert_ReF(0 , "cant find Net Handler\n" );
			 
			pNetHandler->OnClose();  

 			--m_nNetHandlerCount;
		}

		return INetReactor::DelNetHandler(pNetHandler , bEraseHandler);
	}

	CErrno NetReactorSelect::ModNetHandler( INetHandlerPtr  pNetHandler  , ENetHandlerFuncMask objMask )
	{
		Assert_ReF(pNetHandler->GetSession());

		MapNetEventsT::iterator iterEvents = m_mapNetEvents.find(pNetHandler->GetSession()->GetSessionID());
		if (iterEvents != m_mapNetEvents.end())
		{
			iterEvents->second = objMask;
		}
		else
			return  CErrno::Failure();

		return INetReactor::ModNetHandler(pNetHandler , objMask);
	}

}