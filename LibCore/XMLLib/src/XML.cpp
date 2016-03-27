#include "XMLLib/inc/xml.h"
#include "fstream"
#include "LogLib/inc/Log.h"
extern "C"
{
#include <expat.h> 
};

namespace XML
{
	INT32	g_nStartLoadXMLStaticLib = 0;
	INT32	g_nEndLoadXMLStaticLib = g_nStartLoadXMLStaticLib;
	void XMLCALL XML::StartElement( void * pUserData , const char * pName , const char ** pAttr )
	{
		MsgAssert(pUserData && pName, "StartElement error" << pName);

		XMLValue * pValues = (XMLValue*)pUserData;
		pValues->GetVectorParents().push_back(pName);
	}

	void XMLCALL XML::DataHandler( void * pUserData , const char * pValue , int nLength )
	{
		MsgAssert(pUserData && pValue , "DataHandler error" << pValue);

		XMLValue * pValues = (XMLValue*)pUserData;

		std::string strKey , strValue(pValue , nLength);
		strKey = pValues->GetParents();
		pValues->GetXMLValues().insert(std::make_pair(strKey , strValue));
	}

	void XMLCALL XML::EndElement( void * pUserData , const char * pName )
	{
		MsgAssert(pUserData && pName, "EndElement error" << pName);

		XMLValue * pValues = (XMLValue*)pUserData;

		pValues->GetVectorParents().pop_back();   //5 将最后一个值弹出去.
	}

	CErrno XML::LoadFromFile( const std::string strFilePath )
	{
		std::fstream fStream(strFilePath.c_str());

		if (!fStream.good())
		{
			return CErrno::Failure();
		}
		
		return LoadFromStream(fStream);
	}

	CErrno XML::LoadFromStream( std::istream & objStream )
	{
		XMLValue   objValue; 
		XML_Parser objParser = XML_ParserCreate(NULL);

		XML_SetUserData(objParser , &objValue);
		XML_SetCharacterDataHandler(objParser , DataHandler);
		XML_SetElementHandler(objParser , StartElement , EndElement);

		char szBuf[512] = {'\0'};
		while(1)
		{
			objStream.getline(szBuf , sizeof(szBuf));
			bool bEOF = objStream.eof();

			if (XML_Parse(objParser , szBuf , (INT32)strlen(szBuf) , bEOF ? 1 : 0) == XML_STATUS_ERROR)
			{
				gErrorStream( XML_ErrorString(XML_GetErrorCode(objParser))  << "at line " <<  XML_GetCurrentLineNumber(objParser));
				return CErrno::Failure();
			}
			if(bEOF)
				break;
		}

		XML_ParserFree(objParser);
		m_mapXMLValues = objValue.GetXMLValues();

		return CErrno::Success();
	}

	CErrno XML::LoadFromString( const std::string strXMLContent )
	{ 
		XMLValue   objValue; 
		XML_Parser objParser = XML_ParserCreate(NULL);

		XML_SetUserData(objParser , &objValue);
		XML_SetCharacterDataHandler(objParser , DataHandler);
		XML_SetElementHandler(objParser , StartElement , EndElement);
		 
		if (XML_Parse(objParser , strXMLContent.c_str() , (INT32)strXMLContent.length() , 1) == XML_STATUS_ERROR)
		{
			gErrorStream( XML_ErrorString(XML_GetErrorCode(objParser))  << "at line " <<  XML_GetCurrentLineNumber(objParser));
			return CErrno::Failure();
		} 

		XML_ParserFree(objParser);
		m_mapXMLValues = objValue.GetXMLValues();

		return CErrno::Success();
	}

	std::string XML::GetXMLValue( std::string strXMLPath , std::string strDefaultValue )
	{
		MapValuesT::iterator iter = m_mapXMLValues.find(strXMLPath);
		if(iter == m_mapXMLValues.end())
			return strDefaultValue;
		return iter->second;
	}

}
