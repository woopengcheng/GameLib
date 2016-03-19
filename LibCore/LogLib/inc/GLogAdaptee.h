// #ifndef __log_glog_adaptee_h__
// #define __log_glog_adaptee_h__  
// #include "LogLib/inc/LogAdapter.h"
// #include "glog/logging.h"
// 
// namespace Log
// {    
// 	class DLL_EXPORT GLogAdaptee : public LogAdapter
// 	{
// 	public:
// 		GLogAdaptee() {}
// 		virtual ~GLogAdaptee(){} 
// 
// 	public: 
// 		virtual  CErrno  Init(const char * pLogName , const char * pLayoutConfig = NULL , BOOL bMultCategory = TRUE) ; 
// 		virtual  CErrno  Cleanup(); 
// 
// 	public:
// 		virtual  void    Debug( std_ostream & os ); 
// 		virtual  void    Error( std_ostream & os); 
// 		virtual  void    Warning( std_ostream & os); 
// 		virtual  void    Trace( std_ostream & os); 
// 		virtual  void    Other( std_ostream & os); 
// 
// 	public: 
// 		virtual  CErrno	 CreateLog( LOG_TYPE objLogType , const char * pLayoutConfig , const char * pFile);  
// 	};
// 
// } 
// #endif