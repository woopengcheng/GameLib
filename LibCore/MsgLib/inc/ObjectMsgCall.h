#ifndef __msg_object_msg_call_h__
#define __msg_object_msg_call_h__
#include "MsgLib/inc/MsgCommon.h"
#include "CUtil/inc/Parameters.h"
#include "MsgLib/inc/Object.h"

#ifdef WIN32
#pragma warning(disable : 4200)
#endif

namespace Msg
{ 
	class DLL_EXPORT  ObjectMsgCall : public CUtil::Marshal
	{ 
	public:
		ObjectMsgCall()
			: m_ullMsgID(0) 
			, m_unTargetsCount(0) 
			, m_usPriority(0) 
			, m_aTargets(NULL)
		{  
			RefreshTargets();
		}
		virtual ~ObjectMsgCall()
		{ 
		}

	public:
		
		//5 重载New.这个只是简单的调用底层的New.只是大小不确定.所以在这里更改了一下.每次只传入柔性数组的大小和参数的大小以及本体的大小.
		static void * operator new(size_t size , UINT32 unExtra)throw()
		{
			return malloc(size + unExtra); 
		}

		static void operator delete(void * p)throw()
		{
			free(p);
		}

		static void operator delete(void * p, UINT32 unExtra)throw()
		{
			free(p);
		}

		bool operator < (const ObjectMsgCall & obj)
		{
			return m_usPriority > obj.m_usPriority;  //5 从小到达,m_usPriority小的优先级高
		}

	public: 
		virtual UINT32				RefreshSize();   //5 注意:更改了发送对象的数量.一定要调用这个函数.
		virtual void				RefreshTargets();
		virtual UINT32				GetPacketSize( void );

	public:
		void						SetMethodName(const char * pName){ memcpy(m_szMsgMethod , pName , strlen(pName) + 1);}
		void						SetMethodNameByAddSuffix(const char * pName){ memcpy(m_szMsgMethod + strlen(m_szMsgMethod), pName , strlen(pName) + 1);}
		void						SetMethodNameBySubSuffix(const char * pName){ Assert_((strlen(m_szMsgMethod) - strlen(pName)) > 0);	m_szMsgMethod[strlen(m_szMsgMethod) - strlen(pName)] = '\0'; }
		CErrno						CopyTo(ObjectMsgCall *& pMsg);
		CErrno						CopyExcludeParams(ObjectMsgCall *& pMsg);
		CErrno						CopyExcludeParamsAndTargets(ObjectMsgCall *& pMsg ,const std::vector<Msg::Object> & vecTargets , Msg::Object objSrc);
		UINT32						GetTargetsCount(){ return m_unTargetsCount; }
		void						SetTargetsCount(UINT32 unTargetsCount){ m_unTargetsCount = unTargetsCount;  }

	public: 
		virtual CUtil::CStream &	marshal(CUtil::CStream & cs) const override;
		virtual CUtil::CStream &	unMarshal(CUtil::CStream & cs) override;

	public:      
		UINT64						m_ullMsgID;                                 //5 消息的ID,在进程内部.这个变量基本上无用了.
		CUtil::Parameters			m_objParams;                                //5 消息函数的参数. 
		UINT16						m_usPriority;								//5 优先级
		Object						m_objSource;								//5 消息源
		char						m_szMsgMethod[MAX_MSG_METHOD_NAME_LENGTH];  //5 调用消息的函数
		UINT32						m_unTargetsCount;
		Object		*				m_aTargets;									//5 本来想采用柔性数组.但是要继承就不可以.所以就替换了方案.
	};

	
}
#endif