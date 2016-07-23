#include "GRpc.h"

Msg::ObjectMsgCall * Robot::GRpc::UpdateRobotStart_RpcServer(INT32 nSessionID, Msg::Object objSrc , CUtilChunk & file_list/* = CUtil::Chunk()*/)
{
	CUtilChunk file_list_re = CUtil::Chunk();


	std::cout << "GRpc::UpdateRobotStart_RpcServer "<< std::endl;
	Return(file_list_re);
}

