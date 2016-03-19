#ifndef __net_server_session_h__
#define __net_server_session_h__ 
#include "NetLib/inc/ISession.h"

namespace Net 
{  
	class DLL_EXPORT ServerSession : public ISession
	{
	public:
		ServerSession(const std::string & strAddress, INT16 usSocktPort, const std::string & strCurNodeName, const std::string & strRemoteName = "", INT32 nSessionID = -1 , INT32 nNetState = NET_STATE_CONNECTED, NetSocket socket = -1 ,INT64 llTimeout = 0)
			: ISession(strAddress, usSocktPort , strCurNodeName , strRemoteName , nSessionID , nNetState , socket , llTimeout)
		{
			SetClosed(FALSE);
		}
		virtual ~ServerSession(){}
	};  

}

#endif
