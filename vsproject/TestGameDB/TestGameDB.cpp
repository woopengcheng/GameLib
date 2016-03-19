// TestLibCore.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Marshal/CStream.h"
#include "Marshal/MashalTest.h"
#include "Common/CUtil.h"
#include "ctype.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CUtil::Init("InnerMsg");  

	printf("%d" , 0|-1 | -2);
	CUtil::CStream objCS;

	CUtil::MarshalTest objTest(1);
	CUtil::MarshalTest objTest2(2);
	objCS << objTest;
	objCS >> objTest2;

	std::set<int> s;
	s.insert(s.end() , 1);

	objCS << s;

	CUtil::Cleanup();
	return 0;
}

