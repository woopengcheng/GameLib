#ifndef _msg_handler_0F990BBA_3827_4E93_B8F9_12E8FC9896A0_include_
#define _msg_handler_0F990BBA_3827_4E93_B8F9_12E8FC9896A0_include_

#include "pw_def.h"
#include "pwdist_def.h"
#include "accsrv.pb.h"

namespace pwaccsrv
{
	class MsgHandler
	{
	public:
		virtual bool IsAccept(int msgid) { return true; }
	public:
		MsgHandler(void* context) : mContext(context){}
		virtual ~MsgHandler(){}
	public:
		int Process(pwdist::sint64 conn,pwdist::uint32 msgid,const char* buf,size_t len);
	public:
		int Process(pwdist::sint64 conn,::pwngs::protocol::accsrv::CSLogin& msg);// msgid:11 name:CSLogin
	public:
		void* mContext;
	};

} //end namespace pwaccsrv

#endif //_msg_handler_0F990BBA_3827_4E93_B8F9_12E8FC9896A0_include_
