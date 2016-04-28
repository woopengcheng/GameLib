// attention dont't change this line:INT32 id;std::map<std::string , bool> boolCommon;std::map<std::string , bool> boolCommon2;std::map<std::string , INT32> intCommon;std::map<std::string , INT64> int64Common;std::map<std::string , double> doubleCommon;std::map<std::string , std::string> stringCommon;std::map<std::string , Timer::Date> dateCommon;
/************************************
FileName	:	_CommonDataBase.h
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.40.128
Version		:	0.0.1
Date		:	2016-04-28 14:59:17
Description	:	csv读取文件
************************************/
#ifndef __Config__CommonDataBase_define_h__
#define __Config__CommonDataBase_define_h__
#include "_CommonDataLoad.h"
#include "../Condition.h"
#include "CUtil/inc/CSVConfig.h"

namespace Config
{


	struct S_CommonData
	{
		INT32							id;	//暂时无用
		std::map<std::string , bool>		boolCommon;	//bool公用值
		std::map<std::string , bool>		boolCommon2;	//bool公用值2
		std::map<std::string , INT32>		intCommon;	//int公用值
		std::map<std::string , INT64>		int64Common;	//int64公用值
		std::map<std::string , double>		doubleCommon;	//double公用值
		std::map<std::string , std::string>		stringCommon;	//string公用值
		std::map<std::string , Timer::Date>		dateCommon;	//date公用值
	};


	class _CommonDataBase: public CUtil::CSVConfig
	{
	public:
		typedef std_unordered_map<INT32 , S_CommonData> MapConfigsT;

	public:
		bool				LoadFrom(const std::string& filepath);
		S_CommonData *	Get_CommonData(INT32 id , std::string strFilePath = "");

	public:


	private:
		MapConfigsT			m_mapConfigs;

	};
}

#endif// end  __Config__CommonData_define_h__
