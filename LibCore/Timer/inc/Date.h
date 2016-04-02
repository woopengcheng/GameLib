#ifndef __timer_date_h__
#define __timer_date_h__ 
#include "Timer/inc/TimerCommon.h"

namespace Timer
{
	enum EDateType
	{
		DATE_TYPE_INVALID = 0,
		DATE_TYPE_YEAR,
		DATE_TYPE_MON,
		DATE_TYPE_DAY,
		DATE_TYPE_HOUR,
		DATE_TYPE_MIN,
		DATE_TYPE_SEC,

		DATE_TYPE_MAX,
	};
	extern std::string g_strDateTypeMakup[DATE_TYPE_MAX];

	class Date
	{
	public:
		explicit Date(std::string strDate = "", EDateType type = DATE_TYPE_INVALID);
		~Date()
		{
		} 

		std::string			GetDateStr() const { return m_strDate; }
		INT64				GetDateInt();
		void				SetDate(std::string val) { m_strDate = val; }
		Timer::EDateType	GetDateType() const { return m_eDateType; }
		void				SetDateType(Timer::EDateType val) { m_eDateType = val; }

	public:
		bool				operator == (const Date & date)
		{
			if (date.GetDateType() == m_eDateType && date.GetDateStr() == m_strDate)
			{
				return true;
			}

			return false;
		}

	private:
		EDateType			m_eDateType;
		std::string			m_strDate;
	};

}
#endif