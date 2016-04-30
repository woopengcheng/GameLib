/************************************
FileName	:	ActionConfigLoad.h
Author		:	generate by tools
HostName	:	DESKTOP-5AT4DK2
IP			:	192.168.16.104
Version		:	0.0.1
Date		:	2016-04-30 02:32:23
Description	:	csv配置文件
************************************/
#ifndef __ActionConfigLoad_define_h__
#define __ActionConfigLoad_define_h__
#include "CUtil/inc/Common.h "
#include "Timer/inc/Date.h "

namespace Config
{
	struct SActionConfigLoad
	{
		INT64							curve_id;	//轨迹ID
		std::vector<INT32>				validStages;	//生效场景--不配表示所有场景中生效--否则仅在配置场景中生效
		std::string						wrapMode;	//循环方式
		std::vector<std::string>		InterfaceIcon;	//界面中图标
		double							attr_mod_val;	//属性修改值
		std::vector<double>				start_x;	//起始点
		INT64							col_1;	//备注1
		bool							isLocal;	//坐标系

		//结构体
		struct STestStruct
		{
			bool						test1;
			INT64						test2;
			double						test3;
			INT32						test4;
			std::string					test6;
		}TestStruct;

		//结构体数组
		struct STestStructArray
		{
			bool						test1;
			INT64						test2;
			double						test3;
			INT32						test4;
			std::string					test5;
		};
		std::vector<STestStructArray>		vecTestStructArray;
	};


	class ActionConfigLoad
	{
	public:
		typedef std::vector<SActionConfigLoad> CollectionConfigsT;

	public:
		bool				LoadFrom(const std::string& filename);

	public:
		SActionConfigLoad &	Get(size_t row);

	public:
		inline size_t		Count(){ return m_vtConfigs.size(); }

	public:
		BOOL				xxCheckcurve_id(SActionConfigLoad & conf);
		BOOL				xxCheckvalidStages(SActionConfigLoad & conf);
		BOOL				xxCheckwrapMode(SActionConfigLoad & conf);
		BOOL				xxCheckInterfaceIcon(SActionConfigLoad & conf);
		BOOL				xxCheckattr_mod_val(SActionConfigLoad & conf);
		BOOL				xxCheckstart_x(SActionConfigLoad & conf);
		BOOL				xxCheckcol_1(SActionConfigLoad & conf);
		BOOL				xxCheckisLocal(SActionConfigLoad & conf);
		BOOL				xxCheckTestStruct(SActionConfigLoad & conf);
		BOOL				xxCheckTestStructArray(SActionConfigLoad & conf);
	
	private:
		CollectionConfigsT	m_vtConfigs;
	};
}

#endif// end  __ActionConfigLoad_define_h__
