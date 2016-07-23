// E:\GitHub\GameLib\Tools\src\RobotCtrl\DlgRobotCommand.cpp : 实现文件
//

#include "stdafx.h"
#include "RobotCtrl.h"
#include "DlgRobotCommand.h"
#include "afxdialogex.h"


// CDlgRobotCommand 对话框

IMPLEMENT_DYNAMIC(CDlgRobotCommand, CDialogEx)

CDlgRobotCommand::CDlgRobotCommand(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_ROBOT_COMMAND, pParent)
{

}

CDlgRobotCommand::~CDlgRobotCommand()
{
}

void CDlgRobotCommand::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRobotCommand, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgRobotCommand::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgRobotCommand::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgRobotCommand 消息处理程序

void CDlgRobotCommand::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CDlgRobotCommand::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
