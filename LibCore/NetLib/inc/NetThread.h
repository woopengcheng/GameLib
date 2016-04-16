#ifndef __net_net_thread_h__
#define __net_net_thread_h__  
#include "NetLib/inc/NetCommon.h"
#include "NetLib/inc/NetHandlerPing.h"
#include "NetLib/inc/NetHandlerListener.h"
#include "ThreadPool/inc/ThreadTask.h"
#include "CUtil/inc/Chunk.h"
#include "CUtil/inc/CUtil.h"
#include "json/json.h"

namespace Net
{
	struct SCreateInfo  //5 这个用来动态创建时需要的参数 
	{
		std::string strUUID;
		std::string strNodeName;
		std::string strAddress;
		UINT16		usPort;
		BOOL		bReconnect;

		SCreateInfo()
			: strAddress("")
			, strUUID("")
			, strNodeName("")
			, usPort(0)
			, bReconnect(false)
		{
		}
		SCreateInfo(const std::string & nodeName, const std::string & uuid, const std::string & address, UINT16 port , BOOL reconnect)
			: strAddress(address)
			, strUUID(uuid)
			, strNodeName(nodeName)
			, usPort(port)
			, bReconnect(reconnect)
		{
		}
	};

	struct SPeerKey
	{
		std::string strUUID;
		std::string strNodeName;
		std::string strAddress;
		UINT16		usPort;

		SPeerKey()
			: strAddress("")
			, strUUID("")
			, strNodeName("")
			, usPort(0)
		{
		}
		SPeerKey(const std::string & nodeName , const std::string & uuid , const std::string & address , UINT16 port)
			: strAddress(address)
			, strUUID(uuid)
			, strNodeName(nodeName)
			, usPort(port)
		{
		}
	};

	struct PeerCreateInfoHashFunc
	{
		size_t operator()(const SPeerKey & key) const
		{
			return /*CUtil::BKDRHash(key.strNodeName.c_str()) + key.usPort + CUtil::BKDRHash(key.strAddress.c_str() +*/CUtil::BKDRHash(key.strUUID.c_str());
		}
	};
	struct PeerCreateInfoCmpFunc //比较函数 ==  
	{
		bool operator()(const SPeerKey & key1, const SPeerKey & key2) const
		{
			if (key1.strUUID == key2.strUUID/*key1.strNodeName == key2.strNodeName && key2.usPort == key1.usPort && key1.strAddress == key2.strAddress*/)
			{
				return true;
			}
			return false;
		}
	};

	class DLL_EXPORT  NetThread : public ThreadPool::ThreadSustainTask
	{
	public:
		typedef tbb::concurrent_queue<SPeerInfo>				QueAcceptSessionT;
		typedef tbb::concurrent_queue<SCreateInfo>				QueCreateClientsT;
		typedef std::unordered_map<SPeerKey, SPeerInfo, PeerCreateInfoHashFunc, PeerCreateInfoCmpFunc>			MapPeerSessionT;

	public:
		NetThread(void);
		virtual ~NetThread(void);
		static NetThread & GetInstance();

	public:
		virtual CErrno		Init(Json::Value & conf);
		virtual CErrno		Cleanup(void);
		virtual CErrno		Update(void);
		virtual CErrno		StartupServer(const std::string & strNetNodeName, const std::string & strType, const std::string & strAddress, INT32 nPort);
		virtual CErrno		StartupClient(const Json::Value & clients);

	public:
		const std::string	GetServerAddress()	const { return m_strAddress; }
		const std::string	GetNetNodeName()	const { return m_strNetNodeName; }
		const std::string	GetServerType()		const { return m_strRpcType; }
		UINT16				GetServerPort() { return m_usServerPort; }
		INetReactor		*	GetNetReactor() { return m_pNetReactor; }
		static INT32		InternalUpdate(void * pArg);

	public:
		CErrno				UpdateHandlers(void);
		INetHandlerPtr		CreateClientHandler(std::string strNodeName, std::string strUUID , const char * pAddress, UINT16 usPort , Net::NetSocket socket = 0, void * context = NULL);
		INT32				SendMsg(INT32 nSessionID, const char * pBuffer, UINT32 unLength);
		CErrno				FetchSession(std::vector<SPeerInfo> & vecSessions);
		CErrno				FetchMsgs(INT32 nSessionID, CollectMsgChunksVec & queMsgs);
		SPeerInfo			GetPeerInfo(const SPeerKey & objInfo);
		CErrno				AddPeerSession(const SPeerKey & objKey, const SPeerInfo & objPeerInfo);
		CErrno				RefreshPeerSession(const SPeerKey & objKey, ISession * pSession);
		CErrno				InsertClientsQueue(const std::string & strNodeName , const std::string & strAddress, UINT16 usPort, BOOL bReconnect = FALSE);

	protected:
		CErrno				DeliverMsg();
		CErrno				FetchClientsQueue();

	protected:
		std::string			m_strAddress;
		std::string			m_strNetNodeName;
		std::string			m_strRpcType;
		UINT16				m_usServerPort;
		INetReactor		*	m_pNetReactor;         //5 没有访问机会.不会加锁
		MapPeerSessionT		m_mapPeerSessions;
		QueCreateClientsT	m_queCreateClients;
		QueAcceptSessionT	m_queAceeptSessions;
		BOOL				m_bInitFirst;
		Json::Value			m_objInitConf;
	};

}


#endif