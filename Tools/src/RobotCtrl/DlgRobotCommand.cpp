// DlgRobotCommand.cpp : 实现文件
//

#include "stdafx.h"
#include "RobotCtrl.h"
#include "DlgRobotCommand.h"
#include "afxdialogex.h"
#include "CUtil/inc/CUtil.h"
#include "DlgShowRobot.h"
#include "RPCCallFuncs.h"

// CDlgRobotCommand 对话框

IMPLEMENT_DYNAMIC(CDlgRobotCommand, CDialogEx)

CDlgRobotCommand::CDlgRobotCommand(CDlgShowRobot * pDlg, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_ROBOT_COMMAND, pParent)
	, m_nCurParamsCount(0)
	, m_pDlgShowRobot(pDlg)
{

}

CDlgRobotCommand::~CDlgRobotCommand()
{
}

void CDlgRobotCommand::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SELECT_COMMAND, m_cobSelectCommand);

	//5 IDC_COMMAND_TXT1需要保证他们的Index都是连续的.
	for (INT32 i = 0; i < cnCommandParamCount; i++)
	{
		DDX_Control(pDX, IDC_COMMAND_TXT1 + i, m_txtCommandType[i]);
		DDX_Control(pDX, IDC_COMMAND_EDIT1 + i, m_editCommandValue[i]);
		DDX_Control(pDX, IDC_COMMAND_NOTE1 + i, m_txtCommandNote[i]);
	}
}

BEGIN_MESSAGE_MAP(CDlgRobotCommand, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgRobotCommand::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgRobotCommand::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT_COMMAND, &CDlgRobotCommand::OnCbnSelchangeComboSelectCommand)
END_MESSAGE_MAP()


// CDlgRobotCommand 消息处理程序

void CDlgRobotCommand::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CDlgShowRobot* pDlg = dynamic_cast<CDlgShowRobot*>(m_pDlgShowRobot);
	if (pDlg)
	{
		RobotGroup * pRobot = pDlg->GetCurRobotGroup();
		if (pRobot)
		{
			std::vector<Msg::Object>  vecItems;
			pDlg->GetAllSelectedItems(vecItems);
			if (vecItems.size() <= 0)
			{
				return;
			}

			CUtil::Parameter p[cnCommandParamCount];
			for (INT32 i = 0;i < m_nCurParamsCount;++i)
			{
				INT32 nIndex = m_mapRobotParamType[i];
				switch (nIndex)
				{
				case ROBOT_TYPE_ERR:
				{

				}break;
				case ROBOT_TYPE_INT8:
				{
					CString str;
					m_editCommandValue[i].GetWindowTextA(str);
					INT8 value = (INT8)(CUtil::atoi(str.GetBuffer()));
					p[i].MakeParameter(value);
				}break;
				case ROBOT_TYPE_UINT8:
				{
					CString str;
					m_editCommandValue[i].GetWindowTextA(str);
					UINT8 value = (UINT8)(CUtil::atoi(str.GetBuffer()));
					p[i].MakeParameter(value); 
				}break;
				case ROBOT_TYPE_INT16:
				{
					CString str;
					m_editCommandValue[i].GetWindowTextA(str);
					INT16 value = (INT16)(CUtil::atoi(str.GetBuffer()));
					p[i].MakeParameter(value);
				}break;
				case ROBOT_TYPE_UINT16:
				{
					CString str;
					m_editCommandValue[i].GetWindowTextA(str);
					UINT16 value = (UINT16)(CUtil::atoi(str.GetBuffer()));
					p[i].MakeParameter(value);
				}break;
				case ROBOT_TYPE_INT32:
				{
					CString str;
					m_editCommandValue[i].GetWindowTextA(str);
					INT32 value = (INT32)(CUtil::atoi(str.GetBuffer()));
					p[i].MakeParameter(value);
				}break;
				case ROBOT_TYPE_UINT32:
				{
					CString str;
					m_editCommandValue[i].GetWindowTextA(str);
					UINT32 value = (UINT32)(CUtil::atoi(str.GetBuffer()));
					p[i].MakeParameter(value);
				}break;
				case ROBOT_TYPE_INT64:
				{
					CString str;
					m_editCommandValue[i].GetWindowTextA(str);
					INT64 value = (INT64)(CUtil::atoi(str.GetBuffer()));
					p[i].MakeParameter(value);
				}break;
				case ROBOT_TYPE_UINT64:
				{
					CString str;
					m_editCommandValue[i].GetWindowTextA(str);
					UINT64 value = (UINT64)(CUtil::atoi(str.GetBuffer()));
					p[i].MakeParameter(value);
				}break;
				case ROBOT_TYPE_STRING:
				{
					CString str;
					m_editCommandValue[i].GetWindowTextA(str);
					std::string value = std::string(str.GetBuffer());
					p[i].MakeParameter(value);
				}break;
				default:
					break;
				}
			}

			INT32 nIndex = m_cobSelectCommand.GetCurSel();
			INT32 nCommandType = m_mapRobotCommands[nIndex].type;
			CUtil::Parameters ps;
			ps.GenParams(p[0], p[1], p[2], p[3], p[4], p[5]);
			CUtil::CStream cs;
			cs << nCommandType << ps;

			CUtilChunk & chunk = cs.GetData();
			rpc_SendRobotCommand(*(pRobot->GetRobotServer()), pRobot->GetRobotSessionID(), vecItems, pRobot->GetObjectID() , chunk);
		}
	}

	CDialogEx::OnOK();
}

