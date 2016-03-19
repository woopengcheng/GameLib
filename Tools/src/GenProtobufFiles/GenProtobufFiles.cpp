// GenProtobufFiles.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "CUtil/inc/Common.h"
#include "GenerateHelper.h"
#include "GenerateManager.h"
#include "GenerateCpp.h"
#include <vector>

int _tmain(int argc, _TCHAR * argv[])
{ 
	if (argc != 4)
	{
		std::cout << "argv is not enough." <<std::endl;
		return FALSE;
	}

	std::string strInputFile = argv[1];
	std::string strOutputFile = argv[2];
	std::string strNamespace = argv[3];
	std::string strDirectoy = ProtobufGenerate::GenerateHelper::GetFilePath(strInputFile);
	if(strDirectoy.size() == 0 || strInputFile.size() == 0 || strNamespace.size() == 0 || strOutputFile.size() == 0)
	{
		std::cout << "The file length is wrong." << std::endl;
		return FALSE;
	}
	std::vector<std::string> strFiles;
	strFiles.push_back(strDirectoy + "google/protobuf/descriptor.proto");
	strFiles.push_back(strDirectoy + "options.proto");
	strFiles.push_back(strInputFile);

	ProtobufGenerate::GenerateManager objGenerateManager;
	if(!objGenerateManager.Init(strFiles))
	{
		std::cout << "ProtobufGenerate::GenerateManager Init fail." << std::endl;
		return FALSE;
	}

	ProtobufGenerate::GenerateCPP objGenerateCpp;
	objGenerateCpp.Init(objGenerateManager.GetPool() , strInputFile , strOutputFile , strNamespace);
	objGenerateManager.PushGenerater(&objGenerateCpp);

	objGenerateManager.DoGenerate();

	objGenerateCpp.Cleanup();
	return 0;
}

