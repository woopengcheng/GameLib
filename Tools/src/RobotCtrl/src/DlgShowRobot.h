#pragma once

#include "RobotGroup.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <string>

// CDlgShowRobot 对话框

class CRobotBtn;

using namespace Robot;
namespace Robot
{
	class CRobot;
}

class CDlgShowRobot : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowRobot)

public:
	typedef std_unordered_map<INT32, CRobotBtn *>	MapRobotBtnT;
	typedef std_unordered_map<INT32, INT32>			MapRobot2ViewIndexT;	//5 这里对应上了,就可以通过Btn找到RobotIndex了.

public:
	enum EListColType
	{
		LIST_START = 0,
		LIST_NAME = 0,
		LIST_PROF,
		LIST_LEVEL,
		LIST_STATUE,
		LIST_NOTE,

		LIST_NUM
	};
	const std::string cAStrListName[LIST_NUM] =
	{
		"名字",
		"职业",
		"等级",
		"状态",
		"备注",
	};
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

public:
	void				OnCreateRobot(RobotGroup * pRobotGroup, CRobot * pRobot);
	void				OnDeleteRobot(RobotGroup * pRobotGroup, CRobot * pRobot);

public:
	RobotGroup	*		GetCurRobotGroup() const { return m_pCurRobotGroup; }
	void				SetCurRobotTab(INT32 val, RobotGroup * pRobot);
	INT32				GetCurRobotTabIndex() const { return m_nCurRobotTabIndex; }
	INT32				GetCurListCtrlIndex() const { return m_nCurListCtrlIndex; }
	void				SetCurListCtrlIndex(INT32 val) { m_nCurListCtrlIndex = val; }

protected:
	BOOL				InsertRobotList(RobotGroup * pRobotGroup, CRobot * pRobot);
	void				InitListCtrl();

protected://5 此下面的针对按钮的功能,暂时去掉.
	void				ShowRobotBtn(CRobot * pRobot);
	BOOL				ClearRobotBtns();
	void				SetScrollBarRange();
	void				ChangeDlgSize();
	void				CreateRobotBtn();
	void				CreateRobotBtns();
	BOOL				ShowRobotBtns(BOOL bShow = FALSE);
	BOOL				ShowRobotBtn(INT32 nIndex, BOOL bShow/* = FALSE*/);
	void				OnRobotBtnClicked(UINT nBtnID);
	BOOL				InsertRobot2ViewIndex(INT32 nRobotIndex , INT32 nViewIndex, std::string strName="");

protected:
	CListCtrl			m_listShowRobots;

	RobotGroup		*	m_pCurRobotGroup;
	INT32				m_nCurRobotTabIndex;		//5 这里和主界面的tab的Index是对应的.
	INT32				m_nCurListCtrlIndex;		//5 当前RobotCtrl中的list控件选中的序号.
	MapRobot2ViewIndexT	m_mapRobot2ViewIndex;		//5 建立机器人Index对应的ViewIndex.
	INT32				m_nRobotCount;				//5 这个记录的是当前RobotGroup中应该有的Btn数量.

protected://5 此下面的针对按钮的,暂时去掉.
	CStatic				m_txtRobotPlatform;			// 显示按钮需要这么个平台.
	INT32				m_nBtnCount;				//5 这个记录的是当前RobotGroup中应该有的Btn数量.
	INT32				m_nCreateBtnIndex;			//5 这个用来第一次创建Btn的时候用于生成Index的.只在第一次有用..
	INT32				m_nBtnTopLeftX;				//5 这里需要计算按钮左上角的x位置
	INT32				m_nBtnTopLeftY;				//5 这里需要计算按钮左上角的y位置
	MapRobotBtnT		m_mapRobotBtns;
public:
	afx_msg void OnSendRobotCommand();
	afx_msg void OnNMRClickListShowRobot(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCreateRobots();
};
