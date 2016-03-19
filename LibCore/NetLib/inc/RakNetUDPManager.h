#ifndef __net_raknet_udp_manager_h__
#define __net_raknet_udp_manager_h__
#include "NetLib/inc/NetCommon.h"
#include "NetLib/inc/NetBase.h"

namespace RakNet
{
	struct Packet;
	class  RakPeerInterface; 
}

namespace Net
{
	class RaknetUDPManager : public NetBase
	{
	public:
		RaknetUDPManager();
		virtual ~RaknetUDPManager();

	public:
		virtual CErrno Init(void);
		virtual CErrno Cleanup(void);
		virtual CErrno Connect(char* pIPAddress, UINT16 usPort);
		virtual CErrno CloseConnection(void);
		virtual CErrno CloseConnection(UINT32 unIndex);
		virtual CErrno SendMsg(MsgWrapper* pMsg);
		virtual MsgWrapper *   RecvMsg(void);

	public:
		//		virtual INT32 SendSpace(); 
		virtual void Run(void);  

	private:
		unsigned char GetPacketIdentifier(RakNet::Packet * pPacket);
		MsgWrapper  * RecvMsgInStream(RakNet::Packet * pPacket);

	private:
		RakNet::RakPeerInterface * m_pRakPeerInstance;
	}; 
}
#endif