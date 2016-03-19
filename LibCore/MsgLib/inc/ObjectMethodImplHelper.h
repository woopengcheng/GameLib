#ifndef __msg_object_method_impl_helper_h__
#define __msg_object_method_impl_helper_h__ 
#include "MsgLib/inc/MethodImpl.h" 
#include "MsgLib/inc/ParaseMsgCall.h"
#include "CUtil/inc/ParameterHelper.h"
#include "MsgLib/inc/Object.h"

namespace Msg
{  
	class DLL_EXPORT  ObjectMethodImplHelper
	{
	public:
		template< typename ClassObject , typename Func>
		static ObjectMsgCall * CallFuncEntry(ParaseMsgCall * pPMC)   //5 类成员方法函数的入口
		{
			Assert_Re0(pPMC && pPMC->m_pMehtodImpl && pPMC->m_pMsgCall && pPMC->m_pObj);

			ClassObject * pObj = static_cast<ClassObject*>(pPMC->m_pObj); 

			ObjectMethodImpl< ClassObject , Func> * pMehtodImpl = static_cast<ObjectMethodImpl< ClassObject , Func>*>(pPMC->m_pMehtodImpl);
			return CallFunc< ClassObject>(pPMC, *pObj , pMehtodImpl->m_objFunc);
		}

	public:
		template< typename ClassObject>
		static ObjectMsgCall * CallFunc(ParaseMsgCall * pPMC,ClassObject & obj , ObjectMsgCall * (ClassObject::*func)(INT32 nSessionID , Object ))
		{
			Assert_Re0(pPMC && pPMC->m_pMsgCall);
			return (obj.*func)(pPMC->m_nSessionID , pPMC->m_pMsgCall->m_objSource);
		}

		template< typename ClassObject , typename P1>
		static ObjectMsgCall *  CallFunc(ParaseMsgCall * pPMC,ClassObject & obj , ObjectMsgCall * (ClassObject::*func)(INT32 nSessionID , Object , P1 ))
		{
			Assert_Re0(pPMC && pPMC->m_pMsgCall);
			return (obj.*func)(pPMC->m_nSessionID , pPMC->m_pMsgCall->m_objSource , CUtil::ParameterHelper<P1>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[0]));
		}

