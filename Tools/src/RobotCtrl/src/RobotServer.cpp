#include "stdafx.h"
#include "RobotServer.h"
#include "RobotCtrl.h"
#include "DlgRobotCtrl.h"
#include "LogLib/inc/Log.h"

namespace Robot
{
	RobotServer::RobotServer(const std::string & val, INT32 nSessionID , Msg::Object id, Msg::RpcManager * pRpcManager)
		: Msg::IRpcMsgCallableObject(id, pRpcManager)
		, m_nCurRobotGroupCount(0)
		, m_nSessionID(nSessionID)
		, m_strName(val)
	{
		m_pRpcListener = new RobotServerListener(this);
	}


	RobotServer::~RobotServer()
	{
		SAFE_DELETE(m_pRpcListener);
	}

	CErrno RobotServer::Init(Json::Value & conf)
	{
		m_objConf = conf;
		Json::Value objRobot = m_objConf.get("robot_ctrl_server", Json::Value());
		return RpcInterface::Init(objRobot);
	}

	CErrno RobotServer::Cleanup(void)
	{
		MapRobotGroupToTab temp = m_mapRobotGroupToTab; //5 删除的时候.底层会删除key,所以这里需要用temp

		MapRobotGroupToTab::iterator iter = temp.begin();
		for (; iter != temp.end(); ++iter)
		{
			DeleteRobotGroup(iter->first);
		}

		return RpcInterface::Cleanup();
	}

	CErrno RobotServer::Update(void)
	{
		MapRobotGroups::iterator iter = m_mapRobotGroups.begin();
		for (; iter != m_mapRobotGroups.end(); ++iter)
		{
			if (iter->second)
			{
				iter->second->Update();
			}
		}

		return Msg::RpcInterface::Update();
	}

	CErrno RobotServer::CreateRobotGroup(INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (!bReconnect)
		{
			MapRobotGroups::iterator iter = m_mapRobotGroups.find(nSessionID);
			if (iter == m_mapRobotGroups.end())
			{
				RobotGroup * pRobot = new RobotGroup(strNetNodeName, nSessionID, this, m_nCurRobotGroupCount, this->GetRpcManager());
				pRobot->SetRobotTabIndex(m_nCurRobotGroupCount);
				pRobot->Init(m_objConf);
				m_mapRobotGroups.insert(std::make_pair(nSessionID, pRobot));

				m_mapTabToRobotGroup.insert(std::make_pair(m_nCurRobotGroupCount, nSessionID));
				m_mapRobotGroupToTab.insert(std::make_pair(nSessionID, m_nCurRobotGroupCount));

				OnCreateRobotGroup(pRobot);

				++m_nCurRobotGroupCount;
			}
			else
			{
				gErrorStream("CreateRobotGroup err , it already exist. sessionID=" << nSessionID);
				return CErrno::Failure();
			}
		}
		return CErrno::Success();
	}

	void RobotServer::OnCreateRobotGroup(RobotGroup * pRobot)
	{
		if (pRobot)
		{
			CDlgRobotCtrl * pRobotDlg = dynamic_cast<CDlgRobotCtrl*>(theApp.m_pMainWnd);
			if (pRobotDlg && pRobotDlg->GetCurListCtrlIndex() == m_nListCtrlIndex)
			{
				pRobotDlg->OnCreateRobotGroup(this, pRobot);
			}
		}

	}

	CErrno RobotServer::DeleteRobotGroup(INT32 nSessionID)
	{
		MapRobotGroups::iterator iter = m_mapRobotGroups.find(nSessionID);
		if (iter != m_mapRobotGroups.end())
		{
			OnDeleteRobotGroup(iter->second);

			delete iter->second;
			m_mapRobotGroups.erase(iter);

			--m_nCurRobotGroupCount;
		}
		else
		{
			MsgAssert_ReF(0, "DeleteRobotGroup err , it not exist. sessionID=" << nSessionID);
		}
		return CErrno::Success();
	}

	void RobotServer::OnDeleteRobotGroup(RobotGroup * pRobot)
	{
		if (pRobot)
		{
			INT32 nIndex = pRobot->GetRobotTabIndex();
			ReplaceSimpleMapRightKey(m_mapTabToRobotGroup, m_mapTabToRobotGroup, nIndex);
			ResetMapKey(m_mapTabToRobotGroup, m_mapRobotGroupToTab);
			ResetRobotGroupIndex();

			CDlgRobotCtrl * pRobotDlg = dynamic_cast<CDlgRobotCtrl*>(theApp.m_pMainWnd);
			if (pRobotDlg && pRobotDlg->GetCurListCtrlIndex() == m_nListCtrlIndex)
			{
				pRobotDlg->OnDeleteRobotGroup(this, pRobot);
			}
			pRobot->Cleanup();
		}
	}

	void RobotServer::ResetRobotGroupIndex()
	{
		MapRobotGroupToTab::iterator iter = m_mapRobotGroupToTab.begin();
		for (; iter != m_mapRobotGroupToTab.end(); ++iter)
		{
			MapRobotGroups::iterator iter2 = m_mapRobotGroups.find(iter->first);
			if (iter2 != m_mapRobotGroups.end())
			{
				RobotGroup * pRobot = iter2->second;
				pRobot->SetRobotTabIndex(iter->second);
			}
			else
			{
				MsgAssert(0, "ResetRobotGroupIndex err , it not exist. sessionID=" << iter->first);
			}
		}
	}

	void RobotServer::DebugConnect()
	{
		int nIndex = rand() % 123;
		CString str;
		str.Format("%d", nIndex);
		m_pRpcListener->OnConnected(this, nIndex, (const char*)(str.GetBuffer()), false);
	}

	void RobotServer::DebugDisconnect()
	{
		INT32 nIndex = m_mapRobotGroupToTab.begin() != m_mapRobotGroupToTab.end() ? m_mapRobotGroupToTab.begin()->first : 0;
		if (nIndex != 0)
		{
			m_pRpcListener->OnDisconnected(this, nIndex, 0);
		}
	}

	RobotGroup			* RobotServer::OnUpdateRobotTab(INT32 nRobotTabIndex)
	{
		MapTabToRobotGroup::iterator iter = m_mapTabToRobotGroup.find(nRobotTabIndex);
		MsgAssert_Re0(iter != m_mapTabToRobotGroup.end(), "OnUpdateCtrlServer MapTabToRobotGroup listctrl index not exist. index=" << nRobotTabIndex);

		MapRobotGroups::iterator iter2 = m_mapRobotGroups.find(iter->second);
		MsgAssert_Re0(iter2 != m_mapRobotGroups.end(), "OnUpdateCtrlServer MapRobotGroups listctrl index not exist. index=" << iter->second);

		return iter2->second;
	}

	CErrno RobotServerListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (m_pManager)
		{
			m_pManager->CreateRobotGroup(nSessionID, strNetNodeName, bReconnect);
		}

		gDebugStream("connected from sessionID=" << nSessionID);
		return CErrno::Success();
	}

	CErrno RobotServerListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{
		if (m_pManager)
		{
			m_pManager->DeleteRobotGroup(nSessionID);
		}

		gDebugStream("disconnected from sessionID=" << nPeerSessionID);
		return CErrno::Success();
	}
}