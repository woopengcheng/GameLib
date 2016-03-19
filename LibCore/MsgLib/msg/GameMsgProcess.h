#ifndef __msg_process_25FF8816_C9BC_4458_A94C_FF9576670B17_h__
#define __msg_process_25FF8816_C9BC_4458_A94C_FF9576670B17_h__
#include "MsgProcess.h"
#include "Common.h"
#include "accsrv.pb.h"

namespace Game
{
	class GameMsgProcess: public Net::MsgProcess
	{
	public:
		virtual BOOL IsAccept(UINT32 unMsgID) { return true; }

	public:
		GameMsgProcess(){}
		virtual ~GameMsgProcess(){}

	public:
		virtual INT32 Process(Net::ISession * pSession , UINT32 unMsgID, const char* pBuffer, UINT32 unLength);
	public:
		INT32 Process(Net::ISession * pSession ,::Net::accsrv::CSLogin & msg);// msgid:1 name:CSLogin
	};
} //::Net::accsrv

#endif // __msg_process_25FF8816_C9BC_4458_A94C_FF9576670B17_h__
