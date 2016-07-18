#pragma once
#include "RobotCommon.h"


class Robot
{
public:
	Robot(const std::string & val, INT32 nSessionID);
	~Robot();

public:
	virtual CErrno				Init(void);
	virtual CErrno				Cleanup(void);
	virtual CErrno				Update(void);

public:
	std::string					GetName() const { return m_strName; }
	void						SetName(std::string val) { m_strName = val; }
	INT32						GetSessionID() const { return m_nSessionID; }
	void						SetSessionID(INT32 val) { m_nSessionID = val; }
	INT32						GetRobotIndex() const { return m_nRobotIndex; }
	void						SetRobotIndex(INT32 val) { m_nRobotIndex = val; }

protected:
	std::string					m_strName;
	INT32						m_nSessionID;
	INT32						m_nRobotIndex;		//5 这里需要关联ShowRobots界面上的index.
};

