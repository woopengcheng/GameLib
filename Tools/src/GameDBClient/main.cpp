// GenRpcs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include <fstream>
#include <iostream>
#include <conio.h>

#include "CUtil/inc/CUtil.h" 
#include "CUtil/inc/Chunk.h" 
#include "MsgLib/inc/Object.h"
#include "MsgLib/inc/RpcManager.h"
#include "Timer/inc/TimerHelp.h" 
#include "CUtil/inc/carg_parser.h"
#include "GameDB/inc/RemoteNodeDefine.h"
#include "json/json.h" 
#include "MsgNameDefine.h"  
#include "RPCCallFuncs.h"
#include "DBClient.h" 
#include "ClientCommands.h"
#include "DBClientHttpServer.h"

#ifdef WIN32
#include <windows.h>
#include <process.h>
#include <stdio.h>

#define BUFFER_MAX 1024

char g_nbstdin_buffer[2][BUFFER_MAX];
HANDLE g_input[2];
HANDLE g_process[2];

DWORD WINAPI console_input(LPVOID lpParameter)
{
	for (;;) {
		int i;
		for (i = 0; i < 2; i++) {
			fgets(g_nbstdin_buffer[i], BUFFER_MAX, stdin);
			SetEvent(g_input[i]);
			WaitForSingleObject(g_process[i], INFINITE);
		}
	}
	return 0;
}

void create_nbstdin()
{
	int i;
	DWORD tid;
	CreateThread(NULL, 1024, &console_input, 0, 0, &tid);
	for (i = 0; i < 2; i++) {
		g_input[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_process[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_nbstdin_buffer[i][0] = '\0';
	}
}

char* nbstdin()
{
	DWORD n = WaitForMultipleObjects(2, g_input, FALSE, 0);
	if (n == WAIT_OBJECT_0 || n == WAIT_OBJECT_0 + 1) {
		n = n - WAIT_OBJECT_0;
		SetEvent(g_process[n]);
		return g_nbstdin_buffer[n];
	}
	else {
		return 0;
	}
}

char * ReadLine(const Client::ClientCommands & cc)
{
	char *line = nbstdin();
	if (line && strlen(line) > 1) {
		line[strlen(line)-1] = 0;
	}
	else {
		Sleep(0);
	}

	return line;
}
#endif
// 
// char * ReadLine(const Client::ClientCommands & cc)
// {
// 	static char line[1024] = "";
// 	memset(line , 0 , sizeof(line));
// 
// 	std::cout << "GameDBClient>";
// 	Timer::sleep(10);
// 	
// 	std::string strTab , strLastTab;
// 	char ch = 1;
// 	size_t i = 0;
// 	INT32 nTabCount = 0;
// 	while (ch != '\n')
// 	{ 
// 		ch = getch();
// #ifdef WIN32
// 		if (ch != '\r')
// #else
// 		if (ch != '\n')
// #endif
// 		{
// 			if (ch == '\t')
// 			{
// 				strTab = cc.GetNearestCommand(strLastTab , nTabCount);
// 				fflush(stdout);
// 				memset(line , 0 , sizeof(line));
// 				memcpy(line , strTab.c_str() , strTab.length() + 1);
// 				for (size_t j = i;j > 0;--j)
// 				{
// 					std::cout << '\b' << ' '<< '\b';
// 				}
// 				std::cout << strTab;
// 				i = strTab.length();
// 				++nTabCount;
// 				continue;
// 			}
// 			else if (ch == '\b')
// 			{
// 				if (i > 0)
// 				{
// 					std::cout << '\b' << ' ' << '\b';
// 					--i;
// 				}
// 				continue;
// 			}
// 			else
// 			{
// 				std::cout << ch;
// 			}
// 			line[i] = ch;
// 			strLastTab = line;
// 		}
// 		else
// 		{
// 			break;
// 		}
// 		++i;
// 	}
// 	std::cout << std::endl;
// 	 
// 	return line;
// }

void ParseLine(char * line , INT32 & argc , char ** argv)
{ 
	int result = 0;
	char* curr = line;
	char* last = curr;
	char  pre = ' ';
	while(*curr != 0)
	{
		if (*curr == ' ' && pre == ' ')
		{
			pre = *curr;
			++curr;
			continue;
		}

		if(*curr == ' ')
		{
			argv[result] = last;
			last = &curr[1];
			++result;
			*curr = 0;
		}
		pre = *curr;
		++curr;
	}
	if(*last != 0)
	{
		argv[result] = last;
		result++;
	}
	argc = result;
} 

void PackParams( std::vector<std::string> & vecParams , INT32 argc , char ** argv)
{ 
	if (argc <= 0)
	{
		return;
	}
	else
	{
		int i = 0;
		while(i < argc)
		{
			vecParams.push_back(argv[i]);
			++i;
		}
	}
} 

int _tmain(int argc, _TCHAR* argv[])
{  
#ifdef WIN32
	create_nbstdin();
#endif
	CUtil::Init("DBClient"); 

	std::string defaultConf = "./gdbClient.conf";
	if(argc > 1)
		defaultConf = argv[1];
	
	Json::Value root;
	JsonParase(defaultConf.c_str() , root); 
	 
	//5 建立一个单线程.用来处理Http链接
	Client::DBClientHttpServer::GetInstance().Init(root);

	//5 连接服务器,并建立双连接..
	Client::DBClient::GetInstance().Init(root);  
	while (1)
	{
		if (Client::DBClient::GetInstance().GetRpcManager()->IsConnected(g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER]) &&
			Client::DBClient::GetInstance().GetServerID() > 0)
		{
			break;
		}
		Client::DBClient::GetInstance().Update(); 
		Timer::sleep(1);
	}

	Client::ClientCommands & clientComands = *Client::g_pClientCommands;

	std::string strName = root.get("user" , "admin" ).asString();
	std::string strPwd = root.get("pwd" , "admin").asString();
	 
	Client::rpc_HandleUserAuth(g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER], Client::DBClient::GetInstance().GetServerID() , 0 , strName , strPwd, 0 , Msg::SYNC_TYPE_SYNC);

	std::vector<std::string> vecParams;
	int nargc = 0;
	char pargv[10][256];
	char **parg = (char **)pargv;
	while (1)
	{
		Client::DBClient::GetInstance().Update(); 

		if (Client::DBClient::GetInstance().GetRpcManager()->IsConnected(g_strGameDBNodes[NETNODE_DBCLIENT_TO_DBSERVER]))
		{
			char * pLine = ReadLine(clientComands);
			if (!pLine)
			{
				continue;
			}
			if (strcmp("exit" , pLine) == 0)
			{
				return -1;
			}

			vecParams.clear();

			ParseLine(pLine , nargc , parg);
			PackParams(vecParams , nargc , parg);
			 
			clientComands.Execute(&Client::DBClient::GetInstance(), vecParams);
			printf("GameDBClient>");
		}
	}

	Client::DBClient::GetInstance().Cleanup(); 
	CUtil::Cleanup();
	 
	system("pause");
	return 0;














	CUtil::Cleanup(); 
	return 0;
}

