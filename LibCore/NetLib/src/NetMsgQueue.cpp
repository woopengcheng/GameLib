#include "NetLib/inc/NetMsgQueue.h"
#include "NetLib/inc/INetReactor.h"

namespace Net
{ 	
	CErrno NetMsgQueue::HandleMsg(const char* pBuffer, UINT32 unLength)
	{
		MsgHeader * pHeader = (MsgHeader*)pBuffer;

		if (m_pNetReactor->IsMutilThread() && pHeader && pHeader->unMsgID != DEFAULT_MSG_PING_ID)
		{
			CUtil::Chunk objChunk(pBuffer, unLength);
			m_queRecvMsgQueues.push(objChunk);

			return CErrno::Success();
		}
		else
		{
			return NetHandlerTransit::HandleMsg(pBuffer, unLength);
		}
	}
	
	INT32 NetMsgQueue::SendMsg(const char * pBuf, UINT32 unSize)
	{
		if (m_pNetReactor->IsMutilThread())
		{
			CUtil::Chunk objChunk(pBuf, unSize);
			m_queSendMsgQueues.push(objChunk);
		}
		else
		{
			return NetHandlerTransit::SendMsg(pBuf, unSize);
		}

		return unSize;
	}

	CErrno NetMsgQueue::FetchMsgs(CollectMsgChunksVec & vecMsgs)
	{
		CUtil::Chunk objChunk;
		while (m_queRecvMsgQueues.try_pop(objChunk))
		{
			vecMsgs.push_back(objChunk);
		} 

		return CErrno::Success();
	}

	CErrno NetMsgQueue::DeliverMsg()
	{
		INT32 nSize = 0;
		CErrno err;
		CUtil::Chunk objChunk;

		while (m_queSendMsgQueues.try_pop(objChunk))
		{
			nSize = NetHandlerTransit::SendMsg((const char *)(objChunk.Begin()), objChunk.GetDataLen());
			if (nSize <= 0 && err.IsSuccess())
			{
				err = CErrno::Failure();
			}
		}

		return err;
	}


}