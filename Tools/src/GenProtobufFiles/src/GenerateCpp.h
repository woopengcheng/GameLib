#ifndef __protobuf_generate_generate_cpp_h__
#define __protobuf_generate_generate_cpp_h__
#include "CUtil/inc/Common.h"
#include "GenerateBase.h"

namespace ProtobufGenerate
{
	class GenerateCPP : public GenerateBase
	{
	public:
		virtual UINT32 Init(google::protobuf::DescriptorPool & objPool , std::string strInputFile , std::string strOutputFile , std::string strNamespace);
		virtual UINT32 Cleanup();
		virtual UINT32 Generate();

	private:
		BOOL  GenerateMsgDefines();        //5 生成文件,包含所有的消息ID.
		BOOL  GenerateMsgProcess();        //5 生成每个消息对应的处理文件.每个消息会有一个Process.
		BOOL  GenerateMsgHandlerHeader();  //5 生成文件.根据消息ID找到对应的消息处理函数process.
		BOOL  GenerateMsgHandlerSource();  //5 生成文件.msgHandler的实现文件.
	};

}

#endif