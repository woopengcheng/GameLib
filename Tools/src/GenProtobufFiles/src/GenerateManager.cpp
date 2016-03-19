#include "GenerateManager.h"
#include <algorithm>
#include <iostream>
#include "tokenizer.h"

namespace ProtobufGenerate
{
	BOOL GenerateManager::Init(std::vector<std::string> strFiles)
	{
		std::vector<std::string>::iterator iter = strFiles.begin();
		for (int i = 0;iter != strFiles.end();++iter , ++i)
		{
			std::string strFile = *iter;
			{
				google::protobuf::FileDescriptorProto objFDP;
				if(!ParseProtoFile(strFile , &objFDP))
				{
					std::cout << "parse file fails." << std::endl;
					return FALSE;
				} 
				if (strFile.find("google/protobuf/descriptor.proto") != std::string::npos)
				{
					objFDP.set_name("google/protobuf/descriptor.proto");
				}
				if (strFile.find("options.proto") != std::string::npos)
				{
					objFDP.set_name("options.proto");
				} 

				const google::protobuf::FileDescriptor * pFileDescriptor = m_objPool.BuildFile(objFDP);
				if (!pFileDescriptor)
				{
					std::cout << "DescriptorPool buildFile fail." << std::endl;
					return FALSE;
				}
			}
		}
		return TRUE;
	}

	BOOL GenerateManager::PushGenerater( GenerateBase * pGenerater )
	{
		vectorGeneraterT::iterator iter = std::find(m_vtGenerators.begin() , m_vtGenerators.end() , pGenerater);
		if (iter == m_vtGenerators.end())
		{
			m_vtGenerators.push_back(pGenerater);
			return TRUE;
		}

		return FALSE;
	}

	void GenerateManager::DoGenerate()
	{
		vectorGeneraterT::iterator iter = m_vtGenerators.begin();
		for (;iter != m_vtGenerators.end();++iter)
		{
			(*iter)->Generate();
		}
	}

	google::protobuf::DescriptorPool & GenerateManager::GetPool()
	{
		return m_objPool;
	}

	BOOL GenerateManager::ParseProtoFile( std::string strFile , google::protobuf::FileDescriptorProto * pFDP )
	{ 
		FILE * pProtoFile = fopen(strFile.c_str() , "r");
		if (pProtoFile)
		{
			google::protobuf::io::FileInputStream objInStream(fileno(pProtoFile));  //5 这里就是打开文件在流里面
			google::protobuf::io::Tokenizer objTokenizer(&objInStream , NULL);
			google::protobuf::compiler::Parser objParser;
			
			if (!objParser.Parse(&objTokenizer , pFDP))          //5 将文件解析在pFDP中.
			{
				fclose(pProtoFile);
				return FALSE;
			}
		}
		if (!pFDP->has_name())
		{
			pFDP->set_name(strFile);
		}

		if (pProtoFile)
		{
			fclose(pProtoFile);
		}
		return TRUE;
	}


}