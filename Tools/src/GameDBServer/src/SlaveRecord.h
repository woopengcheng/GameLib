#ifndef __server_slave_record_h__
#define __server_slave_record_h__ 
#include "GameDB/inc/UserAuth.h"

namespace Server
{  
	class SlaveRecord
	{ 
	public:
		SlaveRecord(MasterHandler * pHandler)
			: m_pMaster(pHandler)
			, m_objSlaveID(-1)
			, m_nSlaveSessionID(0)
		{
		}

		~SlaveRecord()
		{
			 
		}

	public:
		INT32				GetSlaveSessionID() const { return m_nSlaveSessionID; }
		void				SetSlaveSessionID(INT32 val) { m_nSlaveSessionID = val; }
		Msg::Object			GetSlaveID() const { return m_objSlaveID; }
		void				SetSlaveID(Msg::Object val) { m_objSlaveID = val; }
		void				SetDBName(std::string strName){ m_strDBName = strName; }
		std::string			GetDBName( ){ return m_strDBName; }
		void				SetUserAuth(GameDB::UserAuth & objUserInfo){ m_objAuthInfo = objUserInfo; }
		GameDB::UserAuth	GetUserAuth( ){ return m_objAuthInfo; }

	private:
		INT32				m_nSlaveSessionID;
		Msg::Object			m_objSlaveID;		
		MasterHandler	*	m_pMaster;
		std::string			m_strDBName;
		GameDB::UserAuth	m_objAuthInfo;		
	}; 
}

#endif