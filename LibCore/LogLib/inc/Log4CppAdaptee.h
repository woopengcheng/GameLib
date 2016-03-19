// #ifndef __log_log4cpp_interface_h__
// #define __log_log4cpp_interface_h__ 
// #include "LogLib/inc/LogCommon.h"    
// #include "log4cpp/CategoryStream.hh"
// #include "LogLib/inc/LogAdapter.h"
//  
// namespace Log
// {  
// 	class DLL_EXPORT  Log4CppAdaptee : public LogAdapter
// 	{
// 	public:
// 		Log4CppAdaptee() {}
// 		virtual ~Log4CppAdaptee(){} 
// 			
// 	public: 
// 		virtual  CErrno  Init(const char * pLogName , const char * pLayoutConfig = NULL , BOOL bMultCategory = TRUE) ; 
// 		virtual  CErrno  Cleanup( void ); 
// 
// 	public:
// 		virtual  void    Debug( std_ostream & os); 
// 		virtual  void    Error( std_ostream & os); 
// 		virtual  void    Warning( std_ostream & os); 
// 		virtual  void    Trace( std_ostream & os); 
// 		virtual  void    Other( std_ostream & os); 
// 
// 	private: 
// 		virtual  CErrno CreateLog(const char * pName , const char * pLayoutConfig ,const char * pFile); 
// 	};
// } 
// 
// #endif