#include "stdafx.h"
#include "RobotGroup.h"
#include "RobotCtrl.h"
#include "DlgRobotCtrl.h"
#include "RobotServer.h"

RobotGroup::RobotGroup(const std::string & val, INT32 nSessionID, RobotServer * pRobotServer)
	: m_nCurRobotCount(0)
	, m_nSessionID(nSessionID)
	, m_strName(val)
	, m_nRobotTabIndex(-1)
	, m_pRobotServer(pRobotServer)
{
	m_pRpcListener = new RobotListener(this);
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

CErrno RobotGroup::CreateRobot(INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
{
	if (!bReconnect)
	{
		MapRobots::iterator iter = m_mapRobots.find(nSessionID);
		if (iter == m_mapRobots.end())
		{
			Robot * pRobot = new Robot(strNetNodeName, nSessionID , this);
			pRobot->SetRobotIndex(m_nCurRobotCount);
			m_mapRobots.insert(std::make_pair(nSessionID, pRobot));

			m_mapTabToRobot.insert(std::make_pair(m_nCurRobotCount, nSessionID));
			m_mapRobotToTab.insert(std::make_pair(nSessionID, m_nCurRobotCount));

			OnCreateRobot(pRobot);

			++m_nCurRobotCount;
		}
		else
		{
			gErrorStream("CreateRobot err , it already exist. sessionID=" << nSessionID);
			return CErrno::Failure();
		}
	}
	return CErrno::Success();
}

void RobotGroup::OnCreateRobot(Robot * pRobot)
{
	if (pRobot)
	{
		CDlgRobotCtrl * pRobotDlg = dynamic_cast<CDlgRobotCtrl*>(theApp.m_pMainWnd);
		if (pRobotDlg && m_pRobotServer && 
			pRobotDlg->GetCurListCtrlIndex() == m_pRobotServer->GetListCtrlIndex() &&
			pRobotDlg->GetCurRobotTabIndex() == m_nRobotTabIndex)
		{
			pRobotDlg->GetDlgCurShowRobot().OnCreateRobot(this, pRobot);
		}
	}
}

CErrno RobotGroup::DeleteRobot(INT32 nSessionID)
{
	MapRobots::iterator iter = m_mapRobots.find(nSessionID);
	if (iter != m_mapRobots.end())
	{
		OnDeleteRobot(iter->second);

		delete iter->second;
		m_mapRobots.erase(iter);

		--m_nCurRobotCount;
	}
	else
	{
		MsgAssert_ReF(0, "DeleteRobot err , it not exist. sessionID=" << nSessionID);
	}
	return CErrno::Success();
}

void RobotGroup::OnDeleteRobot(Robot * pRobot)
{
	if (pRobot)
	{
		INT32 nIndex = pRobot->GetRobotIndex();

		MapTabToRobot::iterator iter = m_mapTabToRobot.find(nIndex);
		if (iter != m_mapTabToRobot.end())
		{
			m_mapTabToRobot.erase(iter);
		}
		MapRobotToTab::iterator iter2 = m_mapRobotToTab.find(nIndex);
		if (iter2 != m_mapRobotToTab.end())
		{
			m_mapRobotToTab.erase(iter2);
		}

		CDlgRobotCtrl * pRobotDlg = dynamic_cast<CDlgRobotCtrl*>(theApp.m_pMainWnd);
		if (pRobotDlg && m_pRobotServer &&
			pRobotDlg->GetCurListCtrlIndex() == m_pRobotServer->GetListCtrlIndex() &&
			pRobotDlg->GetCurRobotTabIndex() == m_nRobotTabIndex)
		{
			pRobotDlg->GetDlgCurShowRobot().OnDeleteRobot(this, pRobot);
		}

		pRobot->Cleanup();
	}
}



void RobotGroup::DebugConnect()
{
//	int nIndex = rand() % 123;
	int nIndex = m_nCurRobotCount;
	CString str;
	str.Format("%d", nIndex);
	m_pRpcListener->OnConnected(this, nIndex, (const char*)(str.GetBuffer()), false);
}

void RobotGroup::DebugDisconnect()
{
	INT32 nIndex = m_mapRobotToTab.begin() != m_mapRobotToTab.end() ? m_mapRobotToTab.begin()->first : 0;
	if (nIndex != 0)
	{
		m_pRpcListener->OnDisconnected(this, nIndex, 0);
	}
}

CErrno RobotListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
{
	if (m_pManager)
	{
		m_pManager->CreateRobot(nSessionID, strNetNodeName, bReconnect);
	}

	gDebugStream("connected from sessionID=" << nSessionID);
	return CErrno::Success();
}

CErrno RobotListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
{
	if (m_pManager)
	{
		m_pManager->DeleteRobot(nSessionID);
	}

	gDebugStream("disconnected from sessionID=" << nPeerSessionID);
	return CErrno::Success();
}
