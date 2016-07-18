#pragma once
#include "RobotInfo.h"
#include "json/json.h"

class RobotGroup
{
public:
	RobotGroup(const std::string & val, INT32 nSessionID);
	~RobotGroup();

public:
	virtual CErrno				Init(Json::Value & conf);
	virtual CErrno				Cleanup(void);
	virtual CErrno				Update(void);

public:
	typedef std_unordered_map<INT32, RobotInfo>	MapRobotInfos;
	
public:
	std::string					GetName() const { return m_strName; }
	void						SetName(std::string val) { m_strName = val; }
	INT32						GetSessionID() const { return m_nSessionID; }
	void						SetSessionID(INT32 val) { m_nSessionID = val; }
	INT32						GetRobotTabIndex() const { return m_nRobotTabIndex; }
	void						SetRobotTabIndex(INT32 val) { m_nRobotTabIndex = val; }

protected:
	std::string					m_strName;
	INT32						m_nSessionID;
	INT32						m_nRobotTabIndex;		//5 这里需要关联界面上的Tab的index.删除用.
	INT32						m_nCurRobotGroupCount;
	MapRobotInfos				m_mapRobotInfos;
};

