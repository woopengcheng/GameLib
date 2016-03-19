#include "GameMsgProcess.h"
#include "MsgDefine.h"


namespace Game
{
	INT32 GameMsgProcess::Process(Net::ISession * pSession , UINT32 unMsgID, const char* pBuffer, UINT32 unLength)
	{
		using namespace ::Net::accsrv;
		
		if(!IsAccept(unMsgID))
			return -1;
		
		switch(unMsgID)
		{
		case 1:
			{
				CSLogin objProtoMsg;
				if(!objProtoMsg.ParseFromArray(pBuffer,unLength))
					return -1;
				return Process(pSession , objProtoMsg);
			}
			break;
		default:
			{
				printf("%d" , unMsgID);
			} //end default
			break;
		} //end switch
		return -1;
	}

} //end namespace ::Net::accsrv
