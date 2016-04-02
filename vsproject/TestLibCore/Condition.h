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
		void Mail(INT32 nMailID) { }
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

	enum EActionType
	{
		ACTION_TRIGER_TASK,
		ACTION_MAIL,
		ACTION_SAY,
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
}

#endif

