#ifndef __net_net_handler_ping_h__
#define __net_net_handler_ping_h__
#include "CUtil/inc/Chunk.h"
#include "NetLib/inc/NetCommon.h"  
#include "NetLib/inc/NetMsgQueue.h"

namespace Net
{
	enum EPingState
	{
		PING_STATE_VALID,
		PING_STATE_PINGING,
		PING_STATE_LOST,
		PING_STATE_PINGED,
	};

	struct SPeerInfo
	{
		UINT16		usPeerPort;
		INT32		nSessionID;
		INT32		nPeerSessionID;
		EPingState	nState;
		std::string strAddress;
		std::string strCurNodeName;
		std::string strRemoteNodeName;
		bool		bReconect;

		SPeerInfo()
		{
			Clear();
		}

		void Clear()
		{
			bReconect = false;
			usPeerPort = 0;
			nSessionID = -1;
			nPeerSessionID = -1;
			nState = (PING_STATE_VALID);
			strAddress = ("");
			strCurNodeName = ("");
			strRemoteNodeName = ("");

		}
	};

	struct SPing
	{
		UINT16  usPeerPort;
		char    szAddress[MAX_ADDRESS_LENGTH];
		char	szNodeName[MAX_NAME_LENGTH];
		char	szUUID[MAX_NAME_LENGTH];

		SPing()
		{
			memset(this, 0, sizeof(SPing));
		}
	};

	class DLL_EXPORT NetHandlerPing : public NetMsgQueue
	{
	public:
		NetHandlerPing(INetReactor * pNetReactor, ISession * pSession)
			: NetMsgQueue(pNetReactor, pSession)
			, m_llLastSendPing(0)
		{}
		~NetHandlerPing() {}

	public:
		virtual CErrno		Update(void) override;
		virtual CErrno		HandlePing(ISession * pSession, SPing * pPing);
		virtual	CErrno		HandleMsg(const char* pBuffer, UINT32 unLength) final;
		virtual CErrno		HandleMsg(ISession * pSession, UINT32 unMsgID, const char* pBuffer, UINT32 unLength) override;
		virtual CErrno		UpdatePing(void);
		virtual CErrno		Ping(void);

	private:
		INT64				m_llLastSendPing;
	};
};

#endif