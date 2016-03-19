#ifndef __cutil_parameters_h__
#define __cutil_parameters_h__ 
#include "CUtil/inc/Parameter.h"
#include "CUtil/inc/GenParamHelper.h" 

namespace CUtil
{ 
	class GenParamHelper;

	class DLL_EXPORT Parameters : public CUtil::Marshal
	{
	public: 
		Parameters() : m_unParamCount(0){}
		~Parameters(){}

		Parameters(const Parameters & objParameters) 
		{ 
			m_unParamCount = objParameters.m_unParamCount;
			for (int i = 0;i < MSG_MAX_PARAMETER_NUMBER;++i)
			{
				m_aParameter[i] = objParameters.m_aParameter[i];
			}
		}

	public:
		Parameters & operator = (const Parameters & objParameters)
		{
			m_unParamCount = objParameters.m_unParamCount;
			for (int i = 0;i < MSG_MAX_PARAMETER_NUMBER;++i)
			{
				m_aParameter[i] = objParameters.m_aParameter[i];
			}
			return *this;
		} 

		bool IsEqual(const Parameters & objParams) const
		{
			if(m_unParamCount != objParams.m_unParamCount)
			{
				return false;
			}

			for (int i = 0;i < MSG_MAX_PARAMETER_NUMBER;++i)
			{
				if(m_aParameter[i] != objParams.m_aParameter[i])
				{
					return false;
				}
			}
			return true;
		}

		bool operator ==(const Parameters & objParams) const
		{ 
			return IsEqual(objParams);
		}

		bool operator !=(const Parameters & objParams) const
		{
			return !IsEqual(objParams);
		}

		friend	std::ostream&	operator<<(std::ostream&os,const Parameters & objParams)
		{
			os << "Parameter:ParamCount=" << objParams.m_unParamCount ;
			for (int i = 0;i < MSG_MAX_PARAMETER_NUMBER;++i)
			{
				os << "Parameter:ParamCount=" << objParams.m_aParameter[i];
			}
			return os;
		}

		Parameter & operator [] (INT32 i)
		{
			MsgAssert_Re(i >= 0 && i <= MSG_MAX_PARAMETER_NUMBER , m_aParameter[0] , "error index of params.");
			return m_aParameter[i];
		} 
	public:  
		UINT32		GetParamCount() const{ return m_unParamCount;}
		UINT32		GetSize();
		CErrno		CopyTo(Parameters & pParam); 
		void		Clear(void);

	public:
		//5 这个接口性能不够高 
		template<typename T>
		T GetValue(INT32 nPos = 0)
		{
			T t;
			UINT8 unType = 0;
			MsgAssert_Re( nPos >= 0 && nPos <= MSG_MAX_PARAMETER_NUMBER , t , "GetValue pos error."); 

			Parameter param;
			param.MakeParameter<T>(t);

			m_aParameter[nPos].GetParamStream()>> Marshal::Begin >> unType;
			MsgAssert_Re( unType == param.GetType() , t , "GetValue type error:param_type=" << param.GetType() << ":get_value_type=" << unType); 
			
			m_aParameter[nPos].GetParamStream() >> t >> Marshal::Rollback;
			
			return t;
		}

		template<typename P1>
		BOOL GetValue(UINT32 nStartPos , P1 & p1)
		{
			UINT8 unType = 0;
			MsgAssert_Re0( nStartPos + 1 <= m_unParamCount , "GetValue pos error."); 

			Parameters params;
			params.GenParams(p1);

			for (UINT32 i = nStartPos;i < m_unParamCount;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i - nStartPos].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i - nStartPos].GetType() << ":get_value_type=" << unType); 

