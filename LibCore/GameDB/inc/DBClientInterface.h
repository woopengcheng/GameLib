#ifndef __gamedb_db_client_interface_h__
#define __gamedb_db_client_interface_h__   
#include "GameDB/inc/DBCommon.h" 
#include "MsgLib/inc/RpcInterface.h" 

namespace GameDB
{     
	class DLL_EXPORT DBClientInterface : public Msg::RpcInterface
	{   
	public:
		DBClientInterface( );
		virtual ~DBClientInterface(void);

	public: 
		virtual CErrno  Init(Json::Value & conf) override; 
		virtual CErrno  Cleanup(void) override;
		virtual CErrno  Update(void) override;
	};
}

#endif




// #ifndef __gamedb_db_client_h__
// #define __gamedb_db_client_h__   
// #include "GameDB/inc/DatabaseCommon.h"
// #include "ThreadPool/inc/ThreadTask.h"
// #include "NetLib/inc/INetReactor.h"
// #include "NetLib/inc/ISession.h"
// #include "NetLib/inc/NetReactorSelect.h"  
// #include "NetLib/inc/NetHandlerTransit.h"  
// #include "NetLib/inc/NetHelper.h"  
// #include "MsgLib/inc/MsgDispatcher.h" 
// #include "GameDB/inc/DBClientNetHandler.h"
// 
// 
// namespace GameDB
// {     
// 	class DLL_EXPORT DBClient : public Msg::MsgDispatcher , public ThreadPool::ThreadSustainTask
// 	{   
// 	public:
// 		DBClient( );
// 		virtual ~DBClient(void);
// 
// 	public:
// 		virtual INT32  Init(Json::Value & conf); 
// 		virtual INT32  Cleanup(void);
// 		virtual INT32  Update(void);  
// 
// 	public: 
// 		Net::INetReactor *			GetNetReactor() { return m_pNetReactor; }
// 		DBClientNetHandlerPtr	    GetNetHandler() { return m_pHandlers;}
// 		ErrCode					 	CreateNetHandler(const char * pAddress , UINT16 usPort);
// 
// 	protected:   
// 		Net::INetReactor		  * m_pNetReactor;          
// 		DBClientNetHandlerPtr	    m_pHandlers;  
// 	};
// 
// 	DECLARE_BOOST_POINTERS(DBClient); 
// }
// 
// #endif
