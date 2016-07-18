#pragma once

#include "RobotGroup.h"
#include "afxwin.h"

// CDlgShowRobot 对话框

class CRobotBtn;

class CDlgShowRobot : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowRobot)

public:
	typedef std_unordered_map<int, CRobotBtn *>	MapRobotButtonT;

public:
	CDlgShowRobot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgShowRobot();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SHOW_ROBOT };
#endif

protected:
	virtual void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL		OnInitDialog();
	afx_msg void		OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int			OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	BOOL				Cleanup();
	void				SetScrollBarRange();
	BOOL				ClearRobotBtns();
	void				OnRobotBtnClicked(UINT nBtnID);
public:
	RobotGroup	*		GetCurRobotGroup() const { return m_pCurRobotGroup; }
	INT32				GetCurRobotTabIndex() const { return m_nCurRobotTabIndex; }
	void				SetCurRobotTabIndex(INT32 val) { m_nCurRobotTabIndex = val; }
	INT32				GetCurListCtrlIndex() const { return m_nCurListCtrlIndex; }
	void				SetCurListCtrlIndex(INT32 val) { m_nCurListCtrlIndex = val; }

public:
	void				OnCreateRobot(RobotGroup * pRobotGroup, Robot * pRobot);
	void				OnDeleteRobot(RobotGroup * pRobotGroup, Robot * pRobot);

protected:
	void				CreateRobotBtn(Robot * pRobot);

protected:
	CStatic				m_txtRobotPlatform;			// 显示按钮需要这么个平台.

	INT32				m_nBtnCount;
	INT32				m_nBtnTopLeftX;				//5 这里需要计算按钮左上角的x位置
	INT32				m_nBtnTopLeftY;				//5 这里需要计算按钮左上角的y位置
	INT32				m_nCurRobotTabIndex;		//5 这里和主界面的tab的Index是对应的.
	INT32				m_nCurListCtrlIndex;		//5 当前list中选中的序号.
	RobotGroup		*	m_pCurRobotGroup;
	MapRobotButtonT		m_mapRobotBtns;

};
