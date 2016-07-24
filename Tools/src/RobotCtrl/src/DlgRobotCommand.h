#pragma once
#include "afxwin.h"
#include "RobotCommon.h"
#include "Lua/ILua.h"

// CDlgRobotCommand 对话框

const INT32 cnCommandParamCount = 6;

class CDlgShowRobot;

enum RobotParamType
{
	ROBOT_TYPE_ERR = 0,
	ROBOT_TYPE_INT8 = 1,
	ROBOT_TYPE_UINT8 = 2,
	ROBOT_TYPE_INT16 = 3,
	ROBOT_TYPE_UINT16 = 4,
	ROBOT_TYPE_INT32 = 5,
	ROBOT_TYPE_UINT32 = 6,
	ROBOT_TYPE_INT64 = 7,
	ROBOT_TYPE_UINT64 = 8,
	ROBOT_TYPE_STRING = 9

};
struct RobotParams
{
	INT32			type;
	std::string		strDefault;
	std::string		strNote;

	RobotParams()
		: type(ROBOT_TYPE_ERR)
	{

	}
};

typedef std_unordered_map<INT32, RobotParams>			MapRobotParamsT;
struct RobotCommand
{
	INT32			type;
	std::string		command;
	MapRobotParamsT	mapRobotParams;
	RobotCommand()
		: type(0)
	{
		mapRobotParams.reserve(1024);
	}
public:
	MapRobotParamsT & GetParams() { return mapRobotParams; }
	void SetParams(INT32 nIndex , const RobotParams & val) { mapRobotParams.insert(std::make_pair(nIndex , val)); }
};
typedef std::map<INT32, RobotCommand>		MapRobotCommands;
typedef std::map<INT32, std::string>		MapRobotTypesT;

class CDlgRobotCommand : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRobotCommand)

public:
	typedef std::map<INT32, INT32>			MapIndexToConfigIndex;
	typedef std::map<INT32, INT32>			MapRobotParamType;

public:
	CDlgRobotCommand(CDlgShowRobot * pDlg , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRobotCommand();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_ROBOT_COMMAND };
#endif

protected:
	afx_msg void			OnBnClickedOk();
	afx_msg void			OnBnClickedCancel();
	virtual BOOL			OnInitDialog();
	afx_msg void			OnCbnSelchangeComboSelectCommand();
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	BOOL					InitDialog();
	void					ShowDlgCtrls(INT32 nCount, BOOL bShow = FALSE);
	
protected:
	CStatic					m_txtCommandType[cnCommandParamCount];	//5 IDC_COMMAND_TXT1需要保证他们的Index都是连续的.
	CStatic					m_txtCommandNote[cnCommandParamCount];
	CEdit					m_editCommandValue[cnCommandParamCount];
	CComboBox				m_cobSelectCommand;
	MapIndexToConfigIndex	m_mapIndex2ConfigIndex;
	INT32					m_nCurParamsCount;
	MapRobotParamType		m_mapRobotParamType;
	CDlgShowRobot		*	m_pDlgShowRobot;

public:
	static MapRobotTypesT	m_mapRobotTypes;
	static MapRobotCommands m_mapRobotCommands;
};
