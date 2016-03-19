#ifndef __net_msg_wrapper_h__
#define __net_msg_wrapper_h__ 
#include "NetLib/inc/NetCommon.h"

namespace Net
{
	class MsgWrapper
	{
	public: 
		MsgWrapper(void) : m_unMsgID(0) , m_pMsg(NULL) , m_unMsgSize(0){}
		MsgWrapper(UINT32 unMsgID ,const char * pBuffer , UINT32 unMsgSize) : m_unMsgID(unMsgID) , m_unMsgSize(unMsgSize)
		{
			m_pMsg = new char[unMsgSize];
			memcpy(m_pMsg , pBuffer , unMsgSize);
		}
		~MsgWrapper(void)
		{
			SAFE_DELETE_ARRAY(m_pMsg);
		};

	public:
		UINT32            GetMsgID(void)   { return m_unMsgID; }
		UINT32            GetMsgSize(void) { return m_unMsgSize; }
		const char  *     GetMsg(void)     { return m_pMsg; }

	private:
		UINT32            m_unMsgID;
		char        *     m_pMsg;
		UINT32            m_unMsgSize;  //5 发送消息的大小.不包含消息头的大小
	};

}

#endif