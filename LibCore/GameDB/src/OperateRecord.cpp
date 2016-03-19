#include "GameDB/inc/OperateRecord.h"

namespace GameDB
{
	void OperateRecord::Insert(const Slice& dbKey,const Slice& dbValue)
	{
		std::string strKey = dbKey.ToString() , strValue = dbValue.ToString() ;
		m_objStream << OPERATE_RECODE_INSERT;
		m_objStream << strKey << strValue;

		++m_nCount;
	}

	void OperateRecord::Delete(const Slice& dbKey)
	{
		std::string strKey = dbKey.ToString();
		m_objStream << OPERATE_RECODE_DELETE;
		m_objStream << strKey; 

		++m_nCount;
	}

	CUtil::Chunk  OperateRecord::GetData()
	{ 
		if (!m_bAddCount)
		{
			m_objStream.Insert(m_objStream.GetData().Begin() , &m_nCount , sizeof(m_nCount));
			m_bAddCount = TRUE;
		}
		return m_objStream.GetData();
	}

}