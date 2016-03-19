#ifndef __msg_rpc_check_params_h__
#define __msg_rpc_check_params_h__
#include "MsgLib/inc/MsgCommon.h"     
#include "CUtil/inc/Chunk.h"
#include "CUtil/inc/Parameter.h"
#include "CUtil/inc/GenParamHelper.h"
#include "CUtil/inc/Parameters.h"
 

#define	CheckParams_0(PARAM) CUtil::GenParamHelper::CheckParams(*PARAM) 
#define	CheckParams_1(PARAM , p1) CUtil::GenParamHelper::CheckParams(*PARAM , p1 ) 
#define	CheckParams_2(PARAM , p1 , p2) CUtil::GenParamHelper::CheckParams(*PARAM , p1 , p2 ) 
#define	CheckParams_3(PARAM , p1 , p2 , p3) CUtil::GenParamHelper::CheckParams(*PARAM , p1 , p2 , p3 ) 
#define	CheckParams_4(PARAM , p1 , p2 , p3, p4) CUtil::GenParamHelper::CheckParams(*PARAM , p1 , p2 , p3, p4 ) 
#define	CheckParams_5(PARAM , p1 , p2 , p3, p4 , p5) CUtil::GenParamHelper::CheckParams(*PARAM , p1 , p2 , p3, p4 , p5 ) 
#define	CheckParams_6(PARAM , p1 , p2 , p3, p4 , p5 , p6) CUtil::GenParamHelper::CheckParams(*PARAM , p1 , p2 , p3, p4 , p5 , p6 ) 
#define	CheckParams_7(PARAM , p1 , p2 , p3, p4 , p5 , p6 , p7) CUtil::GenParamHelper::CheckParams(*PARAM , p1 , p2 , p3, p4 , p5 , p6 , p7 )  
#define	CheckParams_8(PARAM , p1 , p2 , p3, p4 , p5 , p6 , p7 , p8) CUtil::GenParamHelper::CheckParams(*PARAM , p1 , p2 , p3, p4 , p5 , p6 , p7 , p8) 

#define GET_PARAMETER CUtil::Parameters * pParameter = NULL;\
	if (bDeliver)\
	{\
		pParameter = Msg::RpcCheckParams::GetInstance()->GetDeliverParams(pFunName);\
	}\
	else\
	{\
		pParameter = Msg::RpcCheckParams::GetInstance()->GetReturnParams(pFunName);\
	} \
	Assert_Re0(pParameter) 

namespace Msg
{
	class DLL_EXPORT RpcCheckParams
	{
	public:
		typedef std_unordered_map<std::string , CUtil::Parameters> MapCollectParametersT;

	public:
		RpcCheckParams()   
		{  
		} 

	public:
		static RpcCheckParams * GetInstance(){ static RpcCheckParams sInstance; return &sInstance; }

	public: 
		BOOL CheckRpcParams(BOOL bDeliver , const char * pFunName)
		{
			GET_PARAMETER;
			return CheckParams_0(pParameter);
		}

		template<typename P1> 
		BOOL CheckRpcParams(BOOL bDeliver , const char * pFunName , P1 &p1 )
		{ 
			GET_PARAMETER;
			return CheckParams_1(pParameter , p1); 
		}

		template<typename P1 , typename P2 >
		BOOL CheckRpcParams(BOOL bDeliver , const char * pFunName , P1 &p1 , P2 &p2)
		{ 
			GET_PARAMETER;
			return CheckParams_2(pParameter , p1 , p2 );	
		}

		template<typename P1 , typename P2 , typename P3>	
		BOOL CheckRpcParams(BOOL bDeliver , const char * pFunName , P1 &p1 , P2 &p2 , P3 &p3 ) 
		{ 
			GET_PARAMETER;
			return CheckParams_3(pParameter , p1 , p2 , p3); 
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 > 
		BOOL CheckRpcParams(BOOL bDeliver , const char * pFunName , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 )
		{ 
			GET_PARAMETER;
			return CheckParams_4(pParameter , p1 , p2 , p3, p4 );	
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 >
		BOOL CheckRpcParams(BOOL bDeliver , const char * pFunName , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 ) 
		{ 
			GET_PARAMETER;
			return CheckParams_5(pParameter , p1 , p2 , p3, p4 , p5 );
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6> 
		BOOL CheckRpcParams(BOOL bDeliver , const char * pFunName , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 )
		{
			GET_PARAMETER;
			return CheckParams_6(pParameter , p1 , p2 , p3, p4 , p5 , p6);
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7>	
		BOOL CheckRpcParams(BOOL bDeliver , const char * pFunName , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7) 
		{ 
			GET_PARAMETER;
			return CheckParams_7(pParameter , p1 , p2 , p3, p4 , p5 , p6 , p7);
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8>
		BOOL CheckRpcParams(BOOL bDeliver , const char * pFunName , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7 , P8 & p8)
		{ 
			GET_PARAMETER;
			return CheckParams_8(pParameter , p1 , p2 , p3, p4 , p5 , p6 , p7 , p8); 
		}

	public:  
		CUtil::Parameters * GetDeliverParams(std::string strFunName);
		CUtil::Parameters * GetReturnParams(std::string strFunName);
		void         InsertDeliverParams(std::string strFunName , CUtil::Parameters & objParam);
		void         InsertReturnParams(std::string strFunName , CUtil::Parameters & objParam);

	public:  
		MapCollectParametersT   m_mapReturnParams;
		MapCollectParametersT   m_mapDeliverParams;
	}; 
} 


#endif