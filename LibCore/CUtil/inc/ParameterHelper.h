#ifndef __cutil_parameter_helper_h__
#define __cutil_parameter_helper_h__
#include "CUtil/inc/Parameter.h"
#include "CUtil/inc/Chunk.h"
#include "Marshal/inc/CStream.h"
#include "Marshal/inc/CommonMarshal.h"
#include "MsgLib/inc/Object.h"

namespace CUtil 
{ 
#define GEN_PARAMTER_HELPER_REFER(type_name , type_macro )	template<> class ParameterHelper<type_name &>\
	{\
	public:\
		static UINT8 GetParameterType()\
		{\
			return (UINT8)type_macro ;\
		}\
		\
		static type_name GetParameterValue(Parameter & objParam)\
		{ \
			return ParameterHelper<type_name>::GetParameterValue(objParam);\
		}\
		static BOOL GetParameterValue(Parameter & objParam , type_name & val)\
		{ \
			return ParameterHelper<type_name>::GetParameterValue(objParam , val);\
		}\
		\
		static UINT32 MakeParameter(Parameter & objParam , type_name & val)\
		{ \
			return ParameterHelper<type_name>::MakeParameter(objParam , val); \
		}\
		\
		static BOOL CheckParamType(Parameter & objParam)\
		{\
			if (objParam.GetType() == (UINT8)type_macro)\
			{\
				return TRUE;\
			}\
			return FALSE;\
		}\
	};


#define GEN_PARAMTER_HELPER(type_name , type_macro )	template<> class ParameterHelper<type_name>\
	{\
	public:\
		static UINT8 GetParameterType()\
		{\
			return (UINT8)type_macro ;\
		}\
		\
		static type_name GetParameterValue(Parameter & objParam)\
		{ \
			UINT8 unType = 0;\
			type_name  val;\
			\
			objParam.GetParamStream() >> CUtil::Marshal::Begin >> unType >> val >> CUtil::Marshal::Rollback;   \
			MsgAssert_Re(unType == type_macro , val , "获取参数值错误.");\
			\
			return val;\
		}\
		\
		static BOOL GetParameterValue(Parameter & objParam , type_name & val)\
		{ \
			UINT8 unType = 0;\
			\
			objParam.GetParamStream() >> CUtil::Marshal::Begin >> unType >> val >> CUtil::Marshal::Rollback;   \
			MsgAssert_Re0(unType == type_macro , "获取参数值错误.");\
			\
			return TRUE;\
		}\
		\
		static UINT32 MakeParameter(Parameter & objParam , type_name & value)\
		{ \
			objParam.GetParamStream() << (UINT8)type_macro;\
			\
			UINT32 unOldLen = 0;\
			unOldLen = objParam.GetParamStream().GetDataLen();\
			objParam.GetParamStream() << value;\
			return  objParam.GetParamStream().GetDataLen() - unOldLen;\
		}\
		\
		static BOOL CheckParamType(Parameter & objParam)\
		{\
			if (objParam.GetType() == (UINT8)type_macro)\
			{\
				return TRUE;\
			}\
			return FALSE;\
		}\
	};\
	GEN_PARAMTER_HELPER_REFER(type_name , type_macro)
		 
	template<typename T> class ParameterHelper
	{
	public:
		static UINT8 GetParameterType(){ return (UINT8)PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS; } 

		static T GetParameterValue(Parameter & objParam)
		{
			UINT8 nType = 0 ;
			T Value = T();

			objParam.GetParamStream() >> CUtil::Marshal::Begin >> nType ;
			MsgAssert_Re(nType == PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS , Value , "paramter type is error:" << nType << ":cur=" << PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS);

			objParam.GetParamStream() >> Value >> CUtil::Marshal::Rollback;

			return Value; 
		}

		static BOOL GetParameterValue(Parameter & objParam , T & Value)
		{
			UINT8 nType = 0 ; 

			objParam.GetParamStream() >> CUtil::Marshal::Begin >> nType ;
			MsgAssert_Re0(nType == PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS , "paramter type is error:" << nType << ":cur=" << PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS);

			objParam.GetParamStream() >> Value >> CUtil::Marshal::Rollback;

			return TRUE; 
		}

		static UINT32 MakeParameter(Parameter & objParam , T & value)
		{   
			UINT32 unOldLen = 0;
			objParam.GetParamStream() << (UINT8)PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS;  
			unOldLen = objParam.GetParamStream().GetDataLen();
			objParam.GetParamStream() << value;
			return  objParam.GetParamStream().GetDataLen() - unOldLen;
		} 

		static BOOL CheckParamType(Parameter & objParam)
		{
			if (objParam.GetType() == (UINT8)PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS)
			{
				return TRUE;
			}
			return FALSE;
		}

		static BOOL CheckParamType(Parameter & objParam , T & value = T())
		{
			if (objParam.GetType() == (UINT8)PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS)
			{
				return TRUE;
			}
			return FALSE;
		}
	};  

