#ifndef __log_log_common_h__
#define __log_log_common_h__
#include "CUtil/inc/Common.h"

namespace Log
{
#define MAX_FILE_SIZE  (1*1024*1024)

typedef std::basic_ostringstream<char> std_ostream;

	enum LOG_TYPE
	{
		LOG_DEBUG    = 0 ,
		LOG_ERROR    ,
		LOG_WARNING  ,
		LOG_TRACE    ,
		LOG_OTHERS   ,
		LOG_ALL_TYPE ,

		MAX_LOG_TYPE , 
	}; 
	extern std::string g_strLogTypeName[MAX_LOG_TYPE]; 
}

#endif