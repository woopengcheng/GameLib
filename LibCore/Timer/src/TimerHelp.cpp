#include "Timer/inc/TimerHelp.h"
#include "Timer/inc/Date.h"
#include "LogLib/inc/Log.h"
#include "CUtil/inc/CUtil.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace Timer
{  
#ifdef WIN32
	UINT64 GetTickCount()
	{  
		UINT64  llCount = 0;
		QueryPerformanceCounter((LARGE_INTEGER *)(&llCount));

		return llCount;
	}

	UINT64 GetTickFrequency()
	{
		static UINT64  s_llFrequency = 0;
		if (!s_llFrequency)
		{
			QueryPerformanceFrequency((LARGE_INTEGER *)(&s_llFrequency));
		}

		return s_llFrequency;
	} 
#else
	UINT64 GetTickCount()
	{ 
		timespec t;
		::clock_gettime(CLOCK_MONOTONIC,&t);              //5 CLOCK_REALTIME不同.这个是过去的某个逝去的时间点的次数.
		return (UINT64)t.tv_sec * TIME_MICRO_PRECISE + t.tv_nsec / (1e9 / TIME_MICRO_PRECISE);   //5 微妙级别
	}

	UINT64 GetTickFrequency()
	{  
		return TIME_MICRO_PRECISE;
	}  
#endif

	UINT64 GetTickSecond(UINT64 llTime /*= -1*/ )
	{
		if (llTime == -1)
		{
			return  GetTickCount() / GetTickFrequency(); 
		} 
		else
		{
			return  llTime / GetTickFrequency(); 
		}
	}

	UINT64 GetTickMicroSecond(UINT64 llTime /*= -1*/ )
	{
		if (llTime == -1)
		{
			return  GetTickCount() * 1000  / GetTickFrequency(); 
		} 
		else
		{
			return  llTime * 1000  / GetTickFrequency(); 
		} 
	}

	UINT64 GetTickMilliSecond(UINT64 llTime /*= -1*/ )
	{
		if (llTime == -1)
		{
			return  GetTickCount() * 1000000 / GetTickFrequency(); 
		} 
		else
		{
			return  llTime * 1000000 / GetTickFrequency(); 
		} 
	}

	UINT64 GetMilliSecond(UINT64 llTime )
	{
		return llTime * GetTickFrequency() / 1000000;

	}

	UINT64 GetMicroSecond(UINT64 llTime )
	{
		return llTime * GetTickFrequency() / 1000;
	}

	UINT64 GetSecond(UINT64 llTime )
	{
		return llTime * GetTickFrequency();
	}

	UINT64 DiffMilliSecond(UINT64 llTime1 , UINT64 llTime2 )
	{
		return ::abs((long long)(llTime1 - llTime2)) * 1000000 / GetTickFrequency(); 
	}

	UINT64 DiffMicroSecond(UINT64 llTime1 , UINT64 llTime2 )
	{
		return ::abs((long long)(llTime1 - llTime2)) * 1000 / GetTickFrequency();

	}

	UINT64 DiffSecond(UINT64 llTime1 , UINT64 llTime2 )
	{
		return ::abs((long long)(llTime1 - llTime2)) / GetTickFrequency();
	}

	std::string GetDate(std::string strFormat/* = "%Y-%m-%d"*/)
	{
		time_t t = time(0); 
		char tmp[64]; 
		strftime( tmp, sizeof(tmp), strFormat.c_str(),localtime(&t) );  
		return std::string(tmp); 
	}

	static INT32 strptime_getnum(const char *buf,int *num,int *bi,int *fi,int min,int max)
	{
		int i = 0, tmp = 0;

		while (isdigit(buf[i]))
		{
			tmp = (tmp * 10) + (buf[i] - '0');
			if (max && (tmp > max))
				return 0;
			++i;
		}

		if (tmp < min)
			return 0;
		else if (i)
		{
			*num = tmp;
			(*fi)++;
			*bi += i;
			return 1;
		}
		else
			return 0;
	}

	static char * StrMakeTime(const char *buf, const char *format, struct tm *tm)
	{
		int bi = 0;
		int fi = 0;
		bool percent = false;

		if (!buf || !format)
			return 0;

		while (format[fi] != '\0')
		{
			if (percent)
			{
				percent = false;
				switch (format[fi])
				{
					case '%':                        // an escaped %
						if (buf[bi] == '%')
						{
							++fi;
							++bi;
						}
						else
							return const_cast<char*> (buf + bi);
						break;

						/* not supported yet: weekday via locale long/short names
						case 'a':                        / * weekday via locale * /
						/ * FALL THROUGH * /
						case 'A':                        / * long/short names * /
						break;
						*/

						/* not supported yet:
						case 'b':                        / * month via locale * /
						/ * FALL THROUGH * /
						case 'B':                        / * long/short names * /
						/ * FALL THROUGH * /
						case 'h':
						break;
						*/

						/* not supported yet:
						case 'c':                        / * %x %X * /
						break;
						*/

						/* not supported yet:
						case 'C':                        / * date & time -  * /
						/ * locale long format * /
						break;
						*/

					case 'd':                        /* day of month (1-31) */
													 /* FALL THROUGH */
					case 'e':
						if (!strptime_getnum
							(buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
							return const_cast<char*> (buf + bi);

						break;

					case 'D':                        /* %m/%d/%y */
						if (!strptime_getnum
							(buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
							return const_cast<char*> (buf + bi);

						--fi;
						tm->tm_mon--;

						if (buf[bi] != '/')
							return const_cast<char*> (buf + bi);

						++bi;

						if (!strptime_getnum
							(buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
							return const_cast<char*> (buf + bi);

						--fi;
						if (buf[bi] != '/')
							return const_cast<char*> (buf + bi);
						++bi;
						if (!strptime_getnum
							(buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
							return const_cast<char*> (buf + bi);
						if (tm->tm_year < 69)
							tm->tm_year += 100;
						break;

					case 'H':                        /* hour (0-23) */
													 /* FALL THROUGH */
					case 'k':
						if (!strptime_getnum
							(buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
							return const_cast<char*> (buf + bi);
						break;

						/* not supported yet:
						case 'I':                        / * hour (0-12) * /
						/ * FALL THROUGH * /
						case 'l':
						break;
						*/

					case 'j':                        /* day of year (0-366) */
						if (!strptime_getnum
							(buf + bi, &tm->tm_yday, &bi, &fi, 1, 366))
							return const_cast<char*> (buf + bi);

						tm->tm_yday--;
						break;

					case 'm':                        /* an escaped % */
						if (!strptime_getnum
							(buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
							return const_cast<char*> (buf + bi);

						tm->tm_mon--;
						break;

					case 'M':                        /* minute (0-59) */
						if (!strptime_getnum
							(buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
							return const_cast<char*> (buf + bi);

						break;

						/* not supported yet:
						case 'p':                        / * am or pm for locale * /
						break;
						*/

						/* not supported yet:
						case 'r':                        / * %I:%M:%S %p * /
						break;
						*/

					case 'R':                        /* %H:%M */
						if (!strptime_getnum
							(buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
							return const_cast<char*> (buf + bi);

						--fi;
						if (buf[bi] != ':')
							return const_cast<char*> (buf + bi);
						++bi;
						if (!strptime_getnum
							(buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
							return const_cast<char*> (buf + bi);

						break;

					case 'S':                        /* seconds (0-61) */
						if (!strptime_getnum
							(buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
							return const_cast<char*> (buf + bi);
						break;

					case 'T':                        /* %H:%M:%S */
						if (!strptime_getnum
							(buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
							return const_cast<char*> (buf + bi);

						--fi;
						if (buf[bi] != ':')
							return const_cast<char*> (buf + bi);
						++bi;
						if (!strptime_getnum
							(buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
							return const_cast<char*> (buf + bi);

						--fi;
						if (buf[bi] != ':')
							return const_cast<char*> (buf + bi);
						++bi;
						if (!strptime_getnum
							(buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
							return const_cast<char*> (buf + bi);

						break;

					case 'w':                        /* day of week (0=Sun-6) */
						if (!strptime_getnum
							(buf + bi, &tm->tm_wday, &bi, &fi, 0, 6))
							return const_cast<char*> (buf + bi);

						break;

						/* not supported yet: date, based on locale
						case 'x':                        / * date, based on locale * /
						break;
						*/

						/* not supported yet:
						case 'X':                        / * time, based on locale * /
						break;
						*/

					case 'y':                        /* the year - 1900 (0-99) */
						if (!strptime_getnum
							(buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
							return const_cast<char*> (buf + bi);

						if (tm->tm_year < 69)
							tm->tm_year += 100;
						break;

					case 'Y':                        /* the full year (1999) */
						if (!strptime_getnum
							(buf + bi, &tm->tm_year, &bi, &fi, 0, 0))
							return const_cast<char*> (buf + bi);

						tm->tm_year -= 1900;
						break;

					default:                        /* unrecognised */
						return const_cast<char*> (buf + bi);
				} /* switch (format[fi]) */

			}
			else
			{ /* if (percent) */
				if (format[fi] == '%')
				{
					percent = true;
					++fi;
				}
				else
				{
					if (format[fi] == buf[bi])
					{
						++fi;
						++bi;
					}
					else
						return const_cast<char*> (buf + bi);
				}
			} /* if (percent) */
		} /* while (format[fi] */

		return const_cast<char*> (buf + bi);
	}

	INT64 MakeTime(const char* text, const char* fmt /*= "%Y-%m-%d-%H:%M:%S"*/)
	{
		struct tm t = { 0 };
		StrMakeTime(text, fmt, &t);
		return mktime(&t);
	}

	time_t GetTime()
	{
		return time(0);
	}

	void sleep(UINT64 llMillSec)
	{
#ifdef WIN32
		::Sleep((DWORD)llMillSec);
#else  
		struct timespec objTimeSpec;
		objTimeSpec.tv_sec = llMillSec / 1000;
		objTimeSpec.tv_nsec = (llMillSec % 1000) * 1000000L;
		::nanosleep (&objTimeSpec, 0);
#endif
	}
	std::string g_strDateTypeMakup[DATE_TYPE_MAX] =
	{
		{"error"},
		{ "" },
		{ "%Y-" },
		{ "%Y-%m-" },
		{ "%Y-%m-%d-" },
		{ "%Y-%m-%d-%H:" },
		{ "%Y-%m-%d-%H:%M:" },
	};

	Date::Date(std::string strDate, EDateType type /*= DATE_TYPE_INVALID*/)
		: m_eDateType(type)
		, m_strDate(strDate)
	{
		if (type == DATE_TYPE_INVALID)
		{
			std::vector<std::string> vals;
			std::string __tmp = m_strDate;
			CUtil::tokenize(__tmp, vals, "-", "", "\"");
			if (vals.size() == 4)
			{
				m_eDateType = DATE_TYPE_YEAR;
			}
			else if (vals.size() == 3)
			{
				m_eDateType = DATE_TYPE_MON;
			}
			else if (vals.size() == 2)
			{
				m_eDateType = DATE_TYPE_DAY;
			}
			else if (vals.size() == 1)
			{
				__tmp = vals[0];
				vals.clear();
				CUtil::tokenize(__tmp, vals, ":", "", "\"");
				if (vals.size() == 3)
				{
					m_eDateType = DATE_TYPE_HOUR;
				}
				else if (vals.size() == 2)
				{
					m_eDateType = DATE_TYPE_MIN;
				}
				else if (vals.size() == 1)
				{
					m_eDateType = DATE_TYPE_SEC;
				}
			}
		}
	}
	INT64 Date::GetDateInt()
	{
		std::string strDate = m_strDate;
		switch (m_eDateType)
		{
			case DATE_TYPE_INVALID:
			{
				gErrorStream("GetDateInt type error");
			}break;
			case DATE_TYPE_YEAR:
			case DATE_TYPE_MON:
			case DATE_TYPE_DAY:
			case DATE_TYPE_HOUR:
			case DATE_TYPE_MIN:
			case DATE_TYPE_SEC:
			{
				strDate = Timer::GetDate(g_strDateTypeMakup[m_eDateType]) + strDate;
			}break;
			default:
				break;
		}
		return MakeTime(strDate.c_str());
	}

}