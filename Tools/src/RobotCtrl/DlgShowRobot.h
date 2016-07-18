#pragma once


// CDlgShowRobot 对话框

class CDlgShowRobot : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowRobot)

public:
	CDlgShowRobot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgShowRobot();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SHOW_ROBOT };
#endif

public:
	INT32				GetCurRobotIndex() const { return m_nCurRobotIndex; }
	void				SetCurRobotIndex(INT32 val);

protected:
	virtual void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	INT32				m_nCurRobotIndex;			//5 这里和主界面的tab的Index是对应的.
};
