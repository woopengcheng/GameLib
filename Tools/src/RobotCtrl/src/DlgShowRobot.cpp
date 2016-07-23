// DlgShowRobot.cpp : 实现文件
//

#include "stdafx.h"
#include "RobotCtrl.h"
#include "DlgShowRobot.h"
#include "afxdialogex.h"
#include "RobotBtn.h"
#include "DlgRobotCtrl.h"
#include "RPCCallFuncs.h"

// CDlgShowRobot 对话框

IMPLEMENT_DYNAMIC(CDlgShowRobot, CDialogEx)

INT32 GetXBtnNum(CRect rect)
{
	INT32 nXNumber = (rect.BottomRight().x - rect.TopLeft().x) / (cnBtnXLengthSpace + cnBtnXLength);
	return nXNumber;
}

INT32 GetYBtnNum(CRect rect)
{
	INT32 nYNumber = (rect.BottomRight().y - rect.TopLeft().y) / (cnBtnYLengthSpace + cnBtnYLength);
	return nYNumber;
}


BEGIN_MESSAGE_MAP(CDlgShowRobot, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_CREATE()
//	ON_COMMAND_RANGE(cst_btn_id, cst_btn_id + cst_max_btn_id, OnRobotBtnClicked)     //响应那个鼠标的ID，从10000到10000 + 最大的那个
ON_WM_SIZE()
ON_COMMAND(ID_SEND_ROBOT_COMMAND, &CDlgShowRobot::OnSendRobotCommand)
ON_NOTIFY(NM_RCLICK, IDC_LIST_SHOW_ROBOT, &CDlgShowRobot::OnNMRClickListShowRobot)
ON_COMMAND(ID_CREATE_ROBOTS, &CDlgShowRobot::OnCreateRobots)
END_MESSAGE_MAP()

CDlgShowRobot::CDlgShowRobot(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SHOW_ROBOT, pParent)
	, m_nCurRobotTabIndex(-1)
	, m_nCurListCtrlIndex(-1)
	, m_nBtnCount(0)
	, m_nBtnTopLeftX(0)
	, m_nBtnTopLeftY(0)
	, m_pCurRobotGroup(NULL)
	, m_nCreateBtnIndex(0)
	, m_nRobotCount(0)
{

}

CDlgShowRobot::~CDlgShowRobot()
{
 	ClearRobotBtns();
}

BOOL CDlgShowRobot::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CDlgShowRobot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ROBOT_BASE, m_txtRobotPlatform);
	DDX_Control(pDX, IDC_LIST_SHOW_ROBOT, m_listShowRobots);
}

void CDlgShowRobot::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	INT32 btnLineSize = cnBtnYLength + cnBtnYLengthSpace + cnBtnYLengthSpace;

	SCROLLINFO si;
	GetScrollInfo(SB_VERT, &si);//获取滚动条信息，SB_VERT指垂直滚动条，对话框的滚动条不是一个控件，跟对话框是一体的，假控件
	int nMaxPos = si.nMax - si.nPage;//可滚动范围最大值

	INT32 scrValue = 0;//滚动值
	switch (nSBCode)
	{
	case SB_LINEUP://上按钮被单击了一下
		if (si.nPos != 0)
		{
			if (si.nPos - btnLineSize < 0)
				scrValue = si.nPos;
			else
				scrValue = btnLineSize;
		}
		break;
	case SB_LINEDOWN://下按钮被单击了一下
		if (si.nPos != nMaxPos)
		{
			scrValue = nMaxPos - si.nPos;
			if (scrValue < btnLineSize)
				scrValue = -scrValue;
			else
				scrValue = -btnLineSize;
		}
		break;
	case SB_PAGEUP: //滚动条上通道被单击了一下
		if (si.nPos != 0)
		{
			if (si.nPos - si.nPage < 0)//可滚动像素不够si.nPage
				scrValue = si.nPos;
			else
				scrValue = si.nPage;
		}
		break;
	case SB_PAGEDOWN://滚动条下通道被单击了一下
		if (si.nPos != nMaxPos)
		{
			scrValue = nMaxPos - si.nPos;
			if (scrValue < (INT32)si.nPage) //可滚动像素不够si.nPage
				scrValue = -scrValue;
			else
				scrValue = -(INT32)(si.nPage);
		}
		break;
	case SB_THUMBTRACK: //拖动滚动条滑柄，nPos保存位置
		scrValue = si.nPos - nPos;
		break;
	}

// 	if (nMaxPos == nPos)
// 	{
// 		const INT32 cnLastLineSpace = 10;			//5 最后一行显示的优化考虑
// 		scrValue = si.nPos - nPos - cnLastLineSpace;
// 	}
	SetScrollPos(SB_VERT, si.nPos - scrValue);//设置滚动条位置
	ScrollWindow(0, scrValue);//滚动窗口

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


