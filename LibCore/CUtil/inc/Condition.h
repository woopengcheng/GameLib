#ifndef __cutil_condition_h__
#define __cutil_condition_h__
#include "CUtil/inc/Common.h"

namespace CUtil
{
// 	enum EConfigConditionType
// 	{
// 		CONDITION_BOOL,
// 		CONDITION_INT32,
// 		CONDITION_INT64,
// 		CONDITION_DOUBLE,
// 		CONDITION_DATE,
// 		CONDITION_STRING,
// 	};
// 
// 	template<EConfigConditionType type>
// 	class Condition
// 	{
// 	public:
// 		Condition()
// 		{}
// 		~Condition()
// 		{}
// 	};
// 
// 	template<>
// 	class Condition<CONDITION_BOOL>
// 	{
// 	public:
// 		bool		operator > (const bool & a)
// 		{
// 
// 		}
// 	};
	enum EConditionType
	{
		CONDITION_PLAYER_LEVEL,
		CONDITION_VIP_LEVEL,
		CONDITION_LEAGUE_LEVEL,
	};

	class Player
	{
	public:
		Player();
		~Player();

		INT32 GetPlayerLevel()
		{
			return nPlayerLevel;
		}

	private:
		int nPlayerLevel;
	};

	Player::Player()
	{
	}

	Player::~Player()
	{
	}
	template<EConditionType type>
	class Condition
	{
	public:
		Condition()
		{}
		~Condition()
		{}
	};

	template<>
	class Condition<CONDITION_PLAYER_LEVEL>
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
}

#endif

