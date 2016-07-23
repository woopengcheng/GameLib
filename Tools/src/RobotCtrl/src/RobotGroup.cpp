#include "stdafx.h"
#include "RobotGroup.h"
#include "RobotCtrl.h"
#include "DlgRobotCtrl.h"
#include "RobotServer.h"

namespace Robot
{
	RobotGroup::RobotGroup(const std::string & val, INT32 nSessionID, RobotServer * pRobotServer , Msg::Object id, Msg::RpcManager * pRpcManager)
		: Msg::IRpcMsgCallableObject(id, pRpcManager)
		, m_nCurRobotCount(cnRobotStartID)
		, m_nRobotSessionID(nSessionID)
		, m_strName(val)
		, m_nRobotTabIndex(-1)
		, m_pRobotServer(pRobotServer)
	{
		if (m_pRobotServer)
		{
			gErrorStream("RobotGroup create error.");
		}
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
		MapRobots::iterator iter = m_mapRobots.begin();
		for (; iter != m_mapRobots.end();++iter)
		{
			iter->second->Cleanup();
			delete iter->second;
		}
		return CErrno::Success();
	}

	CErrno RobotGroup::Update(void)
	{
		return CErrno::Success();
	}

	INT32  RobotGroup::CreateRobot(const RobotInfo & info)
	{
		CRobot * pRobot = new CRobot(this, m_nCurRobotCount, m_pRobotServer->GetRpcManager());
		pRobot->SetRobotInfo(info);
		m_mapRobots.insert(std::make_pair(m_nCurRobotCount, pRobot));
		
		OnCreateRobot(pRobot);

		++m_nCurRobotCount;

		return m_nCurRobotCount - 1;
	}

	void RobotGroup::OnCreateRobot(CRobot * pRobot)
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

	void RobotGroup::OnDeleteRobot(CRobot * pRobot)
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


// 
// 	void RobotGroup::DebugConnect()
// 	{
// 		//	int nIndex = rand() % 123;
// 		int nIndex = m_nCurRobotCount;
// 		CString str;
// 		str.Format("%d", nIndex);
// 		m_pRpcListener->OnConnected(this, nIndex, (const char*)(str.GetBuffer()), false);
// 	}
// 
// 	void RobotGroup::DebugDisconnect()
// 	{
// 		INT32 nIndex = m_mapRobotToTab.begin() != m_mapRobotToTab.end() ? m_mapRobotToTab.begin()->first : 0;
// 		if (nIndex != 0)
// 		{
// 			m_pRpcListener->OnDisconnected(this, nIndex, 0);
// 		}
// 	}
// 
// 	CErrno RobotGroupListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
// 	{
// 		if (m_pManager)
// 		{
// 			m_pManager->CreateRobot(nSessionID, strNetNodeName, bReconnect);
// 		}
// 
// 		gDebugStream("connected from sessionID=" << nSessionID);
// 		return CErrno::Success();
// 	}
// 
// 	CErrno RobotGroupListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
// 	{
// 		if (m_pManager)
// 		{
// 			m_pManager->DeleteRobot(nSessionID);
// 		}
// 
// 		gDebugStream("disconnected from sessionID=" << nPeerSessionID);
// 		return CErrno::Success();
// 	}
}