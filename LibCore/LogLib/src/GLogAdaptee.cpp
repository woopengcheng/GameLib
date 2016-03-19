// #include "LogLib/inc/GLogAdaptee.h"
// #include "Timer/inc/TimerHelp.h"
// #include "glog/raw_logging.h"
// #include "glog/logging.h"
// 
// namespace Log
// { 
// 	CErrno GLogAdaptee::Init( const char * pLogName , const char * pLayoutConfig /*= NULL */, BOOL bMultCategory /*= TRUE*/ )
// 	{
// 		google::InitGoogleLogging("./"); 
//  		google::SetStderrLogging(0); //5 设置级别高于INFO的输出到屏幕
// 		FLAGS_logbufsecs = 0;                   //5 缓冲日志输出，默认为30秒，此处改为立即输出
// 		FLAGS_max_log_size = 1024;              //5 最大日志大小为 100MB
// 		FLAGS_stop_logging_if_full_disk = true; //5 当磁盘被写满时，停止日志输出
// 		FLAGS_colorlogtostderr = true;          //5 设置输出到屏幕的日志显示相应颜色
//  		FLAGS_v = 3;                         //5 自定义日志输出
// 
// 		std::string strFileName = "";
// 
// 		int nResult = 0;
// #ifdef WIN32
// 		nResult = _mkdir("./logs");
// #else
// 		nResult = mkdir("./logs",S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
// #endif 
// 		strFileName = "./logs/"; 
// 		strFileName += pLogName;
// 		strFileName += "_";
// 
// 		strFileName += Timer::GetDate();
// 
// 		m_bMultCategory = bMultCategory;
// 		if (bMultCategory)
// 		{	
// 			CreateLog(LOG_ALL_TYPE , pLayoutConfig , (strFileName + "_" + g_strLogTypeName[LOG_ALL_TYPE] + ".log").c_str()); 
// 		}
// 		else
// 		{
// 			for (int i = 0;i < MAX_LOG_TYPE - 1;++i)
// 			{
// 				CreateLog((LOG_TYPE)i , pLayoutConfig , (strFileName + "_" + g_strLogTypeName[i] + ".log").c_str()); 
// 			}
// 		}  
// 		return CErrno::Success();
// 	}
// 
// 	CErrno GLogAdaptee::Cleanup()
// 	{
// 		return CErrno::Success();
// 	}
// 
// 	void GLogAdaptee::Debug( std_ostream & os )
// 	{    
// 		RAW_VLOG(LOG_DEBUG , os.str().c_str()); 
// 	}
// 
// 	void GLogAdaptee::Error( std_ostream & os )
// 	{  
// 		RAW_VLOG(LOG_ERROR , os.str().c_str()); 
// 	}
// 
// 	void GLogAdaptee::Warning( std_ostream & os )
// 	{ 
// 		RAW_VLOG(LOG_WARNING , os.str().c_str()); 
// 	}
// 
// 	void GLogAdaptee::Trace( std_ostream & os )
// 	{
// 		RAW_VLOG(LOG_TRACE , os.str().c_str()); 
// 	}
// 
// 	void GLogAdaptee::Other( std_ostream & os )
// 	{
// //		RAW_VLOG(LOG_OTHERS , os.str().c_str());
// 	}
// 
// 	CErrno GLogAdaptee::CreateLog( LOG_TYPE objLogType , const char * pLayoutConfig , const char * pFile )
// 	{ 
// 		switch(objLogType)
// 		{
// 			case LOG_ALL_TYPE:
// 				{ 
// 					google::SetLogDestination(LOG_ERROR, pFile);
// 					google::SetLogDestination(LOG_WARNING, pFile);
// 					google::SetLogDestination(LOG_DEBUG, pFile);
// 					google::SetLogDestination(LOG_TRACE, pFile); 
// 					google::SetLogDestination(LOG_OTHERS, pFile);
// 				}break;
// 			case LOG_DEBUG:
// 				google::SetLogDestination(LOG_DEBUG, pFile);
// 				break; 
// 			case LOG_ERROR:
// 				google::SetLogDestination(LOG_ERROR, pFile);
// 				break; 
// 			case LOG_WARNING:
// 				google::SetLogDestination(LOG_WARNING, pFile);
// 				break; 
// 			case LOG_TRACE:
// 				google::SetLogDestination(LOG_TRACE, pFile); 
// 				break;
// 			case LOG_OTHERS:
// //				google::SetLogDestination(LOG_OTHERS, pFile);
// 				break;
// 			default:
// 				break; 
// 		} 
// 
// 		return CErrno::Success();
// 	}
// 
// }