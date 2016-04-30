#ifndef __cutil_condition_h__
#define __cutil_condition_h__
#include "CUtil/inc/Common.h"

namespace CUtil
{
	enum EConditionType
	{
		PLAYER_LEVEL,
		VIP_LEVEL,
		LEAGUE_LEVEL,
		pow,
		TEST_MULIT_ARGS,
	};

	class League
	{
	public:
		League() {}
		~League() {}
	};

	class Team
	{
	public:
		Team() {}
		~Team() {}
	};

	class Player
	{
	public:
		Player(){}
		~Player() {}

		INT32 GetPlayerLevel()
		{
			return nPlayerLevel;
		}
		void Mail(League * pLeague, Team * pTeam , INT32 nMailID) { }
		void Say(INT32 nChanelID , std::string str) { }

	private:
		int nPlayerLevel;
	};

	template<EConditionType type>
	class Condition;

	template<>
	class Condition<PLAYER_LEVEL>
	{
	public:
		INT32 operator ()(Player * pPlayer , League * pLeague)
		{
			if (pPlayer)
			{
				return pPlayer->GetPlayerLevel();
			}
			return -1;
		}
	};
	template<>
	class Condition<pow>
	{
	public:
		double operator ()(double x , double y)
		{
			return ::pow(x, y);
		}
	};

	template<>
	class Condition<VIP_LEVEL>
	{
	public:
		INT32 operator ()(Player * pPlayer)
		{
			if (pPlayer)
			{
				return pPlayer->GetPlayerLevel();
			}
			return -1;
		}
	};

	template<>
	class Condition<TEST_MULIT_ARGS>
	{
	public:
		INT32 operator ()(Player * pPlayer, bool b1, INT32 n1 , INT64 n2 , double f2 , std::string str , Timer::Date date)
		{
			if (pPlayer)
			{
				return pPlayer->GetPlayerLevel();
			}
			return -1;
		}
	};
	enum EActionType
	{
		ACTION_TRIGER_TASK,
		ACTION_MAIL,
		ACTION_SAY,
		ACTION_CHANGE_VALUE,
		ACTION_MIN_VALUE,
		ACTION_ADD_VALUE,
	};
	template<EActionType type>
	class Action;

	template<>
	class Action<ACTION_TRIGER_TASK>
	{
	public:
		INT32 operator ()(Player * pPlayer , INT32 nTaskID)
		{
			if (pPlayer)
			{
				return pPlayer->GetPlayerLevel();
			}
			return -1;
		}
	};

	template<>
	class Action<ACTION_MAIL>
	{
	public:
		INT32 operator ()(Player * pPlayer , INT32 nMailID)
		{
			if (pPlayer)
			{
				return pPlayer->GetPlayerLevel();
			}
			return -1;
		}
	};

	template<>
	class Action<ACTION_CHANGE_VALUE>
	{
	public:
		INT32 operator ()(double dValue)
		{
			if (pPlayer)
			{
				return pPlayer->GetPlayerLevel();
			}
			return -1;
		}
	};
}

#endif

