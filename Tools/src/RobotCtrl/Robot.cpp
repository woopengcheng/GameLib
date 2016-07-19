#include "stdafx.h"
#include "Robot.h"
#include "RobotGroup.h"


Robot::Robot(const std::string & val, INT32 nSessionID, RobotGroup * pRobotGroup)
	: m_nSessionID(nSessionID)
	, m_strName(val)
	, m_nRobotIndex(-1)
	, m_pRobotGroup(pRobotGroup)
{
}


Robot::~Robot()
{
}

CErrno Robot::Init(void)
{
	return CErrno::Success();
}

CErrno Robot::Cleanup(void)
{
	return CErrno::Success();
}

CErrno Robot::Update(void)
{
	return CErrno::Success();
}