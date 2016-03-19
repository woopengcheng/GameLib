#ifndef __log_log4cplus_adaptee_h__
#define __log_log4cplus_adaptee_h__ 
#include "log4cplus/logger.h"
#include "LogLib/inc/LogAdapter.h"
 
namespace Log
{    
	/***********************************************************************
	（1）"%%"，转义为% 。
	（2）"%c"，输出logger名称，如test.subtest 。也可以控制logger名称的显示层次，比如"%c{1}"时输出"test"，其中数字表示层次。
	（3）"%D"，显示本地时间，比如："2004-10-16 18:55:45"，%d显示标准时间。     可以通过%d{...}定义更详细的显示格式，比如%d{%H:%M:%s}表示要显示小时:分钟：秒。大括号中可显示的
		预定义标识符如下：

		%a -- 表示礼拜几，英文缩写形式，比如"Fri"
		%A -- 表示礼拜几，比如"Friday"
		%b -- 表示几月份，英文缩写形式，比如"Oct"
		%B -- 表示几月份，"October"
		%c -- 标准的日期＋时间格式，如 "Sat Oct 16 18:56:19 2004"
		%d -- 表示今天是这个月的几号(1-31)"16"
		%H -- 表示当前时刻是几时(0-23)，如 "18"
		%I -- 表示当前时刻是几时(1-12)，如 "6"
		%j -- 表示今天是哪一天(1-366)，如 "290"
		%m -- 表示本月是哪一月(1-12)，如 "10"
		%M -- 表示当前时刻是哪一分钟(0-59)，如 "59"
		%p -- 表示现在是上午还是下午， AM or PM
		%q -- 表示当前时刻中毫秒部分(0-999)，如 "237"
		%Q -- 表示当前时刻中带小数的毫秒部分(0-999.999)，如 "430.732"
		%S -- 表示当前时刻的多少秒(0-59)，如 "32"
		%U -- 表示本周是今年的第几个礼拜，以周日为第一天开始计算(0-53)，如 "41"
		%w -- 表示礼拜几，(0-6, 礼拜天为0)，如 "6"
		%W -- 表示本周是今年的第几个礼拜，以周一为第一天开始计算(0-53)，如 "41"
		%x -- 标准的日期格式，如 "10/16/04"
		%X -- 标准的时间格式，如 "19:02:34"
		%y -- 两位数的年份(0-99)，如 "04"
		%Y -- 四位数的年份，如 "2004"
		%Z -- 时区名，比如 "GMT"

	（4）"%F"，输出当前记录器所在的文件名称，比如"main.cpp"
	（5）"%L"，输出当前记录器所在的文件行号，比如"51"
	（6）"%l"，输出当前记录器所在的文件名称和行号，比如"main.cpp:51"
	（7）"%m"，输出原始信息。
	（8）"%n"，换行符。
	（9）"%p"，输出LogLevel，比如"DEBUG"
	（10）"%t"，输出记录器所在的线程ID，比如 "1075298944"
	（11）"%x"，嵌套诊断上下文NDC (nested diagnostic context) 输出，从堆栈中弹出上下文信息，NDC可以用对不同源的log信息（同时地）交叉输出进行区分。
	（12）格式对齐，比如"%-10m"时表示左对齐，宽度是10，当然其它的控制字符也可以相同的方式来使用，比如"%-12d"，"%-5p"等等。
	************************************************************************/

	class DLL_EXPORT Log4CPlusAdaptee : public LogAdapter
	{
	public:
		Log4CPlusAdaptee() {}
		virtual ~Log4CPlusAdaptee(){} 

	public: 
		virtual  CErrno  Init(const char * pLogName , const char * pLayoutConfig = NULL , BOOL bMultCategory = TRUE) override; 
		virtual  CErrno  Cleanup() override;

	public:
		virtual  void    Debug( std_ostream & os ) override;
		virtual  void    Error( std_ostream & os) override;
		virtual  void    Warning( std_ostream & os) override;
		virtual  void    Trace( std_ostream & os) override;
		virtual  void    Other( std_ostream & os) override;

	public: 
		virtual  CErrno				CreateLog(const char * pName , const char * pLayoutConfig , const char * pFile);
//		virtual  log4cplus::Logger  GetLog(LOG_TYPE logType);  
	};

} 
#endif