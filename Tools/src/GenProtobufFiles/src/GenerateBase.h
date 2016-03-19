#ifndef __protobuf_generate_generate_base_h__
#define __protobuf_generate_generate_base_h__
#include "CUtil/inc/Common.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/tokenizer.h>
#include <google/protobuf/compiler/parser.h> 
 
namespace ProtobufGenerate
{ 
	class GenerateBase
	{
	public:
		GenerateBase():m_pPool(NULL){}
	public:
		virtual UINT32 Init(google::protobuf::DescriptorPool & objPool , std::string strInputFile , std::string strOutputFile , std::string strNamespace) = 0;
		virtual UINT32 Cleanup() = 0;
		virtual UINT32 Generate() = 0; 

	protected: 
		std::string                       m_strInputFile;
		std::string                       m_strNamespace;
		std::string                       m_strOutputFile;
		google::protobuf::DescriptorPool* m_pPool;
	};  
}

#endif