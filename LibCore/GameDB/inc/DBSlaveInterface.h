#ifndef __gamedb_db_slave_interface_h__
#define __gamedb_db_slave_interface_h__   
#include "GameDB/inc/DBCommon.h" 
#include "MsgLib/inc/RpcInterface.h" 
#include "GameDB/inc/Environment.h"
#include "json/json.h"

namespace GameDB
{
	struct SDBSlaveInfo
	{
		std::string  strDBName;
		std::string  strDir;
		std::string  strUser;
		std::string  strPswd;

		SDBSlaveInfo()
			: strDBName("")
			, strDir("")
			, strUser("")
			, strPswd("")
		{

		}
	};


	class DLL_EXPORT DBSlaveInterface : public Msg::RpcInterface
	{   
	public:
		DBSlaveInterface( );
		virtual ~DBSlaveInterface(void);

	public: 
		virtual CErrno  Init(Json::Value & conf) override;
		virtual CErrno  Cleanup(void) override;
		virtual CErrno  Update(void) override;
		virtual void	OnCreateDatabase(const SDBSlaveInfo & objInfo){} 

	public:
		CErrno			InitDB(const Json::Value & conf);
		void			SetMasterID(Msg::Object nMasterID) { m_objMasterID = nMasterID; }
		Msg::Object		GetMasterID() { return m_objMasterID; }

	private:
		Msg::Object		m_objMasterID;
	};
}

#endif