	template<typename T> class ParameterHelper<T &>
	{
	public:
		static UINT8 GetParameterType(){ return (UINT8)PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS; } 
		static T GetParameterValue(Parameter & objParam)
		{
			UINT8 nType = 0 ;
			T Value = T();

			objParam.GetParamStream() >> CUtil::Marshal::Begin >> nType ;
			MsgAssert_Re(nType == PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS , Value , "paramter type is error. :" << nType << " cur: " << PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS);

//			CUtil::STLContainer<T> value = CUtil::STLContainer<T>(Value);
			objParam.GetParamStream() >> Value >> CUtil::Marshal::Rollback;

			return Value; 
		}

		static BOOL GetParameterValue(Parameter & objParam , T & Value)
		{
			UINT8 nType = 0 ;

			objParam.GetParamStream() >> CUtil::Marshal::Begin >> nType ;
			MsgAssert_Re0(nType == PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS , "paramter type is error. :" << nType << " cur: " << PARAMETER_TYPE_STD_VECTOR);

//			CUtil::STLContainer<T> value = CUtil::STLContainer<T>(Value);
			objParam.GetParamStream() >> Value >> CUtil::Marshal::Rollback;

			return TRUE; 
		}

		static UINT32 MakeParameter(Parameter & objParam , T & value)
		{   
			UINT32 unOldLen = 0; 
			objParam.GetParamStream() << (UINT8)PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS;  
			unOldLen = objParam.GetParamStream().GetDataLen(); 

			objParam.GetParamStream() << value;
			return  objParam.GetParamStream().GetDataLen() - unOldLen;
		}
		 
		static BOOL CheckParamType(Parameter & objParam)
		{
			if (objParam.GetType() == (UINT8)PARAMETER_TYPE_STD_CONTAINER_OR_OTHERS)
			{
				return TRUE;
			}
			return FALSE;
		}
	};  

	template<> class ParameterHelper<const char *>
	{
	public:
		static UINT8 GetParameterType(){ return (UINT8)PARAMETER_TYPE_STRING; } 
		static const char * GetParameterValue(Parameter & objParam)
		{
			UINT8 nType = 0 ;
			INT32 nSize = 0;

			objParam.GetParamStream() >> CUtil::Marshal::Begin >> nType >> nSize;
			MsgAssert_Re0(nType == PARAMETER_TYPE_STRING , "paramter type is error. :" << nType << " cur: " << PARAMETER_TYPE_STRING);
			MsgAssert_Re0(!((UINT32)nSize > objParam.GetParamStream().GetDataLen() - objParam.GetParamStream().GetCurPos()) , "unMarshal invalid length."); 
			 
			void * pBuf = NULL;
			objParam.GetParamStream().Pop(pBuf , nSize);

			objParam.GetParamStream() >> CUtil::Marshal::Rollback;
			return (const char * )pBuf; 
		}

		static BOOL GetParameterValue(Parameter & objParam , char *& p1)
		{
			UINT8 nType = 0 ;
			INT32 nSize = 0;

			objParam.GetParamStream() >> CUtil::Marshal::Begin >> nType >> nSize;
			MsgAssert_Re0(nType == PARAMETER_TYPE_STRING , "paramter type is error. :" << nType << " cur: " << PARAMETER_TYPE_STRING);
			MsgAssert_Re0(!((UINT32)nSize > objParam.GetParamStream().GetDataLen() - objParam.GetParamStream().GetCurPos()) , "unMarshal invalid length."); 

			void * pBuf = NULL;
			objParam.GetParamStream().Pop(pBuf , nSize);

			objParam.GetParamStream() >> CUtil::Marshal::Rollback;
			p1 = (char *)pBuf;
			return TRUE; 
		}

		static UINT32 MakeParameter(Parameter & objParam , const char * pValue)
		{  
			if (!pValue)
			{
				objParam.GetParamStream() << (UINT8)PARAMETER_TYPE_STRING << 0;  
				objParam.GetParamStream().Pushback((void*)pValue , 0);

				return 0;
			}
			else
			{
				UINT32 unSize = (UINT32)strlen(pValue) + 1;

				objParam.GetParamStream() << (UINT8)PARAMETER_TYPE_STRING << unSize;  
				objParam.GetParamStream().Pushback((void*)pValue , unSize);

				return unSize;
			} 
		}

		static BOOL CheckParamType(Parameter & objParam)
		{
			if (objParam.GetType() == PARAMETER_TYPE_STRING)
			{
				return TRUE;
			}
			return FALSE;
		}
	};