void CDlgRobotCommand::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void ExportClassToLua(lua_State* L)
{
	lua_tinker::class_add<RobotParams>(L, "RobotParams", true);
	lua_tinker::class_con<RobotParams>(L, lua_tinker::constructor<RobotParams>::invoke);
	lua_tinker::class_mem<RobotParams>(L, "type", &RobotParams::type);
	lua_tinker::class_mem<RobotParams>(L, "default", &RobotParams::strDefault);
	lua_tinker::class_mem<RobotParams>(L, "note", &RobotParams::strNote);

	lua_tinker::class_add<RobotCommand>(L, "RobotCommand", true);
	lua_tinker::class_con<RobotCommand>(L, lua_tinker::constructor<RobotCommand>::invoke);
	lua_tinker::class_mem<RobotCommand>(L, "command", &RobotCommand::command);
	lua_tinker::class_mem<RobotCommand>(L, "type", &RobotCommand::type);
	lua_tinker::class_mem<RobotCommand>(L, "params", &RobotCommand::mapRobotParams); 
	lua_tinker::class_def<RobotCommand>(L, "SetParams", &RobotCommand::SetParams);
}

MapRobotCommands CDlgRobotCommand::m_mapRobotCommands;
MapRobotTypesT CDlgRobotCommand::m_mapRobotTypes;

BOOL CDlgRobotCommand::InitDialog()
{
	static lua_State * L = NULL;
	if (!L)
	{
		L = luaL_newstate();
		luaL_openlibs(L);
		lua_tinker::init(L);
		ExportClassToLua(L); 

		lua_tinker::dofile(L, "RobotCommand.lua");
		m_mapRobotTypes = lua_tinker::get<MapRobotTypesT>(L, "RobotTypes");
		m_mapRobotCommands = lua_tinker::get<MapRobotCommands>(L, "RobotCommands");
		  
		lua_close(L);
	}

	ShowDlgCtrls(cnCommandParamCount, FALSE);

	MapRobotCommands::iterator iter = m_mapRobotCommands.begin();
	for (;iter != m_mapRobotCommands.end(); ++iter)
	{
		INT32 nIndex = m_cobSelectCommand.AddString(iter->second.command.c_str());
		m_mapIndex2ConfigIndex.insert(std::make_pair(nIndex, iter->first));
	}
	
	return TRUE;
}

void CDlgRobotCommand::ShowDlgCtrls(INT32 nCount , BOOL bShow)
{
	for (INT32 i = 0; i < cnCommandParamCount; ++i)
	{
		m_txtCommandNote[i].ShowWindow(FALSE);
		m_txtCommandType[i].ShowWindow(FALSE);
		m_editCommandValue[i].ShowWindow(FALSE);
		m_nCurParamsCount = 0;
	}

	for (INT32 i = 0; i < nCount; ++i)
	{
		m_txtCommandNote[i].ShowWindow(bShow);
		m_txtCommandType[i].ShowWindow(bShow);
		m_editCommandValue[i].ShowWindow(bShow);
		if (bShow)
		{
			++m_nCurParamsCount;
		}
	}
}

BOOL CDlgRobotCommand::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgRobotCommand::OnCbnSelchangeComboSelectCommand()
{
	// TODO: 在此添加控件通知处理程序代码

	INT32 nIndex = m_cobSelectCommand.GetCurSel();
	MapIndexToConfigIndex::iterator iter = m_mapIndex2ConfigIndex.find(nIndex);
	if (iter != m_mapIndex2ConfigIndex.end())
	{
		nIndex = iter->second;
		MapRobotCommands::iterator iter2 = m_mapRobotCommands.find(nIndex);
		if (iter2 != m_mapRobotCommands.end())
		{
			RobotCommand & robot = iter2->second;

			ShowDlgCtrls(INT32(robot.mapRobotParams.size()), TRUE);
			for (INT32 i = 0; i < robot.mapRobotParams.size(); ++i)
			{
				m_txtCommandNote[i].SetWindowText(robot.mapRobotParams[i].strNote.c_str());
				m_mapRobotParamType.insert(std::make_pair(i , robot.mapRobotParams[i].type));
				m_txtCommandType[i].SetWindowText(m_mapRobotTypes[robot.mapRobotParams[i].type].c_str());
				m_editCommandValue[i].SetWindowText(robot.mapRobotParams[i].strDefault.c_str());
			}
		}
	}
}
