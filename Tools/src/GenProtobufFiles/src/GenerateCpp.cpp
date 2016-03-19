#include "GenerateCpp.h"
#include "GenerateHelper.h"
#include "iostream"
#include "tokenizer.h" 
#include "options.pb.h" 

namespace ProtobufGenerate
{
	UINT32 GenerateCPP::Init(google::protobuf::DescriptorPool & objPool , std::string strInputFile , std::string strOutputFile , std::string strNamespace)
	{
		m_pPool = &objPool;

		if (strOutputFile.size() == 0)
		{
			return FALSE;
		}
		m_strOutputFile = strOutputFile;

		if (strInputFile.size() == 0)
		{
			return FALSE;
		}
		m_strInputFile = strInputFile;
		 
		m_strNamespace = strNamespace;

		return TRUE;
	}

	UINT32 GenerateCPP::Cleanup()
	{

		return FALSE;
	}

	UINT32 GenerateCPP::Generate( )
	{
		if (!GenerateMsgDefines())
		{
			std::cout<< "GenerateMsgDefines false." <<std::endl;
			return FALSE;
		}
		if (!GenerateMsgProcess())
		{
			std::cout<< "GenerateMsgProcess false." <<std::endl;
			return FALSE;
		}
		if (!GenerateMsgHandlerHeader())
		{
			std::cout<< "GenerateMsgHandlerHeader false." <<std::endl;
			return FALSE;
		}
		if (!GenerateMsgHandlerSource())
		{
			std::cout<< "GenerateMsgHandlerSource false." <<std::endl;
			return FALSE;
		}

		return TRUE;
	}

	BOOL GenerateCPP::GenerateMsgDefines()
	{
		if (!m_pPool)
		{
			std::cout << "GenerateMsgDefines m_pPool is NULL." << std::endl;
			return FALSE;
		}
		else
		{
			std::vector<std::string> vtOutputs;
			const google::protobuf::FileDescriptor *pFileDescriptor = m_pPool->FindFileByName(m_strInputFile);
			if (!pFileDescriptor)
			{
				std::cout << "find FileDescriptor fail." << std::endl;
				return FALSE;
			}

			std::vector<std::string> vtNamespaces;
			tokenize(pFileDescriptor->package() , vtNamespaces , ".");  //5 将proto文件中的packet部分的解析结果按照.分开.
			 
			std::string strUUID = GenerateHelper::GetUUIDString();
			vtOutputs.push_back(std::string("#ifndef __msg_define_") + strUUID + "_h__");
			vtOutputs.push_back(std::string("#define __msg_define_") + strUUID + "_h__");
			vtOutputs.push_back(""); 
			vtOutputs.push_back(""); 

			int nNamespaceSize = vtNamespaces.size();
			for(int i = 0; i < nNamespaceSize; ++i)
			{
				std::string strTabs = GenerateHelper::GenerateTabs(i);
				vtOutputs.push_back(strTabs + "namespace " + vtNamespaces[i]);
				vtOutputs.push_back(strTabs + "{");
			}

			std::string strTabs = GenerateHelper::GenerateTabs(nNamespaceSize);
			for(int i = 0; i < pFileDescriptor->message_type_count(); ++i)
			{
				const google::protobuf::Descriptor* pDescriptor = pFileDescriptor->message_type(i);
				if (!pDescriptor)
				{
					std::cout << "find message type fail." << std::endl;
					return FALSE;
				} 

				int nID = pDescriptor->options().GetExtension(Net::nMsgID);    //5 额外定义的关键字.用来表示protobuf的ID.
				if(nID != 0)
				{
					char buf[100];
					sprintf(buf,"%u",nID);

					vtOutputs.push_back(strTabs + "const int msgid_" + pDescriptor->name() + " = " + buf + ";");
				}
			}

			for(int i = nNamespaceSize - 1; i >= 0; --i)
			{
				std::string strTabs = GenerateHelper::GenerateTabs(i);
				vtOutputs.push_back(strTabs + "} //end namespace " + vtNamespaces[i]);
				vtOutputs.push_back(""); 
			}
			vtOutputs.push_back(std::string("#endif //__msg_define_") + strUUID + "_h__\n");
			vtOutputs.push_back(""); 
		
			if(!GenerateHelper::WriteToFile(vtOutputs , m_strOutputFile + "/MsgDefine.h"))
			{
				std::cout << "WriteToFile msg_define fail" << std::endl;
				return FALSE;
			}
		}
		return TRUE;
	}

