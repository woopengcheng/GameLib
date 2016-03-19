#include "Timer/inc/TimerHelp.h"
#include "LogLib/inc/Log4CPlusAdaptee.h" 

#ifdef WIN32
#include <direct.h>
#else
#endif 

#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/ndc.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/loggingmacros.h>
#include "log4cplus/consoleappender.h"
#include "log4cplus/tstring.h"
#include "log4cplus/loggingmacros.h" 

namespace Log
{   

	CErrno Log4CPlusAdaptee::Init( const char * pLogName , const char * pLayoutConfig , BOOL bMultCategory /* = FALSE*/)
	{
		log4cplus::initialize();    //5 log4cplus 初始化
		log4cplus::helpers::LogLog::getLogLog()->setInternalDebugging(false);

		std::string strFileName = "";

		int nResult = 0;
#ifdef WIN32
		nResult = _mkdir("./logs");
#else
		nResult = mkdir("./logs",S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
#endif 
		strFileName = "./logs/"; 
		strFileName += pLogName;
		strFileName += "_";
		 
		strFileName += Timer::GetDate();
		 
		m_bMultCategory = bMultCategory;
		if (bMultCategory)
		{	
			CreateLog(g_strLogTypeName[LOG_ALL_TYPE].c_str() , pLayoutConfig , (strFileName + "_" + g_strLogTypeName[LOG_ALL_TYPE] + ".log").c_str()); 
		}
		else
		{
			for (int i = 0;i < MAX_LOG_TYPE - 1;++i)
			{
				CreateLog(g_strLogTypeName[i].c_str() , pLayoutConfig , (strFileName + "_" + g_strLogTypeName[i] + ".log").c_str()); 
			}
		}
		return CErrno::Success();
	}

	CErrno Log4CPlusAdaptee::CreateLog( const char * pName , const char * pLayoutConfig , const char * pFile )
	{
		//5 1.创建layout,这里是先定义Layout的格式.
		log4cplus::tstring pattern = LOG4CPLUS_TEXT("");
		if (!pLayoutConfig)
		{
			//
			//5 "%d{%m/%d/%y %H:%M:%S,%Q} [%t] %-5p %c{2} %%%x%% - %X{key} - %m [%l]%n"
			pattern = LOG4CPLUS_TEXT("[%-5p %d{%y/%m/%d %H:%M:%S}] %m %n");
		} 

		//5 2.创建接纳Layout的appender.这个是用来表示是用来什么方式进行存储.可以流式.sys服务.远程服务.文件等等.
#ifdef _DEBUG
		log4cplus::SharedAppenderPtr pAppender(new log4cplus::ConsoleAppender()); 
		pAppender->setName(LOG4CPLUS_TEXT("log4cplus console appender"));
#else
		log4cplus::SharedAppenderPtr pAppender(new log4cplus::FileAppender(LOG4CPLUS_TEXT("log4cplus file appender")));
#endif
		pAppender->setLayout(std::auto_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)));

		//5 3.创建Categteoy策略来装Appender.
		log4cplus::Logger objLogger = log4cplus::Logger::getInstance(log4cplus::helpers::towstring(pName));

		objLogger.setAdditivity(false);
		objLogger.addAppender(pAppender);
		objLogger.setLogLevel(log4cplus::ALL_LOG_LEVEL);

		return CErrno::Success();
	} 

	CErrno Log4CPlusAdaptee::Cleanup()
	{
		log4cplus::Logger::shutdown(); 

		return CErrno::Success();
	}
	  
	void Log4CPlusAdaptee::Debug( std_ostream & os )
	{  
		LOG4CPLUS_DEBUG(log4cplus::Logger::getInstance(log4cplus::helpers::towstring(g_strLogTypeName[LOG_DEBUG].c_str())) , os.str().c_str()); 
	}

	void Log4CPlusAdaptee::Error( std_ostream & os )
	{
		LOG4CPLUS_ERROR(log4cplus::Logger::getInstance(log4cplus::helpers::towstring(g_strLogTypeName[LOG_ERROR].c_str())) , os.str().c_str());  
	}

	void Log4CPlusAdaptee::Warning( std_ostream & os )
	{
		LOG4CPLUS_WARN(log4cplus::Logger::getInstance(log4cplus::helpers::towstring(g_strLogTypeName[LOG_WARNING].c_str())) , os.str().c_str());  
	}

	void Log4CPlusAdaptee::Trace( std_ostream & os )
	{
		LOG4CPLUS_INFO(log4cplus::Logger::getInstance(log4cplus::helpers::towstring(g_strLogTypeName[LOG_TRACE].c_str())) , os.str().c_str());  
	}

	void Log4CPlusAdaptee::Other( std_ostream & os )
	{
		LOG4CPLUS_INFO(log4cplus::Logger::getInstance(log4cplus::helpers::towstring(g_strLogTypeName[LOG_OTHERS].c_str())) , os.str().c_str());  
	} 
}