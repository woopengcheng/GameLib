#include "stdafx.h"
#include "RobotGroup.h"
#include "RobotCtrl.h"
#include "DlgRobotCtrl.h"
#include "RobotServer.h"

namespace Robot
{
	//5 默认按照等级排序
	template<EListColType type = LIST_LEVEL , BOOL bUp = TRUE>
	class CRobotSortCmp
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			if (bUp)
			{
				return pRobot1->GetRobotInfo().nLevel < pRobot2->GetRobotInfo().nLevel;
			}
			else
			{
				return pRobot2->GetRobotInfo().nLevel < pRobot1->GetRobotInfo().nLevel;
			}
		}
	};

	template<>
	class CRobotSortCmp<LIST_NAME, TRUE>
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			return pRobot1->GetRobotInfo().strName < pRobot2->GetRobotInfo().strName;
		}
	};
	template<>
	class CRobotSortCmp<LIST_NAME, FALSE>
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			return pRobot2->GetRobotInfo().strName < pRobot1->GetRobotInfo().strName;
		}
	};

	template<>
	class CRobotSortCmp<LIST_LEVEL, TRUE>
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			return pRobot1->GetRobotInfo().nLevel < pRobot2->GetRobotInfo().nLevel;
		}
	};
	template<>
	class CRobotSortCmp<LIST_LEVEL, FALSE>
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			return pRobot2->GetRobotInfo().nLevel < pRobot1->GetRobotInfo().nLevel;
		}
	};

	template<>
	class CRobotSortCmp<LIST_NOTE, TRUE>
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			return pRobot1->GetRobotInfo().strName < pRobot2->GetRobotInfo().strName;
		}
	};
	template<>
	class CRobotSortCmp<LIST_NOTE, FALSE>
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			return pRobot2->GetRobotInfo().strNote < pRobot1->GetRobotInfo().strNote;
		}
	};
	template<>
	class CRobotSortCmp<LIST_STATUE, TRUE>
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			return pRobot1->GetRobotInfo().nStatue < pRobot2->GetRobotInfo().nStatue;
		}
	};
	template<>
	class CRobotSortCmp<LIST_STATUE, FALSE>
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			return pRobot2->GetRobotInfo().nStatue < pRobot1->GetRobotInfo().nStatue;
		}
	};
	template<>
	class CRobotSortCmp<LIST_PROF, TRUE>
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			return pRobot1->GetRobotInfo().nProf < pRobot2->GetRobotInfo().nProf;
		}
	};
	template<>
	class CRobotSortCmp<LIST_PROF, FALSE>
	{
	public:
		bool	operator()(const CRobot * pRobot1, const CRobot * pRobot2)
		{
			return pRobot2->GetRobotInfo().nProf < pRobot1->GetRobotInfo().nProf;
		}
	};


	RobotGroup::RobotGroup(const std::string & val, INT32 nSessionID, RobotServer * pRobotServer , Msg::Object id, Msg::RpcManager * pRpcManager)
		: Msg::IRpcMsgCallableObject(id, pRpcManager)
		, m_nCurRobotCount(cnRobotStartID)
		, m_nRobotSessionID(nSessionID)
		, m_strName(val)
		, m_nRobotTabIndex(-1)
		, m_pRobotServer(pRobotServer)
		, m_objCurSortType(LIST_LEVEL)
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
		VecRobots::iterator iter = m_vecRobots.begin();
		for (; iter != m_vecRobots.end();++iter)
		{
			(*iter)->Cleanup();
			delete *iter;
		}
		return CErrno::Success();
	}

	CErrno RobotGroup::Update(void)
	{
		return CErrno::Success();
	}

	INT32  RobotGroup::CreateRobot(const RobotInfo & info , Msg::Object objSrc)
	{
		CRobot * pRobot = new CRobot(this, m_nCurRobotCount, m_pRobotServer->GetRpcManager());
		pRobot->SetRobotInfo(info);
		pRobot->SetPeerRobotID(objSrc.m_llObjID);
		m_vecRobots.push_back(pRobot);
		//ResortRobots(m_objCurSortType);
		
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
				pRobotDlg->GetCurRobotTabIndex() == m_nRobotTabIndex - 1)
			{
				pRobotDlg->GetDlgCurShowRobot().OnCreateRobot(this, pRobot);
			}
		}
	}

	BOOL RobotGroup::ResortRobots(EListColType type, BOOL bUp/* = TRUE*/)
	{
		m_objCurSortType = type;

		switch (type)
		{
		case LIST_NAME:
		{
			if (bUp)
			{
				std::sort(m_vecRobots.begin(), m_vecRobots.end(), CRobotSortCmp<LIST_NAME>());
			}
			else
			{
				std::sort(m_vecRobots.begin(), m_vecRobots.end(), CRobotSortCmp<LIST_NAME , FALSE>());
			}
		}break;
		case LIST_PROF:
		{
			if (bUp)
			{
				std::sort(m_vecRobots.begin(), m_vecRobots.end(), CRobotSortCmp<LIST_PROF>());
			}
			else
			{
				std::sort(m_vecRobots.begin(), m_vecRobots.end(), CRobotSortCmp<LIST_PROF, FALSE>());
			}
		}break;
		case LIST_LEVEL:
		{
			if (bUp)
			{
				std::sort(m_vecRobots.begin(), m_vecRobots.end(), CRobotSortCmp<LIST_LEVEL>());
			}
			else
			{
				std::sort(m_vecRobots.begin(), m_vecRobots.end(), CRobotSortCmp<LIST_LEVEL, FALSE>());
			} 
		}break;
		case LIST_STATUE:
		{
			if (bUp)
			{
				std::sort(m_vecRobots.begin(), m_vecRobots.end(), CRobotSortCmp<LIST_STATUE>());
			}
			else
			{
				std::sort(m_vecRobots.begin(), m_vecRobots.end(), CRobotSortCmp<LIST_STATUE, FALSE>());
			}
		}break;
		case LIST_NOTE:
		{
			if (bUp)
			{
				std::sort(m_vecRobots.begin(), m_vecRobots.end(), CRobotSortCmp<LIST_NOTE>());
			}
			else
			{
				std::sort(m_vecRobots.begin(), m_vecRobots.end(), CRobotSortCmp<LIST_NOTE, FALSE>());
			}
		}break;
		default:
			break;
		}

		return TRUE;
	}

}