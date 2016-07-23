#include "stdafx.h"
#include "RobotManager.h"
#include "RobotCtrl.h"
#include "DlgRobotCtrl.h"
#include "LogLib/inc/Log.h"
#include "RPCCallFuncs.h"

namespace Robot
{
	RobotManager::RobotManager()
		: m_nCurRobotServerCount(0)
	{
		m_pRpcListener = new RobotManagerListener(this);
	}

	RobotManager::~RobotManager()
	{
		SAFE_DELETE(m_pRpcListener);
	}

	RobotManager & RobotManager::GetInstance()
	{
		static RobotManager instance;
		return instance;
	}

	CErrno RobotManager::Init(Json::Value & conf)
	{
		m_objConf = conf;
		Json::Value robot = conf.get("robot_ctrl", Json::Value());
		return RpcInterface::Init(robot);
	}

	CErrno RobotManager::Cleanup(void)
	{
		MapRobotServerToTab temp = m_mapRobotServerToTab; //5 删除的时候.底层会删除key,所以这里需要用temp
		MapRobotServerToTab::iterator iter = temp.begin();
		for (; iter != temp.end(); ++iter)
		{
			DeleteRobotServer(iter->first);
		}

		return RpcInterface::Cleanup();
	}

	CErrno RobotManager::Update(void)
	{
		MapRobotServers::iterator iter = m_mapRobotServers.begin();
		for (; iter != m_mapRobotServers.end(); ++iter)
		{
			if (iter->second)
			{
				iter->second->Update();
			}
		}

		return RpcInterface::Update();
	}

	CErrno RobotManager::CreateRobotServer(INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (!bReconnect)
		{
			MapRobotServers::iterator iter = m_mapRobotServers.find(nSessionID);
			if (iter == m_mapRobotServers.end())
			{
				RobotServer * pRobot = new RobotServer(strNetNodeName, nSessionID, m_nCurRobotServerCount, this->GetRpcManager());
				m_mapRobotServers.insert(std::make_pair(nSessionID, pRobot));
				pRobot->Init(m_objConf);

				m_mapTabToRobotServer.insert(std::make_pair(m_nCurRobotServerCount, nSessionID));
				m_mapRobotServerToTab.insert(std::make_pair(nSessionID, m_nCurRobotServerCount));

				OnCreateRobotServer(pRobot);

				//5 同步服务器的Port给RobotServer服务器.因为是随机的.所以让robotserver告诉robotgroup连接到这个端口.
				Robot::rpc_SyncRobotServerID(*this , nSessionID, 0, m_nCurRobotServerCount , pRobot->GetServerPort());

				++m_nCurRobotServerCount;
			}
			else
			{
				gErrorStream("CreateRobotServer err , it already exist. sessionID=" << nSessionID);
				return CErrno::Failure();
			}
		}
		return CErrno::Success();
	}

	void RobotManager::OnCreateRobotServer(RobotServer * pRobot)
	{
		if (pRobot)
		{
			CDlgRobotCtrl * pRobotDlg = dynamic_cast<CDlgRobotCtrl*>(theApp.m_pMainWnd);
			if (pRobotDlg)
			{
				pRobotDlg->OnCreateRobotServer(pRobot);
			}
		}

	}

	CErrno RobotManager::DeleteRobotServer(INT32 nSessionID)
	{
		MapRobotServers::iterator iter = m_mapRobotServers.find(nSessionID);
		if (iter != m_mapRobotServers.end())
		{
			OnDeleteRobotServer(iter->second);

			delete iter->second;
			m_mapRobotServers.erase(iter);

			--m_nCurRobotServerCount;

		}
		else
		{
			MsgAssert_ReF(0, "DeleteRobotServer err , it not exist. sessionID=" << nSessionID);
		}
		return CErrno::Success();
	}

	void RobotManager::OnDeleteRobotServer(RobotServer * pRobot)
	{
		if (pRobot)
		{
			INT32 nIndex = pRobot->GetListCtrlIndex();
			ReplaceSimpleMapRightKey(m_mapTabToRobotServer, m_mapTabToRobotServer, nIndex);
			ResetMapKey(m_mapTabToRobotServer, m_mapRobotServerToTab);
			ResetRobotServerIndex();

			pRobot->Cleanup();

			CDlgRobotCtrl * pRobotDlg = dynamic_cast<CDlgRobotCtrl*>(theApp.m_pMainWnd);
			if (pRobotDlg)
			{
				pRobotDlg->OnDeleteRobotServer(pRobot);
			}
		}
	}

	void RobotManager::ResetRobotServerIndex()
	{
		MapRobotServerToTab::iterator iter = m_mapRobotServerToTab.begin();
		for (; iter != m_mapRobotServerToTab.end(); ++iter)
		{
			MapRobotServers::iterator iter2 = m_mapRobotServers.find(iter->first);
			if (iter2 != m_mapRobotServers.end())
			{
				RobotServer * pRobot = iter2->second;
				pRobot->SetListCtrlIndex(iter->second);
			}
			else
			{
				MsgAssert(0, "ResetRobotServerIndex err , it not exist. sessionID=" << iter->first);
			}
		}
	}

	void RobotManager::DebugConnect()
	{
		int nIndex = rand() % 123123;
		CString str;
		str.Format("%d", nIndex);
		m_pRpcListener->OnConnected(this, nIndex, (const char*)(str.GetBuffer()), false);
	}

	void RobotManager::DebugDisconnect()
	{
		INT32 nIndex = m_mapRobotServerToTab.begin() != m_mapRobotServerToTab.end() ? m_mapRobotServerToTab.begin()->first : 0;
		if (nIndex != 0)
		{
			m_pRpcListener->OnDisconnected(this, nIndex, 0);
		}
	}

	RobotServer * RobotManager::GetRobotServer(INT32 nIndex)
	{
		MapTabToRobotServer::iterator iter = m_mapTabToRobotServer.find(nIndex);
		if (iter == m_mapTabToRobotServer.end())
		{
			gWarniStream("GetRobotServer MapTabToRobotServer listctrl index not exist. index=" << nIndex);
			return NULL;
		}

		MapRobotServers::iterator iter2 = m_mapRobotServers.find(iter->second);
		if (iter2 == m_mapRobotServers.end())
		{
			gWarniStream("GetRobotServer MapRobotServers listctrl index not exist. index=" << iter->second);
			return NULL;
		}
		return iter2->second;
	}

	RobotGroup			* RobotManager::GetRobotGroup(INT32 nListCtrlIndex, INT32 nRobotTabIndex)
	{
		RobotServer * pRobot = GetRobotServer(nListCtrlIndex);
		if (!pRobot)
		{
			gWarniStream("GetRobotGroup listctrl index not exist.index = " << nListCtrlIndex);
			return NULL;
		}

		return pRobot->GetRobotGroup(nRobotTabIndex);

	}

	CErrno RobotManagerListener::OnConnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, const std::string & strNetNodeName, bool bReconnect/* = false*/)
	{
		if (m_pManager)
		{
			m_pManager->CreateRobotServer(nSessionID, strNetNodeName, bReconnect);
		}

		gDebugStream("connected from sessionID=" << nSessionID);
		return CErrno::Success();
	}

	CErrno RobotManagerListener::OnDisconnected(Msg::RpcInterface * pRpcInterface, INT32 nSessionID, INT32 nPeerSessionID)
	{
		if (m_pManager)
		{
			m_pManager->DeleteRobotServer(nSessionID);
		}

		gDebugStream("disconnected from sessionID=" << nPeerSessionID);
		return CErrno::Success();
	}
}