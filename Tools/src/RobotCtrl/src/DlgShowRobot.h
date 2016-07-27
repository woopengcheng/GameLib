#pragma once

#include "RobotGroup.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <string>
#include "DlgRobotCommand.h"
#include "DlgCreateRobotGroup.h"

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
	afx_msg int			OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void		OnSize(UINT nType, int cx, int cy);
	afx_msg void		OnSendRobotCommand();
	afx_msg void		OnNMRClickListShowRobot(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void		OnCreateRobots();
	afx_msg void		OnCloseGroup();
	afx_msg void		OnHdnItemclickListShowRobot(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	BOOL				InitDialog();				//5 这个是解决对话框调用OnInitDialog先后问题.
	BOOL				Init(RobotGroup * pRobotGroup = NULL);
	BOOL				Cleanup();

public:
	void				OnCreateRobot(RobotGroup * pRobotGroup, CRobot * pRobot);

public:
	RobotGroup	*		GetCurRobotGroup() const { return m_pCurRobotGroup; }
	void				SetCurRobotTab(INT32 val, RobotGroup * pRobot);
	INT32				GetCurRobotTabIndex() const { return m_nCurRobotTabIndex; }
	INT32				GetCurListCtrlIndex() const { return m_nCurListCtrlIndex; }
	void				SetCurListCtrlIndex(INT32 val) { m_nCurListCtrlIndex = val; }
	BOOL				GetAllSelectedItems(std::vector<Msg::Object> & vecItems);

protected:
	BOOL				InsertRobotList(RobotGroup * pRobotGroup, CRobot * pRobot);
	void				InitListCtrl();
	void				ChangeDlgSize(); 

protected:
	CListCtrl			m_listShowRobots;
	CDlgRobotCommand	m_dlgRobotCommand;
	CDlgCreateRobotGroup m_dlgCreateRobotGroup;

	RobotGroup		*	m_pCurRobotGroup;
	INT32				m_nCurRobotTabIndex;		//5 这里和主界面的tab的Index是对应的.
	INT32				m_nCurListCtrlIndex;		//5 当前RobotCtrl中的list控件选中的序号.
	INT32				m_nRobotCount;				//5 这个记录的是当前RobotGroup中应该有的Btn数量.
	BOOL				m_bSortUp;

};
