#include "msg.def.h"
#include "msg.handler.h"
#include "pw_logger.h"

namespace pwaccsrv
{
	int MsgHandler::Process(pwdist::sint64 conn,pwdist::uint32 msgid,const char* buf,size_t len)
	{
		using namespace ::pwngs::protocol::accsrv;
		
		if(!IsAccept(msgid))
			return -1;
		
		switch(msgid)
		{
		case msgid_CSLogin:
			{
				CSLogin actualMsg;
				if(!actualMsg.ParseFromArray(buf,len))
					return -1;
				return Process(conn,actualMsg);
			}
			break;
		default:
			{
				gErrorStream << "unexpected msg:" << msgid;
			} //end default
			break;
		} //end switch
		return -1;
	}

} //end namespace pwaccsrv
