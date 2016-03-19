#ifndef __xml_xml_value_h__
#define __xml_xml_value_h__   
#include "CUtil/inc/Common.h"

namespace XML
{  
	typedef std_unordered_map<std::string , std::string>  MapValuesT;

	class XMLValue
	{ 
	public:
		std::string GetParents()
		{
			std::string strResult;
			
			for (int i = 0;i < m_vecParents.size();++i)
			{
				strResult.append("/").append(m_vecParents[i]);
			}

			return strResult;
		}

	public:
		MapValuesT & GetXMLValues(){ return m_mapXMLValues; }
		std::vector<std::string> & GetVectorParents(){ return m_vecParents; }

	private:
		MapValuesT                m_mapXMLValues;
		std::vector<std::string>  m_vecParents;  //5 记录每个值得层级关系 
	};
}

#endif