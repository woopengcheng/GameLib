#include "MysqlInterface.h"
#include "Log.h"

namespace Mysql
{


	MysqlInterface::MysqlInterface( void )
	{
		memset(&m_objMysql , 0 , sizeof(m_objMysql));
		mysql_init(&m_objMysql);

		char cOption = 1;
		mysql_options(&m_objMysql , MYSQL_OPT_RECONNECT , &cOption);
	}

	MysqlInterface::~MysqlInterface( void )
	{
		Cleanup();
	}

	INT32 MysqlInterface::Init( const char *host,const char *user,const char *passwd,const char *db,unsigned int port)
	{
		if (IsConnected())
		{
			return -1;
		}

		m_objMysql.reconnect = 1;
		if (!mysql_real_connect(&m_objMysql , host , user , passwd , db , port , NULL , CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS | CLIENT_TRANSACTIONS ))
		{
			LogMysqlError( "mysql_real_connect error." );
			Cleanup();
			return -2;
		}

		if (mysql_set_character_set(&m_objMysql , "GBK"))
		{
			mysql_close(&m_objMysql);
			Cleanup();
			return -3;
		}

		ExecuteSQL("SET NAMES GBK");

		return TRUE;
	}

	INT32 MysqlInterface::Cleanup( void )
	{
		if (IsConnected())
		{
			mysql_close(&m_objMysql);
		}
		memset(&m_objMysql , 0 , sizeof(m_objMysql));

		return TRUE;
	}

	BOOL MysqlInterface::IsConnected( void )
	{
		if (m_objMysql.net.fd != 0)
		{
			mysql_ping(&m_objMysql);
			return TRUE;
		}

		return FALSE;
	}

	INT64 MysqlInterface::ExecuteSQL( const char* sql )
	{
		if (!IsConnected())
		{
			return -1;
		}

		if (CleanResults())
		{
			return -2;
		}

		if (mysql_real_query(&m_objMysql , sql , strlen(sql)))
		{
			LogMysqlError(sql);
			return -3;
		}

		return m_objMysql.affected_rows;
	}

	INT32 MysqlInterface::CleanResults( void )
	{
		while(mysql_next_result(&m_objMysql) == 0)
		{
			MYSQL_RES * pRes = mysql_store_result(&m_objMysql);
			if (pRes)
			{
				mysql_free_result(pRes);
			}
		}

		return TRUE;
	}

	INT32 MysqlInterface::LogMysqlError( const char * pError )
	{
		gErrorStream("Mysql error:" << pError << "\n" << mysql_error(&m_objMysql));

		return 0;
	}

}