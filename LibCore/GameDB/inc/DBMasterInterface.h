#ifndef __gamedb_db_master_interface_h__
#define __gamedb_db_master_interface_h__   
#include "GameDB/inc/DBCommon.h" 
#include "GameDB/inc/Environment.h"
#include "MsgLib/inc/RpcInterface.h" 
#include "json/json.h"

namespace GameDB
{     
	class DLL_EXPORT DBMasterInterface : public Msg::RpcInterface
	{   
	public:
		DBMasterInterface( );
		virtual ~DBMasterInterface(void);

	public: 
		virtual CErrno  Init(Json::Value & conf) override;
		virtual CErrno  Cleanup(void) override;
		virtual CErrno  Update(void) override;
	};
}

#endif
