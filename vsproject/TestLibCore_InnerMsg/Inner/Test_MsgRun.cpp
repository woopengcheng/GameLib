#ifndef __client_global_rpc_h__
#define __client_global_rpc_h__
#include "MsgDefines.h"

namespace Client
{
	Msg::ObjectMsgCall *  local_run_Test(char p1 , INT16 p2, UINT16 p3 , INT32 p4 , UINT32 p5 , const char * p6 , CUtil::Chunk p7 , std::vector<Msg::Object> vecTargets , Msg::Object objSrc)
	{
		printf("Local_run_test."); 

		return NULL;
	}  

}

#endif