#ifndef __net_net_base_h__
#define __net_net_base_h__
#include "NetLib/inc/NetCommon.h"

namespace Net
{
	class MsgWrapper;

	class NetBase
	{
	public:
		NetBase() : m_bConnected(false){};
		virtual ~NetBase(){};

	public:
		virtual CErrno          Init(void) = 0;
		virtual CErrno          Cleanup(void) = 0;
		virtual CErrno          Connect(char* pIPAddress, UINT16 usPort) = 0;
		virtual CErrno          CloseConnection(void) = 0;
		virtual CErrno          SendMsg(MsgWrapper* pMsg) = 0;
		virtual MsgWrapper *   RecvMsg(void) = 0;
		virtual void           Run(void) = 0;
		virtual BOOL           IsConnected(void){ return 0; }

	public:
//		virtual INT32 SendSpace() = 0;

	protected:
		BOOL    m_bConnected;
	}; 

}

#endif