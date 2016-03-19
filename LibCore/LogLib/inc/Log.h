#ifndef __log_h__
#define __log_h__ 
#include "LogLib/inc/LogInterface.h"

#define gDebugStream(LOG_STREAM)  { Log::std_ostream out;out<<LOG_STREAM; Log::LogInterface::GetInstance().GetLogAdapter()->Debug(out);}
#define gErrorStream(LOG_STREAM)  { Log::std_ostream out;out<<LOG_STREAM; Log::LogInterface::GetInstance().GetLogAdapter()->Error(out);}
#define gWarniStream(LOG_STREAM)  { Log::std_ostream out;out<<LOG_STREAM; Log::LogInterface::GetInstance().GetLogAdapter()->Warning(out);}
#define gTraceStream(LOG_STREAM)  { Log::std_ostream out;out<<LOG_STREAM; Log::LogInterface::GetInstance().GetLogAdapter()->Trace(out);}
#define gOtherStream(LOG_STREAM)  { Log::std_ostream out;out<<LOG_STREAM; Log::LogInterface::GetInstance().GetLogAdapter()->Other(out);}


#endif
