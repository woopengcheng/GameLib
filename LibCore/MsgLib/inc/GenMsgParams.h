// #ifndef __msg_gen_msg_params_h__
// #define __msg_gen_msg_params_h__ 
// #include "MsgLib/inc/MsgCommon.h"
// #include "CUtil/inc/ParameterHelper.h" 
// #include "CUtil/inc/Parameters.h"
// 
// namespace Msg
// {
// 	namespace GenMsgHelper
// 	{ 	
// 		static void GenParams(CUtil::Parameters & objParams)
// 		{
// 			objParams.m_unParamCount = 0; 
// 		}
// 
// 		template<typename P1>
// 		static void GenParams(Parameters & objParams , P1 &p1)
// 		{
// 			objParams.m_unParamCount = 1; 
// 			ParameterHelper<P1>::MakeParameter(objParams.m_aParameter[0] , p1);
// 		}
// 
// 		template<typename P1 , typename P2>
// 		static void GenParams(Parameters & objParams , P1 &p1 , P2 &p2)
// 		{
// 			objParams.m_unParamCount = 2; 
// 			ParameterHelper<P1>::MakeParameter(objParams.m_aParameter[0] , p1);
// 			ParameterHelper<P2>::MakeParameter(objParams.m_aParameter[1] , p2); 
// 		}
// 
// 		template<typename P1 , typename P2 , typename P3>
// 		static void GenParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3)
// 		{
// 			objParams.m_unParamCount = 3; 
// 			ParameterHelper<P1>::MakeParameter(objParams.m_aParameter[0] , p1);
// 			ParameterHelper<P2>::MakeParameter(objParams.m_aParameter[1] , p2); 
// 			ParameterHelper<P3>::MakeParameter(objParams.m_aParameter[2] , p3); 
// 		}
// 
// 		template<typename P1 , typename P2 , typename P3 , typename P4>
// 		static void GenParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4)
// 		{
// 			objParams.m_unParamCount = 4; 
// 			ParameterHelper<P1>::MakeParameter(objParams.m_aParameter[0] , p1);
// 			ParameterHelper<P2>::MakeParameter(objParams.m_aParameter[1] , p2); 
// 			ParameterHelper<P3>::MakeParameter(objParams.m_aParameter[2] , p3); 
// 			ParameterHelper<P4>::MakeParameter(objParams.m_aParameter[3] , p4);  
// 		} 
// 
// 		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5>
// 		static void GenParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5)
// 		{
// 			objParams.m_unParamCount = 5; 
// 			ParameterHelper<P1>::MakeParameter(objParams.m_aParameter[0] , p1);
// 			ParameterHelper<P2>::MakeParameter(objParams.m_aParameter[1] , p2); 
// 			ParameterHelper<P3>::MakeParameter(objParams.m_aParameter[2] , p3); 
// 			ParameterHelper<P4>::MakeParameter(objParams.m_aParameter[3] , p4); 
// 			ParameterHelper<P5>::MakeParameter(objParams.m_aParameter[4] , p5); 
// 		} 
// 
// 		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6>
// 		static void GenParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6)
// 		{
// 			objParams.m_unParamCount = 6; 
// 			ParameterHelper<P1>::MakeParameter(objParams.m_aParameter[0] , p1);
// 			ParameterHelper<P2>::MakeParameter(objParams.m_aParameter[1] , p2); 
// 			ParameterHelper<P3>::MakeParameter(objParams.m_aParameter[2] , p3); 
// 			ParameterHelper<P4>::MakeParameter(objParams.m_aParameter[3] , p4); 
// 			ParameterHelper<P5>::MakeParameter(objParams.m_aParameter[4] , p5); 
// 			ParameterHelper<P6>::MakeParameter(objParams.m_aParameter[5] , p6); 
// 		}
// 
// 		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7>
// 		static void GenParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7)
// 		{
// 			objParams.m_unParamCount = 7; 
// 			ParameterHelper<P1>::MakeParameter(objParams.m_aParameter[0] , p1);
// 			ParameterHelper<P2>::MakeParameter(objParams.m_aParameter[1] , p2); 
// 			ParameterHelper<P3>::MakeParameter(objParams.m_aParameter[2] , p3); 
// 			ParameterHelper<P4>::MakeParameter(objParams.m_aParameter[3] , p4); 
// 			ParameterHelper<P5>::MakeParameter(objParams.m_aParameter[4] , p5); 
// 			ParameterHelper<P6>::MakeParameter(objParams.m_aParameter[5] , p6); 
// 			ParameterHelper<P7>::MakeParameter(objParams.m_aParameter[6] , p7); 
// 		}
// 
// 		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8>
// 		static void GenParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7 , P8 &p8)
// 		{
// 			objParams.m_unParamCount = 8;
// 			ParameterHelper<P1>::MakeParameter(objParams.m_aParameter[0] , p1);
// 			ParameterHelper<P2>::MakeParameter(objParams.m_aParameter[1] , p2); 
// 			ParameterHelper<P3>::MakeParameter(objParams.m_aParameter[2] , p3); 
// 			ParameterHelper<P4>::MakeParameter(objParams.m_aParameter[3] , p4); 
// 			ParameterHelper<P5>::MakeParameter(objParams.m_aParameter[4] , p5); 
// 			ParameterHelper<P6>::MakeParameter(objParams.m_aParameter[5] , p6); 
// 			ParameterHelper<P7>::MakeParameter(objParams.m_aParameter[6] , p7); 
// 			ParameterHelper<P8>::MakeParameter(objParams.m_aParameter[7] , p8); 
// 		}
// 		 
// 		static BOOL CheckParams(Parameters & objParams)
// 		{
// 			bool bResult = true;
// 			if(objParams.m_unParamCount != 0)
// 			{
// 				bResult = false;
// 			}    
// 
// 			return bResult;
// 		}
// 
// 		template<typename P1>
// 		static BOOL CheckParams(Parameters & objParams , P1 &p1)
// 		{
// 			bool bResult = true;
// 			if(objParams.m_unParamCount != 1)
// 			{
// 				bResult = false;
// 			}
// 			else
// 			{
// 				bResult = ParameterHelper<P1>::CheckParamType(objParams.m_aParameter[0] ) && bResult;   
// 			}
// 
// 			return bResult;
// 		}
// 
// 
// 		template<typename P1 , typename P2>
// 		static BOOL CheckParams(Parameters & objParams , P1 &p1 , P2 &p2)
// 		{
// 			bool bResult = true;
// 			if(objParams.m_unParamCount != 2)
// 			{
// 				bResult = false;
// 			}
// 			else
// 			{
// 				bResult = ParameterHelper<P1>::CheckParamType(objParams.m_aParameter[0] ) && bResult;
// 				bResult = ParameterHelper<P2>::CheckParamType(objParams.m_aParameter[1]) && bResult;  
// 			}
// 
// 			return bResult;
// 		}
// 
// 		template<typename P1 , typename P2 , typename P3>
// 		static BOOL CheckParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3)
// 		{
// 			bool bResult = true;
// 			if(objParams.m_unParamCount != 3)
// 			{
// 				bResult = false;
// 			}
// 			else
// 			{
// 				bResult = ParameterHelper<P1>::CheckParamType(objParams.m_aParameter[0] ) && bResult;
// 				bResult = ParameterHelper<P2>::CheckParamType(objParams.m_aParameter[1]) && bResult; 
// 				bResult = ParameterHelper<P3>::CheckParamType(objParams.m_aParameter[2] ) && bResult;  
// 			}
// 
// 			return bResult;
// 		}
// 
// 		template<typename P1 , typename P2 , typename P3 , typename P4>
// 		static BOOL CheckParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4)
// 		{
// 			bool bResult = true;
// 			if(objParams.m_unParamCount != 4)
// 			{
// 				bResult = false;
// 			}
// 			else
// 			{
// 				bResult = ParameterHelper<P1>::CheckParamType(objParams.m_aParameter[0] ) && bResult;
// 				bResult = ParameterHelper<P2>::CheckParamType(objParams.m_aParameter[1]) && bResult; 
// 				bResult = ParameterHelper<P3>::CheckParamType(objParams.m_aParameter[2] ) && bResult; 
// 				bResult = ParameterHelper<P4>::CheckParamType(objParams.m_aParameter[3] ) && bResult;  
// 			}
// 
// 			return bResult;
// 		}
// 
// 		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5>
// 		static BOOL CheckParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5)
// 		{
// 			bool bResult = true;
// 			if(objParams.m_unParamCount != 5)
// 			{
// 				bResult = false;
// 			}
// 			else
// 			{
// 				bResult = ParameterHelper<P1>::CheckParamType(objParams.m_aParameter[0] ) && bResult;
// 				bResult = ParameterHelper<P2>::CheckParamType(objParams.m_aParameter[1]) && bResult; 
// 				bResult = ParameterHelper<P3>::CheckParamType(objParams.m_aParameter[2] ) && bResult; 
// 				bResult = ParameterHelper<P4>::CheckParamType(objParams.m_aParameter[3] ) && bResult; 
// 				bResult = ParameterHelper<P5>::CheckParamType(objParams.m_aParameter[4] ) && bResult;  
// 			}
// 
// 			return bResult;
// 		}
// 
// 		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6>
// 		static BOOL CheckParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6)
// 		{
// 			bool bResult = true;
// 			if(objParams.m_unParamCount != 6)
// 			{
// 				bResult = false;
// 			}
// 			else
// 			{
// 				bResult = ParameterHelper<P1>::CheckParamType(objParams.m_aParameter[0] ) && bResult;
// 				bResult = ParameterHelper<P2>::CheckParamType(objParams.m_aParameter[1]) && bResult; 
// 				bResult = ParameterHelper<P3>::CheckParamType(objParams.m_aParameter[2] ) && bResult; 
// 				bResult = ParameterHelper<P4>::CheckParamType(objParams.m_aParameter[3] ) && bResult; 
// 				bResult = ParameterHelper<P5>::CheckParamType(objParams.m_aParameter[4] ) && bResult; 
// 				bResult = ParameterHelper<P6>::CheckParamType(objParams.m_aParameter[5] ) && bResult;   
// 			}
// 
// 			return bResult;
// 		}
// 
// 		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7>
// 		static BOOL CheckParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7)
// 		{
// 			bool bResult = true;
// 			if(objParams.m_unParamCount != 7)
// 			{
// 				bResult = false;
// 			}
// 			else
// 			{
// 				bResult = ParameterHelper<P1>::CheckParamType(objParams.m_aParameter[0] ) && bResult;
// 				bResult = ParameterHelper<P2>::CheckParamType(objParams.m_aParameter[1]) && bResult; 
// 				bResult = ParameterHelper<P3>::CheckParamType(objParams.m_aParameter[2] ) && bResult; 
// 				bResult = ParameterHelper<P4>::CheckParamType(objParams.m_aParameter[3] ) && bResult; 
// 				bResult = ParameterHelper<P5>::CheckParamType(objParams.m_aParameter[4] ) && bResult; 
// 				bResult = ParameterHelper<P6>::CheckParamType(objParams.m_aParameter[5] ) && bResult; 
// 				bResult = ParameterHelper<P7>::CheckParamType(objParams.m_aParameter[6] ) && bResult;  
// 			}
// 
// 			return bResult;
// 		}
// 
// 		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8>
// 		static BOOL CheckParams(Parameters & objParams , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7 , P8 &p8)
// 		{
// 			bool bResult = true;
// 			if(objParams.m_unParamCount != 8)
// 			{
// 				bResult = false;
// 			}
// 			else
// 			{
// 				bResult = ParameterHelper<P1>::CheckParamType(objParams.m_aParameter[0] ) && bResult;
// 				bResult = ParameterHelper<P2>::CheckParamType(objParams.m_aParameter[1]) && bResult; 
// 				bResult = ParameterHelper<P3>::CheckParamType(objParams.m_aParameter[2] ) && bResult; 
// 				bResult = ParameterHelper<P4>::CheckParamType(objParams.m_aParameter[3] ) && bResult; 
// 				bResult = ParameterHelper<P5>::CheckParamType(objParams.m_aParameter[4] ) && bResult; 
// 				bResult = ParameterHelper<P6>::CheckParamType(objParams.m_aParameter[5] ) && bResult; 
// 				bResult = ParameterHelper<P7>::CheckParamType(objParams.m_aParameter[6] ) && bResult; 
// 				bResult = ParameterHelper<P8>::CheckParamType(objParams.m_aParameter[7] ) && bResult;  
// 			}
// 
// 			return bResult;
// 		}
// 
// 	}  
// }
// 
// #endif