	BOOL GenerateCPP::GenerateMsgProcess()
	{
		if (!m_pPool)
		{
			std::cout << "GenerateMsgProcess m_pPool is NULL." << std::endl;
			return FALSE;
		}
		else
		{
			const google::protobuf::FileDescriptor *pFileDescriptor = m_pPool->FindFileByName(m_strInputFile);
			if (!pFileDescriptor)
			{
				std::cout << "find FileDescriptor fail." << std::endl;
				return FALSE;
			}

			std::vector<std::string> vtNamespaces;
			tokenize(pFileDescriptor->package() , vtNamespaces , ".");  //5 将proto文件中的packet部分的解析结果按照.分开.
			   
			std::string strMsgNamespace = "";
			for(UINT32 i = 0; i < vtNamespaces.size(); ++i)
				strMsgNamespace = strMsgNamespace + "::" + vtNamespaces[i];

			std::string strTabsDefine = GenerateHelper::GenerateTabs(1);
			std::string strTabsImp = GenerateHelper::GenerateTabs(2);
			std::string strTabsCase = GenerateHelper::GenerateTabs(3); 

			std::vector<std::string> vtOutputs;

			//5  生成每一个msg的process文件.
			int nMessageCount = pFileDescriptor->message_type_count();
			for (int i = 0 ;i < nMessageCount; ++i)
			{
				std::string strFile;
				vtOutputs.clear();
				const google::protobuf::Descriptor* pDescriptor = pFileDescriptor->message_type(i);
				if (!pDescriptor)
				{
					std::cout << "find message type fail." << std::endl;
					return FALSE;
				} 

				int nMsgID = pDescriptor->options().GetExtension(Net::nMsgID);    //5 额外定义的关键字.用来表示protobuf的ID.
				if(nMsgID != 0)
				{
					char szMsgID[100];
					sprintf(szMsgID,"%u",nMsgID); 

					strFile = pDescriptor->name();
					if (strFile.substr(0,2) == MSG_HEADER_FLAG)   //5 只生成CS头的文件
					{
						vtOutputs.push_back(std::string("#include \"") + MsgProcessClass + std::string(".h\"")); 
						vtOutputs.push_back("");
						vtOutputs.push_back("");
						vtOutputs.push_back("namespace " + m_strNamespace);
						vtOutputs.push_back("{");

						std::string strMsg = strMsgNamespace + "::" + pDescriptor->name();

						vtOutputs.push_back(strTabsDefine + "// msgid:" + szMsgID + " name:" + pDescriptor->name());
						vtOutputs.push_back(strTabsDefine + "INT32 " + MsgProcessClass + "::Process(Net::ISession * pSession , " + strMsg + "& msg)");
						vtOutputs.push_back(strTabsDefine + "{"); 
						vtOutputs.push_back(strTabsDefine + "\treturn -1;");
						vtOutputs.push_back(strTabsDefine + "}");
						vtOutputs.push_back("} //end namespace " + m_strNamespace);
						vtOutputs.push_back(""); 

						GenerateHelper::WriteToFile(vtOutputs , m_strOutputFile + "\\" + strFile + ".cpp");
					} 
				}

			} 
		}

		return TRUE;
	}

