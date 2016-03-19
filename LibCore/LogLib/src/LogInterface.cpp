#include "LogLib/inc/LogInterface.h" 

#ifdef USE_LOG4CPP
#include "LogLib/inc/Log4CppAdaptee.h"
#elif defined USE_LOG4CPLUS
#include "LogLib/inc/Log4CPlusAdaptee.h"
#elif defined USE_GLOG
#include "LogLib/inc/GLogAdaptee.h"
#endif

namespace Log
{   
	std::string g_strLogTypeName[MAX_LOG_TYPE] = {"debug" , "error" , "warning" , "trace" , "others" , "all"};

	LogInterface::LogInterface()
	{
#ifdef USE_LOG4CPP
		m_pLogAdapter = new Log4CppAdaptee; 
		std::cout << "USE_LOG4CPP"<<std::endl;
#elif defined USE_LOG4CPLUS
		m_pLogAdapter = new Log4CPlusAdaptee;
		std::cout << "USE_LOG4CPLUS"<<std::endl;
#elif defined USE_GLOG
		m_pLogAdapter = new GLogAdaptee;
		std::cout << "USE_GLOG"<<std::endl;
#endif

	}

	CErrno LogInterface::Init( const char * pLogName , const char * pLayoutConfig /*= NULL */, BOOL bMultCategory /*= TRUE*/ )
	{
		if (m_pLogAdapter)
		{
			return m_pLogAdapter->Init(pLogName , pLayoutConfig , bMultCategory);
		}
		return CErrno::Failure();
	}

	CErrno LogInterface::Cleanup()
	{
		if (m_pLogAdapter)
		{ 
			m_pLogAdapter->Cleanup();
			SAFE_DELETE(m_pLogAdapter);
			return CErrno::Success();
		}
		return  CErrno::Failure();
	} 

	LogInterface & LogInterface::GetInstance() 
	{
		static LogInterface sLogInterface;
		return sLogInterface; 
	}

	CErrno  Init(const char * pLogName , const char * pLayoutConfig  , BOOL bMultCategory ) 
	{ 
		return LogInterface::GetInstance().Init(pLogName, pLayoutConfig, bMultCategory);
	}

	CErrno  Cleanup()
	{
		return Log::LogInterface::GetInstance().Cleanup(); 
	}
}