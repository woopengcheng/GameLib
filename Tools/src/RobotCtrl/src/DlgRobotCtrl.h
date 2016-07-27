
// RobotCtrlDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DlgShowRobot.h"
#include "ListBoxEx.h"

using namespace Robot;
namespace Robot
{
	class RobotServer;
	class RobotGroup;
}

// CRobotCtrlDlg 对话框
class CDlgRobotCtrl : public CDialogEx
{
// 构造
public:
	CDlgRobotCtrl(CWnd* pParent = NULL);	// 标准构造函数

public:

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROBOTCTRL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
// 实现
public:
	// 生成的消息映射函数
	virtual BOOL		OnInitDialog();
	afx_msg void		OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void		OnPaint();
	afx_msg HCURSOR		OnQueryDragIcon();
	afx_msg void		OnTimer(UINT_PTR nIDEvent);
	afx_msg void		OnSelchangeListCtrlServer();
	afx_msg void		OnBnClickedStop();
	afx_msg void		OnBnClickedOk();
	afx_msg void		OnClose();
	afx_msg void		OnRBtnListServer();
	afx_msg void		OnTcnSelchangeTabShowRobot(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void		OnCloseAll();
	DECLARE_MESSAGE_MAP()

public:
	void				UpdateRobotTab(INT32 nIndex);
	void				UpdateCtrlServer(INT32 nIndex);
	void				OnCreateRobotGroup(RobotServer * pRobotServer, RobotGroup * pRobotGroup , BOOL bUpdate = FALSE);
	void				OnDeleteRobotGroup(RobotServer * pRobotServer, RobotGroup * pRobotGroup);
	void				OnCreateRobotServer(RobotServer * pRobot);
	void				OnDeleteRobotServer(RobotServer * pRobot);

public:
	CDlgShowRobot	&	GetDlgCurShowRobot() { return m_dlgCurShowRobot; }
	INT32				GetCurRobotTabIndex() const { return m_nCurRobotTabIndex; }
	INT32				GetCurListCtrlIndex() const { return m_nCurListCtrlIndex; }

protected:
	void				InitShowRobotDlg();

protected:
	HICON				m_hIcon;
	CEdit				m_editRobotStartNum;
	CEdit				m_editRobotEndNum;
	CStatic				m_txtTotalRobotNum;
	CStatic				m_txtOnlineRobotNum;
	CStatic				m_txtOfflineRobotNum;
	CStatic				m_txtShowRobotStatue;
	CTabCtrl			m_tabShowRobots;
	CListBoxEx			m_listCtrlServer;			//5 所有与这台计算机连接的机器人的电脑
	CDlgShowRobot		m_dlgCurShowRobot;			//5 显示当前机器人状态的tab页

	INT32				m_nCurRobotTabIndex;		//5 当前tab页显示机器人的tab index
	INT32				m_nShowRobotCount;			//5 当前tab页的数量
	INT32				m_nCurListCtrlIndex;		//5 当前list中选中的序号.
public:
};
