#ifndef __protobuf_generate_generate_manager_h__
#define __protobuf_generate_generate_manager_h__
#include "CUtil/inc/Common.h"
#include "GenerateBase.h"
#include <vector>

namespace ProtobufGenerate
{
	class GenerateManager
	{
	public:
		typedef std::vector<GenerateBase *> vectorGeneraterT;

	public:
		google::protobuf::DescriptorPool & GetPool();

	public:
		BOOL  Init(std::vector<std::string> strFiles);
		BOOL  PushGenerater(GenerateBase * pGenerater); 
		void  DoGenerate();

	private:
		BOOL  ParseProtoFile(std::string strFile ,google::protobuf::FileDescriptorProto * pFDP);

	private:
		vectorGeneraterT                  m_vtGenerators;
		google::protobuf::DescriptorPool  m_objPool; 
	};
	 
}

#endif