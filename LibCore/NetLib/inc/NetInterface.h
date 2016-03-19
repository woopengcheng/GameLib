#ifndef __net_net_interface_h__
#define __net_net_interface_h__
#include "NetLib/inc/NetCommon.h" 

namespace Net
{
	class MsgProcess;
	class MsgHandler;
	class NetBase;

	class DLL_EXPORT NetInterface
	{
	public:
		NetInterface(void);
		virtual ~NetInterface(void); 

	public:
		static NetInterface & GetInstance();

	public:
		virtual CErrno  Init(void);
		virtual CErrno  Cleanup(void); 
		virtual INT32	 AddClient(MsgProcess * pMsgProcess , char* pAddress, UINT16 usPort , BOOL bAutoManagerMemeory = FALSE);   
		
		//************************************
		// Method:    IsConnected
		// FullName:  Net::NetInterface::IsConnected
		// Access:    virtual public 
		// Returns:   BOOL 返回当前的连接是否成功
		// Qualifier:
		// Parameter: UINT32 unIndex
		//************************************
		virtual BOOL  IsConnected(UINT32 unClientIndex);
		  
		virtual CErrno  SendMsg(UINT32 unClientIndex , UINT32 unMsgID, const char* pBuffer, UINT32 unLength);
	  
	private: 
		MsgHandler   *   m_pMsgHandler; 

	};
}
#endif