				if (i == nStartPos)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				} 
			} 
			return TRUE;
		}
		template<typename P1 , typename P2>
		BOOL GetValue(UINT32 nStartPos , P1 &p1 , P2 &p2) 
		{
			UINT8 unType = 0;
			MsgAssert_Re0( nStartPos + 2 <= m_unParamCount , "GetValue pos error."); 

			Parameters params;
			params.GenParams(p1 , p2);

			for (UINT32 i = nStartPos;i < m_unParamCount;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i - nStartPos].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i - nStartPos].GetType() << ":get_value_type=" << unType); 

				if (i == nStartPos)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}  
				else if(i == nStartPos + 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				} 
			} 

			return TRUE;
		}
		template<typename P1 , typename P2 , typename P3 >
		BOOL GetValue(UINT32 nStartPos , P1 &p1 , P2 &p2 , P3 &p3) 
		{
			UINT8 unType = 0;
			MsgAssert_Re0( nStartPos + 3 <= m_unParamCount , "GetValue pos error."); 

			Parameters params;
			params.GenParams(p1 , p2 , p3);

			for (UINT32 i = nStartPos;i < m_unParamCount;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i - nStartPos].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i - nStartPos].GetType() << ":get_value_type=" << unType); 

				if (i == nStartPos)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}  
				else if(i == nStartPos + 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				} 
			} 

			return TRUE;
		}
		template<typename P1 , typename P2 , typename P3 , typename P4 >
		BOOL GetValue(UINT32 nStartPos , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 ) 
		{
			UINT8 unType = 0;
			MsgAssert_Re0( nStartPos + 4 <= m_unParamCount , "GetValue pos error."); 

			Parameters params;
			params.GenParams(p1 , p2 , p3 , p4);

			for (UINT32 i = nStartPos;i < m_unParamCount;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i - nStartPos].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i - nStartPos].GetType() << ":get_value_type=" << unType); 

				if (i == nStartPos)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}  
				else if(i == nStartPos + 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 3)
				{
					m_aParameter[i].GetParamStream() >> p4 >> Marshal::Rollback;
				} 
			} 

			return TRUE;
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5>
		BOOL GetValue(UINT32 nStartPos , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 ) 
		{
			UINT8 unType = 0;
			MsgAssert_Re0( nStartPos + 5 <= m_unParamCount , "GetValue pos error."); 

			Parameters params;
			params.GenParams(p1 , p2 , p3 , p4 , p5);

			for (UINT32 i = nStartPos;i < m_unParamCount;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i - nStartPos].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i - nStartPos].GetType() << ":get_value_type=" << unType); 

				if (i == nStartPos)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}  
				else if(i == nStartPos + 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 3)
				{
					m_aParameter[i].GetParamStream() >> p4 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 4)
				{
					m_aParameter[i].GetParamStream() >> p5 >> Marshal::Rollback;
				} 
			} 

			return TRUE;
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6>
		BOOL GetValue(UINT32 nStartPos , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6) 
		{
			UINT8 unType = 0;
			MsgAssert_Re0( nStartPos + 6 <= m_unParamCount , "GetValue pos error."); 

			Parameters params;
			params.GenParams(p1 , p2 , p3 , p4 , p5 , p6);

			for (UINT32 i = nStartPos;i < m_unParamCount;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i - nStartPos].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i - nStartPos].GetType() << ":get_value_type=" << unType); 

				if (i == nStartPos)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}  
				else if(i == nStartPos + 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 3)
				{
					m_aParameter[i].GetParamStream() >> p4 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 4)
				{
					m_aParameter[i].GetParamStream() >> p5 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 5)
				{
					m_aParameter[i].GetParamStream() >> p6 >> Marshal::Rollback;
				} 
			} 

			return TRUE;
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7>
		BOOL GetValue(UINT32 nStartPos , P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7) 
		{
			UINT8 unType = 0;
			MsgAssert_Re0( nStartPos + 7 <= m_unParamCount , "GetValue pos error."); 

			Parameters params;
			params.GenParams(p1 , p2 , p3 , p4 , p5 , p6 , p7);

			for (UINT32 i = nStartPos;i < m_unParamCount;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i - nStartPos].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i - nStartPos].GetType() << ":get_value_type=" << unType); 

				if (i == nStartPos)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}  
				else if(i == nStartPos + 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 3)
				{
					m_aParameter[i].GetParamStream() >> p4 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 4)
				{
					m_aParameter[i].GetParamStream() >> p5 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 5)
				{
					m_aParameter[i].GetParamStream() >> p6 >> Marshal::Rollback;
				}
				else if(i == nStartPos + 6)
				{
					m_aParameter[i].GetParamStream() >> p7 >> Marshal::Rollback;
				}
			} 

			return TRUE;
		}

		//5 这个是从头开始获取.参数依次传递,c++11支持默认模板参数.这里还是多写一遍.跨版本用.
		template<typename P1>
		BOOL GetValueEx(P1 &p1)
		{
			UINT8 unType = 0;
			MsgAssert_Re0(m_unParamCount >= 1 , "GetValueEx size error:size=" << m_unParamCount ); 

			Parameters params;
			params.GenParams(p1);

			for (UINT32 i = 0;i < 1;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i].GetType() << ":get_value_type=" << unType); 

				if (i == 0)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				} 
			} 
			return TRUE;
		}
		template<typename P1 , typename P2 >
		BOOL GetValueEx(P1 &p1 , P2 &p2 )
		{
			UINT8 unType = 0;
			MsgAssert_Re0(m_unParamCount >= 2 , "GetValueEx size error:size=" << m_unParamCount ); 

			Parameters params;
			params.GenParams(p1 , p2);

			for (UINT32 i = 0;i < 2;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i].GetType() << ":get_value_type=" << unType); 

				if (i == 0)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}
				else if(i == 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				} 
			} 
			return TRUE;
		}
		template<typename P1 , typename P2 , typename P3  >
		BOOL GetValueEx(P1 &p1 , P2 &p2 , P3 &p3   )
		{
			UINT8 unType = 0;
			MsgAssert_Re0(m_unParamCount >= 3 , "GetValueEx size error:size=" << m_unParamCount ); 

			Parameters params;
			params.GenParams(p1 , p2 , p3 );

			for (UINT32 i = 0;i < 3;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i].GetType() << ":get_value_type=" << unType); 

				if (i == 0)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}
				else if(i == 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				} 
			} 
			return TRUE;
		}
		template<typename P1 , typename P2 , typename P3 , typename P4 >
		BOOL GetValueEx(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 )
		{
			UINT8 unType = 0;
			MsgAssert_Re0(m_unParamCount >= 4 , "GetValueEx size error:size=" << m_unParamCount ); 

			Parameters params;
			params.GenParams(p1 , p2 , p3 , p4 );

			for (UINT32 i = 0;i < 4 ;++i)
			{				
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i].GetType() << ":get_value_type=" << unType); 

				if (i == 0)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}
				else if(i == 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				}
				else if(i == 3)
				{
					m_aParameter[i].GetParamStream() >> p4 >> Marshal::Rollback;
				} 
			}
			 
			return TRUE;
		}
		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5  >
		BOOL GetValueEx(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 )
		{
			UINT8 unType = 0;
			MsgAssert_Re0(m_unParamCount >= 5 , "GetValueEx size error:size=" << m_unParamCount ); 

			Parameters params;
			params.GenParams(p1 , p2 , p3 , p4 , p5 );

			for (UINT32 i = 0;i < 5;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i].GetType() << ":get_value_type=" << unType); 

				if (i == 0)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}
				else if(i == 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				}
				else if(i == 3)
				{
					m_aParameter[i].GetParamStream() >> p4 >> Marshal::Rollback;
				}
				else if(i == 4)
				{
					m_aParameter[i].GetParamStream() >> p5 >> Marshal::Rollback;
				} 
			}
			 
			return TRUE;
		}
		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 >
		BOOL GetValueEx(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6   )
		{
			UINT8 unType = 0;
			MsgAssert_Re0(m_unParamCount >= 6 , "GetValueEx size error:size=" << m_unParamCount ); 

			Parameters params;
			params.GenParams(p1 , p2 , p3 , p4 , p5 , p6 );

			for (UINT32 i = 0;i < 6;++i)
			{	
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i].GetType() << ":get_value_type=" << unType); 

				if (i == 0)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}
				else if(i == 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				}
				else if(i == 3)
				{
					m_aParameter[i].GetParamStream() >> p4 >> Marshal::Rollback;
				}
				else if(i == 4)
				{
					m_aParameter[i].GetParamStream() >> p5 >> Marshal::Rollback;
				}
				else if(i == 5)
				{
					m_aParameter[i].GetParamStream() >> p6 >> Marshal::Rollback;
				} 
			}
			 
			return TRUE;
		}
		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7>
		BOOL GetValueEx(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7)
		{
			UINT8 unType = 0;
			MsgAssert_Re0(m_unParamCount >= 7 , "GetValueEx size error:size=" << m_unParamCount ); 

			Parameters params;
			params.GenParams(p1 , p2 , p3 , p4 , p5 , p6 , p7);

			for (UINT32 i = 0;i < 7;++i)
			{		
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i].GetType() << ":get_value_type=" << unType); 

				if (i == 0)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}
				else if(i == 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				}
				else if(i == 3)
				{
					m_aParameter[i].GetParamStream() >> p4 >> Marshal::Rollback;
				}
				else if(i == 4)
				{
					m_aParameter[i].GetParamStream() >> p5 >> Marshal::Rollback;
				}
				else if(i == 5)
				{
					m_aParameter[i].GetParamStream() >> p6 >> Marshal::Rollback;
				}
				else if(i == 6)
				{
					m_aParameter[i].GetParamStream() >> p7 >> Marshal::Rollback;
				}
			} 
			return TRUE;
		}
		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8>
		BOOL GetValueEx(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7 , P8 &p8 )
		{
			UINT8 unType = 0;
			MsgAssert_Re0(m_unParamCount >= 8 , "GetValueEx size error:size=" << m_unParamCount ); 

			Parameters params;
			params.GenParams(p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8);

			for (UINT32 i = 0;i < 8;++i)
			{				
				m_aParameter[i].GetParamStream() >> Marshal::Begin >> unType;
				MsgAssert_Re0( unType == params.m_aParameter[i].GetType() , "GetValueEx type error:param_type=" << params.m_aParameter[i].GetType() << ":get_value_type=" << unType); 
			
				if (i == 0)
				{
					m_aParameter[i].GetParamStream() >> p1 >> Marshal::Rollback;
				}
				else if(i == 1)
				{
					m_aParameter[i].GetParamStream() >> p2 >> Marshal::Rollback;
				}
				else if(i == 2)
				{
					m_aParameter[i].GetParamStream() >> p3 >> Marshal::Rollback;
				}
				else if(i == 3)
				{
					m_aParameter[i].GetParamStream() >> p4 >> Marshal::Rollback;
				}
				else if(i == 4)
				{
					m_aParameter[i].GetParamStream() >> p5 >> Marshal::Rollback;
				}
				else if(i == 5)
				{
					m_aParameter[i].GetParamStream() >> p6 >> Marshal::Rollback;
				}
				else if(i == 6)
				{
					m_aParameter[i].GetParamStream() >> p7 >> Marshal::Rollback;
				}
				else if(i == 7)
				{
					m_aParameter[i].GetParamStream() >> p8 >> Marshal::Rollback;
				}
			}
			return TRUE;
		}

	public: 
		virtual CUtil::CStream & marshal(CUtil::CStream & cs) const override;
		virtual CUtil::CStream & unMarshal(CUtil::CStream & cs) override;

	public:
// 		UINT32 GenParams()
// 		{
// 			return CUtil::GenMsgHelper::GenParams(*this);
// 		} 
// 
		template<typename P1>
		UINT32 GenParams(P1 &p1)
		{
			return CUtil::GenParamHelper::GenParams(*this , p1);
		}

		template<typename P1 , typename P2>
		UINT32 GenParams(P1 &p1 , P2 &p2)
		{
			return CUtil::GenParamHelper::GenParams(*this , p1 , p2);
		}

		template<typename P1 , typename P2 , typename P3>
		UINT32 GenParams(P1 &p1 , P2 &p2 , P3 &p3)
		{
			return CUtil::GenParamHelper::GenParams(*this , p1 , p2 , p3);
		}

		template<typename P1 , typename P2 , typename P3 , typename P4>
		UINT32 GenParams(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4)
		{
			return CUtil::GenParamHelper::GenParams(*this , p1 , p2 , p3 , p4);
		} 

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5>
		UINT32 GenParams(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5)
		{
			return CUtil::GenParamHelper::GenParams(*this , p1 , p2 , p3 , p4 , p5 );
		} 

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6>
		UINT32 GenParams(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6)
		{
			return CUtil::GenParamHelper::GenParams(*this , p1 , p2 , p3 , p4 , p5 , p6 );
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7>
		UINT32 GenParams(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7)
		{
			return CUtil::GenParamHelper::GenParams(*this , p1 , p2 , p3 , p4 , p5 , p6 , p7);
		}

		template<typename P1 , typename P2 , typename P3 , typename P4 , typename P5 , typename P6 , typename P7 , typename P8>
		UINT32 GenParams(P1 &p1 , P2 &p2 , P3 &p3 , P4 &p4 , P5 &p5 , P6 &p6 , P7 &p7 , P8 &p8)
		{
			return GenParamHelper::GenParams(*this , p1 , p2 , p3 , p4 , p5 , p6 , p7 , p8);
		}

	public:
		UINT32     m_unParamCount;
		Parameter  m_aParameter[MSG_MAX_PARAMETER_NUMBER];
	};

}
#endif