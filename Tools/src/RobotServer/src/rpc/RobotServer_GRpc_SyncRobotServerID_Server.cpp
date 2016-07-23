#include "GRpc.h"
#include "RobotServer.h"

Msg::ObjectMsgCall * Robot::GRpc::SyncRobotServerID_RpcServer(INT32 nSessionID, Msg::Object objSrc , INT32 server_port/* = 0*/)
{
	INT32 res = 0;

	Robot::RobotServer::GetInstance().SetServerID(objSrc.m_llObjID);
	Robot::RobotServer::GetInstance().SetRobotGroupConnectPort(server_port);

	std::cout << "GRpc::SyncRobotServerID_RpcServer "<< std::endl;
	Return(res);
}

