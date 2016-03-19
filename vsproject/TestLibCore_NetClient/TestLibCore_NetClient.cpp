// TestNetLib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"  
#include "CUtil/inc/CUtil.h"
#include "MsgLib/inc/InnerMsg.h"
#include "ThreadPool/inc/ThreadTask.h"  
#include "ThreadPool/inc/ThreadPoolInterface.h"  
#include "client.h"

int _tmain(int argc, _TCHAR* argv[])
{   
	CUtil::Init("Client");

	int nCount = ThreadPool::ThreadPoolInterface::GetInstance().GetThreadsCount(); 	
	Client client;
 	client.Init();
//	client.InitUDP();
// 	client.InitZMQ();
//	client.InitRakNet();

	client.Update();


	CUtil::Cleanup(); 

	return 0;
}