int CDlgShowRobot::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}


void CDlgShowRobot::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	ChangeDlgSize();
}


// CDlgShowRobot 消息处理程序

void CDlgShowRobot::SetScrollBarRange()
{
	CRect rect;
	GetClientRect(rect);
	INT32 nXNumber = GetXBtnNum(rect);
//	INT32 nYNumber = GetYBtnNum(rect);

	int nPos = GetScrollPos(SBS_VERT);

	SCROLLINFO si;
	si.nMin		= 0;
	si.nMax		= (m_nBtnCount / nXNumber + 1) * (cnBtnYLengthSpace + cnBtnYLength);
	si.nPage	= rect.Height();//可见窗口高度和页大小设置一致
	si.nPos		= nPos;
	si.fMask	= SIF_ALL;
	si.cbSize	= sizeof(SCROLLINFO);
	SetScrollInfo(SBS_VERT, &si);
}

void CDlgShowRobot::ChangeDlgSize()
{
	//5 这里需要改变这个对话框内部控件放按钮的大小//5 暂时不用按钮显示通过list处理.
	const int cst_tab_space = 0;
	const int cst_scroll_space = 0;
	CRect rect;
	GetClientRect(rect);
	INT32 nXNumber = GetXBtnNum(rect);
	if (GetDlgItem(IDC_STATIC_ROBOT_BASE))
	{
		GetDlgItem(IDC_STATIC_ROBOT_BASE)->MoveWindow(CRect(rect.TopLeft().x, rect.TopLeft().y + cst_tab_space,
			rect.BottomRight().x - cst_scroll_space, (cnMaxRobotBtnNum / nXNumber + 1) * (cnBtnYLengthSpace + cnBtnYLength)));

		GetDlgItem(IDC_STATIC_ROBOT_BASE)->ShowWindow(FALSE);
		SetScrollBarRange();
	}

	//5 List控件更改大小
	if (GetDlgItem(IDC_LIST_SHOW_ROBOT))
	{
		GetDlgItem(IDC_LIST_SHOW_ROBOT)->MoveWindow(CRect(rect.TopLeft().x, rect.TopLeft().y + cst_tab_space,
			rect.BottomRight().x, rect.BottomRight().y));
	}
}


BOOL CDlgShowRobot::InsertRobotList(RobotGroup * pRobotGroup, CRobot * pRobot)
{
	if (pRobot)
	{
		std::string str;
		INT32 nItem = m_listShowRobots.InsertItem(m_listShowRobots.GetItemCount(), "");
		for (INT32 i = 0;i < LIST_NUM;++i)
		{
			switch (i)
			{
			case LIST_NAME:
			{
				str = (char*)(pRobot->GetRobotInfo().strName.c_str());
			}break;
			case LIST_PROF:
			{
				str = CUtil::itoa(pRobot->GetRobotInfo().nProf).c_str();
			}break;
			case LIST_LEVEL:
			{
				str = CUtil::itoa(pRobot->GetRobotInfo().nLevel).c_str();
			}break;
			case LIST_STATUE:
			{
				str = CUtil::itoa(pRobot->GetRobotInfo().nStatue).c_str();
			}break;
			case LIST_NOTE:
			{
				str = (char*)(pRobot->GetRobotInfo().strNote.c_str());
			}break;
			default:
				break;
			}
			m_listShowRobots.SetItemText(nItem, i, str.c_str());
		}
		pRobot->SetListCtrlIndex(nItem);

		InsertRobot2ViewIndex(pRobot->GetRobotIndex() , nItem);
	}
// 	else
// 	{
// 		char * pName = "123123";
// 		LVITEM lvItem;
// 		lvItem.mask = LVIF_TEXT | LVIF_STATE;
// 		lvItem.iItem = 0;
// 		lvItem.iSubItem = 0;
// 		lvItem.pszText = pName;
// 		lvItem.lParam = (LPARAM)pRobot;
// 		INT32 nItem = m_listShowRobots.InsertItem(&lvItem);
// 
// 		m_listShowRobots.SetItemText(nItem, 0, pName);
// 
// 	}

	return TRUE;
}

