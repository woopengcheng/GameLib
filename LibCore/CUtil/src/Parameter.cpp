#include "CUtil/inc/Parameter.h"
#include "Marshal/inc/CStream.h"
#include "CUtil/inc/ParameterHelper.h"

namespace CUtil
{  

	Parameter::Parameter() 
		: m_unSize(0)
	{   
	}
	
	Parameter::Parameter( const Parameter & objParameter )
	{
		UINT8 unType = remove_const(objParameter).GetType();
		if (unType == PARAMETER_TYPE_PARAMETER)
		{
			Parameter objParam;
			CUtil::ParameterHelper<Parameter>::GetParameterValue(remove_const(objParameter) , objParam);
			m_unSize = objParam.GetSize();
			m_objParamStream = objParam.GetParamStream();
		}
		else
		{
			m_unSize = objParameter.GetSize();
			m_objParamStream = objParameter.GetParamStream();
		}
	}

	Parameter & Parameter::operator=( const Parameter & objParameter )
	{ 
		m_unSize = objParameter.m_unSize;
		m_objParamStream = objParameter.m_objParamStream; 

		return *this;
	} 

	CErrno Parameter::CopyTo(Parameter & objParameter )
	{ 
		objParameter.m_unSize = m_unSize;
		objParameter.m_objParamStream.Copy(m_objParamStream);  //5 这里拷贝是反过来的.需要注意

		return CErrno::Success();
	}

	CUtil::CStream & Parameter::marshal( CUtil::CStream & cs )const 
	{  
		cs << m_unSize;
		cs << m_objParamStream; 
		 
		return cs; 
	}

	CUtil::CStream & Parameter::unMarshal( CUtil::CStream & cs )
	{  
		cs >> m_unSize;
		cs >> m_objParamStream;
		return cs; 
	}

	UINT32 Parameter::GetSize() const
	{ 
		return m_unSize;
// 		UINT32 unSize = 0 ;
// 		UINT8 unType = 0;
// 		m_objParamStream >> CUtil::Marshal::Begin >> unType >> unSize >> CUtil::Marshal::Rollback;
// 
// 		return unSize;
	}

	UINT8 Parameter::GetType()
	{ 
		UINT8 unType = 0;
		m_objParamStream >> CUtil::Marshal::Begin >> unType >> CUtil::Marshal::Rollback;

		return unType;
	}

	void    * Parameter::GetValue(void)
	{
		UINT8 unType = 0;
		void * pBuf = NULL;

		m_objParamStream >> CUtil::Marshal::Begin >> unType;
		m_objParamStream.Pop(pBuf , m_unSize);
		m_objParamStream >> CUtil::Marshal::Rollback; 

		return pBuf;
	}

	void      Parameter::Clear( void )
	{ 
		m_unSize = 0;
		m_objParamStream.Clear();
	}

	void * Parameter::GetStreamData(void)
	{
		return m_objParamStream.Begin();
	}

}