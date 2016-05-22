// attention dont't change this line:INT32 id;std::map<std::string , bool> boolCommon;std::map<std::string , bool> boolCommon2;std::map<std::string , INT32> intCommon;std::map<std::string , INT64> int64Common;std::map<std::string , double> doubleCommon;std::map<std::string , std::string> stringCommon;std::map<std::string , Timer::Date> dateCommon;
/************************************
FileName	:	_CommonData.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-05-22 11:43:23
Description	:	csv读取文件
************************************/
#ifndef __Config__CommonData_define_h__
#define __Config__CommonData_define_h__
#include "_CommonDataBase.h"

namespace Config
{

	class _CommonData: public _CommonDataBase
	{
	public:
		static _CommonData&	GetInstance(){ static _CommonData s__CommonData; return s__CommonData; }
		static _CommonData*	GetInstancePtr(){ return &GetInstance(); }
		
	public:
		virtual BOOL		OnLoad();
		
	private:

	};
}

#endif// end  __Config__CommonData_define_h__
