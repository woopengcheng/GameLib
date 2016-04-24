#ifndef __client_client_orm_helper_h__
#define __client_client_orm_helper_h__
#include "GameDB/inc/OrmHelper.h"
#include "RPCCallFuncs.h"
#include "GameDB/inc/RemoteNodeDefine.h"

namespace Client
{
	struct STableKeyVal
	{
		std::string strTable;
		std::string strKey;
		std::string strVal;
	};
	typedef INT32(*OrmHandleHSetOW)(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & table, std_string & key, std_string & value, Msg::RpcCallbackPtr pCallback/* = NULL*/, UINT16 usPriority/* = 0*/, Msg::EMSG_SYNC_TYPE objSyncType/* = Msg::SYNC_TYPE_SYNC*/);
	typedef INT32(*OrmHandleHSet)(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & table, std_string & key, std_string & value, Msg::RpcCallbackPtr pCallback/* = NULL*/, UINT16 usPriority/* = 0*/, Msg::EMSG_SYNC_TYPE objSyncType /*= Msg::SYNC_TYPE_SYNC*/);
	typedef INT32(*OrmHandleHDel)(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & table, std_string & key, Msg::RpcCallbackPtr pCallback/* = NULL*/, UINT16 usPriority/* = 0*/, Msg::EMSG_SYNC_TYPE objSyncType/* = Msg::SYNC_TYPE_SYNC*/);
	typedef INT32(*OrmHandleHGet)(const std::string & pSessionName, Msg::Object objTarget, Msg::Object objSrc, std_string & table, std_string & key, Msg::RpcCallbackPtr pCallback/* = NULL*/, UINT16 usPriority/* = 0*/, Msg::EMSG_SYNC_TYPE objSyncType/* = Msg::SYNC_TYPE_SYNC*/);

	static CErrno OrmInsert(GameDB::Orm * obj, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		OrmHandleHSetOW pFunc = rpc_HandleHSetOW;
		return GameDB::OrmHelper::OrmInsert<>(obj, pFunc, g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], DBClient::GetInstance().GetServerID() ,  Msg::Object(0) , objMask);
	}
	static CErrno OrmUpdate(GameDB::Orm * obj, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		OrmHandleHSet pFunc = rpc_HandleHSet;
		return GameDB::OrmHelper::OrmUpdate<>(obj, pFunc, g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], DBClient::GetInstance().GetServerID(), Msg::Object(0), objMask);
	}
	static CErrno OrmDelete(GameDB::Orm * obj, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		OrmHandleHDel pFunc = rpc_HandleHDel;
		return GameDB::OrmHelper::OrmDelete<>(obj, pFunc, g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], DBClient::GetInstance().GetServerID(), Msg::Object(0), objMask);
	}
	static CErrno OrmQuery(GameDB::Orm * obj, Msg::RpcCallbackPtr pCallback = NULL, GameDB::EORM_MASK objMask = GameDB::ORM_NONE)
	{
		OrmHandleHGet pFunc = rpc_HandleHGet;
		return GameDB::OrmHelper::OrmQuery<>(obj, pFunc, g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], DBClient::GetInstance().GetServerID(), Msg::Object(0), pCallback , objMask);
	}

}


#endif