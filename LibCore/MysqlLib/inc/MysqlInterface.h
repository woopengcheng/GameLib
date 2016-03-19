#ifndef __mysql_mysql_interface_h__
#define __mysql_mysql_interface_h__
#include "mysql.h"
#include "MysqlCommon.h"

namespace Mysql
{
	class MysqlInterface
	{
	public:
		MysqlInterface( void );
		virtual ~MysqlInterface( void );
	
	public:
		virtual INT32 Init( const char *host,const char *user,const char *passwd,const char *db,unsigned int port );
		virtual INT32 Cleanup( void );

	public:
		INT64 ExecuteSQL(const char* sql);

	protected:
		INT32  CleanResults(void);
		BOOL   IsConnected(void);
		INT32  LogMysqlError(const char * pError);

	protected:
		MYSQL m_objMysql;
	}; 
}


#endif