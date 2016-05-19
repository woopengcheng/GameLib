// TestLibCore.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "CUtil/inc/CUtil.h"
#include "LogLib/inc/Log.h"
#include <windows.h>

#include "behaviac/behaviac.h"
#include "behaviac/agent/taskmethod.h"
#include "src/BTTask.h"
#include "src/TaskCommon.h"

static void SetExePath()
{
	char szCurPath[_MAX_PATH];

	GetModuleFileNameA(NULL, szCurPath, _MAX_PATH);

	char* p = szCurPath;

	while (strchr(p, '\\'))
	{
		p = strchr(p, '\\');
		p++;
	}

	*p = '\0';

	SetCurrentDirectoryA(szCurPath);
}

static bool Register()
{
	behaviac::Agent::Register<Task::BTTask>();

	return true;
}

static bool UnRegister()
{
	behaviac::Agent::UnRegister<Task::BTTask>();

	return true;
}

Task::BTTask * g_btTask = NULL;
bool InitTask(const char* pszTreeName)
{
	g_btTask = behaviac::Agent::Create<Task::BTTask>();

	bool bRet = false;
	bRet = g_btTask->btload(pszTreeName);
	assert(bRet);

	g_btTask->btsetcurrent(pszTreeName);

	g_btTask->SetActive(false);

	return bRet;
}

Task::BTTask * g_btPostTask = NULL;
bool InitPostTask(const char* pszTreeName)
{
	g_btPostTask = behaviac::Agent::Create<Task::BTTask>();

	bool bRet = false;
	bRet = g_btPostTask->btload(pszTreeName);
	assert(bRet);

	g_btPostTask->btsetcurrent(pszTreeName);

	return bRet;
}

bool InitTaskMethod()
{
// 	CTagObjectDescriptor* objectDesc = NULL;
// 	behaviac::CCustomMethod* customeMethod = NULL;
// 	BEHAVIAC_UNUSED_VAR(objectDesc);
// 	BEHAVIAC_UNUSED_VAR(customeMethod);
// 
// 	customeMethod = BEHAVIAC_NEW behaviac::CTaskMethod("Task::BTTask", "event_task");
// 	customeMethod->AddParamType("int");
// 	customeMethod->AddParamType("bool");
// 	customeMethod->AddParamType("float");
// 	objectDesc->ms_methods.push_back(customeMethod);

	return true;
}

void CleanupTask()
{
	behaviac::Agent::Destroy(g_btTask); 
}

void CleanupPostTask()
{
	behaviac::Agent::Destroy(g_btPostTask);
}
static bool InitBehavic(behaviac::Workspace::EFileFormat ff,
	const char* szFilePath = "./Tasks/exported",
	const char* szExportMetaFile = "./Tasks/Task.xml")
{
	gDebugStream("InitBehavic");

	behaviac::Config::SetSocketing(true);
// #if !BEHAVIAC_COMPILER_MSVC
 	behaviac::Config::SetHotReload(false);
// #endif
	behaviac::Config::SetSocketBlocking(false);
	behaviac::Config::SetSocketPort(60636);
	
	Register();

	behaviac::Workspace::GetInstance()->SetFilePath(szFilePath);
	behaviac::Workspace::GetInstance()->SetFileFormat(ff);

	behaviac::Workspace::GetInstance()->ExportMetas(szExportMetaFile);

	behaviac::Agent::SetIdMask(kIdMask_BTTask);

	InitTask("marry");
	InitPostTask("posttask");
	InitTaskMethod();
	return true;
}

void UpdateLoop()
{
	behaviac::Workspace::GetInstance()->Update();
	static INT32 nCount = 0;
	if(nCount++ % 1000 == 0)
	{
// 		g_btTask->FireEvent("root");
		g_btPostTask->FireEvent("root");
	}
// 	int frames = 0;
// 	behaviac::EBTStatus status = behaviac::BT_RUNNING;
// 	
// 	while (status == behaviac::BT_RUNNING)
// 	{
// 		status = g_btTask->btexec();
// 	}
}

static bool CleanupBehaviac()
{
	CleanupTask();
	CleanupPostTask();
	UnRegister();
	behaviac::Workspace::GetInstance()->Cleanup();

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{   
	CUtil::Init();
	SetExePath();
	behaviac::Workspace::EFileFormat ff = behaviac::Workspace::EFF_xml;
	InitBehavic(ff);

	while (true)
	{
		UpdateLoop();
	}
	CleanupBehaviac();
	CUtil::Cleanup();
	return 0;
}

