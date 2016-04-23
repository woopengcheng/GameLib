extern "C"
{
#include "zmq.h" 
}
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"
#include "NetLib/inc/NetHelper.h"
#include "NetLib/inc/ISession.h"
#include "NetLib/inc/INetHandler.h"
#include "NetLib/inc/NetReactorIOCP.h"
#include "CUtil/inc/CUtil.h"


#ifdef _WIN32
#include <windows.h>  
#endif 

namespace Net
{
	INT32 InitNet()
	{
#ifdef WIN32
		WSADATA wsd;
		return WSAStartup(MAKEWORD(2, 2), &wsd);  //5 0代表成功
#else
		return 0;
#endif
	}

	INT32 CleanNet()
	{
#ifdef WIN32
		return WSACleanup();
#else
		return 0;
#endif

	}

	std::string NetHelper::GenerateRemoteName( const char * pNetType , const char * pAddress , const char * pPort )
	{
		std::string str = pNetType;
		str += "://";
		str += pAddress;
		str += ":";
		str += pPort;

		return str;
	}

	std::string NetHelper::GenerateRemoteName( const char * pNetType , const char * pAddress , UINT16 usPort )
	{ 
		char szPort[MAX_NAME_LENGTH]; 
		CUtil::itoa(szPort , (INT64)usPort); 

		return GenerateRemoteName(pNetType , pAddress , szPort); 
	}

	std::string NetHelper::GenerateRemoteName( const char * pNetType , const char * pAddress , UINT16 usPort ,UINT16 usRemotePort )
	{
		char szPort[MAX_NAME_LENGTH] , szRemotePort[MAX_NAME_LENGTH];
		CUtil::itoa(szPort , (INT64)usPort);  
		CUtil::itoa(szRemotePort , (INT64)usRemotePort);  

		return GenerateRemoteName(pNetType , pAddress , szPort , szRemotePort); 
	}

	std::string NetHelper::GenerateRemoteName( const char * pNetType , const char * pAddress , const char * pPort , const char * pRemotePort )
	{ 
		std::string str = pNetType;
		str += "-";
		str += pAddress;
		str += "-";
		str += pPort;
		str += "-";
		str += pRemotePort;

		return str;
	}

	UINT8 NetHelper::GetPacketIdentifier(RakNet::Packet * pPacket)
	{
		if (!pPacket)
			return 255;

		if ((unsigned char)pPacket->data[0] == ID_TIMESTAMP)
		{
			RakAssert(pPacket->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
			return (unsigned char)pPacket->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
		}
		else
			return (unsigned char)pPacket->data[0];
	}

#define		ANY_SIZE	 120
#ifdef WIN32
	BOOL NetHelper::IsSocketPortUsed(UINT32 nPort)
	{
		//连接信息结构体
		typedef struct _MIB_TCPROW
		{
			DWORD dwState;							//TCP连接状态
			DWORD dwLocalAddr;						//本地IP
			DWORD dwLocalPort;						//本地端口
			DWORD dwRemoteAddr;						//远程机器IP
			DWORD dwRemotePort;						//远程机器端口
		} MIB_TCPROW, *PMIB_TCPROW;

		//TCP连接表
		typedef struct _MIB_TCPTABLE
		{
			DWORD      dwNumEntries;				//当前包含MIB_TCPROW类型的总数
			MIB_TCPROW table[ANY_SIZE];             //存放每个TCP连接的端口和IP
		} MIB_TCPTABLE, *PMIB_TCPTABLE;

		if (nPort < 1024 || nPort > 65000)
			FALSE;

		PMIB_TCPTABLE ptcptable = NULL;
		DWORD dwSize = 0;

		HINSTANCE hInst;		//动态链接库模块句柄
		hInst = LoadLibraryA("iphlpapi.dll");		//动态加载iphlpapi.dll

													//定义函数指针类型
		typedef long(*ADDPROC) (PMIB_TCPTABLE pTcpTable, PDWORD pdwSize, BOOL bOrder);

		//获取iphlpapi.dll导出函数
		ADDPROC GetTcpTable = (ADDPROC)GetProcAddress(hInst, "GetTcpTable");

		if (GetTcpTable(ptcptable, &dwSize, TRUE) == ERROR_INSUFFICIENT_BUFFER)		//pTcpTable空间不足
		{
			ptcptable = new MIB_TCPTABLE[dwSize];		//为pTcpTable申请足够的空间

			if (GetTcpTable(ptcptable, &dwSize, TRUE) == NO_ERROR)     //GetTcpTable调用成功
			{
				//检测端口nPort是否在
				for (UINT i = 0; i < ptcptable->dwNumEntries; i++)
				{
					if (nPort != ptcptable->table[i].dwLocalPort)
						continue;

					//释放资源
					SAFE_DELETE_ARRAY(ptcptable);
					FreeLibrary((HMODULE)hInst);

					return TRUE;
				}
			}
		}
		//释放资源
		SAFE_DELETE_ARRAY(ptcptable);
		FreeLibrary((HMODULE)hInst);

		return FALSE;
	}
#else
	BOOL NetHelper::IsSocketPortUsed(UINT32 nPort)
	{
		const char * pAddress = "127.0.0.1";
		sockaddr_in addr = { 0 };
		addr.sin_family = AF_INET;
		addr.sin_port = htons(nPort);
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
		if (pAddress != 0 && strlen(pAddress) > 0)
		{
			addr.sin_addr.s_addr = inet_addr(ip);
		}
		NetSocket socket = NetHelper::CreateSocket(AF_INET, SOCK_STREAM, 0);
		INT32 nValueTrue = 1;
		setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&nValueTrue, sizeof(nValueTrue));
		::bind(socket, (sockaddr*)&addr, sizeof(sockaddr_in));
		CloseSocket(socket);
		return TRUE;
	}
#endif

