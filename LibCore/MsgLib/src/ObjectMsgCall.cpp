#include "MsgLib/inc/ObjectMsgCall.h"
#include "Marshal/inc/CStream.h"

namespace Msg
{

	UINT32 ObjectMsgCall::RefreshSize()
	{  
		UINT32 unMsgLength =  m_unTargetsCount * sizeof(Object) + sizeof(m_ullMsgID) + sizeof(m_unTargetsCount) +
			sizeof(m_szMsgMethod) + m_objParams.GetSize() + sizeof(m_usPriority) + sizeof(m_objSource);
		return unMsgLength;
	}  

	void ObjectMsgCall::RefreshTargets()
	{  
		m_aTargets = (Object *)((const char * )this + sizeof(ObjectMsgCall));
	}

	CErrno ObjectMsgCall::CopyTo(ObjectMsgCall *& pMsg)
	{  
		pMsg = new(sizeof(Object)*m_unTargetsCount)ObjectMsgCall;

		pMsg->m_ullMsgID       = m_ullMsgID;
		pMsg->m_objSource      = m_objSource;
		pMsg->m_usPriority     = m_usPriority;
		m_objParams.CopyTo(pMsg->m_objParams);  
		memcpy(pMsg->m_szMsgMethod , m_szMsgMethod , MAX_MSG_METHOD_NAME_LENGTH); 

		pMsg->SetTargetsCount(m_unTargetsCount); 
		for (UINT32 i = 0;i < m_unTargetsCount;++i)
		{
			pMsg->m_aTargets[i] = m_aTargets[i];
		} 

		return CErrno::Success();
	} 

	CErrno ObjectMsgCall::CopyExcludeParams(ObjectMsgCall *& pMsg)
	{
		pMsg = new(sizeof(Object)*m_unTargetsCount)ObjectMsgCall;

		pMsg->m_ullMsgID       = m_ullMsgID;
		pMsg->m_objSource      = m_objSource;
		pMsg->m_usPriority     = m_usPriority; 
		memcpy(pMsg->m_szMsgMethod , m_szMsgMethod , MAX_MSG_METHOD_NAME_LENGTH); 

		pMsg->SetTargetsCount(m_unTargetsCount); 
		for (UINT32 i = 0;i < m_unTargetsCount;++i)
		{
			pMsg->m_aTargets[i] = m_aTargets[i];
		} 

		return CErrno::Success(); 
	}

	CErrno ObjectMsgCall::CopyExcludeParamsAndTargets(ObjectMsgCall *& pMsg ,const std::vector<Msg::Object> & vecTargets , Msg::Object objSrc)
	{
		pMsg = new(sizeof(Object)*(UINT32)(vecTargets.size()))ObjectMsgCall;

		pMsg->m_ullMsgID       = m_ullMsgID;
		pMsg->m_objSource      = objSrc;
		pMsg->m_usPriority     = m_usPriority; 
		memcpy(pMsg->m_szMsgMethod , m_szMsgMethod , MAX_MSG_METHOD_NAME_LENGTH); 

		pMsg->SetTargetsCount((UINT32)(vecTargets.size())); 
		for (UINT32 i = 0;i < vecTargets.size();++i)
		{
			pMsg->m_aTargets[i] = vecTargets[i];
		} 

		return CErrno::Success(); 
	}

	CUtil::CStream & ObjectMsgCall::marshal( CUtil::CStream & cs )const
	{ 
		cs << m_unTargetsCount;
		cs.Pushback(m_aTargets , m_unTargetsCount * sizeof(Object));
		cs << m_objSource << m_usPriority;
		cs.Pushback((void*)m_szMsgMethod , MAX_MSG_METHOD_NAME_LENGTH);
		cs << m_ullMsgID << m_objParams; 

		return cs;
	}

	CUtil::CStream & ObjectMsgCall::unMarshal( CUtil::CStream & cs )
	{
		cs >> m_unTargetsCount;

		void * pBuf = NULL;
		cs.Pop(pBuf , m_unTargetsCount * sizeof(Object));
		memcpy(m_aTargets , pBuf , m_unTargetsCount * sizeof(Object));
		 
		cs >> m_objSource >> m_usPriority;
		cs.Pop(pBuf , sizeof(m_szMsgMethod));
		memcpy(m_szMsgMethod , pBuf ,  sizeof(m_szMsgMethod)); 
		cs >> m_ullMsgID >> m_objParams; 

		return cs;
	}

	UINT32 ObjectMsgCall::GetPacketSize( void )
	{
		return RefreshSize();
	}

}