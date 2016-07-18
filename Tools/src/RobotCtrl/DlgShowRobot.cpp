// DlgShowRobot.cpp : 实现文件
//

#include "stdafx.h"
#include "RobotCtrl.h"
#include "DlgShowRobot.h"
#include "afxdialogex.h"
#include "RobotBtn.h"
#include "DlgRobotCtrl.h"

// CDlgShowRobot 对话框

IMPLEMENT_DYNAMIC(CDlgShowRobot, CDialogEx)

INT32 GetXBtnNum(CRect rect)
{
	INT32 nXNumber = (rect.BottomRight().x - rect.TopLeft().x) / (cst_btn_pos_x_space + cst_btn_pos_x_length);
	return nXNumber;
}

INT32 GetYBtnNum(CRect rect)
{
	INT32 nYNumber = (rect.BottomRight().y - rect.TopLeft().y) / (cst_btn_pos_y_space + cst_btn_pos_y_length);
	return nYNumber;
}


BEGIN_MESSAGE_MAP(CDlgShowRobot, CDialogEx)
	ON_WM_VSCROLL()
	ON_WM_CREATE()
//	ON_COMMAND_RANGE(cst_btn_id, cst_btn_id + cst_max_btn_id, OnRobotBtnClicked)     //响应那个鼠标的ID，从10000到10000 + 最大的那个
END_MESSAGE_MAP()

CDlgShowRobot::CDlgShowRobot(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SHOW_ROBOT, pParent)
	, m_nCurRobotTabIndex(-1)
	, m_nCurListCtrlIndex(-1)
	, m_nBtnCount(0)
	, m_nBtnTopLeftX(0)
	, m_nBtnTopLeftY(0)
	, m_pCurRobotGroup(NULL)
{

}

CDlgShowRobot::~CDlgShowRobot()
{
	Cleanup();
}

BOOL CDlgShowRobot::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CRect rect;
	GetClientRect(rect);
	INT32 nXNumber = GetXBtnNum(rect);
	GetDlgItem(IDC_STATIC_ROBOT_BASE)->MoveWindow(CRect(0, -5, rect.BottomRight().x + 20, (MAX_ROBOT_BTN_NUM / nXNumber + 1) * (cst_btn_pos_y_space + cst_btn_pos_y_length)));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CDlgShowRobot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ROBOT_BASE, m_txtRobotPlatform);
}

void CDlgShowRobot::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	INT32 btnLineSize = cst_btn_pos_y_length + cst_btn_pos_y_space + cst_btn_pos_y_space;

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
	si.nMax		= (m_nBtnCount / nXNumber + 1) * (cst_btn_pos_y_space + cst_btn_pos_y_length);
	si.nPage	= rect.Height();//可见窗口高度和页大小设置一致
	si.nPos		= nPos;
	si.fMask	= SIF_ALL;
	si.cbSize	= sizeof(SCROLLINFO);
	SetScrollInfo(SBS_VERT, &si);
}

BOOL CDlgShowRobot::ClearRobotBtns()
{
	MapRobotButtonT::iterator iter = m_mapRobotBtns.begin();
	for (;iter != m_mapRobotBtns.end();++iter)
	{
		iter->second->DestroyWindow();
		delete iter->second;
	}
	m_mapRobotBtns.clear();

	return TRUE;
}

BOOL CDlgShowRobot::Cleanup()
{
	ClearRobotBtns();
	m_nBtnCount = 0;
	m_nBtnTopLeftX = 0;
	m_nBtnTopLeftY = 0;

	SetScrollPos(SBS_VERT, 0);
	SetScrollRange(SBS_VERT, 0, 0);

	return TRUE;
}

void CDlgShowRobot::OnRobotBtnClicked(UINT nBtnID)
{

	MessageBox(L"点击成功.");
}

void CDlgShowRobot::OnCreateRobot(RobotGroup * pRobotGroup, Robot * pRobot)
{
	CDlgRobotCtrl * pRobotDlg = dynamic_cast<CDlgRobotCtrl*>(theApp.m_pMainWnd);
	if (pRobotDlg)
	{
		if (m_nCurRobotTabIndex == pRobotDlg->GetCurRobotTabIndex() &&
			m_nCurListCtrlIndex == pRobotDlg->GetCurListCtrlIndex())
		{
			CreateRobotBtn(pRobot);
			SetScrollBarRange(); 
		}
	}
}

void CDlgShowRobot::OnDeleteRobot(RobotGroup * pRobotGroup, Robot * pRobot)
{

}

void CDlgShowRobot::CreateRobotBtn(Robot * pRobot)
{
	INT32 nIndex = pRobot->GetRobotIndex();

	m_nBtnTopLeftX += cst_btn_pos_x_length + cst_btn_pos_x_space;	//5计算左上角X的位置.需要考虑btn的长度和btn之间的间隙

	CRect rect;
	m_txtRobotPlatform.GetClientRect(&rect);
	if (m_nBtnTopLeftX > rect.BottomRight().x)
	{
		m_nBtnTopLeftX = cst_btn_pos_x_length + cst_btn_pos_x_space;
		m_nBtnTopLeftY += cst_btn_pos_y_length + cst_btn_pos_y_space;
	}

	CRobotBtn * pBtn = new CRobotBtn(pRobot);
	CRect pos = CRect(m_nBtnTopLeftX - cst_btn_pos_x_length
		, m_nBtnTopLeftY + cst_btn_pos_y_space + rect.top, m_nBtnTopLeftX
		, m_nBtnTopLeftY + cst_btn_pos_y_length + cst_btn_pos_y_space + rect.top);
	pBtn->SetRectPos(pos);
	pBtn->SetRobotIndex(nIndex);

	CString str;
	str.Format(L"%s", pRobot->GetName().c_str());
	pBtn->Create(str, WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE | BS_NOTIFY,
		pos, &m_txtRobotPlatform, cst_btn_id + nIndex);

	m_mapRobotBtns.insert(std::make_pair(nIndex, pBtn));
	++m_nBtnCount;
}