	void NetHelper::SetSocket(NetSocket & socket)
	{
		linger s = {0};
		s.l_onoff = 1;
		setsockopt(socket,SOL_SOCKET,SO_LINGER,(char*)&s,sizeof(linger));

		int valuetrue = 1;
		setsockopt(socket,SOL_SOCKET,SO_KEEPALIVE,(char*)&valuetrue,sizeof(int));

		SetIOCtrl(socket,FIOASYNC,&valuetrue);
	}

	BOOL NetHelper::IsUDPBroadCast(const std::string & strAddress)
	{
		return 0 == CUtil::stricmp(strAddress.c_str(), "255.255.255.255");
	}

	INT32 NetHelper::RecvMsg(NetSocket socket , char * pBuf , UINT32 unSize)
	{
#ifdef __linux
			return ::recv(socket, pBuf, unSize, MSG_DONTWAIT);
#else
			return ::recv(socket, pBuf, unSize, 0);
#endif
	}

	INT32 NetHelper::RecvMsg(NetSocket socket, char * pBuf, UINT32 unSize, struct sockaddr * from , int  * fromlen)
	{
#ifdef _LINUX
		return ::recv(socket, pBuf, unSize, MSG_DONTWAIT, from, fromlen);
#else
		return ::recvfrom(socket, pBuf, unSize , 0 , from, fromlen);
#endif
	}

	INT32 NetHelper::RecvMsg(NetSocket socket, char * pBuf, UINT32 unSize , INT32 & recv_fd)
	{
		INT32  ret = 0; 
#ifdef _LINUX
		struct  msghdr msg;
		char  recvchar;
		struct  iovec vec;
		char  cmsgbuf[CMSG_SPACE(sizeof(recv_fd))];
		struct  cmsghdr *p_cmsg;
		int  *p_fd;
		vec.iov_base = pBuf;
		vec.iov_len = unSize;
		msg.msg_name = NULL;
		msg.msg_namelen = 0;
		msg.msg_iov = &vec;
		msg.msg_iovlen = 1;
		msg.msg_control = cmsgbuf;
		msg.msg_controllen = sizeof(cmsgbuf);
		msg.msg_flags = 0;

		p_fd = (int  *)CMSG_DATA(CMSG_FIRSTHDR(&msg));
		*p_fd = -1;
		ret = recvmsg(sock_fd, &msg, 0);
		if (ret < 1)
			ERR_EXIT("recvmsg");

		p_cmsg = CMSG_FIRSTHDR(&msg);
		if (p_cmsg == NULL)
			ERR_EXIT("no passed fd");

		p_fd = (int  *)CMSG_DATA(p_cmsg);
		recv_fd = *p_fd;
		if (recv_fd == -1)
			ERR_EXIT("no passed fd");
#endif
		return ret;
	}

	INT32 NetHelper::RecvMsg(RakNet::Packet * pPacket, char * pBuf, UINT32 unSize)
	{
		UINT32 unPacketSize = 0;
		RakNet::BitStream bs(pPacket->data, pPacket->bitSize, false);
		RakNet::MessageID objMsgID;
		bs.Read(objMsgID);  //5 这里用不到,需要去掉.如果需要的话.需要特殊处理.
		bs.Read(unPacketSize);
		bs.Read(pBuf);

		if (unPacketSize != pPacket->length - sizeof(RakNet::MessageID))
		{
			return -1;
		}

		return pPacket->length - sizeof(RakNet::MessageID);
	}

	INT32 NetHelper::RecvMsg(zmqSocketPtr pZmqSocket, zmq_msg_t * pZmqMsg, char * pBuf, UINT32 unSize)
	{
		INT32 nResult = zmq_recvmsg(pZmqSocket, pZmqMsg, ZMQ_DONTWAIT);
		if (nResult < 0)
		{
			if (zmq_errno() == EAGAIN)
			{
				nResult = zmq_msg_close(pZmqMsg);
				if (nResult != 0)
				{
					gErrorStream("error in zmq_msg_close: %s" << zmq_strerror(errno));
					return  -1;
				}
				return 0;
			}

			gErrorStream("error in zmq_recvmsg: %s" << zmq_strerror(errno));
			return 0;
		}

		pBuf = (char *)zmq_msg_data(pZmqMsg);
		size_t usSize = zmq_msg_size(pZmqMsg);

		return  (INT32)usSize;
	}