void CDlgShowRobot::InitListCtrl()
{
	CRect rect;
	GetClientRect(rect);
	INT32 nDlgWidth = rect.right - rect.left;
	float fWidthRatio = (float)nDlgWidth / (float)cnMinRobotDlgWidth;

	INT32 nCount = LIST_START;
	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = (INT32)(cnColumnWidth * fWidthRatio);
	lvColumn.pszText = (char *)(cAStrListName[nCount].c_str());
	this->m_listShowRobots.InsertColumn(nCount++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = (INT32)(cnColumnWidth * fWidthRatio);
	lvColumn.pszText = (char *)(cAStrListName[nCount].c_str());
	this->m_listShowRobots.InsertColumn(nCount++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = (INT32)(cnColumnWidth * fWidthRatio);
	lvColumn.pszText = (char *)(cAStrListName[nCount].c_str());
	this->m_listShowRobots.InsertColumn(nCount++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = (INT32)(cnColumnWidth * 2 * fWidthRatio);
	lvColumn.pszText = (char *)(cAStrListName[nCount].c_str());
	this->m_listShowRobots.InsertColumn(nCount++, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = (INT32)(cnColumnWidth * 2 * fWidthRatio);
	lvColumn.pszText = (char *)(cAStrListName[nCount].c_str());
	this->m_listShowRobots.InsertColumn(nCount++, &lvColumn);

	//网格线（只适用与report风格的listctrl）
	//选中某行使整行高亮（只适用与report风格的listctrl）
	this->m_listShowRobots.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

BOOL CDlgShowRobot::ShowRobotBtn(INT32 nIndex, BOOL bShow/* = FALSE*/)
{
	MapRobotBtnT::iterator iter = m_mapRobotBtns.find(nIndex);
	if (iter != m_mapRobotBtns.end())
	{
		iter->second->ShowWindow(bShow);
	}

	SetScrollBarRange();
	return TRUE;
}

void CDlgShowRobot::ShowRobotBtn(CRobot * pRobot)
{
	if (m_nBtnCount < m_nCreateBtnIndex)
	{
		CRobotBtn * pBtn = m_mapRobotBtns[m_nBtnCount];
		if (pBtn)
		{
			pBtn->SetRobot(pRobot);
			pBtn->SetRobotIndex(pRobot->GetRobotIndex());
			pBtn->ShowWindow(TRUE);
			pBtn->Invalidate(TRUE);
			m_mapRobot2ViewIndex.insert(std::make_pair(pRobot->GetRobotIndex(), pBtn->GetRobotIndex()));
			++m_nBtnCount;
		}
	}
	else
	{
		gErrorStream("ShowRobotBtn error. cur btn count=" << m_nBtnCount << " max btn count=" << m_nCreateBtnIndex);
	}

	SetScrollBarRange();
}

BOOL CDlgShowRobot::InitDialog()
{
	InitListCtrl();

	ChangeDlgSize();
//	CreateRobotBtns();			//5 初始化先创建出所有的按钮.后续通过显隐来处理.暂时不通过按钮来处理了.

	return TRUE;
}

BOOL CDlgShowRobot::Init(RobotGroup * pRobotGroup /*= NULL*/)
{
//	ShowRobotBtns(FALSE);	//5 暂时不用显示按钮的方式,改为list
// 
// 	if (pRobotGroup)
// 	{
// 		RobotGroup::MapRobots mapRobots = pRobotGroup->GetMapRobots();
// 		RobotGroup::MapRobots::const_iterator iter = mapRobots.begin();
// 		for (; iter != mapRobots.end(); ++iter)
// 		{
// 			ShowRobotBtn(iter->second);
// 		}
// 	}

	m_pCurRobotGroup = pRobotGroup;
	if (m_pCurRobotGroup)
	{
		RobotGroup::MapRobots mapRobots = pRobotGroup->GetMapRobots();
		RobotGroup::MapRobots::const_iterator iter = mapRobots.begin();
		for (; iter != mapRobots.end(); ++iter)
		{
			InsertRobotList(pRobotGroup, iter->second);
		}
	}

	return TRUE;
}

BOOL CDlgShowRobot::Cleanup()
{
// 	ShowRobotBtns(FALSE);	//5 暂时不用显示按钮的方式,改为list

	m_pCurRobotGroup	= NULL;
	m_nCurRobotTabIndex = -1;
	m_nRobotCount		= 0;

	m_mapRobot2ViewIndex.clear();
	m_listShowRobots.DeleteAllItems();


/*  //5 暂时不用按钮显示通过list处理.
	m_nBtnCount			= 0;
	m_nBtnTopLeftX		= 0;
	m_nBtnTopLeftY		= 0;
	SetScrollPos(SBS_VERT, 0);
	SetScrollRange(SBS_VERT, 0, 0);

	SetScrollBarRange(); 
*/
	return TRUE;
}

void CDlgShowRobot::OnRobotBtnClicked(UINT nBtnID)
{

	MessageBox("点击成功.");
}

BOOL CDlgShowRobot::InsertRobot2ViewIndex(INT32 nRobotIndex, INT32 nViewIndex , std::string  strName)
{
	MapRobot2ViewIndexT::iterator iter = m_mapRobot2ViewIndex.find(nRobotIndex);
	if (iter == m_mapRobot2ViewIndex.end())
	{
		m_mapRobot2ViewIndex.insert(std::make_pair(nRobotIndex , nViewIndex));
	}
	else
	{
		iter->second = nViewIndex;
		gErrorStream("InsertRobot2ViewIndex is exist.strName=" << strName << ":nRobotIndex=" << nRobotIndex << ":nViewIndex=" << nViewIndex);
	}

	return TRUE;
}

void CDlgShowRobot::SetCurRobotTab(INT32 nIndex, RobotGroup * pRobot)
{
	if (nIndex != m_nCurRobotTabIndex)
	{
		Cleanup();
		if (nIndex != -1)
		{ 
			m_nCurRobotTabIndex = nIndex;
			Init(pRobot);
		}
	}
}

void CDlgShowRobot::CreateRobotBtns()
{
	for (INT32 i = 0; i < cnMaxRobotBtnNum; ++i)
	{
		CreateRobotBtn();
	}
}

BOOL CDlgShowRobot::ClearRobotBtns()
{
	MapRobotBtnT::iterator iter = m_mapRobotBtns.begin();
	for (; iter != m_mapRobotBtns.end(); ++iter)
	{
		iter->second->DestroyWindow();
		delete iter->second;
	}
	m_mapRobotBtns.clear();

	return TRUE;
}

void CDlgShowRobot::OnDeleteRobot(RobotGroup * pRobotGroup, CRobot * pRobot)
{

}

void CDlgShowRobot::OnCreateRobot(RobotGroup * pRobotGroup, CRobot * pRobot)
{
	CDlgRobotCtrl * pRobotDlg = dynamic_cast<CDlgRobotCtrl*>(theApp.m_pMainWnd);
	if (pRobotDlg)
	{
		if (m_nCurRobotTabIndex == pRobotDlg->GetCurRobotTabIndex() &&
			m_nCurListCtrlIndex == pRobotDlg->GetCurListCtrlIndex() &&
			m_pCurRobotGroup != NULL)
		{
			InsertRobotList(pRobotGroup , pRobot);
			//ShowRobotBtn(pRobot);	//5 暂时不用显示按钮的方式,改为list
		}
	}
}

void CDlgShowRobot::CreateRobotBtn()
{
	m_nBtnTopLeftX += cnBtnXLength + cnBtnXLengthSpace;	//5计算左上角X的位置.需要考虑btn的长度和btn之间的间隙

	CRect rect;
	this->GetClientRect(&rect);
	if (m_nBtnTopLeftX > rect.BottomRight().x)
	{
		m_nBtnTopLeftX = cnBtnXLength + cnBtnXLengthSpace;
		m_nBtnTopLeftY += cnBtnYLength + cnBtnYLengthSpace;
	}

	CRobotBtn * pBtn = new CRobotBtn(NULL);
	CRect pos = CRect(m_nBtnTopLeftX - cnBtnXLength
		, m_nBtnTopLeftY + cnBtnYLengthSpace + rect.top, m_nBtnTopLeftX
		, m_nBtnTopLeftY + cnBtnYLength + cnBtnYLengthSpace + rect.top);
	pBtn->SetRectPos(pos);
	pBtn->SetRobotIndex(-1);

	CString str;
	str.Format("%s", "None");
	pBtn->Create(str, WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_3STATE,
		pos, &m_txtRobotPlatform, cst_btn_id + m_nCreateBtnIndex); 

	m_mapRobotBtns.insert(std::make_pair(m_nCreateBtnIndex, pBtn));
	++m_nCreateBtnIndex;
}

BOOL CDlgShowRobot::ShowRobotBtns(BOOL bShow/* = FALSE*/)
{
	MapRobotBtnT::iterator iter = m_mapRobotBtns.begin();
	for (; iter != m_mapRobotBtns.end(); ++iter)
	{
		iter->second->ShowWindow(bShow);
	}
	m_mapRobot2ViewIndex.clear();

	m_txtRobotPlatform.Invalidate(TRUE);
	return TRUE;
}

void CDlgShowRobot::OnNMRClickListShowRobot(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	POINT   pt;
	GetCursorPos(&pt);

	CMenu temp, *pSubMenu;
	temp.LoadMenu(IDR_MENU_ROBOT_COMMAND);
	pSubMenu = temp.GetSubMenu(0);
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	


	*pResult = 0;
}

void CDlgShowRobot::OnSendRobotCommand()
{
	// TODO: 在此添加命令处理程序代码
}

void CDlgShowRobot::OnCreateRobots()
{
	// TODO: 在此添加命令处理程序代码

	if (m_pCurRobotGroup && m_pCurRobotGroup->GetRobotServer())
	{
		RobotServer * pRobotServer = m_pCurRobotGroup->GetRobotServer();
		rpc_CreateRobots(*pRobotServer , m_pCurRobotGroup->GetRobotSessionID() , 0 , m_pCurRobotGroup->GetObjectID() , 1 , 300);
	}

}
