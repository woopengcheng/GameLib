#include "ServerHandler.h"
#include "GameDB/inc/HashTable.h"
#include "GameDB/inc/Environment.h"
#include "GameDB/inc/Database.h"
#include "GameDB/inc/Operate.h"
#include "GameDB/inc/User.h"
#include "RPCCallFuncs.h"

Msg::ObjectMsgCall * Server::ServerHandler::HandleCreateUser_RpcServer(INT32 nSessionID , Msg::Object objSrc , std_string &name/* = std::string()*/ , std_string & pwd/* = std::string()*/, SINT8 issys/* = 0*/  )
{ 
	INT32 res = -1;

	if (name.size() == 0 || pwd.size() == 0  && !GetDBServer() && !GetDBServer()->GetEnvironment())
	{
		Return(res);
	}

	GameDB::Database * pDB = GetDBServer()->GetEnvironment()->GetDatabase(g_szSystemDatabase);
	if (!pDB)
	{
		Return(res);
	}

	//5 ºÏ≤È»®œﬁ
	if (!m_objAuthInfo.CheckSysPermission())
	{
		Return(res);
	}

	std::string strValue;
	GameDB::User objUser;
	objUser.set_name(name.c_str());
	objUser.set_pswd(pwd.c_str());
	objUser.set_sysuser(issys != 0);
	objUser.ToBson(strValue);

	GameDB::Operate oper;
	GameDB::HashTable::HSet(*pDB , oper , GameDB::User::TableName() , name , strValue);
	if (oper.IsSuccess())
	{
		rpc_SyncDataToSlave("tcp://127.0.0.1:9001" , 0 , GetObjectID() , m_strDatabaseName , oper.GetOperateRecord().GetData());

		res = 0;
		gOtherStream("create user: name:" << name  << "success.");
	}
	 
	gDebugStream("HandleCreateUser_RpcServer :"<< res);
	Return(res);
}