	BOOL NetHelper::IsSocketEagain()
	{ 
#ifdef _LINUX
		int e = errno;
		return (e == EAGAIN || e == EINTR);
#else
		int e = WSAGetLastError();
		return e == WSAEWOULDBLOCK;
#endif 

	}

	INT32 NetHelper::SetIOCtrl( int s,long cmd,int *argp )
	{
#ifdef _LINUX
		return ::ioctl(s,cmd,argp);
#else
		return ioctlsocket(s,cmd,(u_long*)argp);
#endif

	}

	INT32 NetHelper::NetToN( const char *ip, struct in_addr *addr )
	{
#ifdef _LINUX
		return ::inet_aton(ip,addr);
#else
		return inet_pton(AF_INET, ip, (PVOID*)(&addr->S_un.S_addr)); 
#endif
	}

#ifdef __linux
	BOOL NetHelper::GetIPByHost( char* ip , char* host = NULL)
	{ 
		char hostname[256]; 
		if (host == NULL)
		{
			int ret = gethostname(hostname,sizeof(hostname));
			if (ret==SOCKET_ERROR)
			{
				return false;
			}

			host = hostname;
		}

		struct hostent *answer;
		char ipstr[16];

		answer = gethostbyname(host);
		if (answer == NULL)
			return false;
		inet_ntop(AF_INET, (answer->h_addr_list)[0], ip, 16);
		return true;
	}
#else
	BOOL NetHelper::GetIPByHost( char* ip ,const char * host )
	{
		char hostname[256]; 
		if (host == NULL)
		{
			int ret=gethostname(hostname,sizeof(hostname));
			if (ret==SOCKET_ERROR)
			{
				return false;
			}

			host = hostname;
		}

		struct hostent *answer;

		answer = gethostbyname(host);
		if (answer == NULL)
			return false;
#ifdef _LINUX
		char* ipstr = inet_ntoa(*(in_addr*)(answer->h_addr_list)[0]);
#else
		char ipstr[MAX_ADDRESS_LENGTH];
		inet_ntop(AF_INET, (answer->h_addr_list)[0], ipstr, MAX_ADDRESS_LENGTH);
#endif
		if(ipstr == NULL)
			return false;
		CUtil::strncpy(ip, strlen(ipstr) + 1 , ipstr);
		return true;
	}

	BOOL NetHelper::GetAddressAndPortByAddrIn( sockaddr_in * pAddrIn , char * pAddress , UINT16 & usPort )
	{
		Assert_Re0(pAddrIn);
#ifdef _LINUX
		char* ipstr = inet_ntoa(*(in_addr*)(answer->h_addr_list)[0]);
#else
		char ipstr[MAX_ADDRESS_LENGTH];
		inet_ntop(AF_INET, pAddrIn, ipstr, MAX_ADDRESS_LENGTH);
#endif
		sprintf(pAddress, ipstr);
		usPort = pAddrIn->sin_port;

		return TRUE;
	}

	INT32 NetHelper::SetDefaultSocket( NetSocket & socket , INT32 nSendBufSize/* = DEFAULT_SOCKET_BUFFER_SIZE */ , INT32 nRecvBufSize/* = DEFAULT_SOCKET_BUFFER_SIZE */)
	{ 
		INT32 nRes = 0;
		if (socket > 0)
		{
			INT32 valtrue = 1;

			nRes += setsockopt(socket,IPPROTO_TCP,TCP_NODELAY,(char*)&valtrue,sizeof(valtrue));
			nRes += setsockopt(socket,SOL_SOCKET,SO_KEEPALIVE,(char*)&valtrue,sizeof(valtrue));
			 
			nRes += setsockopt(socket,SOL_SOCKET,SO_SNDBUF,(char*)&nSendBufSize,sizeof(nSendBufSize));
			nRes += setsockopt(socket,SOL_SOCKET,SO_RCVBUF,(char*)&nSendBufSize,sizeof(nSendBufSize)); 
		} 
		return nRes;
	}

	void NetHelper::CloseSocket( NetSocket  socket )
	{
		::closesocket(socket); 
	}

	void NetHelper::CloseSocket( ISession * pSession )
	{
		if (pSession)
		{
			CloseSocket(pSession->GetSocket());
			pSession->SetSocket(-1);
		}

	}

	NetSocket NetHelper::CreateSocket( INT32 af, INT32 type, INT32 protocol )
	{
		return ::socket(af , type , protocol);
	}

#endif
}