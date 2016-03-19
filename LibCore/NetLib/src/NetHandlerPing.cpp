#include "NetLib/inc/NetHandlerPing.h"
#include "NetLib/inc/INetReactor.h"
#include "NetLib/inc/NetHelper.h"
#include "NetLib/inc/NetThread.h"
#include "Timer/inc/TimerHelp.h"

namespace Net
{
	CErrno NetHandlerPing::Update(void)
	{
		UpdatePing();
		return NetMsgQueue::Update();
	}

	CErrno NetHandlerPing::UpdatePing(void)
	{
		if (m_pNetReactor && m_pNetReactor->GetNetThread())
		{
			INT64 ullCurTime = Timer::GetTickMicroSecond();
			if (ullCurTime - m_llLastSendPing >= DEFAULT_PING_TIME_OUT / 5)
			{
				m_llLastSendPing = Timer::GetTickMicroSecond();

				return Ping();
			}
		}

		return CErrno::Success();
	}

	CErrno NetHandlerPing::Ping(void)
	{
		if (m_pNetReactor && m_pNetReactor->GetNetThread() && m_pSession)
		{
			NetThread * pNetThread = m_pNetReactor->GetNetThread();
			if (pNetThread && m_pSession->GetNetState() == NET_STATE_CONNECTED && !m_pSession->IsClosed())
			{
				char pBuf[1024] = { 0 };
				SPing * pPing = (SPing *)(pBuf + sizeof(MsgHeader));
				pPing->usPeerPort = pNetThread->GetServerPort();
				memcpy(pPing->szAddress, pNetThread->GetServerAddress().c_str(), pNetThread->GetServerAddress().length() + 1);
				memcpy(pPing->szNodeName, m_pSession->GetCurNodeName().c_str(), m_pSession->GetCurNodeName().length() + 1);
				memcpy(pPing->szUUID, m_pSession->GetPeerUUID().c_str(), m_pSession->GetPeerUUID().length() + 1);

				((MsgHeader*)pBuf)->unMsgID = DEFAULT_MSG_PING_ID;
				((MsgHeader*)pBuf)->unMsgLength = sizeof(SPing) + sizeof(MsgHeader);

				if (SendMsg((const char *)(pBuf), ((MsgHeader*)pBuf)->unMsgLength) <= 0)
				{
					return CErrno::Failure();
				}
// 				gDebugStream("send ping.address=" << pPing->szAddress << ":port=" << pPing->usPeerPort << ":nodeName=" << pPing->szNodeName );
			}
		}

		return CErrno::Success();

	}

	CErrno NetHandlerPing::HandleMsg(const char* pBuffer, UINT32 unLength)
	{
		MsgHeader * pHeader = (MsgHeader*)pBuffer;

		if (m_pNetReactor->IsMutilThread() && pHeader && pHeader->unMsgID == DEFAULT_MSG_PING_ID)
		{
			return NetHandlerPing::HandleMsg(m_pSession , pHeader->unMsgID, pBuffer + sizeof(MsgHeader)  , unLength - sizeof(MsgHeader));
		}
		else
		{
			return NetMsgQueue::HandleMsg(pBuffer, unLength);
		}
	}

	CErrno NetHandlerPing::HandleMsg(ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength)
	{
		if (m_pNetReactor->IsMutilThread())
		{
			switch (unMsgID)
			{
				case DEFAULT_MSG_PING_ID:
				{
					Assert_ReF(pBuffer && unLength == sizeof(SPing));

					SPing * pPing = (SPing *)pBuffer;

					return HandlePing(pSession, pPing);
				}break;
			}
		}
		return NetMsgQueue::HandleMsg(pBuffer, unLength);
	}

	CErrno NetHandlerPing::HandlePing(ISession * pSession, SPing * pPing)
	{
		if (pPing && m_pNetReactor && m_pNetReactor->GetNetThread())
		{
			NetThread * pThread = m_pNetReactor->GetNetThread();
			if (pThread)
			{
				SPeerKeey objInfo;
				objInfo.strAddress = pPing->szAddress;
				objInfo.strNodeName = pPing->szNodeName;
				objInfo.usPort = pPing->usPeerPort;
				objInfo.strUUID = pPing->szUUID;

				SPeerInfo  peerInfo = pThread->GetPeerInfo(objInfo);
				if (peerInfo.nState != PING_STATE_PINGED)
				{
					if (peerInfo.nState == PING_STATE_VALID)
					{
						INetHandlerPtr pHandler = pThread->CreateClientHandler(pThread->GetNetNodeName(), pPing->szUUID , pPing->szAddress, pPing->usPeerPort);
						if (!pHandler)
						{
							gErrorStream("recv ping createClientHandler failed.address=" << pPing->szAddress << ":port=" << pPing->usPeerPort << ":nodeName=" << pPing->szNodeName);
							return CErrno::Failure();
						}
					}
					
					if (peerInfo.nState == PING_STATE_PINGING)
					{
						peerInfo.Clear();

						peerInfo.nPeerSessionID = pSession->GetSessionID();
						peerInfo.nSessionID = -1;
						peerInfo.nState = PING_STATE_PINGING;
						peerInfo.usPeerPort = pPing->usPeerPort;
						peerInfo.strAddress = pPing->szAddress;
						peerInfo.strRemoteNodeName = pPing->szNodeName;
						peerInfo.bReconect = false;

						pSession->SetRemoteName(pPing->szNodeName );
						pThread->AddPeerSession(objInfo, peerInfo);
					}
				}
				else
				{
					if (peerInfo.nPeerSessionID != pSession->GetSessionID())
					{
						peerInfo.Clear();

						peerInfo.nPeerSessionID = pSession->GetSessionID();
						peerInfo.nSessionID = -1;
						peerInfo.nState = PING_STATE_PINGING;
						peerInfo.usPeerPort = pPing->usPeerPort;
						peerInfo.strAddress = pPing->szAddress;
						peerInfo.strRemoteNodeName = pPing->szNodeName;
						peerInfo.bReconect = true;

						pSession->SetRemoteName(pPing->szNodeName);
						pThread->AddPeerSession(objInfo, peerInfo);
					}
				}
//				gDebugStream("recv ping.address=" << pPing->szAddress << ":port=" << pPing->usPeerPort << ":nodeName=" << pPing->szNodeName);
			}
		}

		return CErrno::Success();
	}
}