	template<> class ParameterHelper<std::string>
	{
	public:
		static UINT8 GetParameterType(){ return (UINT8)PARAMETER_TYPE_STD_STRING; } 
		static std::string GetParameterValue(Parameter & objParam)
		{
			UINT8 nType = 0 ;

			objParam.GetParamStream() >> CUtil::Marshal::Begin >> nType;
			MsgAssert_Re0(nType == PARAMETER_TYPE_STD_STRING , "paramter type is error. :" << nType << " cur: " << PARAMETER_TYPE_STD_STRING);

			std::string strValue;
			objParam.GetParamStream() >> strValue >> CUtil::Marshal::Rollback;

			return strValue; 
		}

		static BOOL GetParameterValue(Parameter & objParam , std::string & strValue)
		{
			UINT8 nType = 0;

			objParam.GetParamStream() >> CUtil::Marshal::Begin >> nType;
			MsgAssert_Re0(nType == PARAMETER_TYPE_STD_STRING , "paramter type is error. :" << nType << " cur: " << PARAMETER_TYPE_STD_STRING);
 
			objParam.GetParamStream() >> strValue >> CUtil::Marshal::Rollback;

			return TRUE; 
		}

		static UINT32 MakeParameter(Parameter & objParam , std::string & strValue)
		{   
				UINT32 unOldLen = 0;

				objParam.GetParamStream() << (UINT8)PARAMETER_TYPE_STD_STRING;  
				unOldLen = objParam.GetParamStream().GetDataLen();
				objParam.GetParamStream() << strValue;

				return objParam.GetParamStream().GetDataLen() - unOldLen;
		}

		static BOOL CheckParamType(Parameter & objParam)
		{
			if (objParam.GetType() == PARAMETER_TYPE_STD_STRING)
			{
				return TRUE;
			}
			return FALSE;
		}
	};

	template<> class ParameterHelper<std::string &>
	{
	public:
		static UINT8 GetParameterType(){ return (UINT8)PARAMETER_TYPE_STD_STRING; } 
		static std::string GetParameterValue(Parameter & objParam)
		{
			return ParameterHelper<std::string>::GetParameterValue(objParam);
		}

		static BOOL GetParameterValue(Parameter & objParam , std::string & p1)
		{
			return ParameterHelper<std::string>::GetParameterValue(objParam , p1);
		}

		static UINT32 MakeParameter(Parameter & objParam , std::string & strValue)
		{   
			return ParameterHelper<std::string>::MakeParameter(objParam , strValue);
		}

		static BOOL CheckParamType(Parameter & objParam)
		{
			if (objParam.GetType() == PARAMETER_TYPE_STD_STRING)
			{
				return TRUE;
			}
			return FALSE;
		}
	}; 

	template<> class ParameterHelper<const std::string>
	{
	public:
		static UINT8 GetParameterType(){ return (UINT8)PARAMETER_TYPE_STD_STRING; } 
		static std::string GetParameterValue(Parameter & objParam)
		{
			return ParameterHelper<std::string>::GetParameterValue(objParam);
		}

		static BOOL GetParameterValue(Parameter & objParam , std::string & p1)
		{
			return ParameterHelper<std::string>::GetParameterValue(objParam , p1);
		}

		static UINT32 MakeParameter(Parameter & objParam , std::string & strValue)
		{   
			return ParameterHelper<std::string>::MakeParameter(objParam , strValue);
		}

		static BOOL CheckParamType(Parameter & objParam)
		{
			if (objParam.GetType() == PARAMETER_TYPE_STD_STRING)
			{
				return TRUE;
			}
			return FALSE;
		}
	};  

	GEN_PARAMTER_HELPER(bool , PARAMETER_TYPE_BOOL); 
	GEN_PARAMTER_HELPER(char , PARAMETER_TYPE_SINT8); 
	GEN_PARAMTER_HELPER(UINT8 , PARAMETER_TYPE_UINT8);
	GEN_PARAMTER_HELPER(INT16 , PARAMETER_TYPE_INT16);
	GEN_PARAMTER_HELPER(UINT16 , PARAMETER_TYPE_UINT16);
	GEN_PARAMTER_HELPER(long , PARAMETER_TYPE_LONG); 
	GEN_PARAMTER_HELPER(INT32 , PARAMETER_TYPE_INT32); 
	GEN_PARAMTER_HELPER(UINT32 , PARAMETER_TYPE_UINT32);
	GEN_PARAMTER_HELPER(INT64 , PARAMETER_TYPE_INT64);
	GEN_PARAMTER_HELPER(UINT64 , PARAMETER_TYPE_UINT64); 
	GEN_PARAMTER_HELPER(float , PARAMETER_TYPE_FLOAT);
	GEN_PARAMTER_HELPER(double , PARAMETER_TYPE_DOUBLE);  
	GEN_PARAMTER_HELPER(CUtil::Chunk , PARAMETER_TYPE_CHUNK);  
	GEN_PARAMTER_HELPER(CUtil::Parameter , 	PARAMETER_TYPE_PARAMETER);
}

#endif
