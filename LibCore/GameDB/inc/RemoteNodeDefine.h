#ifndef __remote_nodes_0xfffffffff_h__
#define __remote_nodes_0xfffffffff_h__
#include <string>

// ------------------------------------------------

enum ENET_NODES
{
	NETNODE_INVALID,
	NETNODE_DBCLIENT_TO_DBSERVER,
	NETNODE_DBSERVER_TO_DBCLIENT,
	NETNODE_DBSLAVE_TO_DBMASTER,
	NETNODE_DBMASTER_TO_DBSLAVE,
	NETNODE_DBMASTER_TO_DBSERVER,
	NETNODE_DBSERVER_TO_DBMASTER,
	NETNODE_DBMASTER_TO_SYSDB,
	NETNODE_DBMASTER_TO_TEST,
	NETNODE_TEST_TO_DBMASTER,
	NETNODE_SYSDB_TO_DBMASTER,

	NETNODE_NUM,
};

//5 这里每个文件都有一份.网络类型节点.需要先宏定义,然后和json中的net_node_name相对应,不然可能不会生效
//5 node节点只有一一对应的功能,不存在一对多和多对多.这个相当于取巧.一个简便方法.
 static std::string g_strGameDBNodes[NETNODE_NUM] = 
{
	"" ,									//5 NETNODE_INVALID
	"dbclient_To_dbserver",					//5 NETNODE_DBCLIENT_TO_DBSERVER
	"dbserver_To_dbclient",					//5 NETNODE_DBSERVER_TO_DBCLIENT
	"dbslave_To_dbmaster" ,					//5 NETNODE_DBSLAVE_TO_DBMASTER  
	"dbmaster_To_dbslave" ,					//5 NETNODE_DBMASTER_TO_DBSLAVE  
	"dbmaster_To_dbserver",					//5 NETNODE_DBMASTER_TO_DBSERVER
	"dbserver_To_dbmaster",					//5 NETNODE_DBSERVER_TO_DBMASTER
	"dbmaster_To_.sys" ,					//5 NETNODE_DBMASTER_TO_SYSDB  
	"dbmaster_To_testdb" ,					//5 NETNODE_DBMASTER_TO_TEST
	"testdb_To_dbmaster" ,					//5 NETNODE_TEST_TO_DBMASTER
	".sys_To_dbmaster" ,					//5 NETNODE_SYSDB_TO_DBMASTER
};

#endif
