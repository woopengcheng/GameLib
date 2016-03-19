#ifndef __gamedb_operate_recode_h__
#define __gamedb_operate_recode_h__ 
#include "GameDB/inc/DBCommon.h"

namespace GameDB
{ 
	class DLL_EXPORT OperateRecord
	{
	public:
		static const INT32 OPERATE_RECODE_INSERT = 0;
		static const INT32 OPERATE_RECODE_DELETE = 1;

	public:
		OperateRecord()
			: m_nCount(0)
			, m_bAddCount(FALSE)
		{

		}

	public:
		void    Insert(const Slice& dbKey,const Slice& dbValue);
		void	Delete(const Slice& dbKey);
		CUtil::Chunk  GetData();

		CUtil::CStream & GetStream()
		{
			return m_objStream;
		}
	private:
		BOOL             m_bAddCount;
		INT32            m_nCount;
		CUtil::CStream m_objStream;
	};
}

#endif
