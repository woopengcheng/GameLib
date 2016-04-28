/************************************
FileName	:	_ConditionConfigLoad.cpp
Author		:	generate by tools
HostName	:	devuser-PC
IP			:	10.236.40.128
Version		:	0.0.1
Date		:	2016-04-28 14:59:17
Description	:	csv读取文件实现
************************************/
#include "_ConditionConfigLoad.h"
#include "CUtil/inc/CUtil.h"
#include "CUtil/inc/CSVReader.h"

namespace Config
{
	bool _ConditionConfigLoad::LoadFrom(const std::string & filepath)
	{
		CUtil::CSVReader csv;
		if(csv.Load(filepath) != 0)
			return false;

		size_t index_nConditionID = csv.GetIndex("nConditionID", 2);
		MsgAssert_Re0(index_nConditionID != (size_t)-1 , "error nConditionID");

		size_t index_strServerCondition = csv.GetIndex("strServerCondition", 2);
		MsgAssert_Re0(index_strServerCondition != (size_t)-1 , "error strServerCondition");

		size_t index_strServerAction = csv.GetIndex("strServerAction", 2);
		MsgAssert_Re0(index_strServerAction != (size_t)-1 , "error strServerAction");

		size_t index_strClientCondition = csv.GetIndex("strClientCondition", 2);
		MsgAssert_Re0(index_strClientCondition != (size_t)-1 , "error strClientCondition");

		size_t index_strClientAction = csv.GetIndex("strClientAction", 2);
		MsgAssert_Re0(index_strClientAction != (size_t)-1 , "error strClientAction");

		for (size_t row = 4; row < csv.Count(); ++row)
		{
			S_ConditionConfigLoad conf;

			conf.nConditionID = csv.GetInt32(row , index_nConditionID);
			conf.strServerCondition = csv.GetString(row , index_strServerCondition);
			conf.strServerAction = csv.GetString(row , index_strServerAction);
			conf.strClientCondition = csv.GetString(row , index_strClientCondition);
			conf.strClientAction = csv.GetString(row , index_strClientAction);
			m_vtConfigs.push_back(conf);
		}

		return true;
	}

	S_ConditionConfigLoad & _ConditionConfigLoad::Get(size_t row)
	{
		return m_vtConfigs.at(row);
	}

}

