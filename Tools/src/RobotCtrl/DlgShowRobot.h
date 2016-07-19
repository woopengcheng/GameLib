#pragma once

#include "RobotGroup.h"
#include "afxwin.h"

// CDlgShowRobot 对话框

class CRobotBtn;

class CDlgShowRobot : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowRobot)

public:
	typedef std_unordered_map<INT32, CRobotBtn *>	MapRobotBtnT;
	typedef std_unordered_map<INT32, INT32>			MapRobot2BtnIndexT;	//5 这里对应上了,就可以通过Btn找到RobotIndex了.

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
	afx_msg void		OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	BOOL				InitDialog();				//5 这个是解决对话框调用OnInitDialog先后问题.
	BOOL				Init(RobotGroup * pRobotGroup = NULL);
	BOOL				Cleanup();
	void				OnRobotBtnClicked(UINT nBtnID);
	BOOL				ShowRobotBtns(BOOL bShow = FALSE);
	BOOL				ShowRobotBtn(INT32 nIndex, BOOL bShow/* = FALSE*/);

public:
	void				OnCreateRobot(RobotGroup * pRobotGroup, Robot * pRobot);
	void				OnDeleteRobot(RobotGroup * pRobotGroup, Robot * pRobot);

public:
	RobotGroup	*		GetCurRobotGroup() const { return m_pCurRobotGroup; }
	void				SetCurRobotTab(INT32 val, RobotGroup * pRobot);
	INT32				GetCurRobotTabIndex() const { return m_nCurRobotTabIndex; }
	INT32				GetCurListCtrlIndex() const { return m_nCurListCtrlIndex; }
	void				SetCurListCtrlIndex(INT32 val) { m_nCurListCtrlIndex = val; }

protected:
	void				CreateRobotBtns();
	void				CreateRobotBtn();
	void				ShowRobotBtn(Robot * pRobot);
	BOOL				ClearRobotBtns();
	void				SetScrollBarRange();
	void				ChangeDlgSize();

protected:
	CStatic				m_txtRobotPlatform;			// 显示按钮需要这么个平台.

	INT32				m_nBtnCount;				//5 这个记录的是当前RobotGroup中应该有的Btn数量.
	INT32				m_nCreateBtnIndex;			//5 这个用来第一次创建Btn的时候用于生成Index的.只在第一次有用..
	INT32				m_nBtnTopLeftX;				//5 这里需要计算按钮左上角的x位置
	INT32				m_nBtnTopLeftY;				//5 这里需要计算按钮左上角的y位置
	INT32				m_nCurRobotTabIndex;		//5 这里和主界面的tab的Index是对应的.
	INT32				m_nCurListCtrlIndex;		//5 当前list中选中的序号.
	RobotGroup		*	m_pCurRobotGroup;
	MapRobotBtnT		m_mapRobotBtns;
	MapRobot2BtnIndexT	m_mapRobot2BtnIndex;		//5 建立机器人Index对应的BtnIndex.

};
