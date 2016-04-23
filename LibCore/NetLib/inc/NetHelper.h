#ifndef __net_net_helper_h__
#define __net_net_helper_h__ 
#include "NetLib/inc/NetCommon.h" 
#include "NetLib/inc/INetHandler.h"

struct in_addr;
struct sockaddr_in;

namespace RakNet
{
	struct Packet;
}
struct zmq_msg_t;
typedef void * zmqSocketPtr;

namespace Net 
{
	class ISession;

	class UDSContext
	{
	public:
		UDSContext()
			: m_nTransferFD(0)
		{}
		~UDSContext() {}

	public:
		INT32			GetTransferFD() const { return m_nTransferFD; }
		void			SetTransferFD(INT32 val) { m_nTransferFD = val; }

	private:
		INT32			m_nTransferFD;
	};

	class DLL_EXPORT NetHelper
	{
	public: 
		static void			SetSocket(NetSocket & socket);
		static INT32		SetDefaultSocket(NetSocket & socket , INT32 nSendBufSize = DEFAULT_SOCKET_BUFFER_SIZE , INT32 nRecvBufSize = DEFAULT_SOCKET_BUFFER_SIZE );
		static INT32		SetIOCtrl(int s, long cmd, int *argp);
		static INT32		RecvMsg(NetSocket socket, char * pBuf, UINT32 unSize);
		static INT32		RecvMsg(NetSocket socket, char * pBuf, UINT32 unSize, INT32 & recv_fd);
		static INT32		RecvMsg(RakNet::Packet * pPacket, char * pBuf, UINT32 unSize);
		static INT32		RecvMsg(zmqSocketPtr pZmqSocket, zmq_msg_t * pZmqMsg, char * pBuf, UINT32 unSize);
		static INT32		RecvMsg(NetSocket socket, char * pBuf, UINT32 unSize, struct sockaddr * from, int * fromlen);
		static BOOL			IsSocketEagain();
		static INT32		NetToN( const char *ip, struct in_addr *addr );
		static BOOL			GetIPByHost( char* ip , const char* host = NULL);
		static BOOL			GetAddressAndPortByAddrIn(sockaddr_in * pAddrIn, char * pAddress, UINT16 & usPort);
		static void			CloseSocket(NetSocket socket);
		static void			CloseSocket(ISession * pSession);
		static NetSocket	CreateSocket(INT32 af, INT32 type, INT32 protocol);
		static BOOL			IsUDPBroadCast(const std::string & strAddress);
		static std::string	GenerateRemoteName(const char * pNetType, const char * pAddress, UINT16 usPort);
		static std::string	GenerateRemoteName(const char * pNetType, const char * pAddress, const char * pPort);
		static std::string	GenerateRemoteName(const char * pNetType, const char * pAddress, UINT16 usPort, UINT16 usRemotePort);
		static std::string	GenerateRemoteName(const char * pNetType, const char * pAddress, const char * pPort, const char * pRemotePort);
		static UINT8		GetPacketIdentifier(RakNet::Packet * pPacket);
		static BOOL			IsSocketPortUsed(UINT32 unPort);

#ifdef _WIN32
		static INT32		WSARecv(INetHandler * pNetHandler);
		static INT32		WSAAccept(INetHandler * pNetHandler);
		static INT32		WSASend(INetHandler * pNetHandler ,const char* buf, INT32 len);
#endif
	};
}

#endif
