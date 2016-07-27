// DlgCreateRobotGroup.cpp : 实现文件
//

#include "stdafx.h"
#include "RobotCtrl.h"
#include "DlgCreateRobotGroup.h"
#include "afxdialogex.h"
#include "DlgShowRobot.h"
#include "RPCCallFuncs.h"

// CDlgCreateRobotGroup 对话框

IMPLEMENT_DYNAMIC(CDlgCreateRobotGroup, CDialogEx)

CDlgCreateRobotGroup::CDlgCreateRobotGroup(CDlgShowRobot * pDlg, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_CREATE_ROBOT_GROUP, pParent)
	, m_editStartNum(1)
	, m_editEndNum(3)
	, m_pDlgShowRobot(pDlg)
{

}

CDlgCreateRobotGroup::~CDlgCreateRobotGroup()
{
}

void CDlgCreateRobotGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITSTART_NUM, m_editStartNum);
	DDX_Text(pDX, IDC_EDIT_END_EDIT, m_editEndNum);
}


BEGIN_MESSAGE_MAP(CDlgCreateRobotGroup, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCreateRobotGroup::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCreateRobotGroup::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgCreateRobotGroup 消息处理程序


void CDlgCreateRobotGroup::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateData(FALSE);

	CDlgShowRobot* pDlg = dynamic_cast<CDlgShowRobot*>(m_pDlgShowRobot);
	if (pDlg)
	{
		RobotGroup * pRobot = pDlg->GetCurRobotGroup();
		if (pRobot && pRobot->GetRobotServer())
		{
			RobotServer * pRobotServer = pRobot->GetRobotServer();
			rpc_CreateRobots(*pRobotServer, pRobot->GetRobotSessionID(), 0, pRobot->GetObjectID(), m_editStartNum, m_editEndNum);
		}
	}


	CDialogEx::OnOK();
}


void CDlgCreateRobotGroup::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
