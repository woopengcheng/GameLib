#include "CUtil/inc/Common.h"

std::string CErrno::s_strErrorMsg[CErrno::ERR_COUNT] = 
{
	//5 0-4
	"SUCCESS",
	"FAILURE",
	"IO错误",
	"未找到",
	"被占用",

	// 5-9
	"无效字段",
	"数据库已经存在这条记录",
	"无效数据",

};
