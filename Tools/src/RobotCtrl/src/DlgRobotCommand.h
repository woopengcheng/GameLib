#pragma once


// CDlgRobotCommand 对话框

class CDlgRobotCommand : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRobotCommand)

public:
	CDlgRobotCommand(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRobotCommand();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_ROBOT_COMMAND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
