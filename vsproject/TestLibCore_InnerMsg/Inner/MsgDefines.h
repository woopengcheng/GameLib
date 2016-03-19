#ifndef __server_rpc_defines_h__
#define __server_rpc_defines_h__
#include "MsgLib/inc/MsgCommon.h"   
#include "MsgLib/inc/ObjectMsgCall.h"
#include "MsgLib/inc/Object.h"  
#include "CUtil/inc/Chunk.h"

namespace Client
{  
 	extern Msg::ObjectMsgCall *  local_run_Test(char p1 , INT16 p2, UINT16 p3 , INT32 p4 , UINT32 p5 , const char * p6 , CUtil::Chunk p7 , std::vector<Msg::Object> vecTargets , Msg::Object objSrc);

#define  MSG_DEFINE_Object_Test public:\
	Msg::ObjectMsgCall *  local_run_Object_Test(char p1 , INT16 p2, UINT16 p3 , INT32 p4 , UINT32 p5 , const char * p6 ,CUtil::Chunk p7 , std::vector<Msg::Object> vecTargets , Msg::Object objSrc);
}


#endif