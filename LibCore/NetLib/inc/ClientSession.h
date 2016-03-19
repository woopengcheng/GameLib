#ifndef __net_client_session_h__
#define __net_client_session_h__ 
#include "NetLib/inc/ISession.h"

namespace Net 
{   
	class DLL_EXPORT ClientSession : public ISession
	{
	public:
		ClientSession(const std::string & strAddress, INT16 usSocktPort, const std::string & strCurNodeName, const std::string & strRemoteName = "", INT32 nSessionID = -1 , INT32 nNetState = 0 , NetSocket socket = -1 , INT64 llTimeout = 0 , BOOL bReconnect = FALSE )
			: ISession(strAddress, usSocktPort , strCurNodeName , strRemoteName, nSessionID , nNetState , socket , llTimeout)
			, m_bReconnect(bReconnect)
		{}
		virtual ~ClientSession(){}

	public:
		BOOL     IsReconnect( void ) const { return m_bReconnect; }
		void     SetReconnect(BOOL val) { m_bReconnect = val; }

	private:
		BOOL     m_bReconnect;
	}; 
	  
}

#endif
