#ifndef __protobuf_generate_generate_helper_h__
#define __protobuf_generate_generate_helper_h__ 
#include "CUtil/inc/Common.h"
#ifdef WIN32
#include <Windows.h>
#endif

namespace ProtobufGenerate
{
#define MsgProcessClass "GameMsgProcess"
#define MSG_HEADER_FLAG "CS"

#ifdef WIN32
	extern void GetStringFromGUID(GUID& FilterCLSID, char* lpszString); 
#endif

	class GenerateHelper
	{ 
	public: 
		static std::string   GenerateTabs(UINT32 unCount);
		static std::string   GetUUIDString();
		static BOOL          WriteToFile(std::vector<std::string> vtStr , std::string strOutputFile );
		static void          PrintOutput(std::vector<std::string>& output);
		static std::string   GetFilePath(std::string strPath);
		static std::string   GetFileName(std::string strPath);
		static std::string   GetFileNameAndSuffix(std::string strPath);
		static BOOL          IsExistFile(std::string strFile);

	protected:
#ifdef WIN32
		static void GetStringFromGUID(GUID& FilterCLSID, char* lpszString); 
#endif

	}; 
}

#endif