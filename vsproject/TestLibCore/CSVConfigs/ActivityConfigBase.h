// attention dont't change this line:std::string ActivityId;std::string ActivityName;INT32 ActivityLevelLow;INT32 ActivityLevelHigh;bool IsShowEntrance;std::vector<std::string> EntranceIcon;bool IsShowInterface;std::vector<std::string> InterfaceIcon;INT32 StartTimeWeek;INT32 EndTimeWeek;INT32 StartTimeDate;INT32 EndTimeDate;std::string StartTime;std::string EndTime;INT32 Description;std::string RewardIcon;Timer::Date testdate;testDateStructTimer::Date begin;Timer::Date end;std_unordered_map<INT64 , SActionConfig *> actionConfig;std::vector<Timer::Date> dateArray;ActionConfig testConfig;ActionConfig testConfig2;std::map<std::string , INT64> dateCommon;TestStructArraybool test1;INT64 test2;double test3;INT32 test4;std::string test5;std_unordered_map<INT64 , SActionConfig *> test6;TestStructbool test1;INT64 test2;double test3;INT32 test4;std::string test6;std_unordered_map<INT64 , SActionConfig *> test7;
/************************************
FileName	:	ActivityConfigBase.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-22 11:43:23
Description	:	csv读取文件
************************************/
#ifndef __Config_ActivityConfigBase_define_h__
#define __Config_ActivityConfigBase_define_h__
#include "ActivityConfigLoad.h"
#include "../Condition.h"
#include "ConfigDeclare.h"
#include "CUtil/inc/CSVConfig.h"

namespace Config
{

	struct SActivityConfig
	{
		std::string						ActivityId;	//活动id
		std::string						ActivityName;	//活动名称
		INT32							ActivityLevelLow;	//显示等级下限,不到等级（不包括自身）不显示
		INT32							ActivityLevelHigh;	//显示等级上限，超过等级（不包括自身）不显示
		bool							IsShowEntrance;	//是否显示入口图标，入口图标和界面图标是互斥的；入口图标的形式适合单独放在外面的活动，界面图标就是通用活动中的图标
		std::vector<std::string>		EntranceIcon;	//入口图标
		bool							IsShowInterface;	//是否界面中显示，在这里显示，就是通用活动的形式；否则就是在入口图标中显示，显示为单独活动的形式
		std::vector<std::string>		InterfaceIcon;	//界面中图标
		INT32							StartTimeWeek;	//活动开始时间周，即每周的哪一天；这里填写的值优先级最大，与具体开始时间天，结束时间天无关
		INT32							EndTimeWeek;	//活动结束时间周
		INT32							StartTimeDate;	//活动开始时间天，不在此时间内，则活动不会显示
		INT32							EndTimeDate;	//活动结束时间天
		std::string						StartTime;	//活动开始时间点，不在此时间内则活动不显示
		std::string						EndTime;	//活动结束时间点
		INT32							Description;	//活动说明，通用活动界面中的活动描述
		std::string						RewardIcon;	//奖励图标，领奖界面中该活动的领奖图标
		Timer::Date						testdate;	//根据某些condition执行某些action;目前不支持括号和或.全部用&&(与)和!(非)表示满足一个条件可以执行多个action.可以写在一行(支持多个),或者多行.需要保证一个condition和至少一个action.如果没有也没啥.Action:调用玩家的或者帮会等的函数接口.

		//根据某些condition执行某些action;目前不支持括号和或.全部用&&(与)和!(非)表示满足一个条件可以执行多个action.可以写在一行(支持多个),或者多行.需要保证一个condition和至少一个action.如果没有也没啥.Action:调用玩家的或者帮会等的函数接口.
		struct StestDateStruct
		{
			Timer::Date					begin;
			Timer::Date					end;
			std_unordered_map<INT64 , SActionConfig *>	actionConfig;
		}testDateStruct;
		std::vector<Timer::Date>			dateArray;	//根据某些condition执行某些action;目前不支持括号和或.全部用&&(与)和!(非)表示满足一个条件可以执行多个action.可以写在一行(支持多个),或者多行.需要保证一个condition和至少一个action.如果没有也没啥.Action:调用玩家的或者帮会等的函数接口.
		std_unordered_map<INT64 , SActionConfig *>		testConfig;	//测试时间
		std_unordered_map<INT64 , SActionConfig *>		testConfig2;	//测试时间结构体.比如说活动开启结束时间
		std::map<std::string , INT64>		dateCommon;	//测试时间数组

		//嵌套表使用
		struct STestStructArray
		{
			bool						test1;
			INT64						test2;
			double						test3;
			INT32						test4;
			std::string					test5;
			std_unordered_map<INT64 , SActionConfig *>	test6;
		};
		std::vector<STestStructArray>		vecTestStructArray;

		//嵌套表使用
		struct STestStruct
		{
			bool						test1;
			INT64						test2;
			double						test3;
			INT32						test4;
			std::string					test6;
			std_unordered_map<INT64 , SActionConfig *>	test7;
		}TestStruct;
	};


	class ActivityConfigBase: public CUtil::CSVConfig
	{
	public:
		typedef std_unordered_map<std::string , SActivityConfig> MapConfigsT;

	public:
		bool				LoadFrom(const std::string& filepath);
		bool				RepairLoad(const std::string& filepath);
		SActivityConfig	*	GetActivityConfig(std::string id , std::string strFilePath = "");

	public:
		bool				RunUse(std::string id , CUtil::Player * pPlayer = NULL , CUtil::League * pLeague = NULL , CUtil::Team * pTeam = NULL);
		bool				RunUse1(std::string id , CUtil::Player * pPlayer = NULL , CUtil::League * pLeague = NULL , CUtil::Team * pTeam = NULL);


	private:
		MapConfigsT			m_mapConfigs;

	};
}

#endif// end  __Config_ActivityConfig_define_h__
