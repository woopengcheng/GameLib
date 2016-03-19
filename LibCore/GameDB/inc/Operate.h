#ifndef __gamedb_operate_h__
#define __gamedb_operate_h__ 
#include "GameDB/inc/DBCommon.h"
#include "GameDB/inc/OperateRecord.h"
#include "GameDB/inc/OperateReturns.h"

namespace GameDB
{
	class DLL_EXPORT Operate
	{
	public:
		Operate()
			: m_nErrorCode(CErrno::Failure())
		{ 
		}
		virtual ~Operate()
		{

		}
	public:
		bool		IsSuccess(){ return m_nErrorCode == CErrno::Success(); }
		bool		IsNotFound(){ return m_nErrorCode == CErrno::NotFound(); }
		void		SetErrorCode(CErrno code){ m_nErrorCode = code; }
		void		SetErrorCode(const Status& status)
		{
			if(status.ok())
				m_nErrorCode = CErrno::Success();
			else if(status.IsIOError())
				m_nErrorCode = CErrno::IOError();
			else if(status.IsCorruption())
				m_nErrorCode = CErrno::Corruption();
			else if(status.IsNotFound())
				m_nErrorCode = CErrno::NotFound();
			else
				m_nErrorCode = CErrno::InvaildArgument();
		}
		CErrno  GetErrorCode(){ return m_nErrorCode; }

	public:
		OperateRecord  & GetOperateRecord()	{ return m_objOperateRecord;}
		OperateReturns & GetOperateReturns(){ return m_objOperateReturns; }

	private:
		CErrno	         m_nErrorCode;
		OperateRecord    m_objOperateRecord;
		OperateReturns   m_objOperateReturns;
	};
}

#endif
