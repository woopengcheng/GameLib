#include "MsgLib/inc/MsgDispatcher.h"

namespace Msg
{

	MsgDispatcher::~MsgDispatcher()
	{
		m_mapCallableObjects.clear();

		//5 这里不清空.只是简单的删除.
		MapMethodImplT::iterator iterMethodImpl = m_mapMethodImpls.begin();
		for (;iterMethodImpl != m_mapMethodImpls.end();++iterMethodImpl)
		{
			SAFE_DELETE(iterMethodImpl->second);
		} 
		m_mapMethodImpls.clear();
	} 

	BOOL MsgDispatcher::HasSimilarRegisterFunc( std::string strFuncName  ,  std::string strFuncType)
	{
		std::string strFunc =  strFuncName + strFuncType; 

		MapMethodImplT::iterator result = m_mapMethodImpls.find(strFunc);
		if (result != m_mapMethodImpls.end())
		{
			return TRUE;
		} 

		return FALSE;
	}
	 
	CErrno MsgDispatcher::AddCallableObject( ICallableObject * pCallableObject )
	{
		MapCallableObjectsT::iterator result = m_mapCallableObjects.find(pCallableObject->GetObjectID());
		if (result == m_mapCallableObjects.end())
		{
			m_mapCallableObjects.insert(std::make_pair(pCallableObject->GetObjectID() , pCallableObject));

			return CErrno::Success();
		}
		return CErrno::Failure();
	}

	CErrno MsgDispatcher::DelCallableObject( ICallableObject * pCallableObject )
	{
		MapCallableObjectsT::iterator result = m_mapCallableObjects.find(pCallableObject->GetObjectID());
		if (result != m_mapCallableObjects.end())
		{ 
			m_mapCallableObjects.erase(pCallableObject->GetObjectID());

			return CErrno::Success();
		}
		return CErrno::Failure();
	}
	 
	 
	CErrno MsgDispatcher::Dispatcher( ObjectMsgCall * pObjectMsgCall ,Object obj )
	{
		Assert_ReF(pObjectMsgCall);
		ParaseMsgCall objParseMsgCall; 
		objParseMsgCall.m_pMsgCall = pObjectMsgCall;

		objParseMsgCall.m_pMehtodImpl = GetMethodImpl(objParseMsgCall.m_pMsgCall->m_szMsgMethod);  
		Assert_ReF(objParseMsgCall.m_pMehtodImpl); 

		if (objParseMsgCall.m_pMehtodImpl->m_bEnable)
		{
			if (objParseMsgCall.m_pMehtodImpl->m_cMethodType == METHOD_TYPE_STATIC)
			{
				objParseMsgCall.m_pMehtodImpl->m_pMethodImpl(&objParseMsgCall);
			}
			else
			{
				ICallableObject * pCallableObject = GetCallableObject(obj);
				Assert_ReF(pCallableObject);

				MapCallableObjectsT::iterator result = m_mapCallableObjects.find(pCallableObject->GetObjectID());
				if (result != m_mapCallableObjects.end())
				{
					objParseMsgCall.m_pObj = result->second;
					objParseMsgCall.m_pMehtodImpl->m_pMethodImpl(&objParseMsgCall);
				}
				else
					Assert_ReF(0);
			}

			return CErrno::Success();
		}
		else
		{
			gErrorStream("MsgDispatcher::Dispatch enabled = false, method=" << objParseMsgCall.m_pMsgCall->m_szMsgMethod);
		}

		return CErrno::Failure(); 
	}

	MethodImpl * MsgDispatcher::GetMethodImpl( std::string strFuncName )
	{
		MapMethodImplT::iterator result = m_mapMethodImpls.find(strFuncName); 
		if (result != m_mapMethodImpls.end())
		{
			return result->second;
		}
		return NULL; 
	}

	ICallableObject * MsgDispatcher::GetCallableObject( Object obj )
	{
		MapCallableObjectsT::iterator result = m_mapCallableObjects.find(obj);
		if (result != m_mapCallableObjects.end())
		{  
			return result->second;
		}
		return NULL; 
	}

}