	BOOL GenerateCPP::GenerateMsgHandlerHeader()
	{
		if (!m_pPool)
		{
			std::cout << "GenerateMsgProcess m_pPool is NULL." << std::endl;
			return FALSE;
		}
		else
		{
			std::vector<std::string> vtOutputs;
			const google::protobuf::FileDescriptor *pFileDescriptor = m_pPool->FindFileByName(m_strInputFile);
			if (!pFileDescriptor)
			{
				std::cout << "find FileDescriptor fail." << std::endl;
				return FALSE;
			}

			std::string strTabsDefine = GenerateHelper::GenerateTabs(1);
			std::string strTabsFunc = GenerateHelper::GenerateTabs(2);
			std::string strTabsCase = GenerateHelper::GenerateTabs(3); 

			std::vector<std::string> vtNamespaces;
			tokenize(pFileDescriptor->package() , vtNamespaces , ".");  //5 将proto文件中的packet部分的解析结果按照.分开.
			
			std::string strMsgNamespace = "";
			for(UINT32 i = 0; i < vtNamespaces.size(); ++i)
				strMsgNamespace = strMsgNamespace + "::" + vtNamespaces[i];

			std::string strUUID = GenerateHelper::GetUUIDString();
			vtOutputs.push_back(std::string("#ifndef __msg_process_") + strUUID + "_h__");
			vtOutputs.push_back(std::string("#define __msg_process_") + strUUID + "_h__");
			vtOutputs.push_back("#include \"MsgProcess.h\"");
			vtOutputs.push_back("#include \"Common.h\"");
			vtOutputs.push_back("#include \"" + GenerateHelper::GetFileName(m_strInputFile) + ".pb.h\"");
			vtOutputs.push_back("");
			vtOutputs.push_back("namespace " + m_strNamespace);
			vtOutputs.push_back("{");
			vtOutputs.push_back(strTabsDefine + "class " + MsgProcessClass + ": public Net::MsgProcess"); 
			vtOutputs.push_back(strTabsDefine + "{");
			vtOutputs.push_back(strTabsDefine + "public:");
			vtOutputs.push_back(strTabsDefine + "\tvirtual BOOL IsAccept(UINT32 unMsgID) { return true; }");	
			vtOutputs.push_back("");
			vtOutputs.push_back(strTabsDefine + "public:");
			vtOutputs.push_back(strTabsDefine + "\t" + MsgProcessClass + "(){}");
			vtOutputs.push_back(strTabsDefine + "\tvirtual ~" + MsgProcessClass + "(){}");
			vtOutputs.push_back("");
			vtOutputs.push_back(strTabsDefine + "public:");
			vtOutputs.push_back(strTabsFunc + "virtual INT32 Process(Net::ISession * pSession , UINT32 unMsgID, const char* pBuffer, UINT32 unLength);");
			vtOutputs.push_back(strTabsDefine + "public:");

			//5  生成每一个msg的process文件.
			int nMessageCount = pFileDescriptor->message_type_count();
			for (int i = 0 ;i < nMessageCount; ++i)
			{ 
				const google::protobuf::Descriptor* pDescriptor = pFileDescriptor->message_type(i);
				if (!pDescriptor)
				{
					std::cout << "find message type fail." << std::endl;
					return FALSE;
				} 

				int nMsgID = pDescriptor->options().GetExtension(Net::nMsgID);    //5 额外定义的关键字.用来表示protobuf的ID.
				if(nMsgID != 0)
				{
					char szMsgID[100];
					sprintf(szMsgID,"%u",nMsgID); 

					std::string strName = pDescriptor->name();
					if (strName.substr(0,2) == MSG_HEADER_FLAG)   //5 只生成CS头的文件
					{  
						std::string strMsg = strMsgNamespace + "::" + pDescriptor->name();

						std::string strNote = std::string("// msgid:") + szMsgID + " name:" + pDescriptor->name();

						vtOutputs.push_back(strTabsFunc + "INT32 Process(Net::ISession * pSession ," + strMsg + " & msg);" + strNote); 
					} 
				} 
			} 
			vtOutputs.push_back(strTabsDefine + "};"); 
			vtOutputs.push_back("} //" + strMsgNamespace); 
			vtOutputs.push_back("");  
			vtOutputs.push_back("#endif // __msg_process_" + strUUID + "_h__"); 
			GenerateHelper::WriteToFile(vtOutputs , m_strOutputFile + "/" + MsgProcessClass +".h");
		}

		return TRUE; 
	}

