#include "GRpc.h"
#include "RobotGroup.h"

Msg::ObjectMsgCall * Robot::GRpc::SyncRobotGroupID_RpcServer(INT32 nSessionID, Msg::Object objSrc )
{
	INT32 res = 0;

	RobotGroup::GetInstance().SetServerID(objSrc.m_llObjID);

	std::cout << "GRpc::SyncRobotGroupID_RpcServer "<< std::endl;
	Return(res);
}

