#include "stdafx.h"
#include "Robot.h"


Robot::Robot(const std::string & val, INT32 nSessionID)
	: m_nSessionID(nSessionID)
	, m_strName(val)
	, m_nRobotIndex(-1)
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