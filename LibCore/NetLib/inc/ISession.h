#ifndef __net_i_session_h__
#define __net_i_session_h__ 
#include "NetLib/inc/NetCommon.h"
#include "Timer/inc/TimeCount.h"

namespace Net 
{  
	enum ENetState
	{ 
		NET_STATE_LOSTED = 0 ,		//5 连接丢失
		NET_STATE_CONNECTING ,		//5 正在连接中.
		NET_STATE_CONNECTED ,		//5 连接成功
	};

	class DLL_EXPORT ISession
	{
	public:
		ISession(const std::string & strAddress ,INT16 usSocktPort, const std::string & strCurNodeName , const std::string & strRemoteName = "" , INT32 nSessionID = -1 , INT32 nNetState = 0 , NetSocket socket = -1 , INT64 llTimeout = 0);
		virtual ~ISession(void);

	public:
		virtual CErrno		Init();
		virtual	CErrno		Cleanup( void ); 
		virtual	CErrno		OnRecvMsg( void );
		virtual	CErrno		OnSendMsg( void );
		virtual	CErrno		OnClose( void ); 

	public:
		INT32 GetSessionID() const { return m_nSessionID; }
		void SetSessionID(INT32 val) { m_nSessionID = val; }
		void  AutoSetSessionID( void );
		UINT16 GetPort() const { return m_usSocktPort; }
		void SetSocktPort(UINT16 val) { m_usSocktPort = val; }
		std::string GetAddress() { return m_strAddress; }
		void SetAddress(const std::string & val) { m_strAddress = val; }
		Timer::TimeCount GetObjTimeout() const { return m_objTimeout; } 
		BOOL IsClosed(void) const { return m_bClosed; }
		void SetClosed(BOOL val) { m_bClosed = val; }
		std::string GetPeerUUID() const { return m_strPeerUUID; }
		void SetPeerUUID(const std::string & val) { m_strPeerUUID = val; }
		std::string GetCurNodeName() { return m_strCurNodeName; }
		void SetCurNodeName(const std::string & val) { m_strCurNodeName = val;  }
		std::string GetRemoteName() { return m_strRemoteName; }
		void SetRemoteName(const std::string & val) { m_strRemoteName = val; }
		INT32 GetNetState() const { return m_nNetState; }
		void SetNetState(INT32 val) { m_nNetState = val; }
		NetSocket GetSocket() const { return m_socket; }
		void SetSocket(NetSocket val) { m_socket = val; }  
		BOOL IsCanWrite() const { return m_bCanWrite; }
		void SetCanWrite(BOOL val) { m_bCanWrite = val; }
		void SetContext(void * val);
		void * GetContext(void) { return m_pContext; }
		Net::EReactorType GetReactorType() const { return m_objReactorType; }
		void SetReactorType(Net::EReactorType val) { m_objReactorType = val; }
		INT32 GetSendBufSize() const { return m_nSendBufSize; }
		void SetSendBufSize(INT32 val) { m_nSendBufSize = val; }
		INT32 GetRecvBufSize() const { return m_nRecvBufSize; }
		void SetRecvBufSize(INT32 val) { m_nRecvBufSize = val; }
		BOOL IsTimeout(void)
		{ 
			if (m_objTimeout.IsStarted())
			{ 
				return m_objTimeout.IsExpired();
			} 
			else
			{
				return FALSE; 
			}
		} 

	protected:
		NetSocket			m_socket;  
		INT32				m_nNetState;
		BOOL				m_bClosed;
		INT32				m_nSessionID;
		UINT16				m_usSocktPort;
		std::string			m_strAddress;
		std::string			m_strRemoteName;
		std::string			m_strCurNodeName;
		std::string			m_strPeerUUID;
		EReactorType		m_objReactorType;
		void			*	m_pContext;
		INT32				m_nSendBufSize;
		INT32				m_nRecvBufSize;
	private:
		BOOL				m_bCanWrite;
		Timer::TimeCount	m_objTimeout; 
	}; 

}

#endif
