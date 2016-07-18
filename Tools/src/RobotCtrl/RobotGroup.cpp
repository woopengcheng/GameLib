#include "stdafx.h"
#include "RobotGroup.h"


RobotGroup::RobotGroup(const std::string & val, INT32 nSessionID)
	: m_nCurRobotGroupCount(0)
	, m_nSessionID(nSessionID)
	, m_strName(val)
{
}


RobotGroup::~RobotGroup()
{
}

CErrno RobotGroup::Init(Json::Value & conf)
{
	return CErrno::Success();
}

CErrno RobotGroup::Cleanup(void)
{
	return CErrno::Success();
}

CErrno RobotGroup::Update(void)
{
	return CErrno::Success();
}