		template< typename ClassObject , typename P1 , typename P2>
		static ObjectMsgCall * CallFunc(ParaseMsgCall * pPMC,ClassObject & obj , ObjectMsgCall * (ClassObject::*func)(INT32 nSessionID , Object , P1 , P2 ))
		{
			Assert_Re0(pPMC && pPMC->m_pMsgCall);
			return (obj.*func)(pPMC->m_nSessionID , pPMC->m_pMsgCall->m_objSource , 
				CUtil::ParameterHelper<P1>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[0]) , 
				CUtil::ParameterHelper<P2>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[1]));
		}

		template< typename ClassObject , typename P1 , typename P2 , typename P3>
		static ObjectMsgCall * CallFunc(ParaseMsgCall * pPMC,ClassObject & obj , ObjectMsgCall * (ClassObject::*func)(INT32 nSessionID , Object , P1 , P2 , P3 ))
		{
			Assert_Re0(pPMC && pPMC->m_pMsgCall);
			return (obj.*func)(pPMC->m_nSessionID , pPMC->m_pMsgCall->m_objSource , 
				CUtil::ParameterHelper<P1>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[0]) , 
				CUtil::ParameterHelper<P2>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[1]) ,
				CUtil::ParameterHelper<P3>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[2]));
		}

		template< typename ClassObject , typename P1 , typename P2 , typename P3 , typename P4>
		static ObjectMsgCall * CallFunc(ParaseMsgCall * pPMC,ClassObject & obj , ObjectMsgCall * (ClassObject::*func)(INT32 nSessionID , Object , P1 , P2 , P3 , P4 ))
		{
			Assert_Re0(pPMC && pPMC->m_pMsgCall);
			return (obj.*func)(pPMC->m_nSessionID , pPMC->m_pMsgCall->m_objSource , 
				CUtil::ParameterHelper<P1>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[0]) , 
				CUtil::ParameterHelper<P2>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[1]) ,
				CUtil::ParameterHelper<P3>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[2]) ,
				CUtil::ParameterHelper<P4>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[3]));
		}

		template< typename ClassObject , typename P1 , typename P2 , typename P3 , typename P4 , typename P5>
		static ObjectMsgCall * CallFunc(ParaseMsgCall * pPMC,ClassObject & obj , ObjectMsgCall * (ClassObject::*func)(INT32 nSessionID , Object , P1 , P2 , P3 , P4 , P5 ))
		{
			Assert_Re0(pPMC && pPMC->m_pMsgCall);
			return (obj.*func)(pPMC->m_nSessionID , pPMC->m_pMsgCall->m_objSource , 
				CUtil::ParameterHelper<P1>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[0]) , 
				CUtil::ParameterHelper<P2>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[1]) ,
				CUtil::ParameterHelper<P3>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[2]) ,
				CUtil::ParameterHelper<P4>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[3]) ,
				CUtil::ParameterHelper<P5>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[4]));
		}

		template< typename ClassObject , typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 >
		static ObjectMsgCall * CallFunc(ParaseMsgCall * pPMC,ClassObject & obj , ObjectMsgCall * (ClassObject::*func)(INT32 nSessionID , Object , P1 , P2 , P3 , P4 , P5 , P6 ))
		{ 
			Assert_Re0(pPMC && pPMC->m_pMsgCall);
			return (obj.*func)(pPMC->m_nSessionID , pPMC->m_pMsgCall->m_objSource , 
				CUtil::ParameterHelper<P1>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[0]) , 
				CUtil::ParameterHelper<P2>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[1]) ,
				CUtil::ParameterHelper<P3>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[2]) ,
				CUtil::ParameterHelper<P4>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[3]) ,
				CUtil::ParameterHelper<P5>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[4]) ,
				CUtil::ParameterHelper<P6>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[5]));
		}

		template< typename ClassObject , typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7>
		static ObjectMsgCall * CallFunc(ParaseMsgCall * pPMC,ClassObject & obj , ObjectMsgCall * (ClassObject::*func)(INT32 nSessionID , Object , P1 , P2 , P3 , P4 , P5 , P6 , P7 ))
		{ 
			Assert_Re0(pPMC && pPMC->m_pMsgCall);
			return (obj.*func)(pPMC->m_nSessionID , pPMC->m_pMsgCall->m_objSource , 
				CUtil::ParameterHelper<P1>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[0]) , 
				CUtil::ParameterHelper<P2>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[1]) ,
				CUtil::ParameterHelper<P3>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[2]) ,
				CUtil::ParameterHelper<P4>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[3]) ,
				CUtil::ParameterHelper<P5>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[4]) ,
				CUtil::ParameterHelper<P6>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[5]) ,
				CUtil::ParameterHelper<P7>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[6]));
		}

		template< typename ClassObject , typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8>
		static ObjectMsgCall * CallFunc(ParaseMsgCall * pPMC,ClassObject & obj , ObjectMsgCall * (ClassObject::*func)(INT32 nSessionID , Object , P1 , P2 , P3 , P4 , P5 , P6 , P7 , P8 ))	
		{
			Assert_Re0(pPMC && pPMC->m_pMsgCall);
			return (obj.*func)(pPMC->m_nSessionID , pPMC->m_pMsgCall->m_objSource , 
				CUtil::ParameterHelper<P1>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[0]) , 
				CUtil::ParameterHelper<P2>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[1]) ,
				CUtil::ParameterHelper<P3>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[2]) ,
				CUtil::ParameterHelper<P4>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[3]) ,
				CUtil::ParameterHelper<P5>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[4]) ,
				CUtil::ParameterHelper<P6>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[5]) ,
				CUtil::ParameterHelper<P7>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[6]) ,
				CUtil::ParameterHelper<P8>::GetParameterValue(pPMC->m_pMsgCall->m_objParams.m_aParameter[7]));
		}

	};     
	   
}

#endif