	BOOL GenerateCPP::GenerateMsgHandlerSource()
	{
		if (!m_pPool)
		{
			std::cout << "GenerateMsgProcess m_pPool is NULL." << std::endl;
			return FALSE;
		}
		else
		{
			std::vector<std::string> vtOutputs;
			const google::protobuf::FileDescriptor *pFileDescriptor = m_pPool->FindFileByName(m_strInputFile);
			if (!pFileDescriptor)
			{
				std::cout << "find FileDescriptor fail." << std::endl;
				return FALSE;
			}

			std::string strTabsDefine = GenerateHelper::GenerateTabs(1);
			std::string strTabsFunc = GenerateHelper::GenerateTabs(2);
			std::string strTabsCase = GenerateHelper::GenerateTabs(3); 

			std::vector<std::string> vtNamespaces;
			tokenize(pFileDescriptor->package() , vtNamespaces , ".");  //5 将proto文件中的packet部分的解析结果按照.分开.

			std::string strMsgNamespace = "";
			for(UINT32 i = 0; i < vtNamespaces.size(); ++i)
				strMsgNamespace = strMsgNamespace + "::" + vtNamespaces[i];
		
			vtOutputs.push_back(std::string("#include \"") + MsgProcessClass + std::string(".h\"")); 
			vtOutputs.push_back("#include \"MsgDefine.h\"");  
			vtOutputs.push_back("");  
			vtOutputs.push_back("");  
			vtOutputs.push_back("namespace " + m_strNamespace);
			vtOutputs.push_back("{");
			vtOutputs.push_back(strTabsDefine + "INT32 " + MsgProcessClass + "::Process(Net::ISession * pSession , UINT32 unMsgID, const char* pBuffer, UINT32 unLength)");
			vtOutputs.push_back(strTabsDefine + "{");
			vtOutputs.push_back(strTabsFunc + "using namespace " + strMsgNamespace + ";");
			vtOutputs.push_back(strTabsFunc + "");
			vtOutputs.push_back(strTabsFunc + "if(!IsAccept(unMsgID))");
			vtOutputs.push_back(strTabsFunc + "\treturn -1;");
			vtOutputs.push_back(strTabsFunc + "");
			vtOutputs.push_back(strTabsFunc + "switch(unMsgID)");
			vtOutputs.push_back(strTabsFunc + "{");
			  
			//5  生成每一个msg的process文件.
			int nMessageCount = pFileDescriptor->message_type_count();
			for (int i = 0 ;i < nMessageCount; ++i)
			{ 
				const google::protobuf::Descriptor* pDescriptor = pFileDescriptor->message_type(i);
				if (!pDescriptor)
				{
					std::cout << "find message type fail." << std::endl;
					return FALSE;
				} 

				int nMsgID = pDescriptor->options().GetExtension(Net::nMsgID);    //5 额外定义的关键字.用来表示protobuf的ID.
				if(nMsgID != 0)
				{
					char szMsgID[100];
					sprintf(szMsgID,"%u",nMsgID); 

					std::string strName = pDescriptor->name();
					if (strName.substr(0,2) == MSG_HEADER_FLAG)   //5 只生成CS头的文件
					{  
						vtOutputs.push_back(strTabsFunc + "case " + szMsgID + ":");
						vtOutputs.push_back(strTabsCase + "{");
						vtOutputs.push_back(strTabsCase + "\t" + pDescriptor->name() + " objProtoMsg;");
						vtOutputs.push_back(strTabsCase + "\tif(!objProtoMsg.ParseFromArray(pBuffer,unLength))");
						vtOutputs.push_back(strTabsCase + "\t\treturn -1;");
						vtOutputs.push_back(strTabsCase + "\treturn Process(pSession , objProtoMsg);");
						vtOutputs.push_back(strTabsCase + "}");
						vtOutputs.push_back(strTabsCase + "break;");
					} 
				} 
			} 

			vtOutputs.push_back(strTabsFunc + "default:");
			vtOutputs.push_back(strTabsCase + "{");
			vtOutputs.push_back(strTabsCase + "\tprintf(\"%d\" , unMsgID);");
			vtOutputs.push_back(strTabsCase + "} //end default");
			vtOutputs.push_back(strTabsCase + "break;");
			vtOutputs.push_back(strTabsFunc + "} //end switch");
			vtOutputs.push_back(strTabsFunc + "return -1;");
			vtOutputs.push_back(strTabsDefine + "}");
			vtOutputs.push_back("");

			// ----------------------------------------------------

			vtOutputs.push_back("} //end namespace " + strMsgNamespace);

			GenerateHelper::WriteToFile(vtOutputs , m_strOutputFile + "/" + MsgProcessClass +".cpp");
		} 
		return TRUE;
	}


}