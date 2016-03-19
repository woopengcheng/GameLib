#ifndef __client_client_commands_h__ 
#define __client_client_commands_h__
#include "GameDB/inc/DBCommon.h"
#include "DBClient.h"

namespace Client
{
	class ClientCommands;
//	typedef void (* pfnClientCommand)(DBClient * pClient , INT32 argc , char ** argv);
	typedef void (* pfnClientCommand)(DBClient * pClient , std::vector<std::string> & objParams);
	
	class ClientCommands
	{
	public:
		typedef std_unordered_map<std::string , pfnClientCommand> CollectionClientCommandsT;

	public:
		ClientCommands(); 

	public:
		void InitCommands(); 
		void Execute(DBClient * pClient , std::vector<std::string> & objParams);
		std::string GetNearestCommand(const std::string & strCommand , INT32 nCount) const;
		CollectionClientCommandsT & GetClientCommand() { return m_mapCommands; }

	public:
		//5 本地操作
		static void  pfnHandleHelp(DBClient * pClient ,  std::vector<std::string> & objParams);

		//5 数据库相关操作
		static void  pfnHandleSelectDatabase(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleCreateDatabase(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleDeleteDatabase(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleShowDatabases(DBClient * pClient ,  std::vector<std::string> & objParams);

		//5 添加,删除,修改用户权限 
		static void  pfnHandleShowUsers(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleCreateUser(DBClient * pClient ,  std::vector<std::string> & objParams); 
		static void  pfnHandleDeleteUser(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleModifyUser(DBClient * pClient ,  std::vector<std::string> & objParams);

		//5 数据库数据修改
		static void  pfnHandleHSet(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHGet(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHSetNX(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHSetOW(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHDel(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHMultiSet(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHMultiGet(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHMultiDel(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHSetIncr(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHSetIncrFloat(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHGetKeys(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHGetVals(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHKeyVals(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHScan(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHCount(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHDrop(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleHList(DBClient * pClient ,  std::vector<std::string> & objParams);  //5 比HGetKeys性能更好.

		static void  pfnHandleZSet(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleZDel(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleZGet(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleZTop(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleZRTop(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleZDrop(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleZCount(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleZList(DBClient * pClient ,  std::vector<std::string> & objParams);

		static void  pfnHandleDump(DBClient * pClient ,  std::vector<std::string> & objParams);

		//5 Orm测试.
		static void  pfnHandleOrmHSet(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleOrmCollectInsert(DBClient * pClient ,  std::vector<std::string> & objParams);
		static void  pfnHandleOrmCollection(DBClient * pClient ,  std::vector<std::string> & objParams);

	private:
		CollectionClientCommandsT m_mapCommands;
	};

	extern ClientCommands * g_pClientCommands;
}

#endif
