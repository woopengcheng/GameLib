#include "GenerateHelper.h"
#include <fstream>
#include <iostream>
#include <io.h>

#ifdef WIN32
#include <ImageHlp.h>
#pragma comment(lib , "imagehlp.lib")
#endif

namespace ProtobufGenerate
{ 
	std::string GenerateHelper::GenerateTabs( UINT32 unCount )
	{ 
		std::string strTabs;
		for (UINT32 i = 0;i < unCount;++i)
		{
			strTabs += "\t";
		}
		return strTabs;
	}
	
#ifdef WIN32 
	void GenerateHelper::GetStringFromGUID(GUID& FilterCLSID, char* lpszString)
	{
		sprintf(lpszString, "%08X_%04X_%04X_%02X%02X_%02X%02X%02X%02X%02X%02X",
			FilterCLSID.Data1, FilterCLSID.Data2, FilterCLSID.Data3,
			FilterCLSID.Data4[0], FilterCLSID.Data4[1], FilterCLSID.Data4[2], FilterCLSID.Data4[3], 
			FilterCLSID.Data4[4], FilterCLSID.Data4[5], FilterCLSID.Data4[6], FilterCLSID.Data4[7]
		);
	}
#endif 

	std::string GenerateHelper::GetUUIDString()
	{
		char szUUID[200] = "";
#ifdef WIN32
		GUID guid;
		CoCreateGuid(&guid);
		GetStringFromGUID(guid,szUUID);
#else
		uuid_t uid;
		uuid_generate(uid);
		uuid_unparse(uid,szUUID);
#endif
		for(int i = 0; i < sizeof(szUUID); ++i)
		{
			if(szUUID[i] == '-')
				szUUID[i] = '_';
			if(szUUID[i] == 0)
				break;
		}

		return szUUID;
	}

	BOOL GenerateHelper::WriteToFile( std::vector<std::string> vtStr , std::string strOutputFile )
	{
		if(IsExistFile(strOutputFile))
		{
			return FALSE;
		}
		if(!IsExistFile(GenerateHelper::GetFilePath(strOutputFile).c_str()))
		{
			int nResult = 0;
#ifdef WIN32
//			CreateDirectory(GenerateHelper::GetFilePath(strOutputFile).c_str() , NULL);
//			MakeSureDirectoryPathExists(GenerateHelper::GetFilePath(strOutputFile).c_str());
 			_mkdir(GenerateHelper::GetFilePath(strOutputFile).c_str()); 
#else
			nResult = mkdir(GenerateHelper::GetFilePath(strOutputFile).c_str(),S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
#endif  
			std::cout << "create director. " << strOutputFile << std::endl; 
		}

// #ifdef WIN32
// 		_unlink(strOutputFile.c_str());
// #else
// 		unlink(strOutputFile.c_str());
// #endif

		std::fstream f(strOutputFile.c_str(),std::ios::out);
		if(!f.good())
		{
			std::cout << "open file " << strOutputFile << " failed" << std::endl;
// 			f.open(strOutputFile.c_str());
		}

		for(UINT32 i = 0; i < vtStr.size(); ++i)
		{
			f.write(vtStr[i].c_str(),vtStr[i].length());
			f.write("\n",1);
		}

		f.close();
		return TRUE;
	}  

	void GenerateHelper::PrintOutput( std::vector<std::string>& output )
	{ 
		for(UINT32 i = 0; i < output.size(); ++i)
			std::cout << output[i] << std::endl; 
	}

	std::string GenerateHelper::GetFileNameAndSuffix( std::string strPath )
	{
		size_t i = strPath.rfind('/');
		if(i == strPath.npos)
			return "";
		return strPath.substr(i+1); 
	}

	std::string GenerateHelper::GetFileName( std::string strPath )
	{
		std::string strHeaderName = GetFileNameAndSuffix(strPath);
		size_t i = strHeaderName.find(".proto");
		if(i != std::string::npos)
		{
			return strHeaderName.substr(0,i);
		} 
		return "";
	}

	std::string GenerateHelper::GetFilePath( std::string strPath )
	{char *_getcwd(char *buffer, int maxlen);
		size_t i = strPath.rfind('/');
		if(i == strPath.npos)
			return "";
		if("./" == strPath.substr(0,i+1))
		{
			char szFile[250];
#ifdef WIN32
			GetCurrentDirectory(250 , szFile);
#else
			_getcwd(szFile , 250);
#endif
			std::string strFile = szFile;

//			return strFile + "\\";
		}

		return strPath.substr(0,i+1);
	}

	BOOL GenerateHelper::IsExistFile( std::string strFile )
	{
		return access(strFile.c_str(),0) == 0;   //5 有效返回0,否则-1
	}

}
